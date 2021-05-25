/* Create by: Mr.Sarit Saenharn
// Create date: May 17, 21  //
// Last update: May 23, 21  //
// Version: 0.0.3     //
*/
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT_U.h>

char auth[] = "KcEIHIoNdWNkdFBEwDrEd3X1M06rhlUE";

char ssid[] = "HomeWiFi";         // SSID name
char pass[] = "6588de8c9c";       // Password

//DHT22 Sensor connected with Digital pin#5 (D1)
#define DHTPIN 5

//Soil Moisture Sensor connected with Analog pin#A0
int analogPin = A0;
int val = 0;
int setSoilMin;
int setSoilMax;

  BLYNK_WRITE(V100)
  {
    setSoilMin = param.asInt();
    }
  BLYNK_WRITE(V101)
  {
    setSoilMax = param.asInt();
    }
    
// Relay configure with Digital pin#D6, D7, D5, D8
int relay1 = 12;
//int relay2 = 13;
//int relay3 = 14;
//int relay4 = 15;

#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE); 

void setup()
{
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));
  dht.begin();
  pinMode(relay1, OUTPUT);
// pinMode(relay2, OUTPUT);
// pinMode(relay3, OUTPUT);
// pinMode(relay4, OUTPUT);
 
  Blynk.begin(auth, ssid, pass, "blynk.iot-cm.com", 8080); 
  
}
  double mapf(double val, double in_min, double in_max, double out_min, double out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }  
  
void loop()
{
  delay(2000);
  
  int val = map(analogRead(A0), 0, 1023, 0, 1000);
  float valf = val/1000.0;
  float newval = mapf(valf, 0.0, 1.0, 0.0, 100.0);
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

// Set condition for open relay
    Serial.println(newval);
    Serial.println(setSoilMin);
    Serial.println(setSoilMax);

    if (newval >= setSoilMin || newval >= setSoilMax) {
      digitalWrite(relay1, LOW);
     }
    else {
      digitalWrite(relay1, HIGH);
    }
  
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

// Print to Serial Mornitor area
//  Serial.print(F("HUMIDITY: "));
//  Serial.print(h, 1);
//  Serial.print(F("%  TEMP: "));
//  Serial.print(t, 1);
//  Serial.print(F("°C  FEELS LIKE: "));
//  Serial.print(hic, 1);
//  Serial.println(F("°C"));
//  Serial.print(F("MOISTURE: "));
//  Serial.println(newval);

// Blynk Virtual Port
  Blynk.run();
  Blynk.virtualWrite(V0, t);              // Temperature
  Blynk.virtualWrite(V1, h);              // Humidity
  Blynk.virtualWrite(V2, newval);         // Soil Moisture
  }
