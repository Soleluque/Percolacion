#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Apr 18 17:36:34 2019

@author: sol
"""

import matplotlib.pyplot as plt
import csv
import numpy as np


x=[] #dimension
y=[] #masa cluster percolante
fit=[]
fit_fn=[]

with open("masa",'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=" ")
    
    for row in plots:
        x.append(float(row[0]))
        y.append(float(row[1]))
        

        
plt.plot(np.log(x),np.log(y),"o")
fit.append(np.polyfit(np.log(x), np.log(y), 1))
fit_fn.append(np.poly1d(fit[0]))
plt.plot(np.log(x), fit_fn[0](np.log(x)))
plt.xlabel('Dimension')
plt.ylabel('Masa de cluster percolante')
plt.title('Masa del cluster percolante en funcion de la dimension de la red')
plt.legend()
plt.savefig('m en funcion de dim.png')
plt.show()

