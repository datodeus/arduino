#include <Arduino.h>
#include <Print.h>
#include <dht11.h>

#define DHT11PIN 7
dht11 DHT11;

//Celsius to Fahrenheit conversion
double Fahrenheit(double celsius){
    return 1.8 * celsius + 32;
}

//Celsius to Kelvin conversion
double Kelvin(double celsius){
    return celsius + 273.15;
}

// dewPoint function NOAA
// reference (1) : http://wahiduddin.net/calc/density_algorithms.htm
// reference (2) : http://www.colorado.edu/geography/weather_station/Geog_site/about.htm
double dewPointFast(double celsius, double humidity){
    double a = 17.271;
    double b = 237.7;
    double temp = (a * celsius) / (b + celsius) + log(humidity*0.01);
    double Td = (b * temp) / (a - temp);
    return Td;
}

void setup() {
  Serial.begin(9600);
  Serial.println("DHT11 TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();

}

void loop (void){

  Serial.println("\n");
  int chk = DHT11.read(DHT11PIN);
  Serial.println(chk);
  Serial.print("Read sensor: ");
  switch (chk){
    case DHTLIB_OK:
      Serial.println("OK");
      Serial.print("Humidity (%): ");
      Serial.println((float)DHT11.humidity, 2);
      Serial.print("Temperature (°C): ");
      Serial.println((float)DHT11.temperature, 2);
      Serial.println(DHT11.temperature);
      Serial.print("Temperature (°F): ");
      Serial.println(Fahrenheit(DHT11.temperature), 2);
      Serial.print("Temperature (°K): ");
      Serial.println(Kelvin(DHT11.temperature), 2);
      Serial.print("Dew PointFast (°C): ");
      Serial.println(dewPointFast(DHT11.temperature, DHT11.humidity));


      break;

    case DHTLIB_ERROR_CHECKSUM:
      Serial.println("Checksum error");
      break;

    case DHTLIB_ERROR_TIMEOUT:
      Serial.println("Time out error");
      break;

    default:
    Serial.println("Unknown error");
    break;}

    delay(10000);

}
