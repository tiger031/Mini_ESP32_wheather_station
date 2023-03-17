#include <WiFi.h> 
#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;
const char* ssid = "WiFiHome"; 
const char* password = "Pass12345";

WiFiServer server(80);

String header;

void setup() 
{
Serial.begin(9600);
if (!bmp.begin()) 
{
Serial.println("Not connected with BMP180/BMP085 sensor, check connections ");
while (1) {}
}

Serial.print("Connecting to Wifi Network");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("Successfully connected to WiFi.");
Serial.println("IP address of ESP32 is : ");
Serial.println(WiFi.localIP());
server.begin();
Serial.println("Server started");
}

void loop() {
Serial.print("Temp = ");
Serial.print(bmp.readTemperature());
Serial.println(" *C");

Serial.print("Pressure = ");
Serial.print(bmp.readPressure());
Serial.println(" Pa");

Serial.print("Pressure_mm_rt = ");
Serial.print(bmp.readPressure()/133.322);
Serial.println(" mm_rt");

Serial.println();
delay(500);
WiFiClient client = server.available();

if (client) 
{ 
Serial.println("Web Client connected ");
String request = client.readStringUntil('\r'); 
client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println("Connection: close");
client.println();
client.println("<!DOCTYPE html><html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<link rel=\"icon\" href=\"data:,\">");
client.println("</style></head><body><h1>ESP32 Web Server BMP180</h1>");
client.println("<h2>BMP180 Barometic sensor</h2>");
client.println("<table><tr><th>MEASUREMENT</th><th>VALUE</th></tr>");
client.println("<tr><td>Temp. Celsius</td><td><span class=\"sensor\">");
client.println(bmp.readTemperature());
client.println(" *C</span></td></tr>"); 
client.println("<tr><td>Pressure</td><td><span class=\"sensor\">");
client.println(bmp.readPressure());
client.println(" pa</span></td></tr>"); 
client.println("<tr><td>Pressure_mm_rt</td><td><span class=\"sensor\">");
client.println(bmp.readPressure()/133.322);
client.println(" mm</span></td></tr>"); 
client.println("</body></html>"); 
client.println();
Serial.println("Client disconnected.");
Serial.println("");
}
}
