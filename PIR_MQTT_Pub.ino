/* This sketch allows an ESP8266-12 to publish a PIR event to a MQTT server topic.
 * Once it triggers an event, the relay will be activated for at least 30 seconds.
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "ssid";
const char* password = "router password";
const char* server = "192.168.x.x";
const char* pirtopic = "/pir/trigger";
const int CSpin = 10;

int ledPin = 4; // LED connected to digital pin 4;
int inPin = 5;   // sensor connected to digital pin 5
int val = 0;     // variable to store the read value
bool relayState = LOW;

WiFiClient wifiClient;
PubSubClient client(server, 1883, wifiClient);

void setup () {
  Serial.begin(115200);
  delay(10);                            // Connect to wifi
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
}
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


//  Copied from Pubsub

 if (client.connect("ESP8266Client")){
    Serial.println("Connected to MQTT broker");
    Serial.print("Topic is: ");
    Serial.println(pirtopic);
    
    if (client.publish(pirtopic, "hello from ESP8266")) {
      Serial.println("Publish ok");
    }
    else {
      Serial.println("Publish failed");
    }
  }
  else {
    Serial.println("MQTT connect failed");
    Serial.println("Will reset and try again...");
    abort();
  }
}
void loop() {
 
 String payload = "0";
   
    if (client.connect("ESP8266Client")) {
      Serial.println(" ");
      Serial.println("connected");
                
    //Digital Read section
     val = digitalRead(inPin);   // read the input pin 5

      Serial.print("PIR Pin value: ");
      Serial.println(val);
             
      if (val == 1)
      {
        client.publish(pirtopic,"1");
        Serial.println("Publish 1 - Event triggered!");
         delay(30000);    // Wait a few seconds between measurements.
      }
      else if (val != 1)
    {  
        client.publish(pirtopic,"0");
        Serial.println("Publish 0 - No Event.");
         delay(5000);    // Wait a few seconds between measurements.
      }
      
    else {
        Serial.println("Publish failed");
      }
      }
   }

