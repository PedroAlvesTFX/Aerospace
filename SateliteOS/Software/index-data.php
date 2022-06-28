<?php

$root_space=`df | head -n2 | tail -n1 | awk '{ print $5 }' | cut -d% -f1|  tr '\n' ' '`;
$mem_ocupation=`free  | head -n2 | tail -n 1 | awk '{print (($2 - $4)/ $2 * 100) }'|  tr '\n' ' '`;
$e=`uptime`;
$f=`/opt/vc/bin/vcgencmd measure_temp | cut -f2 -d=  | cut -f1 -d\' |  tr '\n' ' '`;
$g=`cat /sys/class/thermal/thermal_zone0/temp|  tr '\n' ' '`/1000;
$h=`vcgencmd measure_volts core | cut -f2 -d=  | cut -f1 -dV |  tr '\n' ' '`;
$cpu_1=`cat /proc/loadavg | awk '{ print $1}' |  tr '\n' ' ' `*100;
#echo "<h2>RPI Zero W</h2><pre>$a\n$b\n$c\n$d\nUptime:$e\n
#Temperatura GPU:$f"."ºC\n
#Temperatura CPU:$g ºC\n";
#echo "Core:$h volts";
echo "{\"temperature CPU\":\"$g\",\"temperature GPU\":\"$f\",\"File System\":\"$root_space\",\"Memory Use\":\"$mem_ocupation\",\"CPU\":\"$cpu_1\"}";

?>