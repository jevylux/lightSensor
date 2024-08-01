 /*
  Mini D1 with BH1750 to measure light intensity and sending with MQTT
*/
#include <WiFi.h>
#include <Wire.h>
#include <BH1750.h>
#include <PubSubClient.h>

//Define the BH1750 object

BH1750 lightMeter;

/* network definition */
const char* ssid = "YourSSID";
const char* password = "YourPassword";

/* mqtt setting */
const char* mqtt_server = "YourMqttServerIP";
const char *topic = "YourTopic";
const char *mqtt_username = "YourMQTTUserid";
const char *mqtt_password = "YourMQTTPassword";
const int mqtt_port = 1883;

// initiate wifi and mqtt clients

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// wifi setup function

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
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
}

// reconnect function

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    String client_id = "YourMQTTClientID";
    Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.publish("YourTopic/IP", WiFi.localIP().toString().c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


// setup

void setup(){
  Serial.begin(9600);
  delay(100);
  Serial.println("Entering setup");
  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  // For Wemos / Lolin D1 Mini Pro and the Ambient Light shield use Wire.begin(D2, D1);

  lightMeter.begin();
  delay(100);
  Serial.println("BH1750 Test begin");
  
  // setup wifi
  
  setup_wifi();
  delay(100);

  // setup mqtt client
  
  Serial.println("Starting MQTT connection");
  client.setServer(mqtt_server, mqtt_port);
  //client.setCallback(callback);
  while (!client.connected()) {
    String client_id = "YourMQTTClientID";
    Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
        Serial.println("MQTT broker connected");
        client.publish("YourTOPIC/IP", WiFi.localIP().toString().c_str());
    } else {
        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(2000);
    }

  }
}

// Loop and send the light intensity value via mqtt every second

void loop() {
  float lux = lightMeter.readLightLevel();
  client.loop();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  // Convert the value to a char array
  char tempString[8];
  dtostrf(lux, 1, 2, tempString);
  Serial.print("Light (String): ");
  Serial.println(tempString);
  Serial.println("publishing to mqtt ...");
  // publish the topic
  client.publish("YourTopic/Lux", tempString);
  delay(1000);
}
