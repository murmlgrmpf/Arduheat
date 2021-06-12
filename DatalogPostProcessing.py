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
plotrooms = True
##############Select File################
Logfile = "Logs/19041901.CSV"
#Logfile = "//DISKSTATION/Installation/Windows Programs/_Programs Heating7Arduino/Logs/15042300.CSV"

with open(Logfile) as f:
    readdata = csv.reader(f)

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

times =times.tolist()[1:-1]
Header = header.tolist()[1:]



data= raw_data[:-1,1:]

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
    for n in range(0,16):
        plt.plot(time, data[:,2+n], label = Header[2+n])
    plt.legend()
    
    plt.figure()
    for n in range(0,16):
        plt.plot(time, data[:,18+n], label = Header[18+n])
    plt.legend()
    
    plt.figure()
    for n in range(0,16):
        plt.plot(time, data[:,35+n], label = Header[35+n])
    plt.legend()
    
    for n in range(0,16):
        plt.figure()
        plt.plot(time, data[:,2+n], label = Header[2+n])
        plt.plot(time, data[:,18+n], label = Header[18+n])
        plt.plot(time, data[:,35+n], label = Header[35+n])
        
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
plotIdax(ax1,'WTifromB')
plotIdax(ax1,'BT1')
plotIdax(ax1,'WTitoB')
plt.legend(); plt.gca().xaxis.grid(True)
ax2 = plt.subplot(2,1,2, sharex=ax1)
plotIdax(ax2,'WS')
plotIdax(ax2,'WG')
#plotIdax(ax2,'WP')
id1 = 'Wn'
e = data[:,Header.index(id1)]/20
ax2.plot(time, e, label = 'Wn = l/min / 20')
ax2.set_ylim(0,1.0)
plt.legend(); plt.gca().xaxis.grid(True)
#plotIdax(ax2,'Wn')

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
plotIdax(ax2,'Tssuht')
plt.legend(); plt.gca().xaxis.grid(True)

plt.figure().suptitle('Mixer')
ax1 = plt.subplot(2,1,1)
id1 = 'RTsHeating'
id2 = 'RTitoR'
id3 = 'RM'
id4 = 'RTitoSys'
e = np.subtract(data[:,Header.index(id1)], data[:,Header.index(id2)])
ax1.plot(time, e, label = 'e Mischer')
ax1.plot(time,data[:,Header.index(id3)], label = (id3))
plt.legend(); plt.gca().xaxis.grid(True)
ax2 = plt.subplot(2,1,2, sharex=ax1)
#plotIdax(ax2,'BncW')
#plotIdax(ax2,'BncH')
#plotIdax(ax2,'BuBurns')
plotIdax(ax2,'Rn')
plotIdax(ax2,'RTitoR')
plotIdax(ax2,'RTsHeating')
plotIdax(ax2,'RTitoSys')
plt.legend(); plt.gca().xaxis.grid(True)

plt.figure().suptitle('Transfer Station')
ax1 = plt.subplot(2,1,1)
plotIdax(ax1,'TsTtoTele')
plotIdax(ax1,'TsTfromTele')
plotIdax(ax1,'TsTOp')
plt.legend(); plt.gca().xaxis.grid(True)
ax2 = plt.subplot(2,1,2, sharex=ax1)
plotIdax(ax2,'Tssuht')
plotIdax(ax2,'TsV')
plotIdax(ax2,'TsPower')
plt.legend(); plt.gca().xaxis.grid(True)

plt.figure().suptitle('Boiler')
ax1 = plt.subplot(2,1,1)
plotIdax(ax1,'BTsc')
plotIdax(ax1,'BT0')
plotIdax(ax1,'BT1')
plotIdax(ax1,'BT2')
plotIdax(ax1,'BT3')
plotIdax(ax1,'BT4')
plotIdax(ax1,'TsTOp')
plotIdax(ax1,'STtoSys')
plt.legend(); plt.gca().xaxis.grid(True)
ax2 = plt.subplot(2,1,2, sharex=ax1)
plotIdax(ax2,'BP')
plotIdax(ax2,'Bdisc')
plotIdax(ax2,'Bc')
plt.legend()

plt.figure().suptitle('Solar')
ax1 = plt.subplot(2,1,1)
plotIdax(ax1,'STtoSys')
plotIdax(ax1,'STfromCol')
plotIdax(ax1,'Toutside')
plt.legend(); plt.gca().xaxis.grid(True)
ax2 = plt.subplot(2,1,2, sharex=ax1)
plotIdax(ax2,'SP')
plotIdax(ax2,'SV')
plotIdax(ax2,'Ssuht')
plotIdax(ax2,'Sprobing')
id1 = 'Sint'
e = data[:,Header.index(id1)]/1000
ax2.plot(time, e, label = 'Sint')
plt.legend(); plt.gca().xaxis.grid(True)