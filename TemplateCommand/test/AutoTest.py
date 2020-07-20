
from time import sleep
import re
from time import localtime, strftime
import csv
import subprocess
import filecmp
import os.path
import os
import sys

class AutoTest:
    def __init__(self, binName, inDir, outDir, expectDir):
        self.binName = binName
        self.inDir = inDir
        self.outDir = outDir
        self.expectDir = expectDir
        
        self.index=0

    def Execute(self, test):
        self.index += 1
        print ("\n------------------------------------------------------------------------------------")
        print("Starting test case {}: {}......".format(self.index, test.name), end= "")
        test.input = self.inDir + test.input 
        test.output = self.outDir + test.output
        test.expected = self.expectDir + test.expected
        
        subprocess.call ([self.binName, test.cmd, test.input , test.output], stdout=subprocess.DEVNULL)
        
        if not os.path.exists(test.expected):
            print("\n", test.expected , " Not found")
        elif not os.path.exists(test.output):
            print("\n", test.output , " Not found")
        else:
            if filecmp.cmp(test.expected, test.output, shallow=False):
                print ("Aproved!!!")
                return True
            print ("Failed!!\nComparisson between {} and {} Unmatched".format(test.output, test.expected))
            
        print("Failed to run: {} {} {} {}".format(self.binName, test.cmd , test.input, test.output))
        return False

class UnitTest:
    def __init__(self, name, cmd, input, output, expected):
        self.name = name
        self.cmd = cmd
        self.input = input
        self.output = output
        self.expected = expected
 

print ("------------------------------------------------------------------------------------")
print ("                        AUTO TEST STARTING            ")
print ("-------------------------------------------------------------------------------------")

print ("Starting Server ...              ")
subprocess.Popen([str(sys.argv[1])], stdout=subprocess.DEVNULL)

at = AutoTest(str(sys.argv[2]), inDir="test/mock/", outDir="~build/x86/debug/", expectDir="test/mock/expected/")

testList = []

testList.append(UnitTest(name="Command Response Test "            , cmd="T_Command1"              , input="cmd1.txt",     output="test1.txt",     expected="Ret_OK.txt"))
testList.append(UnitTest(name="Command Response Async Test "      , cmd="T_Command1Async"         , input="cmd1.txt",     output="test2.txt",     expected="Ret_OK.txt"))
testList.append(UnitTest(name="Command Response 2 Test "          , cmd="T_Command2"              , input="n",            output="test3.txt",     expected="cmd2.txt"))
testList.append(UnitTest(name="Event Test "                       , cmd="T_Event1"                , input="cmd1.txt",     output="test4.txt",     expected="cmd2.txt"))
                                                                                                                                                                                              
approved = True
for test in testList:
    sys.stdout.flush()
    if at.Execute(test) == False:
        approved = False
        break

sys.stdout.flush()

if(not approved):
    os._exit(1)
        
print ("\n------------------------------------------------------------------------------------")    


