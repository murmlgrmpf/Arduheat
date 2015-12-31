#!/usr/bin/python
"""
Copyright (c) 2013-2015 Alan Yorinks All rights reserved.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU  General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


This file demonstrates how to use some of the basic PyMata operations.
"""

import time
import sys
import signal
import json
import os.path
import csv

from bridgeSerial import bridgeSerial

class handleArduheat():
    ext = '.CSV'
    count = 0
    printHeader = True
    logging = False
    date = str()
    arduino = None
    
    def __init__(self, SerialName, Folder):
        self.folder = Folder
        self.ConfFile = self.folder+"SYSTEM.CFG"
        self.printHeader = True
        self.logging = False
        self.date = str()
        self.count = 0
        self.arduino = bridgeSerial(SerialName,self.processJsonString)
        
        self.arduino.open(True)
        self.arduino.start()
        
    def close(self):
        self.arduino.stop()
        self.arduino.close()
        
    def getLogName(self):
        return (self.folder+ time.strftime("%Y%m%d")[2:] + str(self.count).zfill(2) + self.ext)
        
    def readConf(self,objarray):
        with open(self.ConfFile+'_','r') as cfgFile:
            lines = cfgFile.readlines()
            for line in lines:
                try:
                    cfg = json.loads(line)
                    for obj in objarray:
                        if cfg.keys()==obj.keys():
                            print("Success!\n")
                            self.arduino.write(json.dumps(cfg,separators=(',', ':'))+ '\n')
                except:
                    print("Fail read from file!\n")
                    print(line+ '\n')
    
    def writeConf(self,objarray):
        #Clear and write new config file
        if os.path.isfile(self.ConfFile):
            os.remove(self.ConfFile)
        with open(self.ConfFile,'w') as cfgFile:
            for obj in objarray:
                cfgFile.write(json.dumps(obj,separators=(',', ':')))
                cfgFile.write('\n')
    
    def writeLog(self,objarray):
        #Create new logfile, if new day or if not logging
        if not (self.date == time.strftime("%d")):
            self.date = time.strftime("%d")
            self.count = 0
            while os.path.isfile(self.getLogName()):
                self.count += 1
            self.printHeader = True
        with open(self.getLogName(),'a') as csvFile:
            writedata = csv.writer(csvFile)
            Headers = ["Time"]
            Datas = [time.strftime("%Y/%m/%d %H:%M:%S")]
            for obj in objarray:
                for attribute, values in obj.items():
                    idx = 0
                    if isinstance(values, list):
                        for value in values:
                            Headers.append(attribute+str(idx))
                            Datas.append(value)
                            idx+=1
                    else:
                        Headers.append(attribute)
                        Datas.append(values)
            if self.printHeader:
                writedata.writerow(Headers)
                self.printHeader = False
            writedata.writerow(Datas)
    
    def processJsonString(self,jstr):
        print(jstr)
        try:
            var = json.loads(jstr)
            if 'writeLog' in var:
                self.writeLog(var['writeLog'])
            if 'writeConf' in var:
                self.writeConf(var['writeConf'])
            if 'readConf' in var:
                self.readConf(var['readConf'])
        except:
            print("Fail read from Arduino!")
            print(jstr+'\n')
                


def signal_handler(sig, frame):
    print('You pressed Ctrl+C')
    arduino.close()
    sys.exit(0)
signal.signal(signal.SIGINT, signal_handler)   

#json_string = '{"first_name": "Guido", "last_name":"Rossum"}'
json_string = '{"getConf": [{"RTypes": [4, 4, 4, 4, 0, 0, 0, 2, 2, 2, 1, 1, 3, 4, 4, 2], "RTs": [20.0, 20.0, 19.0, 22.0, 15.0], "SetType": 0}]}'
#json_string = '{"getConf":[{"Bcm":5.00}]}'
json_string += '\n'

arduHeat = handleArduheat('/dev/ttyATH0','/mnt/sda1/arduino/www/data/')
arduHeat.arduino.write(json_string)
#arduHeat.arduino.write(json_string)


time.sleep(10)
arduHeat.close()