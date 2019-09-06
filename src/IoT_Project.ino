#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>  
#include "Adafruit_BME280.h" 

WebServer server ( 80 );

const char* ssid     = "iPhone 6s de Anis";
const char* password = "anis19961";
#define ALTITUDE 55.0 // Altitude in PARIS, FRANCE

#define I2C_SDA 27
#define I2C_SCL 26 //half duplex 
#define BME280_ADDRESS 0x76  //0x77 si ça ne marche pas 

float temperature = 0;
float humidity = 0;
float pressure = 0;


Adafruit_BME280 bme(I2C_SDA, I2C_SCL);

void setup() 
{
  Serial.begin(9600);
  initSensor();
  connectToWifi();
  beginServer();
}

void loop() {
 
 server.handleClient();
 
 getTemperature();
 getHumidity();
 getPressure();
 delay(1000);
 
}

void connectToWifi()
{
  WiFi.enableSTA(true);
  
  delay(2000);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void beginServer()
{
  server.on ( "/", handleRoot );
  server.begin();
  Serial.println ( "HTTP server started" );
}

void handleRoot(){ 
    server.send ( 200, "text/html", getPage() );
}

void initSensor()
{
  bool status = bme.begin(BME280_ADDRESS);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

float getTemperature()
{
  temperature = bme.readTemperature();
}

float getHumidity()
{
  humidity = bme.readHumidity();
}

float getPressure()
{
  pressure = bme.readPressure();
  pressure = bme.seaLevelForAltitude(ALTITUDE,pressure);
  pressure = pressure/100.0F;
}


String getPage(){
  String page = "<html lang=fr-FR><head><meta http-equiv='refresh' content='10'/>";
  page += "<title>IoT project - EFREI</title>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; border-top: 6px solid black; border-left: 150px solid black; border-right: 150px solid black; border-bottom: 35px solid black; border-color:#2263bc;} footer { margin-top:15%; text-align:center; border-color:#666; border-width:thin; border-top:dotted; bottom: 0;} IMG.logo { border:8px white solid; float: right; } #entete{ width: 350px; height: 120px; padding-top: 5px; } #gauche{ float: left; } #droite{ text-align: center; width: 350px; margin-left: 300px; } h1{ text-align: center; color: #2263bc; } P.description{ text-align: center; } P.wear{ background-color: #99FF99; } h3{ color: maroon; } table{ margin: auto; border-collapse: collapse; } td{ width:180px; height:34px; border: 1px none black; } TD.important{ font-weight: bold; color: #CC0000; }   P.cas{ background-color: #FF6666; } </style>";
  page += "</head><body>";
  page += "<img class=\"logo\" src=\" https://upload.wikimedia.org/wikipedia/commons/e/e7/Logo-Efrei-Paris-2017.jpg\" width=\"250\" height=\"100\" alt=\"EFREI_logo\">";
  page += "<div id=\"entete\">";
  page += "<div id=\"gauche\">";
  page += "<p class=\"haut\">";
  page += "<b>EFREI Students:</b> <br />";
  page += "- Anis SARDAOUI <br /> - Massinissa BENAISSA  <br /> - Johlian RAJAONA  <br /> </p> </div>";
  page += "<div id=\"droite\"> <p class=\"haut\"><b>Master 1</b> <br />";
  page += "Information System and Cloud Engineering <br /> Promotion 2019  <br /> </p> </div> </div>";
  page += "<h1>IoT Project</h1>";
  page += "<p class=\"description\"><b>WEATHER STATION</b> using ESP32 + BME280 </p> <h3>Measurements:</h3>";
  page += "<table> <tr> <td>Temperature:</td>";
  page += "<td class=\"important\"> ";
  page += temperature;
  page += " &deg;C </td>";
  page += "</tr> <tr> <td>Humidity:</td>";
  page += "<td class=\"important\"> ";
  page += humidity;
  page += " % </td>";
  page += "</tr> <tr> <td>Barometric Pressure:</td>";
  page += "<td class=\"important\">";
  page += pressure;
  page += " hPa </td>";
  page += "</tr> </table>";
  page += "<p class= \"wear\">";
  page += "<b><u>Comment s'habiller ?</u></b>";
  

  
  page += "<br /> <b>-> jusqu'a 10 &deg; CLIMAT FROID :</b> doudoune, manteau ou equivalent Une echarpe et des gants sont necessaires <br /> <br />";
  page += "<b>-> de 10 &deg; a 20 &deg; :</b> veste, pull-over, chemise a longues manches ou equivalent <br /> <br /> ";
  page += "<b>-> des 20 &deg; :</b> chemise a manches courtes ou equivalent </p>";

  page += "<p class=\"cas\">";
  page += "<b><u>Votre CAS :</u></b> <br />";
  page += "<b>RAPPEL temperature : ";
  page += temperature;
  page += "&deg;C </b> ";
  page += "<p class=\"cas\" id=\"demo\"></p>";
  page += "<script>";
  page += "var temp = ";
  page += temperature;
  page += ";";
  page += "if (temp <= 10) {";
  page += "document.getElementById(\"demo\").innerHTML = \"Vous devez prendre : doudoune, manteau ou equivalent. Une echarpe et des gants sont necessaires \" ";
  page += "} else if (temp > 10 && temp <= 20) {";
  page += "document.getElementById(\"demo\").innerHTML = \"Vous devez prendre : veste, pull-over, chemise a longues manches ou equivalent \" ";
  page += "} else {";
  page += "document.getElementById(\"demo\").innerHTML = \"Vous devez prendre : chemise a manches courtes ou equivalent \" ";
  page += "}</script></p>";


  page += "<script>";
  page += "window.onload = function () {";
  page += "var dps = [];";
  page += "var chart = new CanvasJS.Chart(\"chartContainer\", {";
  page += "title :{";
  page += "text: \"Prise de temperature\"";
  page += "},";
  page += "axisY: {";
  page += "includeZero: false";
  page += "},";
  page += "data: [{";
  page += "type: \"line\",";
  page += "dataPoints: dps";
  page += "}]";
  page += "});";
  page += "var xVal = 0;";
  page += "var yVal = 40; ";
  page += "var updateInterval = 5000;";
  page += "var dataLength = 60; ";
  page += "var updateChart = function (count) {";
  page += "count = count || 1;";
  page += "for (var j = 0; j < count; j++) {";
  page += "yVal = ";  
  page += temperature;
  page +=" + Math.random();";
  page += "dps.push({";
  page += "x: xVal,";
  page += "y: yVal";
  page += "});";
  page += "xVal++;";
  page += "}";
  page += "if (dps.length > dataLength) {";
  page += "dps.shift();";
  page += "}";
  page += "chart.render();";
  page += "};";
  page += "updateChart(dataLength);";
  page += "setInterval(function(){updateChart()}, updateInterval);";
  page += "}";
  page += "</script>";
  page += "<div id=\"chartContainer\" style=\"height: 300px; width: 100%;\"></div>";
  page += "<script src=\"https://canvasjs.com/assets/script/canvasjs.min.js\"></script>";

  page += "<footer> Copyright all rights reserved - Tous droits reserves <br />";
  page += " Ceci est la propriete intellectuelle de Mrs SARDAOUI, BENAISSA et RAJAONA </footer>";

  page += "</body></html>";
  return page;
}


