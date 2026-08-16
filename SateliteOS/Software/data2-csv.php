<?php
header("Content-Type: text/csv");
$file=$_GET['file'];
if ($file == ""){
$file=`ls launch-file-* | sort -r | head -n1`;
}

$b=`cat ./$file`;
#$b=str_replace("{","[",$b);
#$b=str_replace("}","]",$b);
#$b="{\n".$b."}";
$json="[".$b."]";
#$json = '{"0":["1","2","3","4","5","6"],"1":["3","3","4","4","5","5"]}';
#$json = '{"data":[ ["1","2"],["3","4"],["5","6"],["7","2"],["8","4"]]}';
#$json = '{"data":[ ["1","2"],["3","4"],["5","6"],["7","2"],["8","5"]],"data2":[ ["1","4"],["3","7"],["5","3"],["7","2"],["8","4"]]}';
#$json = '[0,2,3,4,5]';
#$json = '{"a":1,"b":2,"c":3,"d":4,"e":5,"f":3}';
#echo $json;
#exit;
$array=json_decode($json);
#var_dump($array);
#exit;

echo "date,sec,ax_1000,ay_1000,az_1000,gx,gy,gz,height,pressure\n";
$virgula="";
$drift = $array[0]->{'time'};
for ($t=0;$t<count($array);$t++){
   echo "\"".gmdate('r',$array[0]->{'time'})."\",";
   echo ($array[$t]->{'time'}-$drift).",".($array[$t]->{'ax'}*1000).",".($array[$t]->{'ay'}*1000).",".($array[$t]->{'az'}*1000);
   echo ",".($array[$t]->{'gx'}).",".$array[$t]->{'gy'}.",".$array[$t]->{'gz'};
   echo ",".$array[$t]->{'height'}.",".$array[$t]->{'pressure'}."\n";
}

#var_dump(json_decode($b));
#var_dump(json_decode($json, true));
#echo "$b\n";
#var_dump(json_decode($json));
#var_dump(json_decode($json, true));
?>