#include <ESP8266WiFi.h>
#include <WiFiConnector.h>
#include <FirebaseArduino.h>

WiFiConnector *wifi;
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <DHT.h>
#include <ESP_Adafruit_SSD1306.h>

#include "declare.h"
#include "init_wifi.h"

void init_hardware()
{
    Serial.begin(115200);
    delay(10);
    Serial.println();
    Serial.println("BEGIN");
}

//==================================================
void setup()
{
    //==================================================

    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, HIGH);

    dht.begin(); // dht begin

    delay(2000);

    // WiFi Connect
    Serial.print("CONNECTING TO ");
    Serial.println(wifi->SSID() + ", " + wifi->psk());

    wifi->on_connecting([&](const void *message) {
        char buffer[70];
        //sprintf(buffer, "[%d] connecting -> %s ", wifi->counter, (char*) message);
        //Serial.println(buffer);

        delay(500);
        Serial.print(".");
        //lp++;
        //if(lp>=20) break;
    });

    wifi->on_connected([&](const void *message) {
        // Print the IP address
        Serial.print("WIFI CONNECTED ");
        Serial.println(WiFi.localIP());
    });

    wifi->connect();

    //WiFi.begin(ssid, password);
    Serial.println(""); // Clear the buffer.

    while (WiFi.status() != WL_CONNECTED)
    { // Wait for connection
        delay(500);
        Serial.print(".");
        lp++;
        if (lp >= 20)
            break;
    }

    Serial.println("");
    Serial.print("Connected to ");
    //Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    delay(2000);

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

} //setup

//===============================================================
void loop()
{

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;
        digitalWrite(BUILTIN_LED, LOW);

        // Reading temperature or humidity takes about 250 milliseconds!
        // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
        float h = dht.readHumidity();
        // Read temperature as Celsius
        float t = dht.readTemperature();

        // Check if any reads failed and exit early (to try again).
        if (isnan(h) || isnan(t))
        {
            Serial.println("Failed to read from DHT sensor!");

            h = 0;
            t = 0;
            //delay(2000);
            //return;
        }

        // show in serial monitor
        Serial.print("Humidity: ");
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("Temp: ");
        Serial.print(t);
        Serial.print(" *C \n");

        lp++;
        if (lp >= lp_time)
        {
            lp = 0;

            StaticJsonBuffer<200> jsonBuffer;
            JsonObject &root = jsonBuffer.createObject();
            root["temperature"] = t;
            root["humidity"] = h;

            // append a new value to /logDHT
            String name = Firebase.push("/sensor/dht", root);
            // handle error
            if (Firebase.failed())
            {
                Serial.print("Firebase Pushing /sensor/dht failed:");
                Serial.println(Firebase.error());

                digitalWrite(BUILTIN_LED, HIGH);
                delay(2000);
                lp = 10; // repeat to sent again
                return;
            }
            else
            {
                Serial.print("Firebase Pushed /sensor/dht ");
                Serial.println(name);
                digitalWrite(BUILTIN_LED, HIGH);
                delay(2000);
            }
        }
        digitalWrite(BUILTIN_LED, HIGH);
    }

} //Loop
