<html>
<head>
<title> RPI Satelite </title>
</head>
<body>
<div style="text-align: right;float: right; margin: 0px 15px 15px 0px;">
<img src=logo-sateliteOS-small.png width=180>
</div>
<?php
function sqlite_open($location,$mode)
{
    $handle = new SQLite3($location);
    return $handle;
}
function sqlite_query($dbhandle,$query)
{
    $array['dbhandle'] = $dbhandle;
    $array['query'] = $query;
    $result = $dbhandle->query($query);
    return $result;
}
function sqlite_fetch_array(&$result,$type)
{
    #Get Columns
    $i = 0;
    while ($result->columnName($i))
    {
        $columns[ ] = $result->columnName($i);
        $i++;
    }
   
    $resx = $result->fetchArray(SQLITE3_ASSOC);
    return $resx;
}

$location="teste.db";
/*
$dbh = new PDO('sqlite3:teste.db');
        //loop foreach para exibirmos os valores
    foreach ($dbh->query($query) as $row) {
          print "<b>Nome: </b>" . $row["nome"];
          print "<b> Email: </b>" . $row["email"] . "\n<br />";
        }
*/

$handle = new SQLite3($location);
echo "<pre>";
$query = "select * from weather;";
$result = $handle->query($query);
while($rows = $result->fetcharray()){;
 echo $rows['temperature']."\n";
}

$query = "insert into weather (temperature) values ('".rand()."');";
$result = $handle->query($query);

$query = "select * from weather;";
$result = $handle->query($query);

while($rows = $result->fetcharray()){;
 echo $rows['temperature']."\n";
}



echo "---";
?>
Fim