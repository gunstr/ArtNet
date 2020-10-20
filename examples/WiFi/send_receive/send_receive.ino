#include <Artnet.h>

// WiFi stuff
const char* ssid = "your-ssid";
const char* pwd = "your-password";
const IPAddress ip(192, 168, 1, 201);
const IPAddress gateway(192, 168, 1, 1);
const IPAddress subnet(255, 255, 255, 0);

ArtnetWiFi artnet;
const String target_ip = "192.168.1.200";
uint32_t universe = 1;

const uint16_t size = 512;
uint8_t data[size];
uint8_t value = 0;

void setup()
{
    Serial.begin(115200);

    // WiFi stuff
    WiFi.begin(ssid, pwd);
    WiFi.config(ip, gateway, subnet);
    while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(500); }
    Serial.print("WiFi connected, IP = "); Serial.println(WiFi.localIP());

    artnet.begin(target_ip);

    // if Artnet packet comes to this universe, this function is called
    artnet.subscribe(universe, [&](uint8_t* data, uint16_t size)
    {
        Serial.print("artnet data (universe : ");
        Serial.print(universe);
        Serial.print(", size = ");
        Serial.print(size);
        Serial.print(") :");
        for (size_t i = 0; i < size; ++i)
        {
            Serial.print(data[i]); Serial.print(",");
        }
        Serial.println();
    });

    // if Artnet packet comes, this function is called to every universe
    artnet.subscribe([&](uint32_t univ, uint8_t* data, uint16_t size)
    {
        Serial.print("ArtNet data has come to universe: ");
        Serial.println(univ);
    });
}

void loop()
{
    artnet.parse(); // check if artnet packet has come and execute callback

    value = (millis() / 4) % 256;
    memset(data, value, size);

    artnet.set(universe, data, size);
    artnet.streaming(); // automatically send set data in 40fps
}
