<?php
$g=`cat /sys/class/thermal/thermal_zone0/temp`/1000;
#echo "{'temp':$g}\n";
?>
{"temp":"<?php echo $g;?>","temp2":"<?php echo $g;?>","the":"dweets"}
