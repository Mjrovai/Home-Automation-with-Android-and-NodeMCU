/*************************************************************************************
*   Home Automation with Android App and NodeMCU
*
*  Smart Devices
*   ==> Relay1
*   ==> Relay2
*   ==> Relay3
*   ==> Relay4
*   
*  Groups of devices
*   ==> All Devices ON/OFF
*   
*  Voice activation and response
*
*   Developed by Marcelo Rovai on 27March17
*   Visit my blog: https://MJRoBot.org 
*   
*   WARNING: When uploading a code ==> Power-off Relays
************************************************************************************/
#include <ESP8266WiFi.h>
WiFiClient client;
WiFiServer server(80);
const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";
String  command =""; // Command received from Android device

// Set Relay Pins
int relay1 = 14;
int relay2 = 15;
int relay3 = 3;
int relay4 = 1;

void setup()
{
  Serial.begin(115200);

  pinMode(relay1, OUTPUT); 
  pinMode(relay2, OUTPUT);  
  pinMode(relay3, OUTPUT);  
  pinMode(relay4, OUTPUT);  

  digitalWrite(relay1,HIGH);
  digitalWrite(relay2,HIGH);
  digitalWrite(relay3,HIGH);
  digitalWrite(relay4,HIGH);
      
  connectWiFi();
  server.begin();
}

void loop()
{
    client = server.available();
    if (!client) return; 
    command = checkClient ();

         if (command == "r1on"  || command == "ligar 1"    || command == "ligar um")        digitalWrite(relay1,LOW);
    else if (command == "r1off" || command == "desligar 1" || command == "desligar um")     digitalWrite(relay1,HIGH);
    else if (command == "r2on"  || command == "ligar 2"    || command == "ligar dois")      digitalWrite(relay2,LOW);     
    else if (command == "r2off" || command == "desligar 2" || command == "desligar dois")   digitalWrite(relay2,HIGH);
    else if (command == "r3on"  || command == "ligar 3"    || command == "ligar tres")      digitalWrite(relay3,LOW);
    else if (command == "r3off" || command == "desligar 3" || command == "desligar tres")   digitalWrite(relay3,HIGH);
    else if (command == "r4on"  || command == "ligar 4"    || command == "ligar quatro")    digitalWrite(relay4,LOW);
    else if (command == "r4off" || command == "desligar 4" || command == "desligar quatro") digitalWrite(relay4,HIGH);
    else if (command == "allon" || command == "ligar tudo" || command == "ligar todos") 
    {
      digitalWrite(relay1,LOW);
      digitalWrite(relay2,LOW);
      digitalWrite(relay3,LOW);
      digitalWrite(relay4,LOW);
    }
    else if (command == "alloff" || command == "desligar tudo" || command == "desligar todos") 
    {
      digitalWrite(relay1,HIGH);
      digitalWrite(relay2,HIGH);
      digitalWrite(relay3,HIGH);
      digitalWrite(relay4,HIGH);
    }
    sendBackEcho(command); // send command echo back to android device
    command = "";
}

/* connecting WiFi */
void connectWiFi()
{
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
    delay(300);
    Serial.print("..");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("NodeMCU Local IP is : ");
  Serial.print((WiFi.localIP()));
}

/* check command received from Android Device */
String checkClient (void)
{
  while(!client.available()) delay(1); 
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length()-9,9);
  return request;
}

/* send command echo back to android device */
void sendBackEcho(String echo)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println(echo);
  client.println("</html>");
  client.stop();
  delay(1);
}
