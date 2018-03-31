#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"

// #include <google-maps-device-locator.h>
// GoogleMapsDeviceLocator locator;

#define BME_SCK D4
#define BME_MISO D3
#define BME_MOSI D2

#define BME_CS_INTERNAL D5
#define BME_CS_EXTERNAL D1

#define SEALEVELPRESSURE_HPA (1013.25)

int led = D7;
double voltage;
double batPerc;
FuelGauge fuel;
char envDataInternal[128];
char envDataExternal[128];
char batteryInfo[64];


Adafruit_BME280 bmeInternal(BME_CS_INTERNAL, BME_MOSI, BME_MISO, BME_SCK);
Adafruit_BME280 bmeExternal(BME_CS_EXTERNAL, BME_MOSI, BME_MISO, BME_SCK);

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
//   locator.withLocatePeriodic(30);
  Serial.println(F("Starting BME280 pings..."));
  if (bmeInternal.begin()) {
      Particle.variable("envDataInt", envDataInternal);
  }
  if (bmeExternal.begin()) {
      Particle.variable("envDataExt", envDataExternal);
  }
  Particle.variable("batteryInfo", batteryInfo);
}

void loop() {
    digitalWrite(led, HIGH);
    float intTemp, extTemp, intPressure, extPressure, intAltitude, extAltitude, intHumidity, extHumidity, voltage, batPerc = 0.0F;
    voltage = fuel.getVCell();
    batPerc = fuel.getSoC();
    sprintf(batteryInfo, "{\"voltage\": %.4f, \"batPerc\": %.4f}", voltage, batPerc);
    // locator.loop();
    intTemp = (bmeInternal.readTemperature() * 1.8F) + 32;
    intPressure = bmeInternal.readPressure() / 100.0F;
    intAltitude = bmeInternal.readAltitude(SEALEVELPRESSURE_HPA);
    intHumidity = bmeInternal.readHumidity();
    sprintf(envDataInternal, "{\"iT\": %.2f, \"iP\": %.2f, \"iA\": %.2f, \"iH\": %.2f}", intTemp, intPressure, intAltitude, intHumidity);

    extTemp = (bmeExternal.readTemperature() * 1.8F) + 32;
    extPressure = bmeExternal.readPressure() / 100.0F;
    extAltitude = bmeExternal.readAltitude(SEALEVELPRESSURE_HPA);
    extHumidity = bmeExternal.readHumidity();
    sprintf(envDataExternal, "{\"eT\": %.2f, \"eP\": %.2f, \"eA\": %.2f, \"eH\": %.2f}", extTemp, extPressure, extAltitude, extHumidity);
    // Serial.println(String(envData));
    // Particle.publish("envData", String(envData));
    delay(10000);
    digitalWrite(led, LOW);
    delay(10000);
}
