#include <Arduino.h>
#include <FastLED.h>
#include <DFRobot_SHT3x.h>
#include "DFRobot_VEML7700.h" // I2C VEML7700 Ambient Light Sensor

#define NUM_LEDS 1        // Number of RGB LED beads
#define DATA_PIN D8       // The pin for controlling RGB LED
#define LED_TYPE NEOPIXEL // RGB LED strip type
#define LIQUID_LEVEL_PIN D6

// alias for things
DFRobot_SHT3x sht3x;
DFRobot_VEML7700 als;

CRGB leds[NUM_LEDS]; // Instantiate RGB LED
int baudRate = 15200;
int moistorSensorPin = A4; //  Define analog moistor sensor pin
int moistorSensorVale = 0; // initalize value moistor sensor
int dutyDelay = 1500;
int liquidLevel = 0; // initalize value liquidLevel sensor

void setup()
{
  // led
  FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS); // Initialize RGB LED
  // ledcSetup(ledChannel,freq,resolution);
  // ledcAttachPin(ledPin,ledChannel);    //Set the pin for outputting PWM signals and the channel for generating PWM signals
  // analog pin
  // pinMode(sensorPin, INPUT);
  pinMode(LIQUID_LEVEL_PIN, INPUT);
  Serial.printf("Setting Baudrate to: %i\n", baudRate);
  Serial.begin(115200); // Initialize serial port
  delay(500);
  // Initialize the chip to detect if it can communicate properly.
  while (sht3x.begin() != 0)
  {
    Serial.println("Failed to Initialize the chip, please confirm the wire connection");
    delay(1000);
  }
  /**
   * readSerialNumber Read the serial number of the chip.
   * @return Return 32-digit serial number.
   */
  Serial.print("Chip serial number");
  Serial.println(sht3x.readSerialNumber());
  /**
   * softReset Send command resets via IIC, enter the chip's default mode single-measure mode,
   * turn off the heater, and clear the alert of the ALERT pin.
   * @return Read the register status to determine whether the command was executed successfully,
   * and return true indicates success.
   */
  if (!sht3x.softReset())
  {
    Serial.println("Failed to Initialize the chip....");
  }
  Serial.println("Temperature/Humidity Sensor is ready");

  Serial.println("Init I2C VEML7700 Ambient Light Sensor");
  als.begin(); // Init ambianet light status
}

void loop()
{
  Serial.println();
  Serial.println("Start Loop");
  // LED Ops
  Serial.printf("Turn LED Red:\n");
  leds[0] = CRGB::Red; // LED shows red light
  FastLED.show();
  delay(dutyDelay);
  Serial.printf("Turn LED Green\n");
  leds[0] = CRGB::Green; // LED shows green light
  FastLED.show();
  delay(dutyDelay);
  Serial.printf("Turn LED Blue\n");
  leds[0] = CRGB::Blue; // LED shows blue light
  FastLED.show();
  delay(dutyDelay);
  Serial.printf("Turn LED TO A RANDOM Value\n");
  leds[0] = CRGB(random(0, 255), random(0, 255), random(0, 255)); // LED shows a randomly mixed color
  FastLED.show();
  delay(dutyDelay);
  Serial.printf("Turn LED Off\n");
  FastLED.clear(); // Turn off
  FastLED.show();
  delay(dutyDelay);
  // Read analog moistorSensorVale
  moistorSensorVale = analogRead(moistorSensorPin);
  Serial.printf("sensor value: %d  ", moistorSensorVale);             // Print the read sensor value
  Serial.printf("voltage: %.3fV\n", moistorSensorVale * 3.26 / 4095); // Print the detected voltage

  /**
   * getTemperatureC Get the measured temperature (in degrees Celsius).
   * @return Return the float temperature data.
   */
  Serial.print("Ambient Temperature: ");
  Serial.print(sht3x.getTemperatureC());
  Serial.print(" C ");
  /**
   * getTemperatureF Get the measured temperature (in degrees Fahrenheit).
   * @return Return the float temperature data.
   */
  Serial.print(sht3x.getTemperatureF());
  // delay(100);

  /**
   * getHumidityRH Get measured humidity(%RH)
   * @return Return the float humidity data
   */
  Serial.print(" F ");
  Serial.print(sht3x.getHumidityRH());
  Serial.println(" %RH");
  delay(dutyDelay);

  // Get the measured ambient light value
  // float lux;
  // int luxStatus = als.getALSLux(lux); // Get the measured ambient light value
  // Serial.printf("Ambient Light luxStatus: %d\n", luxStatus);
  // Serial.printf("Ambient Light Lux: %d lx\n", lux);

  float lux;

  als.getALSLux(lux); // Get the measured ambient light value
  Serial.print("Ambient Light Lux: ");
  Serial.print(lux);
  Serial.println(" lx");

  /*
    # the sensor value description
    # 0  ~300     dry soil
    # 300~700     humid soil
    # 700~950     in water
  */
  int moisterLevel = analogRead(A0);
  if (moisterLevel < 300)
  {
    Serial.printf("Soil Moisture is dry: %d\n", moisterLevel);
    leds[0] = CRGB::Red; // LED shows red light
    FastLED.show();
  }
  if (moisterLevel >= 300 && moisterLevel < 700)
  {
    Serial.printf("Soil Moisture is humid: %d\n", moisterLevel);
    leds[0] = CRGB::Green; // LED shows red light
    FastLED.show();
  }
  if (moisterLevel >= 700 && moisterLevel < 950)
  {
    Serial.printf("Soil Moisture  is in water: %d\n", moisterLevel);
    leds[0] = CRGB::Red; // LED shows red light
    FastLED.show();
  }

  // Liquid Level Reading
  liquidLevel = digitalRead(LIQUID_LEVEL_PIN); // 1 is in contact 0 is not in contact
  if (liquidLevel)
  {
    Serial.printf("Liquid Level is Full: %d\n", liquidLevel);
  }
  else
  {
    Serial.printf("Liquid Level is NOT Full: %d \n", liquidLevel);
  }

  // clena things up
  Serial.println();
  delay(5000);
  FastLED.clear(); // Turn off
  FastLED.show();
}