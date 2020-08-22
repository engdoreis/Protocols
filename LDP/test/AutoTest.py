
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
    def __init__(self, binName):
        self.binName = binName
        self.index=0

    def Execute(self, test):
        self.index += 1
        print ("\n------------------------------------------------------------------------------------")
        print("Starting test case {}: {}".format(self.index, test.name))                
        
        subprocess.call ([self.binName, test.cmd, test.input , test.output], stdout=subprocess.DEVNULL)
        #subprocess.call ([self.binName, test.cmd, test.input , test.expected])


        if os.path.exists(test.output):
            if filecmp.cmp(test.expected, test.output, shallow=False):
                print ("Passed!!")
                return True
            print ("Comparisson between {} and {} failed".format(test.output, test.expected))
            
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

at = AutoTest(str(sys.argv[2]))

testList = []

testList.append(UnitTest(name="Command Response Test "      , cmd="LDP_Command1"     , input="test/mock/test1.txt", output="~build/x86/debug/test1.txt", expected="test/mock/expected/test1.txt"))
testList.append(UnitTest(name="Command Response Async Test ", cmd="LDP_Command1Async", input="test/mock/test1.txt", output="~build/x86/debug/test1.txt", expected="test/mock/expected/test1.txt"))
testList.append(UnitTest(name="Event Test "                 , cmd="LDP_Event1"       , input="test/mock/test2.txt", output="~build/x86/debug/test2.txt", expected="test/mock/expected/test2.txt"))
testList.append(UnitTest(name="Status code OK Test "        , cmd="LDP_Command2"     , input="test/mock/test3.txt", output="~build/x86/debug/test3.txt", expected="test/mock/expected/test3.txt"))


for test in testList:
    if at.Execute(test) == False:
    	break
        
print ("\n------------------------------------------------------------------------------------")    
