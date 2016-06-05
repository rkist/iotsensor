#include "DhtSensor.h"
#include <OneWire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <DallasTemperature.h>
#define DS18B20PIN 12
#define SENSOR_RESOLUTION 12 // How many bits to use for temperature values: 9, 10, 11 or 12
#define SENSOR_INDEX 0 // Index of sensors connected to data pin, default: 0




//#define RED_LED 4
#define WHITE_LED 5
#define RESPONSE_LED LED_BUILTIN
#define DHTPIN  13





const char* ssid     = "Kist_2.4";
const char* password = "valar morghulis";

ESP8266WebServer server(80);



float humidity, temp;  // Values read from sensor
String webString = "";   // String to display
// Generally, you should use "unsigned long" for variables that hold time
unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 2000;              // interval at which to read sensor


//DS18B20
OneWire oneWire(DS18B20PIN);
DallasTemperature sensors(&oneWire);
DeviceAddress sensorDeviceAddress;

float temperatureInCelsius;

DhtSensor sensor1(DHTPIN);




void readSensor()
{
  // Wait at least 2 seconds seconds between measurements.
  // if the difference between the current time and last time you read
  // the sensor is bigger than the interval you set, read the sensor
  // Works better than delay for things happening elsewhere also
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you read the sensor
    previousMillis = currentMillis;

    sensors.requestTemperatures(); // Measurement may take up to 750ms
    temperatureInCelsius = sensors.getTempCByIndex(SENSOR_INDEX); 

    humidity = sensor1.ReadHumidity();
    temp = sensor1.ReadTemperature();
  }
}

void handle_root()
{
  digitalWrite(RESPONSE_LED, 1);
  server.send(200, "text/html", "<html><body>Read from <a href=\"/humidity\">/humidity</a>, <a href=\"/temp\">/temp</a> or <a href=\"/all\">/all</a></html></body>");
  delay(100);
  digitalWrite(RESPONSE_LED, 0);
}

void handle_temp()
{
  digitalWrite(RESPONSE_LED, 1);
  readSensor();       // read sensor
  webString = "Temperature01: " + String((int)temp) + " C"; // Arduino has a hard time with float to string
  webString += "\n";
  webString += "Temperature02: " + String(temperatureInCelsius) + " C";
  server.send(200, "text/plain", webString);            // send to someones browser when asked
  digitalWrite(RESPONSE_LED, 0);
}

void handle_humidity()
{
  digitalWrite(RESPONSE_LED, 1);
  readSensor();           // read sensor
  webString = "Humidity: " + String((int)humidity) + "%";
  server.send(200, "text/plain", webString);               // send to someones browser when asked
  digitalWrite(RESPONSE_LED, 0);
}

void handle_all()
{
  digitalWrite(RESPONSE_LED, 1);
  digitalWrite(WHITE_LED, 1);
  readSensor();       // read sensor
  webString = "Temperature01: " + String((int)temp) + " C"; // Arduino has a hard time with float to string
  webString += "\n";
  webString += "Temperature02: " + String(temperatureInCelsius) + " C";
  webString += "\n";
  webString += "Humidity: " + String((int)humidity) + "%";
  server.send(200, "text/plain", webString);            // send to someones browser when asked
  digitalWrite(WHITE_LED, 0);  
  digitalWrite(RESPONSE_LED, 0);
}

void handle_notFound()
{
  digitalWrite(RESPONSE_LED, 1);
  Serial.println("[HTTP] 404 response");
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(RESPONSE_LED, 0);
}


void setup(void)
{
  // You can open the Arduino IDE Serial Monitor window to see what the code is doing
  Serial.begin(115200);  // Serial connection from ESP-01 via 3.3v console cable

  sensors.begin();
  sensors.getAddress(sensorDeviceAddress, 0);
  sensors.setResolution(sensorDeviceAddress, SENSOR_RESOLUTION);

  pinMode(RESPONSE_LED, OUTPUT);
  pinMode(WHITE_LED, OUTPUT);
//  pinMode(RED_LED, OUTPUT);
    

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rWorking to connect");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("DHT Weather Reading Server");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_root);
  server.on("/temp", handle_temp);
  server.on("/humidity", handle_humidity);
  server.on("/all", handle_all);
  server.onNotFound(handle_notFound);

//  digitalWrite(RED_LED, 1);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void)
{
  server.handleClient();
}

