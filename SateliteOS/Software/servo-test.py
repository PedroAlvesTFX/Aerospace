# coding: utf-8

import FaBo9Axis_MPU9250
import time
import sys
import os
from datetime import datetime
import math
from pyGPIO2.gpio import gpio, port
from time import sleep

def servo1(angulo):
    t =0.00023 + angulo*0.00001152
    print(t)
    n=0
    while n < 10:
        gpio.output(pinport, 1)
        sleep(t)
        gpio.output(pinport, 0)
        sleep(0.02-t)
        n +=1

gpio.init()
buttonport=16
pinlaunch=68
pinscrub=15
pinport = 9
gpio.setcfg(buttonport, 0)  #gpio4 as output
gpio.setcfg(pinport, 1)  #gpio4 as output
gpio.setcfg(pinlaunch, 1)  #gpio4 as output
gpio.setcfg(pinscrub, 1)  #gpio4 as output


mpu9250 = FaBo9Axis_MPU9250.MPU9250()
f = open("accel.txt","w")
#f.write("***** Coleta de dados de lançamento ******* \n")
#f.write("                 Acelerometro     Giroscopio   Magnetometro  \n")
#f.write(" <timestamp>  -   ax,ay,az,       gx,gy,gz,      mx,my,mz   \n" )
t = 0
t1 = 0
launch=0
parachute=0

while True:
  print("0")
  servo1(0)
  sleep(4)
  print("180")
  servo1(180)
  sleep(4)
  print("180")
  servo1(180)
  sleep(1)
  print("90")
  servo1(90)
  sleep(1)
