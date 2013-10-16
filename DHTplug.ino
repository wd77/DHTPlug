#include <Narcoleptic.h>
// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"
#include <RF12.h>

#define DHTPIN 4     // what pin we're connected to

struct {
  float humi;
  float temp;
  byte lobat;
} payload;

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  #if SERIAL || DEBUG 
   Serial.begin(9600); 
   Serial.println("[DHTplug]");
   myNodeID=rf12_config();
  #else
   myNodeID=rf12_config(0);
  #endif
  rf12_sleep(RF12_SLEEP); // pwrsave RF12
  dht.begin();
}

// periodic report, i.e. send out a packet and optionally report on serial port
static void doReport() {
    rf12_sleep(RF12_WAKEUP);
    while (!rf12_canSend())
        rf12_recvDone();
    rf12_sendStart(0, &payload, sizeof payload, RADIO_SYNC_MODE);
    rf12_sleep(RF12_SLEEP);

    #if SERIAL
        Serial.print("ROOM ");
        Serial.print((int) payload.humi);
        Serial.print(' ');
        Serial.print((int) payload.temp);
        Serial.print(' ');
        Serial.print((int) payload.lobat);
        Serial.println();
        delay(2); // make sure tx buf is empty before going back to sleep
    #endif
}

void loop() {
  
  delay(200);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("Hu:"); 
    Serial.print(h);
    //Serial.print("% ");
    Serial.print("% Te:"); 
    Serial.print(t);
    Serial.println("C");
  }
  Narcoleptic.delay(1250);
}
