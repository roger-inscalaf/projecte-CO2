#include <MHZ.h>
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

//----------------  Fill in your credentails   ---------------------
char ssid[] = "linksys";             // your network SSID (name) 
char pass[] = "";         // your network password
unsigned long myChannelNumber = 1249954;  // Replace the 0 with your channel number
const char * myWriteAPIKey = "KWYQ01SKZNTA7LRY";    // Paste your ThingSpeak Write API Key between the quotes 
//------------------------------------------------------------------

WiFiClient  client;

// pin for pwm reading
#define CO2_IN D1

MHZ co2(CO2_IN, MHZ19B);

void setup() {
  Serial.begin(9600);
  pinMode(CO2_IN, INPUT);
  delay(100);
  Serial.println("MHZ 19B");
  // enable debug to get addition information
  //co2.setDebug(true);

  if (co2.isPreHeating()) {
    Serial.print("Preheating");
    while (co2.isPreHeating()) {
      Serial.print(".");
      delay(5000);
    }
    Serial.println();
  }

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client); 
}

void loop() {
  Serial.print("\n----- Time from start: ");
  Serial.print(millis() / 1000);
  Serial.println(" s");

  int ppm_pwm = co2.readCO2PWM();
  Serial.print("PPMpwm: ");
  Serial.print(ppm_pwm);

  Serial.println("\n------------------------------");
  delay(5000);

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    //Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  int x = ThingSpeak.writeField(myChannelNumber, 1, ppm_pwm, myWriteAPIKey);
  
  // Check the return code
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  delay(5000); // Wait 20 seconds before sending a new value
}
