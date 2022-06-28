  <script>
    function httpGet(theUrl)
    {
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.open( "GET", theUrl, false ); // false for synchronous request
	xmlHttp.send( null );
	return xmlHttp.responseText;
    }

    function changeFile(){
       var a = httpGet("http://192.168.1.155/data3.php");
       alert(a);  
       document.getElementsByName("tfx-txta").value=a;
    }
  </script>
    <form id="tfx-form">
        Campain:<select id="tfx-select" onChange="changeFile()"><option>1</option><option>2</option></select>
        <textarea id="tfx-txta"" name="tfx-txta">teste</textarea>
    </form>