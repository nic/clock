/**
 * The 27-Leds Clock
 * https://github.com/nic/clock
 * 
 * Fernanda Vassoler & Nic Marcondes (c) 2020
 */
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Adafruit_NeoPixel.h>
#include <BoolArray.h>

#define WIFI_SSID ""
#define WIFI_PASS  ""
#define NTPURL "pool.ntp.org"
#define LED_PIN   0
#define LED_COUNT 27


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTPURL, -3 * 3600, 60000);

Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

BearSSL::WiFiClientSecure wifiClient;

unsigned long previousMillis = 0;
const long interval = 4000;
unsigned int color[] = {0,0,0,0};

BoolArray nextPixels1;
BoolArray nextPixels2;
BoolArray nextPixels3;
BoolArray nextPixels4;


void setup_wifi() {
  WiFi.hostname(ID_MQTT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    pixels.clear();pixels.setPixelColor(LED_COUNT-2, pixels.Color(255, 0, 0));pixels.show();
    delay(5000);
    ESP.restart();
  }
}

void setup() {
  pixels.begin();
  pixels.setBrightness(255);
  pixels.clear();
  pixels.show();

  setup_wifi();
  
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  
  timeClient.begin();
  nextPixels1.begin(3);
  nextPixels2.begin(9);
  nextPixels3.begin(6);
  nextPixels4.begin(9);

}


void reconnectLoop() {
    if (WiFi.status() != WL_CONNECTED) {
      setup_wifi();
    }
}

void setActualColor(int number) {
  switch(number) {
    case 1: color[0] = 255; color[1] = 255; color[2] = 255; color[3] = 255; break;
    case 2: color[0] = 255; color[1] = 0;   color[2] = 0;   color[3] = 0;   break;
    case 3: color[0] = 255; color[1] = 75;  color[2] = 0;   color[3] = 0;   break;
    case 4: color[0] = 255; color[1] = 255; color[2] = 0;   color[3] = 20;  break;
    case 5: color[0] = 0;   color[1] = 255; color[2] = 0;   color[3] = 0;   break;
    case 6: color[0] = 0;   color[1] = 0;   color[2] = 255; color[3] = 0;   break;
    case 7: color[0] = 75;  color[1] = 0;   color[2] = 130; color[3] = 0;   break;
    case 8: color[0] = 255; color[1] = 0;   color[2] = 255; color[3] = 200; break;
    case 9: color[0] = 0;   color[1] = 255; color[2] = 255; color[3] = 0;   break;
  }
  
}


void loop() {
  timeClient.update();
  reconnectLoop();
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    int hours = timeClient.getHours();
    int hours1 = hours/10;
    int hours2 = hours%10;
    
    int minutes = timeClient.getMinutes();
    int minutes1 = minutes/10;
    int minutes2 = minutes%10;
    
    nextPixels1.clear();
    for(int i=0; i<hours1; i++) {
      nextPixels1.set(i, true);
    }

    uint8_t n = 3;
    for (size_t i = 0; i < n - 1; i++)
    {
        size_t j = random(0, n - i);
        uint8_t t = nextPixels1.get(i);
        nextPixels1.set(i, nextPixels1.get(j));
        nextPixels1.set(j, t);
    }
    
    nextPixels2.clear();
    for(int i=0; i<hours2; i++) {
      nextPixels2.set(i, true);
    }
   
    n = 9;
    for (size_t i = 0; i < n - 1; i++)
    {
        size_t j = random(0, n - i);
        uint8_t t = nextPixels2.get(i);
        nextPixels2.set(i, nextPixels2.get(j));
        nextPixels2.set(j, t);
    }

    nextPixels3.clear();
    for(int i=0; i<minutes1; i++) {
      nextPixels3.set(i, true);
    }
    
    n = 6;
    for (size_t i = 0; i < n - 1; i++)
    {
        size_t j = random(0, n - i);
        uint8_t t = nextPixels3.get(i);
        nextPixels3.set(i, nextPixels3.get(j));
        nextPixels3.set(j, t);
    }
    
    nextPixels4.clear();
    for(int i=0; i<minutes2; i++) {
      nextPixels4.set(i, true);
    }
    
    n = 9;
    for (size_t i = 0; i < n - 1; i++)
    {
        size_t j = random(0, n - i);
        uint8_t t = nextPixels4.get(i);
        nextPixels4.set(i, nextPixels4.get(j));
        nextPixels4.set(j, t);
    }
    
    ///////////////////

    int pixelAtual = 0;
    
    for(int i=0; i<9; i++) {
      setActualColor(minutes2);
      pixels.setPixelColor(pixelAtual, nextPixels4.get(i) ? pixels.Color(color[0], color[1], color[2], color[3]): pixels.Color(0, 0, 0) );
      pixelAtual++;
    }
    
    for(int i=0; i<6; i++) {
      setActualColor(minutes1);
      pixels.setPixelColor(pixelAtual, nextPixels3.get(i) ? pixels.Color(color[0], color[1], color[2], color[3]): pixels.Color(0, 0, 0) );
      pixelAtual++;
    }

    for(int i=0; i<9; i++) {
      setActualColor(hours2);
      pixels.setPixelColor(pixelAtual, nextPixels2.get(i) ? pixels.Color(color[0], color[1], color[2], color[3]): pixels.Color(0, 0, 0) );
      pixelAtual++;
    }
    
    for(int i=0; i<3; i++) {
      setActualColor(hours1);
      pixels.setPixelColor(pixelAtual, nextPixels1.get(i) ? pixels.Color(color[0], color[1], color[2], color[3]): pixels.Color(0, 0, 0) );
      pixelAtual++;
    }
    
    pixels.show();
  }
  
}
