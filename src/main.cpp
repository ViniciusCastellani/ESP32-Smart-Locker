#include <WiFi.h>
#include <FirebaseESP32.h>

// WiFi.
const char *wifi_ssid = "WIFI_SSID";
const char *wifi_password = "WIFI_PASSWORD";

// Firebase.
const char *api_key = "API_KEY";
const char *database_url = "DATABASE_URL";
FirebaseData stream;

// Outros.
const int led_pin = 2;

void setup()
{
  Serial.begin(9600);
  delay(1000);

  // WiFi Setup.
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Não conectado.");
    delay(200);
  }

  Serial.println("Conectado à rede Wi-Fi.");
  Serial.print("Endereço: ");
  Serial.println(WiFi.localIP());

  // Firebase Setup.
  Firebase.begin(database_url, api_key);

  if (Firebase.beginStream(stream, "cofre/aberto"))
  {
    Serial.println("A Stream começou.");
  }

  // LED Setup.
  pinMode(led_pin, OUTPUT);
}

void loop()
{
  if (Firebase.readStream(stream) && stream.to<bool>())
  {
    Serial.println("Cofre: Aberto");
  }
  else
  {
    Serial.println("Cofre: Fechado");
  }

  delay(100);
}