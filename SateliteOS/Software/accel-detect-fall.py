# coding: utf-8

import FaBo9Axis_MPU9250
import time
import sys
import os
from datetime import datetime
import math
from pyGPIO2.gpio import gpio, port
from time import sleep
from bmp280 import BMP280
from subprocess import call
try:
    from smbus2 import SMBus
except ImportError:
    from smbus import SMBus


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
gpio.setcfg(buttonport, 0)  #BCM 16 as input
gpio.setcfg(pinport, 1)     #BCM 9  as output
gpio.setcfg(pinlaunch, 1)   #BCM 68 as output
gpio.setcfg(pinscrub, 1)    #BCM 15 as output


bus = SMBus(0)
bmp280 = BMP280(i2c_dev=bus)


mpu9250 = FaBo9Axis_MPU9250.MPU9250()
f = open("accel.txt","w")
t = 0
t1 = 0
launch=0
parachute=0

try:
    while True:
      try:
        accel = mpu9250.readAccel()   # Leitura Sensor MPU9250
        ax = accel['x']
        ay = accel['y']
        az = accel['z']
        gyro = mpu9250.readGyro()    # Leitura Sensor MPU9250
        gx = gyro['x']
        gy = gyro['y']
        gz = gyro['z']
        mag = mpu9250.readMagnet()   # Leitura Sensor MPU9250
        t = t + 1
        if (launch==1):
           t1 = t1 + 1
           if (t1>1):
              f.write(",")                                                                                                                                                                                                                                                                                                                   #"\"Temp\":\"{:05.2f}\",".format( temp )                                                   
           f.write("{\"time\":\"" + str(datetime.timestamp(datetime.now()))+"\",\"ax\":\"" + str(ax)+"\",\"ay\":\""+str(ay)+"\",\"az\":\""+str(az)+ "\",\"gx\":\""+str(gx)+"\",\"gy\":\""+str(gy)+"\",\"gz\":\""+str(gz) + "\",\"mgx\":\"" + str(mag['x'])+"\",\"mgy\":\""+str(mag['y'])+"\",\"mgz\":\""+str(mag['z'])+"\",\"temp2\":\"{:3.2f}\"".format(temperature)+",\"pressure\":\"{:3.2f}\"".format(pressure)+",\"height\":\"{:3.2f}\"".format(altura)+"}\n" )

        if ( (math.fabs(ax) +  math.fabs(ay) +  math.fabs(az) ) < 0.5   ): # Detecta quando todos os eixos experimentam baixa aceleração
           print("\n ******** ZERO G ********\n   *** Apogeu !!! ***\n" + str(ax)+","+str(ay)+","+str(az) + "\n")
           if (launch==1):
              servo1(180)
              parachute=1

        if ( (math.fabs(ax) +  math.fabs(ay) +  math.fabs(az) ) > 1 and parachute==1   ): # Detecta quando todos os eixos experimentam aterrizagem
           print("\n ******** Aterrizagem !!! ***\n" + str(ax)+","+str(ay)+","+str(az) + "\n")
           f.close()
           os.rename('./accel.txt', 'launch-file-'+datetime.now().strftime("%Y%m%d_%H%M")+'.txt')
#           os.rename('./accel.txt', 'accel'+str(datetime.timestamp(datetime.now()))+'.txt')
           call("nohup shutdown -h now", shell=True)
           
           quit()

        temp        = mpu9250.readTemperature()
        temperature = bmp280.get_temperature()
        pressure    = bmp280.get_pressure()
        altura      = 50952-math.log(pressure)*7356;

        if (parachute==1):   # Se o estado for paraquedas pisca luz branca e laranja
            gpio.output(pinlaunch,1)
            sleep(0.01)
            gpio.output(pinlaunch,0)
            sleep(0.01)
            gpio.output(pinscrub,1)
            sleep(0.01)
            gpio.output(pinscrub,0)
        else:
          if (launch==1):     # Se o estado for lancamento pisca luz branca
            gpio.output(pinlaunch,1)
            sleep(0.01)
            gpio.output(pinlaunch,0)

        if ( (t % 10) == 0):  # A cada 10 leituras (loop) de sensores exite uma ocorrencia 
           os.system("clear")
           print("Button:"+str(gpio.input(buttonport))) # Exibe status do botao
           fs = open("launch-status.txt", "r")
           status = fs.read(1)  # Le arquivo controle (para receber comandos da interface web)
           fs.close()
           print ("status:" + status + "\n")
           acionamento=gpio.input(buttonport)  # Le status do botao, 0 significa acionado
           if (status == "1" and launch==0):   # Se arquivo controle acionado simula botao pressionado para lancamento
              acionamento=0
           if (status == "0" and launch==1):   # Se estado for lancamento e arquivo controle for zero, vai para scrub
              acionamento=0
              gpio.output(pinscrub,1)
              sleep(0.01)
              gpio.output(pinscrub,0)
              sleep(0.1)
              gpio.output(pinscrub,1)
              sleep(0.01)
              gpio.output(pinscrub,0)
              sleep(0.1)
              f.close()
              t1=0
              f = open("accel.txt","w")

           if (acionamento==0 ):   # Processa comando botao ou arquivo controle
               if (launch==0):     # Entra em modo lancamento, isto é, registra leituras sensores no arquivo
                   launch=1
                   while (gpio.input(buttonport)==0):
                       gpio.output(pinlaunch,1)
                       sleep(0.01)
                       gpio.output(pinlaunch,0)
                       sleep(0.1)
                   fs = open("launch-status.txt", "w")
                   fs.write("1")
                   fs.close
                   gpio.output(pinlaunch,0)
                   servo1(0)
               else:         # Modifica estado para Scrub
                   launch=0
                   parachute=0
                   while (gpio.input(buttonport)==0):
                       gpio.output(pinscrub,1)
                       sleep(0.01)
                       gpio.output(pinscrub,0)
                       sleep(0.1)
                   fs = open("launch-status.txt", "w")
                   fs.write("0")
                   fs.close
                   gpio.output(pinscrub,0)
           print("****  T=" + str(t) + " - " + str(t%10) + " ****\n" )
           #   Exibe leitura sensores na tela
           print("{")
           print("\"ax\":\"" , ( ax ),"\",")
           print("\"ay\":\"" , ( ay ),"\",")
           print("\"az\":\"" , ( az ),"\",")
           print("\"gx\":\"" , ( gx ),"\",")
           print("\"gy\":\"" , ( gy ),"\",")
           print("\"gz\":\"" , ( gz ),"\",")

           print("\"mx\":\"" , ( mag['x'] ),"\",")
           print("\"my\":\"" , ( mag['y'] ),"\",")
           print("\"mz\":\"" , ( mag['z'] ),"\",")
           print("\"Temp\":\"" , ( temp ),"\"")
           print("\"Temp2\":\"{:3.2f}\"".format(temperature))
           print("\"Pressure\":{:3.2f}\"".format(pressure))
           print("\"Altura\":{:3.2f}\"".format(altura))
           print("}")
        time.sleep(0.1)
#        quit()

#        time.sleep(0.5)
      except KeyboardInterrupt:
        sleep(1)
        print(".*")
except KeyboardInterrupt:
    f.close()
    sys.exit()

