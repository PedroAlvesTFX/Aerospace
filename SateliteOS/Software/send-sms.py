# SIMSMS1.py

import RPi.GPIO as GPIO
import serial
import time, sys
import datetime
from curses import ascii
from time import sleep

P_BUTTON = 24 # Button, adapt to your wiring

def setup():
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(P_BUTTON, GPIO.IN, GPIO.PUD_UP)


#print ('Parametro 1', sys.argv[1])
#print ('Parametro 2', sys.argv[2])
#print ('Parametro 3', sys.argv[3])

import base64
coded_string = '''Q5YACgA...'''

coded_num = sys.argv[1]
num = base64.b64decode(coded_num)
#print(num.decode())

coded_string = sys.argv[2]
mensagem = base64.b64decode(coded_string)
#print(mensagem.decode())

SERIAL_PORT = "/dev/serial1"   # Raspberry Pi 2
#SERIAL_PORT = "/dev/ttyS0"    # Raspberry Pi 3

ser = serial.Serial(SERIAL_PORT, baudrate = 115200, timeout = 5)
setup()
serialcmd='AT+CMGF=1\r'
ser.write(serialcmd.encode() ) # set to text mode
time.sleep(1)
reply = ser.read(ser.inWaiting()) # Clean buf
#print ( reply.decode())

#print ("Sendinging SMS...")
serialcmd='AT+CMGS="'+ num.decode() +'"\r'
ser.write(serialcmd.encode() ) # set to text mode
sleep(0.5)

serialcmd='Mensagem de teste do Python3\r\n'
#ser.write(serialcmd.encode() ) # set to text mode
ser.write(mensagem ) # set to text mode
sleep(0.5)
ser.write(ascii.ctrl('z').encode() ) # set to text mode

time.sleep(5)
reply = ser.read(ser.inWaiting()) # Clean buf

resp = ""
for line in reply.decode().split('\n'):
    if '>' not in line:
        resp = resp + line + '\n\r'

try:
    resp.index('+CMGS:')
except ValueError:
    print ('SMS para' + num.decode()+ ': \n' + resp + '\n\r')
#    print ('SMS para' + num.decode()+ ': Fail\n' + resp + '\n[' + reply.decode())
else:
    print ('SMS para' + num.decode()+ ':' + resp  )





#ser.write(msg + chr(26))
#print ("Sent SMS...")
