# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import numpy as np
import csv as csv
#import pandas as pd
import matplotlib.pyplot as plt
#import matplotlib.markers as markers

plt.close('all')

###############Select Plots##############
plotrooms = 0
##############Select File################

Logfile = "Logs/15040401.CSV"
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
    plt.plot(time, data[:,Header.index(id)],'--', label = id, marker= 'o', markersize=7, markevery=(10*plotId.i,60))
    plotId.i+=1
plotId.i = 0


def plotIdax(ax,id):
    ax.plot(time, data[:,Header.index(id)],'--', label = id, marker= 'o', markersize=7, markevery=(10*plotIdax.i,60))
    plotIdax.i+=1
plotIdax.i = 0

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
        plotId('RTitoR')
        plotId('RTitoSys')
        plt.legend()
        
OverrideOffsets = data[:data.shape[0]-3,17:33]

average = []
for column in OverrideOffsets.T:
    c = column.mean()
    average.append(20-c)


# Plot system
#plt.figure()
#plotId('Toutside')

# PID

plt.figure().suptitle('Warm water')
ax1 = plt.subplot(2,1,1)
plotIdax(ax1,'WTi')
plotIdax(ax1,'WTitoB')
plt.legend(); plt.gca().xaxis.grid(True)
ax2 = plt.subplot(2,1,2, sharex=ax1)
plotIdax(ax2,'WP')
#plotIdax(ax2,'WarmWaterNeed')

plt.figure().suptitle('Rooms Pump')
ax1 = plt.subplot(2,1,1)
id1 = 'RTsHeating'
id2 = 'RTitoSys'
id3 = 'RP'
e = np.subtract(data[:,Header.index(id1)], data[:,Header.index(id2)])-5
ax1.plot(time, e, label =  ('e', id3))
ax1.plot(time,data[:,Header.index(id3)], label = (id3))
plt.legend(); plt.gca().xaxis.grid(True)
ax2 = plt.subplot(2,1,2, sharex=ax1)
plotIdax(ax2,'BncW')
plotIdax(ax2,'Rn')
plotIdax(ax2,'BuBurns')
plt.legend(); plt.gca().xaxis.grid(True)

plt.figure().suptitle('Mixer')
ax1 = plt.subplot(2,1,1)
id1 = 'RTsHeating'
id2 = 'RTitoR'
id3 = 'RM'
e = np.subtract(data[:,Header.index(id1)], data[:,Header.index(id2)])
ax1.plot(time, e, label = 'e Mischer')
ax1.plot(time,data[:,Header.index(id3)], label = (id3))
plt.legend(); plt.gca().xaxis.grid(True)
ax2 = plt.subplot(2,1,2, sharex=ax1)
plotIdax(ax2,'Rn')
plotIdax(ax2,'BncW')
plotIdax(ax2,'BncH')
plotIdax(ax2,'BuBurns')
plotIdax(ax2,'RTitoR')
plt.legend(); plt.gca().xaxis.grid(True)

plt.figure().suptitle('Boiler Charge Pump')
ax1 = plt.subplot(2,1,1)
plotIdax(ax1,'BTsc')
plotIdax(ax1,'BT0')
plotIdax(ax1,'BuT')
plt.legend(); plt.gca().xaxis.grid(True)
ax2 = plt.subplot(2,1,2, sharex=ax1)
plotIdax(ax2,'BP')
plotIdax(ax2,'BncW')
plotIdax(ax2,'BncH')
plotIdax(ax2,'BuBurns')
plt.legend(); plt.gca().xaxis.grid(True)

plt.figure().suptitle('Boiler')
ax1 = plt.subplot(2,1,1)
plotIdax(ax1,'BTsc')
plotIdax(ax1,'BT0')
plotIdax(ax1,'BT1')
plotIdax(ax1,'BT2')
plotIdax(ax1,'BT3')
plotIdax(ax1,'BT4')
plt.legend(); plt.gca().xaxis.grid(True)
ax2 = plt.subplot(2,1,2, sharex=ax1)
plotIdax(ax2,'BP')
plotIdax(ax2,'Bdisc')
plotIdax(ax2,'Bc')
plt.legend()

plt.figure().suptitle('Solar')
ax1 = plt.subplot(2,1,1)
plotIdax(ax1,'STfromCol')
plotIdax(ax1,'STtoCol')
plotIdax(ax1,'STfromSys')
plotIdax(ax1,'STtoSys')
plt.legend(); plt.gca().xaxis.grid(True)
ax2 = plt.subplot(2,1,2, sharex=ax1)
plotIdax(ax2,'SP')
plotIdax(ax2,'SV')
plotIdax(ax2,'Ssuht')
plotIdax(ax2,'Sprobing')
plt.legend(); plt.gca().xaxis.grid(True)
