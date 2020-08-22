
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

        subprocess.call(["rm",outDir + "*.*"], stdout=subprocess.DEVNULL)
        
        self.index=0

    def Execute(self, testFeature):
        self.index += 1
        print("| Test case {}: {: <60}".format(self.index, testFeature.name), end= "")        

        args = []
        args.append(self.binName)
        
        for test in testFeature.scenario:
            test.input = self.inDir + test.input
            test.output = self.outDir + test.output
            test.expected = self.expectDir + test.expected
            args.append(test.cmd) 
            args.append(test.input)
            args.append(test.output)
            
        subprocess.call (args, stdout=subprocess.DEVNULL)
        
        result=True
        for test in testFeature.scenario:
            result=False
            if not os.path.exists(test.expected):
                print(" Failed! |\n", test.expected , " Not found")
            elif not os.path.exists(test.output):
                print(" Failed! |\n", test.output , " Not found")
            else:
                if filecmp.cmp(test.expected, test.output, shallow=False):
                        result=True
                else:
                        print ("Failed!!\nComparisson between {} and {} Unmatched".format(test.output, test.expected))
                        subprocess.call (['WinMergeU.exe', os.path.abspath(test.output), os.path.abspath(test.expected)], stdout=subprocess.DEVNULL)
            if result != True:
                print("Failed to run: ", args)                
                return False
                
        print ("{}".format("Aproved! |"))
        print ("+{:->84}".format('+'))

class TestScenario:
    def __init__(self, cmd, input, expected, output="evidence"):
        self.cmd = cmd
        self.input = input
        self.output = output + "_" + expected
        self.expected = expected

class TestFeature:
    def __init__(self, name, scenario):
        self.name = name
        self.scenario = scenario

print ("+{:->84}".format('+'))
print ("|{: ^83}|".format("STARTING AUTO TEST"))
print ("+{:->84}".format('+'))

#os._exit(0)


print ("Starting Server ...              ")
subprocess.Popen([str(sys.argv[1])], stdout=subprocess.DEVNULL)

at = AutoTest(str(sys.argv[2]), inDir="test/mock/", outDir="~build/x86/", expectDir="test/mock/")

testList = []

testList.append(TestFeature(name="Test case "       ,scenario=[TestScenario(cmd ="send"    , input="Test1.txt",  expected="Test1.txt") ] ))
testList.append(TestFeature(name="Test case "       ,scenario=[TestScenario(cmd ="send"    , input="test2.txt",  expected="test2.txt") ] ))
testList.append(TestFeature(name="Test case "       ,scenario=[TestScenario(cmd ="send"    , input="test3.txt",  expected="test3.txt") ] ))
testList.append(TestFeature(name="Test case "       ,scenario=[TestScenario(cmd ="send"    , input="test4.txt",  expected="test4.txt") ] ))

approved = True
for test in testList:
    sys.stdout.flush()
    if at.Execute(test) == False:
        approved = False
        break

sys.stdout.flush()

if(not approved):
    os._exit(1)
