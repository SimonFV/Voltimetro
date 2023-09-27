#include <Wire.h>
#include <Adafruit_ADS1X15.h>

/*Adafruit_ADS1115 ads;   Use this for the 16-bit version */

Adafruit_ADS1115 ads;     /* Use this for the 16-bit version */
const float multiplier = 0.0001875F;   //Multiplicador
float v0 = 0.0;   //Conversión
int inPin = 7;    // pushbutton connected to digital pin 7
int mode = 0;      // variable to store the read value (scale mode)

int decimals = 3;

void setup(void)
{
  Serial.begin(9600);
 
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

  pinMode(inPin, INPUT);    // sets the digital pin 7 as input
  ads.begin();
}

void loop(void)
{
  
  int16_t adc0;
  
  adc0 = ads.readADC_SingleEnded(0);    // Read the pin to get floating representation of Voltage
  mode = digitalRead(inPin);   // read the input pin

  //Check mode 10 V or 100 V
  if(mode){
    v0 = adc0*multiplier * 2;    // Get real measured voltage
    decimals = 4;
  }else{
    v0 = adc0*multiplier * 20;    // Get real measured voltage
    decimals = 2;
  }

  Serial.print("Lectura"); Serial.println(v0, decimals);     // Print Results

  delay(1000);
}
