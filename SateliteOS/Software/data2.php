<?php

$file=`ls launch-file-* | sort -r`;
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

echo "{\"ax\":[\n";
$virgula="";
for ($t=0;$t<count($array);$t++){
   echo $virgula."[\"".$array[$t]->{'time'}."\",\"".($array[$t]->{'ax'})."\"]\n";
   $virgula=",";
}
echo "],\"ay\":[\n";
$virgula="";
for ($t=0;$t<count($array);$t++){
   echo $virgula."[\"".$array[$t]->{'time'}."\",\"".($array[$t]->{'ay'})."\"]\n";
   $virgula=",";
}

echo "],\"az\":[\n";
$virgula="";
for ($t=0;$t<count($array);$t++){
   echo $virgula."[\"".$array[$t]->{'time'}."\",\"".($array[$t]->{'az'})."\"]\n";
   $virgula=",";
}

echo "],\"high\":[\n";
$virgula="";
for ($t=0;$t<count($array);$t++){
   echo $virgula."[\"".$array[$t]->{'time'}."\",\"".($array[$t]->{'high'})."\"]\n";
   $virgula=",";
}

echo "],\"pressure\":[\n";
$virgula="";
for ($t=0;$t<count($array);$t++){
   echo $virgula."[\"".$array[$t]->{'time'}."\",\"".($array[$t]->{'pressure'})."\"]\n";
   $virgula=",";
}


echo "] }";

#var_dump(json_decode($b));
#var_dump(json_decode($json, true));
#echo "$b\n";
#var_dump(json_decode($json));
#var_dump(json_decode($json, true));
?>