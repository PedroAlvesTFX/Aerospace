"""
Released under the MIT License
Copyright 2015-2016 MrTijn/Tijndagamer
"""

from bmp280 import BMP280

#bmp = bmp280()
try:
    from smbus2 import SMBus
except ImportError:
    from smbus import SMBus

# Initialise the BMP280
bus = SMBus(0)
bmp = BMP280(i2c_dev=bus)



print("Temp:","{:.2f}".format(bmp.get_temp())," Celcius" )
print("Pressure:","{:.2f}".format(bmp.get_pressure()), " Pascal")
print("Altitude:","{:.2f}".format(bmp.get_altitude()) , " meter")
