#include <ESP8266WiFi.h>

const char* ssid = ""; // Enter your ssid - WiFi's Name
const char* password = ""; // Leave blank for open WiFi

// WebApp Link: https://iot-tinkering.000webhostapp.com/
// Accessing Data from: https://iot-tinkering.000webhostapp.com/light.txt
const char* host = "iot-tinkering.000webhostapp.com";
const char* path = "light.txt";

void setup()
{
  //LED THAT WE'LL CONTROL
  pinMode(D1, OUTPUT);
  pinMode(D0, OUTPUT);
  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
  
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  
  // To indicate that you are connected to WiFi
  for(int i = 0; i < 5; i++){
    digitalWrite(D0, HIGH);
    delay(100);
    digitalWrite(D0, LOW);
    delay(100);
    Serial.println("In Loop");
  }
  Serial.println(" connected");
}


void loop()
{
  WiFiClient client;

  Serial.printf("\n[Connecting to %s ... ", host);
  if (client.connect(host, 80))
  {
    Serial.println("connected");

    Serial.println("[Sending a request]");
    client.print(String("GET /") + path +
                 " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n" +
                 "\r\n"
                );

    Serial.println("[Response:]");
    while (client.connected())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\n');
        Serial.println(line);
        if(line == "ON"){
          digitalWrite(D1, HIGH);
        } else if(line == "OFF"){
          digitalWrite(D1, LOW);
        }
        //Serial.printf("\n\n %d \n\n", line.length());
      }
    }
    client.stop();
    Serial.println("\n[Disconnected]");
  }
  else
  {
    Serial.println("connection failed!]");
    client.stop();
  }
  delay(50);
}
