#!/usr/bin/env python

import time
import pigpio
import sys

TX=17
RX=27


def bytes2hex(s):
   return ":".join("{:02x}".format(c) for c in s)

pi = pigpio.pi()

if not pi.connected:
   exit(0)

pi.set_mode(RX, pigpio.INPUT)
pi.set_mode(TX, pigpio.OUTPUT)
pi.bb_serial_read_open(RX, 9600, 8)

#f = open('/dev/fifo','w')
#sys.stdout = f
#print "DATA - SOFTWARE SERIAL:"
data1=""
try:
   while True:
      (count, data) = pi.bb_serial_read(RX)
      if count:
#         print(bytes2hex(data))
         print((data))
         data=data1 + data
#         loc=data.find("$GPGGA")
#         linha = data[loc:]
         loc2=linha.find("\n")
         data1=data
         if loc2 > 0:
#            linha = linha[:linha.find("\n")]
##            print ("local",loc,linha)
            print (linha)
            data1=""
            exit()
      time.sleep(0.1)

except:
   pi.bb_serial_read_close(RX)
   pi.stop()