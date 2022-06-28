import os, sys

if not os.getegid() == 0:
	sys.exit('start script as root')
	
from pyGPIO2.gpio import gpio, port
from time import sleep

gpio.init()
gpio.setcfg(port.GPIO11, 1)  #gpio4 as output
gpio.setcfg(port.GPIO28, 1)  #gpio4 as output
gpio.setcfg(port.GPIO29, 1)  #gpio4 as output
gpio.setcfg(port.GPIO4, 1)  #gpio4 as output
gpio.setcfg(port.GPIO21, 1)  #gpio4 as output

pinport = 9
gpio.setcfg(pinport, 1)  #gpio4 as output


t =0.00023
n=0
print(t)
while n < 10:
	    gpio.output(pinport, 1)
	    sleep(t)
	    gpio.output(pinport, 0)
	    sleep(0.02-t)
	    n +=1

sleep(0.1)
t =0.00255
n=0
print(t)
while n < 10:
	    gpio.output(pinport, 1)
	    sleep(t)
	    gpio.output(pinport, 0)
	    sleep(0.02-t)
	    n +=1
sleep(0.1)

#	    print(t)

sys.exit('finished ;-)')
