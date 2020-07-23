
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

at = AutoTest(str(sys.argv[2]), inDir="test/mock/", outDir="~build/x86/", expectDir="test/mock/expected/")

testList = []

testList.append(UnitTest(name="Read Default Vehicle Status"       , cmd="DVP_ReadVehicleStatus"            , input="n"                          , output="test1.txt",          expected="VehicleStatus_1.txt"))
testList.append(UnitTest(name="Write new Vehicle Status"          , cmd="DVP_WriteVehicleStatus"           , input="VehicleStatus_1.txt"        , output="test2.txt",          expected="Ret_OK.txt"))
testList.append(UnitTest(name="Read new Vehicle Status"           , cmd="DVP_ReadVehicleStatus"            , input="n"                          , output="test3.txt",          expected="VehicleStatus_2.txt"))
testList.append(UnitTest(name="Write Vehicle Config"              , cmd="DVP_WriteVehicleConfig"           , input="VehicleConfig_1.txt"        , output="test4.txt",          expected="Ret_OK.txt"))
testList.append(UnitTest(name="Read Vehicle Config"               , cmd="DVP_ReadVehicleConfig"            , input="n"                          , output="test5.txt",          expected="VehicleConfig_1.txt"))
testList.append(UnitTest(name="Read Vehicle Info"                 , cmd="DVP_ReadVehicleInfo"              , input="n"                          , output="test6.txt",          expected="VehicleInfo_1.txt"))
testList.append(UnitTest(name="Read Battery Status"               , cmd="DVP_ReadBatteryStatus"            , input="n"                          , output="test7.txt",          expected="BatteryStatus_1.txt"))
testList.append(UnitTest(name="Read Battery Info"                 , cmd="DVP_ReadBatteryInfo"              , input="n"                          , output="test8.txt",          expected="BatteryInfo_1.txt"))
testList.append(UnitTest(name="Start Firmware Update"             , cmd="DVP_FirmwareUpdateStart"          , input="FUS_1.txt"                  , output="test9.txt",          expected="Ret_OK.txt"))
testList.append(UnitTest(name="Send Firmware Packet"              , cmd="DVP_FirmwareUpdateLoad"           , input="FUL_1.txt"                  , output="test10.txt",         expected="Ret_OK.txt"))
testList.append(UnitTest(name="Start Authentication"              , cmd="DVP_StartAuthentication"          , input=" "                          , output="test11.txt",         expected="AuthData.txt"))
testList.append(UnitTest(name="Authenticate"                      , cmd="DVP_Authenticate"                 , input="AuthData.txt"               , output="test12.txt",         expected="Ret_OK.txt"))
testList.append(UnitTest(name="UpdatePublicKey"                   , cmd="DVP_UpdatePublicKey"              , input="CSR.txt"                    , output="test13.txt",         expected="Ret_OK.txt"))


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
