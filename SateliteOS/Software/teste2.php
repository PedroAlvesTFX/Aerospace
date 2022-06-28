<?php
include 'PhpSerial.php';

// Let's start the class
$serial = new PhpSerial;

// First we must specify the device. This works on both linux and windows (if
// your linux serial device is /dev/ttyS0 for COM1, etc)

$val=`stty -F /dev/ttyS0`;
echo "--$val--<pre>";

$serial->deviceSet("/dev/ttyS0");

// We can change the baud rate, parity, length, stop bits, flow control
$serial->confBaudRate(115200);
$serial->confParity("none");
$serial->confCharacterLength(8);
$serial->confStopBits(1);
$serial->confFlowControl("none");

// Then we need to open it
$serial->deviceOpen();

// To write into
echo "Send AT\n";
$serial->sendMessage("AT",1);
var_dump($serial->readPort());
$serial->sendMessage("AT+CMGF=1\n\r",1);
var_dump($serial->readPort());
$serial->sendMessage("AT+CMGL=\"ALL\"\n\r",2);
var_dump($serial->readPort());

// If you want to change the configuration, the device must be closed
$serial->deviceClose();