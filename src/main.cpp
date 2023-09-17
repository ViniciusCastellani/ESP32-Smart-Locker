#include <Arduino.h>
#include <WiFi.h>
#include <Firebase.h>
#include <ESP32Servo.h>

// * WiFi.
const char *WIFI_SSID = "WIFI_SSID";
const char *WIFI_PASSWORD = "WIFI_PASSWORD";

// * Firebase.
const char *FIREBASE_URL = "FIREBASE_URL";
const char *FIREBASE_SECRET = "FIREBASE_SECRET";
FirebaseData data;

// * Outros.
const int LED_PIN = 2;
const int SERVO_PIN = 13;
Servo servo;

void setup()
{
  // * Configuração de Outros.
  Serial.begin(9600);
  servo.attach(SERVO_PIN);
  pinMode(LED_PIN, OUTPUT);

  // * Configuração do Wi-Fi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("[*] Conectando ao Wi-Fi.");
  }

  Serial.println("\n[+] Conectado à rede Wi-Fi.");

  // * Configuração do Firebase.
  Firebase.begin(FIREBASE_URL, FIREBASE_SECRET);

  if (Firebase.beginStream(data, "cofre/aberto"))
  {
    Serial.println("[+] Transmissão do Firebase Iniciada.");
  }
}

void loop()
{
  if (Firebase.ready())
  {
    if (Firebase.readStream(data) && data.to<bool>())
    {
      digitalWrite(LED_PIN, HIGH);
      servo.write(0);
    }
    else
    {
      digitalWrite(LED_PIN, LOW);
      servo.write(90);
    }
  }
}