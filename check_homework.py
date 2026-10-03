"""Optional Linux checker. Students edit the C/C++ files, not this Python file.
No third-party packages, custom C headers, syscall tracing, or hidden APIs.
Run only your own code here. Staff must isolate untrusted student submissions.
"""
import argparse
import json
import math
import os
from pathlib import Path
import re
import resource
import shutil
import signal
import subprocess
import tempfile
import time

WEIGHTS = {
 'test_exec_ls.cpp':1, 'test_exec_hello.cpp':1, 'test_exec_cat.cpp':1, 'test_exec_rm.cpp':1,
 'test_wait.cpp':3, 'test_sigaction.cpp':2, 'pipe1a.cpp':3, 'pipe3.cpp':3, 'pipe4.cpp':3,
 'partial_sum.cpp':5, 'pthreads_demo.cpp':3, 'pthreads_message_demo.cpp':2,
 'shared_resource_mutex.c':3, 'partial_sum_threads.cpp':4}

def limit_process():
    resource.setrlimit(resource.RLIMIT_CPU,(15,15))
    resource.setrlimit(resource.RLIMIT_AS,(512*1024*1024,512*1024*1024))
    resource.setrlimit(resource.RLIMIT_FSIZE,(1024*1024,1024*1024))
    resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    resource.setrlimit(resource.RLIMIT_NOFILE,(64,64))
    resource.setrlimit(resource.RLIMIT_NPROC,(128,128))

def run(command,work,timeout=10,signal_test=False):
    start=time.monotonic(); timed_out=False; signal_ok=True
    out_path=work/'_stdout.log'; err_path=work/'_stderr.log'
    with out_path.open('wb') as out,err_path.open('wb') as err:
        proc=subprocess.Popen([str(x) for x in command],cwd=str(work),stdin=subprocess.DEVNULL,
          stdout=out,stderr=err,start_new_session=True,preexec_fn=limit_process,
          env={'PATH':'/usr/bin:/bin','LC_ALL':'C','HOME':str(work)})
        try:
            if signal_test:
                def wait_for(pattern):
                    end=time.monotonic()+3
                    while time.monotonic()<end and proc.poll() is None:
                        if re.search(pattern,out_path.read_text(errors='replace'),re.I):return True
                        time.sleep(.02)
                    return False
                signal_ok=wait_for('Ready')
                if signal_ok:
                    os.killpg(proc.pid,signal.SIGINT)
                    signal_ok=wait_for('Received.*SIGINT|Received.*signal.*2')
                if signal_ok:
                    os.killpg(proc.pid,signal.SIGQUIT)
            proc.wait(timeout=timeout)
        except subprocess.TimeoutExpired:timed_out=True
        finally:
            try:os.killpg(proc.pid,signal.SIGKILL)
            except ProcessLookupError:pass
            proc.wait()
    return {'return_code':proc.returncode,'timeout':timed_out,'signal_sequence_ok':signal_ok,
      'seconds':round(time.monotonic()-start,3),
      'stdout':out_path.read_bytes()[:65536].decode(errors='replace'),
      'stderr':err_path.read_bytes()[:65536].decode(errors='replace')}

def compile_file(source,work,variant=None):
    text=source.read_text(errors='replace')
    if variant is not None:
        constant,value=variant
        text,n=re.subn(r'(^\s*#\s*define\s+'+constant+r'\s+)\d+',lambda m:m.group(1)+str(value),text,flags=re.M)
        if n!=1:
            return None,{'return_code':1,'timeout':False,'seconds':0,'stdout':'','stderr':'Keep the supplied #define '+constant+' so the documented size checks can run.'}
    target=work/source.name; target.write_text(text)
    compiler='gcc' if source.suffix=='.c' else 'g++'
    standard='-std=c11' if source.suffix=='.c' else '-std=c++11'
    executable=work/(source.stem+'_run')
    result=run([compiler,standard,'-O0','-Wall','-Wextra','-pthread',target,'-o',executable],work,timeout=20)
    return executable,result

def cases(filename):
    if filename=='pipe1a.cpp':return [(['cat','pipe1.cpp'],None),(['printf','CSUSB'],None),(['cat','short_text.txt'],None)]
    if filename=='pipe3.cpp':return [(['10','5'],None),(['2','7'],None),(['-3','8'],None),(['0','0'],None)]
    if filename=='pipe4.cpp':return [(['Bilal'],None),(['Group One'],None),(['CSE'],None)]
    if filename in ('partial_sum.cpp','partial_sum_threads.cpp'):return [([],('SIZE',n)) for n in (20,48,100)]
    if filename=='shared_resource_mutex.c':return [([],('iterations',1000000)) for _ in range(5)]
    if filename=='pthreads_message_demo.cpp':return [([],None) for _ in range(5)]
    return [([],None)]

def number_after(label,text,expected):
    match=re.search(label+r'\s*[:=]?\s*(-?\d+(?:\.\d+)?)',text,re.I)
    return bool(match and math.isclose(float(match.group(1)),expected,rel_tol=1e-7,abs_tol=1e-7))

def expected(filename,args,variant,result,work):
    text=result['stdout']
    if filename=='test_exec_ls.cpp':return 'keep_this_file.txt' in text and re.search(r'^[-d][rwx-]{9}',text,re.M) is not None
    if filename=='test_exec_hello.cpp':return 'Hello from CSUSB CSE 4600 Section 01 Fall 2026' in text
    if filename=='test_exec_cat.cpp':return (work/filename).read_text().strip() in text
    if filename=='test_exec_rm.cpp':return not (work/'test.txt').exists() and (work/'keep_this_file.txt').exists()
    if filename=='test_wait.cpp':
        match=re.search(r'Grandchild PID\s*[:=]\s*(\d+).*?Parent PID\s*[:=]\s*(\d+).*?Grandparent PID\s*[:=]\s*(\d+)',text,re.I|re.S)
        return bool(match and len(set(match.groups()))==3 and all(int(x)>0 for x in match.groups())
                    and text.count('This is the child')==5 and text.count('This is the parent')==3
                    and re.search(r'child exited with code\s+9\b',text,re.I))
    if filename=='test_sigaction.cpp':return result['signal_sequence_ok'] and 'Program finished.' in text
    if filename=='pipe1a.cpp':
        want='CSUSB' if args[0]=='printf' else (work/args[1]).read_text()
        return want.strip() in text
    if filename=='pipe3.cpp':return number_after('The sum of the two numbers is',text,int(args[0])+int(args[1]))
    if filename=='pipe4.cpp':return args[0]+'! Welcome to CSE 4600 Operating Systems course' in text
    if filename in ('partial_sum.cpp','partial_sum_threads.cpp'):
        n=variant[1];return number_after('Total sum',text,n*(n+1)//2)
    if filename=='pthreads_demo.cpp':return all(number_after(label+r' of the numbers from 1 to 10',text,value) for label,value in [('Sum',55),('Product',3628800),('Average',5.5)])
    if filename=='pthreads_message_demo.cpp':return 'You are in thread function!' in text and 'You are in the main function!' not in text
    if filename=='shared_resource_mutex.c':return number_after('Shared resource value',text,0)
    return False

def check(source,out,only=None):
    source=Path(source).resolve(); out=Path(out).resolve();out.mkdir(parents=True,exist_ok=True)
    records=[]; compiled={}
    for filename in WEIGHTS:
        if only and filename!=only:continue
        for index,(args,variant) in enumerate(cases(filename),1):
            with tempfile.TemporaryDirectory(prefix='cse4600_simple_') as temp:
                work=Path(temp)
                # Runtime fixtures are disposable; rm never targets user coursework.
                for name in ('pipe1.cpp',):
                    if (source/name).is_file():shutil.copy2(source/name,work/name)
                (work/'short_text.txt').write_text('CSUSB pipe example\n')
                (work/'keep_this_file.txt').write_text('Do not remove this fixture\n')
                (work/'test.txt').write_text('Disposable removal exercise\n')
                src=source/filename
                if not src.is_file():
                    exe=None;build={'return_code':1,'stderr':'Missing file: '+filename,'stdout':'','timeout':False}
                else:exe,build=compile_file(src,work,variant)
                compiled[filename+':'+str(variant)]=build
                if build['return_code']!=0:
                    result={'return_code':None,'timeout':False,'stdout':'','stderr':'Did not compile','seconds':0}; output_ok=False
                else:
                    result=run([exe]+args,work,timeout=10,signal_test=filename=='test_sigaction.cpp')
                    output_ok=expected(filename,args,variant,result,work)
                clean=build['return_code']==0 and result['return_code']==0 and not result['timeout']
                passed=bool(clean and output_ok)
                record={'file':filename,'case':index,'arguments':args,'variant':variant,'output_correct':bool(output_ok),'passed':passed,**result}
                records.append(record)
                print(('PASS' if passed else 'FAIL')+' '+filename+' case '+str(index),flush=True)
    tests=[]; correctness=runtime=0.; cumulative=0.
    for filename,weight in WEIGHTS.items():
        runtime_max=(round((cumulative+weight)*2500/35)-round(cumulative*2500/35))/100
        cumulative+=weight
        selected=[r for r in records if r['file']==filename]
        if not selected:continue
        correct_fraction=sum(r['output_correct'] and not r['timeout'] for r in selected)/len(selected)
        successful_fraction=sum(r['passed'] for r in selected)/len(selected)
        c=round(weight*correct_fraction,2);t=round(runtime_max*successful_fraction,2)
        correctness+=c;runtime+=t
        tests.extend([
          {'name':filename+' correctness','score':c,'max_score':weight,'output':'Required results in %d/%d runs'%(sum(r['output_correct'] and not r['timeout'] for r in selected),len(selected))},
          {'name':filename+' runtime','score':t,'max_score':runtime_max,'output':'Correct result and clean completion in %d/%d runs'%(sum(r['passed'] for r in selected),len(selected))}])
    summary={'correctness':round(correctness,2),'runtime':round(runtime,2),'automatic_subtotal':round(correctness+runtime,2),
      'automatic_max':60,'readability_manual_max':15,'documentation_manual_max':25,'final_total':None,
      'passed':sum(r['passed'] for r in records),'checks':len(records),'partial_check':bool(only)}
    results={'score':summary['automatic_subtotal'],'output':'Automatic result only; readability 15 and documentation 25 still require instructor review. Output checks do not prove correct use of every OS mechanism.','tests':tests,'extra_data':summary}
    (out/'results.json').write_text(json.dumps(results,indent=2)+'\n')
    (out/'details.json').write_text(json.dumps({'summary':summary,'compilation':compiled,'runs':records},indent=2)+'\n')
    print('%d/%d checks passed. Results: %s'%(summary['passed'],summary['checks'],out),flush=True)
    return summary

def main():
    p=argparse.ArgumentParser(description='Optional checker for the supplied C/C++ fill-in exercises')
    p.add_argument('--source',default='.');p.add_argument('--out',default='check_results')
    p.add_argument('--only',choices=list(WEIGHTS));args=p.parse_args()
    summary=check(args.source,args.out,args.only)
    return 0 if summary['passed']==summary['checks'] else 1

if __name__=='__main__':raise SystemExit(main())
