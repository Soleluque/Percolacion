#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Apr 16 13:10:20 2019

@author: sol
"""

import matplotlib.pyplot as plt
import csv
import numpy as np
from scipy.stats import chisquare

ps=[]
colors=['ko','go','ro','bo']
colorsf=["--k", "--g", "--r", "--b"]
fit=[]
fit_fn=[]

x1=[]
x2=[]
x3=[]
x4=[]

x1f=[]
x2f=[]
x3f=[]
x4f=[]

   
y1=[]
y2=[]
y3=[]
y4=[]

y1f=[]
y2f=[]
y3f=[]
y4f=[]


xs=[x1,x2,x3,x4]
ys=[y1,y2,y3,y4]

xsf=[x1f,x2f,x3f,x4f]
ysf=[y1f,y2f,y3f,y4f]

#x1 es s de p1, x2 es s de p2, etc
#y1 es ns de p2, y2 es ns de p2, etc 

i=0

with open('ns64','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=" ")
    for i in range(0,len(xs)):
        for row in plots:
            if len(row)==0:
                i+=1
            elif len(row)==1:
                ps.append(str(float(row[0])))
            else:
                xs[i].append(float(row[0])) #s
                ys[i].append(float(row[1])) #ns
        
        


 
for i in range(len(xs)):
    for j in range(len(xs[i])):
        if ys[i][j] >= 1:
            xsf[i].append(xs[i][j])
            ysf[i].append(ys[i][j])

for i in range(0,len(xsf)):
    plt.plot(np.log(xsf[i]),np.log(ysf[i]),colors[i], label="Probabilidad= "+ps[i+1])
    #plt.loglog(x2,y2,'go', label='p=0.4922')
    #plt.loglog(x3,y3,'ro')
    #plt.loglog(x4,y4,'bo')
    fit.append(np.polyfit(np.log(xsf[i]), np.log(ysf[i]), 1))
    fit_fn.append(np.poly1d(fit[i]))
    #plt.plot(np.log(xs[i]),np.log(ys[i]), 'yo')
    plt.plot(np.log(xsf[i]), fit_fn[i](np.log(xsf[i])), colorsf[i])

    plt.xlabel('Tamaño de clusters (s)')
    plt.ylabel('Distribucion (ns)')
    plt.title('Distribucion de clusters en funcion de su tamaño')
    plt.savefig('ns en funcion de s para dim 4.png')
    plt.legend()
plt.show()    


#print("El chi cuadrado es: " +str(chisquare(ysf[0],fit_fn[0](np.log(xsf[0])))))


