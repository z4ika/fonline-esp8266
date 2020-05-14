#include <ESP8266WiFi.h>
#include <Wire.h>  
#include "SSD1306Wire.h"
#include <OLEDDisplayUi.h>

const char* host = "play.fonline-aop.net";
const uint16_t port = 4000;
const char* ssid = "";
const char* password = "";
const char* online_info = "Ashes of Phoenix Online:";

SSD1306Wire display(0x3c, 0, 2);
OLEDDisplayUi ui ( &display );

void setup()
{
  display.init();
  display.clear();
  display.display();

  display.setFont(ArialMT_Plain_10);
  display.setContrast(255);
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to network");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    display.clear();
    display.flipScreenVertically();
    display.drawString(0, 0, "Connecting to WiFi");
    display.display();
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}
void loop()
{
  Serial.print("Connecting to remote: ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);
  Serial.println("[Connected]");

  WiFiClient client;

  if (client.connect(host, port))
  {
    byte fonline[] = { 0xFF, 0xFF, 0xFF, 0xFF };
    client.write(fonline, sizeof(fonline));

    Serial.println("Write complete. Waiting 10s");
    delay(10000);

    byte* buffer = NULL;
    unsigned int recvLen = client.available();
    if ( recvLen > 0 )
    {
      printf( " bytes recieved %u", recvLen );
      buffer = new byte[recvLen];
      Serial.println("Starting read:");

      for ( unsigned int i = 0; i < recvLen; i ++)
      {
        buffer[i] = client.read();
        Serial.println(i);
        printf("%02X", buffer[i]);
      }
      Serial.println("Read complete");
      Serial.println("Starting print:");

      for ( unsigned int i = 0; i < recvLen; i ++)
      {
        Serial.println(i);
        printf("%02X", buffer[i]);
      }
      Serial.println("Online:");
      unsigned int online = 0;
      online |= (buffer[3] << 24) & 0xFF;
      online |= (buffer[2] << 16) & 0xFF;
      online |= (buffer[1] << 8 ) & 0xFF;
      online |= buffer[0] & 0xFF;
      Serial.println(online);
      
      char converted[33];
      String str_online = itoa( online, converted, 10 );
      
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.clear();
      display.drawString(0, 0, online_info);
      display.drawString(0, 32, str_online);
      display.display();
      
      delete[] buffer;
    }
    client.stop();
    Serial.println("[Disconnected]");
    Serial.println("End of cycle");
  }
  else
  {
    Serial.println("Connection failed or was refused.");
    delay(5000);
    return;
  }
}
