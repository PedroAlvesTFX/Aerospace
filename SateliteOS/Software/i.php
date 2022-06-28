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
    for(i=4; i<n; i++){         
        data.push([x - (n-1-i)*t,0]);      
    }
    data=[ ['21:57:01',0], ['21:57:03',1], ['21:57:07',2], ['21:57:30',3], ['21:57:51',4] ];
    var options = {             
        axes: {                  
            xaxis: {                        
                numberTicks: 4,
                renderer:$.jqplot.DateAxisRenderer,
                tickOptions:{formatString:'%H:%M:%S'}, 
                min : data[0][0],  
                max: data[data.length-1][0]
            },
            yaxis: {             
                min:0,              
                max: 5,             
                numberTicks: 6,
                tickOptions:{formatString:'%.1f'}                 
            }             
       },             
       seriesDefaults: {                  
           rendererOptions: { smooth: true}  }      
       };      
       var plot1 = $.jqplot ('myChart', [data],options); 
       $('button').click( function(){               
           doUpdate();             
           $(this).hide();      
       });    

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