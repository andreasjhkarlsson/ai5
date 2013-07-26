import shutil, glob
import subprocess
import os
import time
import ntpath

print("Building compiler...")
os.chdir("../AI5Compiler")
subprocess.check_output("python build_exe.py build",shell=True)
print("Copying binaries...")
os.chdir("../integration_tests")
shutil.rmtree("build",True)
time.sleep(1) # Stupid windows doesn't delete tree right away...
shutil.copytree("../AI5Compiler/build\\exe.win32-3.3","build")
shutil.copytree("../AI5Compiler/ai5-include","build/ai5-include")
for f in glob.glob('../AI5Runtime/Release/*.dll'):
    shutil.copy(f, 'build')
shutil.copy('../AI5Runtime/Release/AI5Runtime.exe', 'build/ai5r.exe')

TESTS = map(lambda x:os.path.splitext(ntpath.basename(x))[0],glob.glob('tests/*.ai5'))

try:
    for test in TESTS:
        print("Running test:",test)
        subprocess.check_call("build/ai5.exe tests/"+test+".ai5 tests/"+test+".aic")
        subprocess.check_call("build/ai5r.exe tests/"+test+".aic")
    print("All tests passed!")
except:
    print("Test failed:",test)
