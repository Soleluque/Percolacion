#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Apr 16 10:25:44 2019

@author: sol
"""

import matplotlib.pyplot as plt
import csv

x1=[]
y1=[]
x2=[]
y2=[]

with open('desviacion.txt','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=" ")
    for row in plots:
        x1.append(float(row[0])) #desviacion
        y1.append(float(row[1])) #probabilidad critica
        x2.append(int(row[2])) #dimension de la red
        

plt.plot(x1,y1,'o', label='')
plt.xlabel('Desviación')
plt.ylabel('Probabilidad crítica')
plt.title('Probabilidad crítica en función de dispersión por dimensión')
plt.legend()
plt.savefig('pc en funcion de sigma.png')
plt.show()



plt.plot(x2,y1,'ro',     label='')
plt.xlabel('Dimensión de la red')
plt.ylabel('Probabilidad crítica')
plt.title('Probabilidad crítica en función de la dimensión de la red')
plt.legend()
plt.savefig('pc en funcion de l.png')
plt.show()



plt.plot(x2,x1,'go', label='')
plt.xlabel('Dimensión de la red')
plt.ylabel('Desviación')
plt.title('Desviación en función de la dimensión de la red')
plt.legend()
plt.savefig('sigma en funcion de l.png')
plt.show()
