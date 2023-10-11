#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Adafruit_ADS1X15.h>


// Wifi
char ssid[] = "Samsung";
char pass[] = "simonjfv";
WiFiServer server(7070);
String message = "";
int outPinD4 = 2;
uint8_t led = LOW;

// ADS
Adafruit_ADS1115 ads;
const float gain_adj = 0.0001875F; // gain adjustment
float v10 = 0.0; 
float v100 = 0.0;
int inPinD7 = 13;             // digital pin D7
int inPinD5 = 14;             // digital pin D5
int voltage_range = 0;        // variable to store the scale mode
int ac_dc_mode = 0;           // variable to store the ac or dc mode

// Stores the readings from ADC pins A0 and A1
int16_t adc_a0;
int16_t adc_a1;

void setup(void)
{
  Serial.begin(9600);

  pinMode(inPinD7, INPUT);    // sets the digital pin D7 as input
  pinMode(inPinD5, INPUT);    // sets the digital pin D5 as input
  pinMode(outPinD4, OUTPUT);
  ads.begin();

  // Wifi server config
  IPAddress ip(192,168,45,200);     
  IPAddress gateway(192,168,45,1);
  IPAddress subnet(255,255,255,0);
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
    delay(500);
  server.begin();
}

void loop(void)
{
  WiFiClient client = server.available();
  
  if (client) {
    digitalWrite(outPinD4, LOW);

    while(client.connected()){
      
      adc_a0 = ads.readADC_SingleEnded(0);    // Reads the pin A0 of ADC
      adc_a1 = ads.readADC_SingleEnded(1);    // Reads the pin A1 of ADC

      voltage_range = digitalRead(inPinD7);
      ac_dc_mode = digitalRead(inPinD5);

      v10 = adc_a0 * gain_adj * 2;            // Voltage value for the 10 V range
      v100 = adc_a1 * gain_adj * 20;          // Voltage value for the 100 V range

      // Check DC / AC modes
      if (ac_dc_mode == 0)
        message = "DC,";
      else
        message = "AC,";

      // Check voltage range: 10 V or 100 V
      if (voltage_range == 0)
        message += "10," + String(v10, 3);
      else
        message += "100," + String(v100, 2);
      
      client.write(message.c_str());
      
      delay(500);
    }

    client.stop();
  }
  
  if (led == LOW)
    led = HIGH;
  else
    led = LOW;
  Serial.println(led);
  digitalWrite(outPinD4, led);
  delay(500);
}
