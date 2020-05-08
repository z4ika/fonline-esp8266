#include <ESP8266WiFi.h>

const char* host = "game.fonline2.com";
const uint16_t port = 4000;
const char* ssid = "";
const char* password = "";

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to network");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void endianness_swap ( void* ptr, unsigned int len )
{
  if ( len > 1 )
  {
    if ( ptr != NULL )
    {
      char* begin = (char*)ptr;
      for (int i = 0, j = len - 1; i < j; i ++, j --)
      {
        begin[i]                ^= begin[j];
        begin[j]                ^= begin[i];
        begin[i]                ^= begin[j];
      }
    }
  }
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
