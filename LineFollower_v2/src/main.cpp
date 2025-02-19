#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "INFINITUM57C7_2.4";
const char* password = "";

// Direccion del servidor PHP
const char* serverName = "http://127.0.0.1/ejemplo/insert_data.php";

#define ENA          16
#define IN1          17
#define IN2           5
#define IN3          18
#define IN4          19
#define ENB          21
#define infrasensorR 32
#define infrasensorL 35

int valSensorL;
int valSensorR;

#define delayts 1000

void forward() {
  digitalWrite(ENA, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void left() {
  digitalWrite(ENA, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(ENB, LOW);
}

void right() {
  digitalWrite(ENB, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(ENA, LOW);
}

void stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a la red WiFi");

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(infrasensorL, INPUT);
  pinMode(infrasensorR, INPUT);
}

void loop() {
  valSensorL = digitalRead(infrasensorL);
  valSensorR = digitalRead(infrasensorR);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + String("?sensorL=") + String(valSensorL) + "&sensorR=" + String(valSensorR);
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error en la peticion: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Desconectado de WiFi");
  }

  if (valSensorL == 0 && valSensorR == 0) {
    stop();
  } else if (valSensorL == 1 && valSensorR == 0) {
    right();
  } else if (valSensorL == 0 && valSensorR == 1) {
    left();
  } else if (valSensorL == 1 && valSensorR == 1) {
    forward();
  }
  delay(delayts);
}
