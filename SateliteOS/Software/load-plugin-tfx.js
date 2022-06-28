
    var html_TFX="[-]";
    function httpGet(theUrl)
    {
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.open( "GET", theUrl, false ); // false for synchronous request
	xmlHttp.send( null );
	return xmlHttp.responseText;
    }


    function httpGetAsync(theUrl, callback)
    {
     var xmlHttp = new XMLHttpRequest();
     xmlHttp.onreadystatechange = function() { 
        if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
            callback(xmlHttp.responseText);
     }
     xmlHttp.open("GET", theUrl, true); // true for asynchronous 
     xmlHttp.send(null);
    }

       function UpdateData(data1){
    //     alert(data1);
         html_TFX=data1;
         return data1;
       }


       var a = httpGetAsync("http://192.168.1.155/data3.php",UpdateData );
//     var a = httpGet("http://192.168.1.155/data3-html.php");
//       alert(a);

let url = 'http://192.168.1.155/data3.php';

const getJSON = async url => {
  const response = await fetch(url);
  if(!response.ok) // check if response worked (no 404 errors etc...)
    throw new Error(response.statusText);

  const data = response.json(); // get JSON from the response
  return data; // returns a promise, which resolves to this data value
}

console.log("Fetching data...");
getJSON(url).then(data => {
  console.log(data);
  console.log(data['file'][0]);
//  alert('data.file.0:'+data['file'][0]);
  html_TFX=data;
}).catch(error => {
  console.error(error);
});











