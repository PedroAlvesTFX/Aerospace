#include <SFE_BMP180.h>
//#include <EEPROM.h>
#include <ESP_EEPROM.h>

#include <Wire.h>
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "altimetro"
#define APPSK  "bar12345"
#endif

typedef struct  {
  char    date[25];       // 25 bytes
  long    startPressure; // 4 bytes
  float   startAltitude; // 4 bytes
  long    MinPressure;   // 4 bytes
  float   MaxAltitude;   // 4 bytes
  float   temperature;   // 4 bytes

  char    date1[25];       // 25 bytes
  long    startPressure1; // 4 bytes
  float   startAltitude1; // 4 bytes
  long    MinPressure1;   // 4 bytes
  float   MaxAltitude1;   // 4 bytes
  float   temperature1;   // 4 bytes

  char    date2[25];       // 25 bytes
  long    startPressure2; // 4 bytes
  float   startAltitude2; // 4 bytes
  long    MinPressure2;   // 4 bytes
  float   MaxAltitude2;   // 4 bytes
  float   temperature2;   // 4 bytes

  boolean relay0_servo1_1 ; // 1 byte
  boolean relay0_servo1_2 ; // 1 byte
  byte    ang_servo1;       // 1 byte
  byte    ang_servo2;       // 1 byte

  int     OverShootReference; // 2 bytes
  int     UnderShootApogee;   // 2 bytes

  int     track;          // 2 bytes
} config_struct;

const char *ssid = APSSID;      /* Set these to your desired credentials. */
const char *password = APPSK;

//***** Image to Base64 converter ******* https://www.base64-image.de/
const String pre_logo = "<html><head><meta http-equiv=refresh content='15,/'></head><body><div style='width:18%;margin:0;padding:0;right:10;position:absolute;'> \n" ;
const String logo     = "<img src='data:image/png;base64,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WE1MOmNvbS5hZG9iZS54bXAAAAAAADw/eHBhY2tldCBiZWdpbj0i77u/IiBpZD0iVzVNME1wQ2VoaUh6cmVTek5UY3prYzlkIj8+Cjx4OnhtcG1ldGEgeG1sbnM6eD0iYWRvYmU6bnM6bWV0YS8iIHg6eG1wdGs9IlhNUCBDb3JlIDQuNC4wLUV4aXYyIj4KIDxyZGY6UkRGIHhtbG5zOnJkZj0iaHR0cDovL3d3dy53My5vcmcvMTk5OS8wMi8yMi1yZGYtc3ludGF4LW5zIyI+CiAgPHJkZjpEZXNjcmlwdGlvbiByZGY6YWJvdXQ9IiIKICAgIHhtbG5zOmlwdGNFeHQ9Imh0dHA6Ly9pcHRjLm9yZy9zdGQvSXB0YzR4bXBFeHQvMjAwOC0wMi0yOS8iCiAgICB4bWxuczp4bXBNTT0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL21tLyIKICAgIHhtbG5zOnN0RXZ0PSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvc1R5cGUvUmVzb3VyY2VFdmVudCMiCiAgICB4bWxuczpwbHVzPSJodHRwOi8vbnMudXNlcGx1cy5vcmcvbGRmL3htcC8xLjAvIgogICAgeG1sbnM6R0lNUD0iaHR0cDovL3d3dy5naW1wLm9yZy94bXAvIgogICAgeG1sbnM6ZGM9Imh0dHA6Ly9wdXJsLm9yZy9kYy9lbGVtZW50cy8xLjEvIgogICAgeG1sbnM6dGlmZj0iaHR0cDovL25zLmFkb2JlLmNvbS90aWZmLzEuMC8iCiAgICB4bWxuczp4bXA9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC8iCiAgIHhtcE1NOkRvY3VtZW50SUQ9ImdpbXA6ZG9jaWQ6Z2ltcDpjZTA4OTM4Zi05YzhlLTQ3YWQtYTA0Ny04YWYyMTc2YWE0OWIiCiAgIHhtcE1NOkluc3RhbmNlSUQ9InhtcC5paWQ6MjI3MGJhODYtNTQ3MS00OTBlLTlkMGMtM2IxM2ViYjdmMDRmIgogICB4bXBNTTpPcmlnaW5hbERvY3VtZW50SUQ9InhtcC5kaWQ6MGZiZGQ3MGMtMDU1Mi00MDVlLTkxODQtYzViYjRjM2YyOWVlIgogICBHSU1QOkFQST0iMi4wIgogICBHSU1QOlBsYXRmb3JtPSJXaW5kb3dzIgogICBHSU1QOlRpbWVTdGFtcD0iMTY2NTUwNjIyNDMyMTI1NSIKICAgR0lNUDpWZXJzaW9uPSIyLjEwLjIyIgogICBkYzpGb3JtYXQ9ImltYWdlL3BuZyIKICAgdGlmZjpPcmllbnRhdGlvbj0iMSIKICAgeG1wOkNyZWF0b3JUb29sPSJHSU1QIDIuMTAiPgogICA8aXB0Y0V4dDpMb2NhdGlvbkNyZWF0ZWQ+CiAgICA8cmRmOkJhZy8+CiAgIDwvaXB0Y0V4dDpMb2NhdGlvbkNyZWF0ZWQ+CiAgIDxpcHRjRXh0OkxvY2F0aW9uU2hvd24+CiAgICA8cmRmOkJhZy8+CiAgIDwvaXB0Y0V4dDpMb2NhdGlvblNob3duPgogICA8aXB0Y0V4dDpBcnR3b3JrT3JPYmplY3Q+CiAgICA8cmRmOkJhZy8+CiAgIDwvaXB0Y0V4dDpBcnR3b3JrT3JPYmplY3Q+CiAgIDxpcHRjRXh0OlJlZ2lzdHJ5SWQ+CiAgICA8cmRmOkJhZy8+CiAgIDwvaXB0Y0V4dDpSZWdpc3RyeUlkPgogICA8eG1wTU06SGlzdG9yeT4KICAgIDxyZGY6U2VxPgogICAgIDxyZGY6bGkKICAgICAgc3RFdnQ6YWN0aW9uPSJzYXZlZCIKICAgICAgc3RFdnQ6Y2hhbmdlZD0iLyIKICAgICAgc3RFdnQ6aW5zdGFuY2VJRD0ieG1wLmlpZDpmMDAzZGFlYi0yMTlkLTRjM2MtYmY2MC0wZTYzNmMyZmJhYzkiCiAgICAgIHN0RXZ0OnNvZnR3YXJlQWdlbnQ9IkdpbXAgMi4xMCAoV2luZG93cykiCiAgICAgIHN0RXZ0OndoZW49IjIwMjItMTAtMTFUMTM6Mzc6MDQiLz4KICAgIDwvcmRmOlNlcT4KICAgPC94bXBNTTpIaXN0b3J5PgogICA8cGx1czpJbWFnZVN1cHBsaWVyPgogICAgPHJkZjpTZXEvPgogICA8L3BsdXM6SW1hZ2VTdXBwbGllcj4KICAgPHBsdXM6SW1hZ2VDcmVhdG9yPgogICAgPHJkZjpTZXEvPgogICA8L3BsdXM6SW1hZ2VDcmVhdG9yPgogICA8cGx1czpDb3B5cmlnaHRPd25lcj4KICAgIDxyZGY6U2VxLz4KICAgPC9wbHVzOkNvcHlyaWdodE93bmVyPgogICA8cGx1czpMaWNlbnNvcj4KICAgIDxyZGY6U2VxLz4KICAgPC9wbHVzOkxpY2Vuc29yPgogIDwvcmRmOkRlc2NyaXB0aW9uPgogPC9yZGY6UkRGPgo8L3g6eG1wbWV0YT4KICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIAogICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgCiAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAKICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIAogICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgCiAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAKICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIAogICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgCiAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAKICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIAogICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgCiAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAKICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIAogICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgCiAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAKICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIAogICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgCiAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAKICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIAogICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgCiAgICAgICAgICAgICAgICAgICAgICAgICAgIAo8P3hwYWNrZXQgZW5kPSJ3Ij8+prVYvwAAAAZiS0dEAAAAAAAA+UO7fwAAAAlwSFlzAABM5QAATOUBdc7wlQAAAAd0SU1FB+YKCxAlBGLAehEAAAO7SURBVGje3ZpfaFNXGMB/59x7k5iSWYNaUt2oFbsnFXRYuqbJHLg97WFCYXOgbmMvGwjuYSI+uzL881bng/qyl6E+7G2wiqvaKWPQMta6QTZkimIb1z/UJjFJ7/Hh3rpZ0ubeXJN7uy8cbpKT+3F+3O/f+U7Au+wA/gF+AVaxguUIoOyxfdHcTuA88MZKAPkEGAK+qjA3aAP+We9F6B7vTwBJeyyWJqDZfn8z6CAx4PcK328AzgAfAFPAg5VgWhcqfHcaeKWRi5B10PmSPe42EsSLabUAHwFp4D4QAabtuT+APcAV29nrLmLR5y3A7ir3GMCbwDv79umlDw/GoqtXSwXMmwqVmzOzIyNP1h7/MmdMTpIFhoGHQBEoAzM2eKmC7jJwCZj1ApIGvnea1Hp79dzevdGwEGiiwvzUlFnq759Tv42qkMs1fQe86+Xp3P5PYvNzlL1AtAYEQi0Rzh1HrZ4AhfNrXkDSAQIZ/D/4hwI21hp+1+o6EzMzrSXDEKGFCCSENRol9+6Vc22bxrNAW60JMfn54XA+EhZRP+0pkykK4LqXEqWn6/WI744xPFyYr9XRF0BSr3aEyj5zTJ88lccryM7WVj3vJ8Xo6JNcNsuslw2YDoz19U02xeNyBMvBRcgQYU0XhibRpBSalGhSogtphWsBQggh7aBgvTxU4Ge/fpwAPvZaNG4DDtWppHciys4d37zI6rdaGbPGxe+LQCaIO8GfXSa2H4K4Q2wCXnOpeyiIIN01+NBPQQVxI/NBBempwZ8KQQPRgC6Xem80OhI56aJ0ffZpKHLsWNyRwgMHJyYGBsyhIIIk0+kILS1a9cRRVOMDA+b6RvuHU9NKdnQ4a4TcuVNSwBhWmzRQIALobmsznG0zbxfLXreq9QLZemC/3hyLOQtuPw7mfHF0Jz6SNhVcvjz7XIX3rAkq/r0IgdnfX1rf6IzuVC66rK/+8muh1WwmGfT84QSkHetEyo0MBhEkVYM+357Ics6eOnEiyuZ245mDKwWmqVDq+UOPu3+Xpr84ks/76SPLgaTffy9GIlE9+V+9mpuD/C0/o9JSppXo7JTtTiAAbt7Kl/w0q+VAunt7HTftZvr6ClE89KTqCZLatjXsSMGjR/PjhQIh4Fc/QZayndRbb0/RvmmajS8LOncZGIbV1JaaIL5GY906ia4LMpniBj/D7nLSjPujgKN+L1q+gP05eOii1xNku0sdeXuPHjiQxy51nMPjSWy9JC4lYwv2LwRK01C6bl0X+ca3WP948F2eAlO0dkH50yoIAAAAAElFTkSuQmCC'>";
const String html_header  = "</div><style>h1,h2 {padding:0;margin:0;}.height-class { font-weight:600; animation: blinker 1s linear infinite;}@keyframes blinker {50% {    opacity: 0;}}"
                " .form_menu {  margin:0 0 0 0 ;padding:0 0 0 0; display:inline-flex;height: 21;}</style> <center><h1 align=center>Rocket Altimeter</h1><br>\n";

const String html_script = "<script>  var today =  '' ;function update_date(){var objToday = new Date(), weekday = new Array('Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday'),\
  dayOfWeek = weekday[objToday.getDay()],    dayOfMonth = today + ( objToday.getDate() < 10) ? '0' + objToday.getDate() : objToday.getDate() , months = new Array('January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'),\
 curMonth = months[objToday.getMonth()],  monthNum  = objToday.getMonth() < 10 ? '0' + objToday.getMonth() : objToday.getMonth(),\
 curYear = objToday.getFullYear(), curHour = objToday.getHours() > 12 ? objToday.getHours() - 12 : (objToday.getHours() < 10 ? '0' + objToday.getHours() : objToday.getHours()),\
 curMinute = objToday.getMinutes() < 10 ? '0' + objToday.getMinutes() : objToday.getMinutes(), curSeconds = objToday.getSeconds() < 10 ? '0' + objToday.getSeconds() : objToday.getSeconds(),\
 curMeridiem = objToday.getHours() > 12 ? (curHour=curHour+12) : 'AM'; today =  curYear + '-' + monthNum + '-' + dayOfMonth + ' ' +curHour + ':' + curMinute + '.' +curSeconds;}\
   setTimeout(       function() {   update_date();   document.getElementById('date-div').innerText='Date:'+today;  document.getElementById('dateF').value=today;     },  1000);\
 </script> \n";

const String html_body = "<!--  ************* Procurar apogeu apos altitude crescer mais 30 metros e apos pico abaixar 20 metros ******* --> \n<div class=form_menu>"
                         "<form id=log action='/log.txt' method='POST'><input type=submit value='Download Log'></form>  &nbsp;\n <form id=conf action=/config>"
                         "<input type=submit value=Config></form> &nbsp;\n"
                         "<form id=reset method=post><input type=hidden name=dateF id=dateF value=''><input type=submit name=reset id=reset value='Set Reference'> &nbsp; "
                         "<input type=submit name=launch id=launch value='Launch'> &nbsp; </form></div><br><br> \n <br><div style='width:70%'>";
                         
String html_bodyA      = "Parachute deploy will occur only when altitude is over 20m above reference and became 10m smaller then apogee altitude!</div><br>\n";

String html_dynamic = "<table width=70% border=1><tr><td align=center colspan=2><h2>Current Data</h2></td></tr>\
<tr><td width=50%><div id='date-div'>Date</div></td><td>Temperature:26.40 *C</td></tr>\
<tr><td>Pressure:101765.00 Pa</td><td>Altitude:101.64 m</td></tr></table><br>\
<table width=70% border=1><tr><td align=center colspan=2><h2>Last Reference</h2></td></tr>\
<tr><td width=50%>Date:11/10/2022</td><td>Temperature = 25.0 *C</td></tr>\
<tr><td>Pressure Start : 101675 Pa </td><td> Pressure Min : 101506 Pa</td></tr>\
<tr><td>Altitude Start : 108.88 m   </td><td>Altitude Max : 122.89 m</td></tr>\
<tr><td align=center colspan=2><div class=height-class>Max. Height:14.01 m</div></td></tr></table>\n";

float OverShootAltitude  = 20;
float UnderShootAltitude = 10;

String t = "T=";
String p = "<br>P=";
String a = "<br>A=";
String b = "<br>H=";
String c = "<br>Max H=";
String h = "<html><meta http-equiv=refresh content='1,/'><h1>Altimetro BAR</h1>";
String l = "";
String resetTime = "2022/10/20 10:00h";
int    SensorFound = 1;
float  pressureM;
float  temperatureM;
float  altitudeM;
float  startAltitude = 0;
float  MaxAltitude = 0;
float  startPressure = 0;
float  MinPressure = 0;
int    timeToHandle = 0;
int    apogee = 0;
config_struct user_config;
ESP8266WebServer server(80);

/*  Just a little test message.  Go to http://192.168.4.1 in a web browser connected to this access point to see it.  */

const String JScript = ""; // "<script>setTimeout(function() {document.forms[0].submit();}, 5000);</script>";

void handleRoot() {
  String ApogeeMsg = "";
  Serial.println("*****  HandleRoot  *******");
  load_config() ;
  html_bodyA     = "Parachute deploy will occur only when altitude is over " + String(user_config.OverShootReference) + "m above reference and became " + String(user_config.UnderShootApogee) + "m smaller then apogee altitude!</div><br>\n";

  String header1 = String(pre_logo) + String(html_header) + String(html_script) + String(html_body) + String(html_bodyA) + String(logo); // server.header("User-Agent") +server.header("Date") +server.header("Accept");
   if (apogee == 1) { ApogeeMsg = "**** Apogee Detected *****" ;}
  html_dynamic = "<table width=70% border=1><tr><td align=center colspan=2><h2>Current Data</h2></td></tr>\
  <tr><td width=50%><div id='date-div'>Date:" + resetTime + "</div></td><td>Temperature:" + temperatureM + "*C</td></tr>\
  <tr><td>Pressure:" + pressureM + "Pa</td><td>Altitude:" + altitudeM + "m</td></tr>\
  </table><br>\
  <table width=70% border=1><tr><td align=center colspan=2><h2>Last Reference</h2></td></tr>\
  <tr><td width=50%>Date:" + user_config.date + "</td><td>Temperature:" + user_config.temperature + "*C</td></tr>\
  <tr><td>Pressure Start :" + user_config.startPressure + "Pa </td><td> Pressure Min :" + user_config.MinPressure + "Pa</td></tr>\
  <tr><td>Altitude Start :" + user_config.startAltitude + " m   </td><td>Altitude Max :" + user_config.MaxAltitude + " m</td></tr>\
  <tr><td align=center colspan=1>Current Height:" + String(altitudeM - user_config.startAltitude ) + "</td><td><div class=height-class>Max. Height:" + String(user_config.MaxAltitude - user_config.startAltitude )  + " m</div></td></tr></table>"
  "\n " + ApogeeMsg;

  header1 = header1 + String(html_dynamic);
  int  html_content_size = pre_logo.length() + html_header.length() + html_script.length() + html_body.length() + html_dynamic.length() + logo.length() ;
  Serial.println("var:" + String(html_content_size) + ":" + String(header1.length()) );
  server.sendHeader("Content-Length", (String)  header1.length() );
 
  server.sendContent(pre_logo);
  server.sendContent(logo);  //server.sendContent(logo1);server.sendContent(logo2);server.sendContent(logo3);server.sendContent(logo4);server.sendContent(logo5);server.sendContent(logo6);server.sendContent(logo7);
  server.sendContent(html_header);
  server.sendContent(html_script);
  server.sendContent(html_body);
  server.sendContent(html_bodyA);
  server.sendContent(html_dynamic);
  server.sendContent("\n");
  server.client().stop();
}

void handleLogo() {
  server.send(200, "text/html", logo);
}

void handleUpdate() {
  Serial.println("*****  HandleUpdate  *******");
  Serial.println( String(server.arg("dateF")) );
  resetTime = server.arg("dateF");
  save_config();
  handleRoot();
}

void handleLog() {
  Serial.println("*****  HandleLog  *******");
  String Log = "Date=" + resetTime + "\nTemperature=" + String(user_config.temperature) + " *C\nPressure=" + String(pressureM) + " Pa\n";
  Log = Log + "Altitude=" + String(altitudeM) + " m\nHeight=" + String(altitudeM - startAltitude) + " m\n";
  Log = Log + "Max. Height=" + String(MaxAltitude - startAltitude) + " m\n" ;
  Log = Log + "Pressure Start=" +   String(user_config.startPressure) + "\nAltitude Start=" +  String(user_config.startAltitude) +
        "\nPressure Min="   + String(user_config.MinPressure )    + "\nAltitude Max="  +  String(user_config.MaxAltitude);
  server.send(200, "plain/text", Log);
}

void handleConfig() { //*** relay0_servo1_1  relay0_servo1_2  ang_servo1  ang_servo2  OverShootReference   UnderShootApogee
  Serial.println("******** HandleConfig ********");
  if (server.hasArg("relay0_servo1_1")     == true) { user_config.relay0_servo1_1    = (boolean)server.arg("relay0_servo1_1").toInt()    ;};
  if (server.hasArg("relay0_servo1_2")     == true) { user_config.relay0_servo1_2    = (boolean)server.arg("relay0_servo1_2").toInt()    ;};
  if (server.hasArg("ang_servo1")     == true)      { user_config.ang_servo1         = (byte)   server.arg("ang_servo1").toInt()          ;};
  if (server.hasArg("ang_servo2")     == true)      { user_config.ang_servo2         = (byte)   server.arg("ang_servo2").toInt()          ;};
  if (server.hasArg("OverShootReference") == true)  { user_config.OverShootReference = (int)    server.arg("OverShootReference").toInt() ;};
  if (server.hasArg("UnderShootApogee")   == true)  { user_config.UnderShootApogee   = (int)    server.arg("UnderShootApogee").toInt()   ;};
  Serial.println("***" + server.arg("relay0_servo1_1") + " - " + server.arg("relay0_servo1_2") + " - " + server.arg("ang_servo1") + " - " + server.arg("ang_servo2") + " ***");
  Serial.println("***" + server.arg("OverShootReference") + " - " + server.arg("UnderShootApogee") + " ***" );
  Serial.println("***" + String(user_config.OverShootReference) + " - " + String(user_config.UnderShootApogee) + " ***" );
  Serial.println("***" + String(user_config.ang_servo1) + " - " + String(user_config.ang_servo2) + " ***" );

  String out_1, out_2;
  if (user_config.relay0_servo1_1 == 1) { out_1 = "selected";}
  if (user_config.relay0_servo1_2 == 1) { out_2 = "selected";}
  html_dynamic = "<html><body><form action='/config' method=post><table border=1><tr><td>Apogee Output:</td><td>"
                 "<select name='relay0_servo1_1'><option value='0'>Relay</option><option value='1' " + out_1 + ">Servo</option></select></td>"
                 "<td>Apogee Servo Angle:</td><td><input type=text size=3 name='ang_servo1' value='" + user_config.ang_servo1 + "'></td></tr>\n"
                 "<tr><td>Launch Output:</td><td><select name='relay0_servo1_2'><option value='0'>Relay</option><option value='1' " + out_2 + ">Servo</option></select></td>"
                 "<td>Launch Servo Angle:</td><td><input type=text size=3 name='ang_servo2' value='" + user_config.ang_servo2 + "'></td></tr>\n"
                 "<tr><td colspan=2>Detect Apogee only if altitude over:</td><td colspan=2><input type=text name='OverShootReference' size=4 value='" + user_config.OverShootReference + "'>m above reference altitude</td></tr>\n"
                 "<tr><td colspan=2>Open Parachute after altitude smaller:</td><td colspan=2><input type=text name='UnderShootApogee' size=4 value='" + user_config.UnderShootApogee + "'>m of apogee altitude.</td></tr></table>"
                 "<br> &nbsp; <div class=form_menu><input type=submit value='Update'></form><form action='/'> <input type=submit value='Home'> </div> </form> </body></html>\n";
                    
  server.sendHeader("Content-Length", (String) (pre_logo.length() + logo.length() + html_header.length()+ html_dynamic.length()) );
  server.sendContent(pre_logo);
  server.sendContent(logo);  //server.sendContent(logo1);server.sendContent(logo2);server.sendContent(logo3);server.sendContent(logo4);server.sendContent(logo5);server.sendContent(logo6);server.sendContent(logo7);
  server.sendContent(html_header);
  //server.send(200, "text/html", html_dynamic);
  server.sendContent(html_dynamic);
  server.client().stop();
  save_config();
}

void handleLaunch(){
 /*** Acionar Relay2 ou Servo2 ***/
}


void StartAverage () {
  temperatureM = 0;
  pressureM    = 0;
  altitudeM    = 0;
  for (int x = 0; x < 20; x++) {
    altitudeM    = altitudeM    + bmp.readAltitude(103000);
    pressureM    = pressureM    + bmp.readPressure() ;
  }
  temperatureM = bmp.readTemperature();
  pressureM    = pressureM / 20 ;
  altitudeM    = altitudeM  / 20;
}


void average () {
  float leituraP, LastP, LastA;
  float leituraA, res;
  LastP        = pressureM + 0.01;
  LastA        = altitudeM + 0.001;
  temperatureM = 0;
  pressureM    = 0;
  altitudeM    = 0;
  leituraP     = bmp.readPressure();

  for (int x = 0; x < 5; x++) {
    leituraA = bmp.readAltitude(103000);
    res = sqrt(pow( ((leituraA - LastA) / LastA), 2)) ;
    if ( res > 0.05) {
      Serial.print("Fora da Altitude media:");
      Serial.println(res);
      Serial.println(leituraA);
      Serial.println(LastA);
      leituraA = bmp.readAltitude(103000);
      Serial.println(leituraA);
    }
    altitudeM    = altitudeM    + leituraA;
  }

  temperatureM = bmp.readTemperature();
  pressureM    = leituraP ;
  altitudeM    = altitudeM  / 5;
  if (altitudeM > MaxAltitude) {
    MaxAltitude = altitudeM;
    update_altitude();
  }
  if (pressureM < MinPressure) {
    MinPressure = pressureM;
  }
}

void setup() {
  delay(1000);
  EEPROM.begin(150);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  Serial.setDebugOutput(false) ;
  WiFi.softAP(ssid, password);          /***** You can remove the password parameter if you want the AP to be open. ******/

  IPAddress myIP = WiFi.softAPIP();
  // WiFi.setOutputPower(5);
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", HTTP_GET,   handleRoot);     /**** Call the 'handleRoot' function when a client requests URI "/"   ****/
  server.on("/", HTTP_POST,  handleUpdate);
  server.on("/logo.png",     handleLogo);
  server.on("/log.txt",      handleLog);
  server.on("/config",       handleConfig);
  server.on("/launch",       handleLaunch);

  //  const char * headerkeys[] = {"User-Agent","Cookie","Date","Accept"} ;
  //  size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);
  // *******ask server to track these headers
  // server.collectHeaders(headerkeys, headerkeyssize );

  server.begin();
  Serial.println("HTTP server started");
  Serial.setDebugOutput(false) ;

  Wire.pins(0, 2);
  Wire.begin(0, 2);
  if (!bmp.begin()) {
    Serial.println("No BMP180 / BMP085");// we dont wait for this
    SensorFound = 0;
  } else {
    delay(100);
    StartAverage();
    startAltitude = altitudeM;
    MinPressure = pressureM;
  }
  load_config();
}

float getAltitude(float press, float temp) {
  const float sea_press = 1013.25;
  return ((pow((sea_press / press), 1 / 5.257) - 1.0) * (temp + 273.15)) / 0.0065;
}

void ApogeeFunc(){
 /*** Acionar Relay1 ou Servo1 ***/
  apogee = 1;
}

void loop() {
  timeToHandle++;
  if (timeToHandle > 2) { /*** Verifica se existe requisição http e envia dados serial ********/
    timeToHandle = 0;
    server.handleClient();
    Serial.println(t); Serial.println(p); Serial.println(a); Serial.println(b); Serial.println("Time:" + resetTime); Serial.println(l);
  }

  if (SensorFound) {
    average();
    /*      Serial.println(bmp.readSealevelPressure());
    t = "<br>Current Temperature=" + String(temperatureM) + " *C";
    p = "<br>Current Pressure=" + String(pressureM) + " Pa";
    a = "<br>Current Altitude=" + String(altitudeM) + " m";// insert pressure at sea level
    b = "<br>Current Height=" + String(altitudeM - startAltitude) + " m"; // insert pressure at sea level
    c = "<br>Max. Height=" + String(user_config.MaxAltitude - user_config.startAltitude) + " m";
    l = "<br>Pressure Start = " +   String(user_config.startPressure) + "<br>Altitude Start =" +  String(user_config.startAltitude) +
        "<br>Pressure Min = "   + String(user_config.MinPressure)     + "<br>Altitude Max = "  +  String(user_config.MaxAltitude);
     */
     if (user_config.MaxAltitude - user_config.startAltitude > user_config.OverShootReference ) {
         if (user_config.MaxAltitude - altitudeM > user_config.UnderShootApogee ) {
             Serial.println("******  Apogee ******");
             ApogeeFunc();  
         }
     }
  } else {
    t = "No Sensor Found";
  }
  // delay(1);
}
