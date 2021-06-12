# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import numpy as np
import csv as csv
#import pandas as pd
import matplotlib.pyplot as plt

###############Select Plots##############
plotrooms = 1
##############Select File################

#Logfile = "Log141226.CSV"
Logfile = "150103.CSV"
#Logfile = "RoomsOffsetCalibration.CSV"

readdata = csv.reader(open(Logfile))

#########################################
plt.close("all")

def num(s):
    try:
        return int(s)
    except ValueError:
        try:
            return float(s)
        except:
            return(s)

def plotId(id):
    plt.plot(time, data[:,Header.index(id)], label = id)


times = np.genfromtxt(Logfile, delimiter=',', usecols=0, dtype=str)
header = np.genfromtxt(Logfile, delimiter=',', dtype=str)[0,:]
raw_data = np.genfromtxt(Logfile, delimiter=',',skip_header=1)

times =times. tolist()[1:]
Header = header.tolist()[1:]

data= raw_data[:,1:]

itime=[]
days = 0
lasthh=0
for row in times:
    [date, istrtime] = row.split()
    [hh, mm, ss] = istrtime.split(':')
    hh= num(hh)
    if hh<lasthh:
        days = days+1
    lasthh = hh
    hh= hh+24*days
    mm =num(mm)
    ss= num(ss)
    itime.append(hh+mm/60 +ss/3600)

time = np.array(itime)

# Plot Rooms
if plotrooms:
    plt.figure()
    for n in range(1,17):
        plt.plot(time, data[:,n], label = Header[n])
    plt.legend()
    
    plt.figure()
    for n in range(1,17):
        plt.plot(time, data[:,16+n], label = Header[16+n])
    plt.legend()
    
    for n in range(1,17):
        plt.figure()
        plt.plot(time, data[:,n], label = Header[n])
        plt.plot(time, data[:,16+n], label = Header[16+n])
        plotId('IsTempHeatingLead')
        plotId('IsTempHeatingReturn')
        plt.legend()
        
OverrideOffsets = data[:data.shape[0]-3,17:33]

average = []
for column in OverrideOffsets.T:
    c = column.mean()
    average.append(20-c)


# Plot system
#plt.figure()
#plotId('TempOutside')

plt.figure()
plotId('SpHeating')

# PID
plt.figure()
#plotId('IsTempHeatingLead')
#plotId('IsTempHeatingReturn')
id1 = 'IsTempHeatingLead'
id2 = 'IsTempHeatingReturn'
id3 = 'PumpHeating'
e = np.subtract(data[:,Header.index(id1)], data[:,Header.index(id2)])-5
plt.plot(time, e, label =  ('e', id3))
plt.plot(time,data[:,Header.index(id3)], label = Header.index(id3))

plt.figure()
id1 = 'IsTempHeatingLead'
id2 = 'SpHeating'
id3 = 'PumpHeating'
e = np.subtract(data[:,Header.index(id1)], data[:,Header.index(id2)])
plt.plot(time, e, label = 'e Mischer')
#
#plotId('IsTempHeatingLead')
#plotId('SpHeating')

