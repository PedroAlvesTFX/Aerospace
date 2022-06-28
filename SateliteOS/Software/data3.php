{"file":[<?php

$lista=`ls -c1 -t launch-fil* | head -n 10`;
$altitude=number_format( (50952-log($p1)*7356),1);
$array = explode("\n",$lista);

for($t=0;$t<count($array);$t++){
  if ($array[$t] <> "")
     echo "\"".$array[$t]."\"\n";
  if ($array[$t+1] <> "") echo ",";
}
?>
]}