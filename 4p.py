#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Apr 23 15:24:50 2019

@author: sol
"""

import matplotlib.pyplot as plt
import csv
import numpy as np

name=[]

#fit=[]
#fit_fn=[]

for i in range(20,34):
    name.append("scaling_"+str(i))
    x=[] 
    y=[]     

    with open(name[i-20],'r') as csvfile:
        plots = csv.reader(csvfile, delimiter=" ")
    
        for row in plots:
            if len(row)==2:
                x.append(float(row[0]))
                y.append(float(row[1]))
        
    #xt=[]
    #yt=[]

    #for i in range(len(x)):
     #   if y[i]!=0:
      #      xt.append(x[i])
       #     yt.append(y[i])
    plt.plot(x,y,"o",label="s=" +str(i))
    
#fit.append(np.polyfit(np.log(x), np.log(y), 1))
#fit_fn.append(np.poly1d(fit[0]))
#plt.plot(np.log(x), fit_fn[0](np.log(x)))
#plt.axis([0.1,0.7,0,0.0])
plt.xlabel('Probabilidad')
plt.ylabel('f(z)')
plt.title("Funcion f(z) en funcion de la probabilidad de ocupacion p")
plt.legend()
plt.savefig('f(z) en funcion de p.png')
plt.show()

