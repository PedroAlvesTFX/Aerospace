<HTML> 
<HEAD> 
<!--[if lt IE 9]>
<script type="text/javascript" src="http://cdn.jsdelivr.net/excanvas/r3/excanvas.js"></script>
<![endif]--> 
<script type="text/javascript" src="http://code.jquery.com/jquery-1.9.1.min.js"></script> 
<script type="text/javascript" src="http://cdn.jsdelivr.net/jqplot/1.0.8/jquery.jqplot.min.js"></script> 
<script type="text/javascript" src="http://cdn.jsdelivr.net/jqplot/1.0.8/plugins/jqplot.dateAxisRenderer.min.js"></script> 
<link href="http://cdn.jsdelivr.net/jqplot/1.0.8/jquery.jqplot.min.css" rel="stylesheet" type="text/css" />
<script type="text/javascript">

$(document).ready(function(){    
    var t = 2000;    
    var x = (new Date()).getTime(); 
    // current time    
    var n = 20;    
    data = [];    

    data=[ [1,0], [2,10], [6,2], [9,3], [12,4] ];
    var options = {             
        axes: {                  
            xaxis: {                        
                numberTicks: 10,
                min : data[0][0],  
                max: data[data.length-1][0],
                tickOptions:{formatString:'%.1f'}                 
            },
            yaxis: {             
                min:0,              
                max: 15,             
                numberTicks: 10,
                tickOptions:{formatString:'%.1f'}                 
            }             
       },             
       seriesDefaults: {                  
           rendererOptions: { smooth: true}  }      
       };      
       var plot1 = $.jqplot ('myChart', [data],options); 

       function doUpdate() {             
            if(data.length > n-1){          
                 data.shift();       
            }       
            var y = Math.random();       
            var x = (new Date()).getTime();       
            data.push([x,y]);       
            if (plot1) {      
                 plot1.destroy();       
            }       
            plot1.series[0].data = data;        
            options.axes.xaxis.min = data[0][0];
            options.axes.xaxis.max = data[data.length-1][0];
            plot1 = $.jqplot ('myChart', [data],options);
            setTimeout(doUpdate, t);    
       } 
}); 
</script> 
<div id="myChart" style="height:300px; width:500px;"></div> 
<button>Start Updates</button> 
</BODY> 
</HTML>
<?php

?>