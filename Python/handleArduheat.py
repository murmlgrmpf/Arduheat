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

import threading
import time
import sys
import signal
import json
import os.path
import csv

from datetime import datetime

from bridgeSerial import bridgeSerial

class handleArduheat(threading.Thread):
    ext = '.CSV'
    count = 0
    date = str()
    arduino = None
    arduinoAnswered = False
    conf = []
    data = []
    
    def __init__(self, SerialName, Folder):
        threading.Thread.__init__(self)
        self.daemon = False
        self.folder = Folder
        self.ConfFile = self.folder+"System.cfg"
        self.date = str()
        self.count = 0
        self.arduino = bridgeSerial(SerialName,self.processJsonString)
        
        self.arduino.open(True)
        self.arduino.start()
        
        self.syncConf()
        
        self.stop_event = threading.Event()
    
    def stop(self):
        self.stop_event.set()
    def is_stopped(self):
        return self.stop_event.is_set()
    
    def open_(self):
        self.arduino.open(True)
        self.arduino.start()
    
    def close(self):
        self.arduino.stop()
        self.arduino.close()
        
    def getLogName(self):
        return (self.folder+ time.strftime("%Y%m%d")[2:] + str(self.count).zfill(2) + self.ext)
    
    #Configuration file handling
    def syncConf(self):
        #Wait until arduino sends config
        while (not(self.conf)):
            #Trigger Arduino to send config
            self.arduinoAnswered = False
            self.arduino.write(b"{\"getConf\":[1]}\n")
            print("Pull Config.")
            time.sleep(5)
        print("Pull of Arduino config: success.")
        
        #Synchronize with config file
        self.readConf()
        self.writeConf()
    
    def readConf(self):
        if os.path.isfile(self.ConfFile):
            with open(self.ConfFile,'r') as cfgFile:
                lines = cfgFile.readlines()
                for line in lines:
                    try:
                        cfg = json.loads(line)
                        for obj in self.conf:
                            if cfg.keys()==obj.keys():
                                print("Success!\n")
                                self.arduinoAnswered = False
                                self.arduino.write(json.dumps(cfg,separators=(',', ':'))+ '\n')
                                time.sleep(1)
                                #while not(self.arduinoAnswered):
                                #todo: timeout instead of waiting forever for an answer
                                    
                    except:
                        print("Fail read from file!\n")
                        print(line+ '\n')
                    

    def writeConf(self):
        #Clear and write new config file
        if os.path.isfile(self.ConfFile):
            os.remove(self.ConfFile)
        with open(self.ConfFile,'w') as cfgFile:
            for obj in self.conf:
                cfgFile.write(json.dumps(obj,separators=(',', ':')))
                cfgFile.write('\n')
    
    # Communication with arduino
    def processJsonString(self,jstr):
        print("Raw Message from Arduino:")
        print(jstr)
        try:
            var = json.loads(jstr)
            if 'Log' in var:
                self.Log(var['Log'])
            if 'Conf' in var:
                self.Conf(var['Conf'])
            if 'Reset' in var:
                self.Reset()
            if 'Time' in var:
                self.Time()
        except:
            print("Failed to read json from Arduino!")
            # print(jstr+'\n')
    
    def Time(self):
        print("Update Time.")
        ts = time.time()
        utc_offset = (datetime.fromtimestamp(ts) - datetime.utcfromtimestamp(ts)).total_seconds()
        timestampLong = (int(ts) + int(utc_offset))
        timeString = bytes("{\"TimeRasp\":" + str(timestampLong) + "}\n",'utf-8')
        self.arduino.write(timeString)
        print(timeString)

    def Reset(self):
        print("Reset Config.")
        self.conf = []
        self.date = str() # reset log
        
    def Conf(self, objarray):
        #Update config
        self.conf = objarray
        self.arduinoAnswered = True
    
    def Log(self,objarray):
        #Update data
        self.data = objarray
        printHeader = False
        #Create new logfile, if new day or if not logging
        if not (self.date == time.strftime("%d")):
            self.date = time.strftime("%d")
            printHeader = True
            self.count = 0
            while os.path.isfile(self.getLogName()):
                self.count += 1
        #Write into logfile
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
            if printHeader:
                writedata.writerow(Headers)
            writedata.writerow(Datas)
    
    def run(self):
        while not self.is_stopped():
            if not(self.conf):
                self.syncConf()
            else:
                time.sleep(.1)
	

def signal_handler(sig, frame):
    print('You pressed Ctrl+C')
    arduHeat.close()
    sys.exit(0)
signal.signal(signal.SIGINT, signal_handler)   

#json_string = '{"first_name": "Guido", "last_name":"Rossum"}'
#json_string = '{"getConf": [{"RTypes": [4, 4, 4, 4, 0, 0, 0, 2, 2, 2, 1, 1, 3, 4, 4, 2], "RTs": [20.0, 20.0, 19.0, 22.0, 15.0], "SetType": 0}]}'
#json_string = '{"getConf":[{"Bcm":5.00}]}'
#json_string += '\n'

arduHeat = handleArduheat('/dev/ttyACM0','/media/heat/USB-STICK/www/data/')
arduHeat.start()
#arduHeat.arduino.write(json_string)
#arduHeat.arduino.write(json_string)

#while(True):
    #time.sleep(1)
    
#arduHeat.close()
