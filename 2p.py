#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Apr 18 15:39:20 2019

@author: sol
"""

import matplotlib.pyplot as plt
import csv
import numpy as np


dim=["4","8","16","32","64","128"]

x1=[] #probabilidad
y1=[] #intensidad cluster percolante

x2=[] 
y2=[]

x3=[] 
y3=[]

x4=[] 
y4=[]

x5=[]
y5=[]

x6=[] 
y6=[]

x=[x1,x2,x3,x4,x5,x6]
y=[y1,y2,y3,y4,y5,y6]

i=0


 
with open("percolante4",'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=" ")
    
    for row in plots:
        if len(row)==0:
            pass
        else:
            x1.append(float(row[0]))
            y1.append(float(row[1]))

with open("percolante8",'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=" ")
    
    for row in plots:
        if len(row)==0:
            pass
        else:
            x2.append(float(row[0]))
            y2.append(float(row[1]))

with open("percolante16",'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=" ")
    
    for row in plots:
        if len(row)==0:
            pass
        else:
            x3.append(float(row[0]))
            y3.append(float(row[1]))

with open("percolante32",'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=" ")
    
    for row in plots:
        if len(row)==0:
            pass
        else:
            x4.append(float(row[0]))
            y4.append(float(row[1]))

with open("percolante64",'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=" ")
    
    for row in plots:
        if len(row)==0:
            pass
        else:
            x5.append(float(row[0]))
            y5.append(float(row[1]))
            
            
            
with open("percolante128",'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=" ")
    
    for row in plots:
        if len(row)==0:
            pass
        else:
            x6.append(float(row[0]))
            y6.append(float(row[1]))


for i in range(len(dim)):

    plt.plot(x[i],y[i],label="Dimension= "+dim[i])
    plt.xlabel('Probabilidad(p)')
    plt.ylabel('Intensidad del cluster percolante(F)')
    plt.title('Intensidad del cluster percolante en funcion de la probabilidad de ocupacion')
    plt.savefig('intensidad de cluster percolante en funcion de p.png')
    plt.legend()
    
plt.show()        