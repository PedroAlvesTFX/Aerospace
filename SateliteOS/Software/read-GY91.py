# coding: utf-8
## @package faboMPU9250
#  This is a library for the FaBo 9AXIS I2C Brick.
#
#  http://fabo.io/202.html
#
#  Released under APACHE LICENSE, VERSION 2.0
#
#  http://www.apache.org/licenses/
#
#  FaBo <info@fabo.io>

import FaBo9Axis_MPU9250
import time
import sys

mpu9250 = FaBo9Axis_MPU9250.MPU9250()

from bmp280 import BMP280

try:
    from smbus2 import SMBus
except ImportError:
    from smbus import SMBus

#import numpy
import math
# Initialise the BMP280
bus = SMBus(0)
bmp280 = BMP280(i2c_dev=bus)




try:
    while True:
        accel = mpu9250.readAccel()
        ax = accel['x']
        ay = accel['y']
        az = accel['z']
#        accel = mpu9250.readAccel()
#        ax = (accel['x'] + ax)/2
#        ay = (accel['y'] + ay)/2
#        az = (accel['z'] + az)/2
#        accel = mpu9250.readAccel()
#        ax = (accel['x'] + ax)/2
#        ay = (accel['y'] + ay)/2
#        az = (accel['z'] + az)/2
        gyro = mpu9250.readGyro()
        gx = gyro['x']
        gy = gyro['y']
        gz = gyro['z']
        mag = mpu9250.readMagnet()

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

        temp = mpu9250.readTemperature()
        print("\"TempMPU9250\":\"{:05.2f}\",".format( temp ))

        temperature = bmp280.get_temperature()
        pressure = bmp280.get_pressure()
        altura = 50952-math.log(pressure)*7356;
#        x = numpy.log(10)

#print(temperature, pressure)

        print('"TemperaureBMP280(C)":"{:05.2f}",\n"Pressao(hPa)":"{:05.2f}",\n"Altura(m)":"{:05.2f}"'.format(temperature, pressure,altura))
#	time.sleep(1)


        print("}")
        quit()

#        time.sleep(0.5)

except KeyboardInterrupt:
    sys.exit()
