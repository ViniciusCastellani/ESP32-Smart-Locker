#include <Arduino.h>
#include <WiFi.h>
#include <Firebase.h>

// * WiFi.
const char *SSID = "WIFI_SSID";
const char *PASSWORD = "WIFI_PASSWORD";

// * Firebase.
const char *URL = "FIREBASE_URL";
const char *SECRET = "FIREBASE_SECRET";
FirebaseData data;

// * Outros.
const int LED_PIN = 2;

void setup()
{
  // * Configuração de Outros.
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

  // * Configuração do Wi-Fi.
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("[*] Conectando ao Wi-Fi.");
  }

  Serial.println("[+] Conectado à rede Wi-Fi.");

  // * Configuração do Firebase.
  Firebase.begin(URL, SECRET);

  if (Firebase.beginStream(data, "cofre/aberto"))
  {
    Serial.println("[+] Stream iniciada.");
  }
}

void loop()
{
  if (Firebase.ready() && Firebase.readStream(data) && data.to<bool>())
  {
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
  }
}