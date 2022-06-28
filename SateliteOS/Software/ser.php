<form>
<table><tr><td>Número:</td><td><input type=text name=numero>(ex:+552198762-1211)<br></td></tr>
<tr><td>Mensagem:</td><td><textarea name=mensagem></textarea><br></td></tr>
<tr><td colspan=2><input type=submit></td></tr></table>
</form>

<?php

$num=@$_GET['numero'];
$msg=@$_GET['mensagem'];

$nume=base64_encode($num);
$msge=base64_encode($msg);

$cmd=`python3 send-sms.py $nume $msge`;

echo "Numero:$num<br>";
echo "Mensagem:$msg<br>";
echo "Status:$cmd";

?>