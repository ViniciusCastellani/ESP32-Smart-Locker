#include <Arduino.h>
#include <WiFi.h>
#include <Firebase.h>
#include <ESP32Servo.h>

// * WiFi.
const char *WIFI_SSID = "COFRINHOHIGHTECH";
const char *WIFI_PASSWORD = "cofre1234";

// * Firebase.
const char *FIREBASE_URL = "https://projeto-segundo-semestre-default-rtdb.firebaseio.com";
const char *FIREBASE_SECRET = "GbkVEVm4Clqkq85kS00uBRr3UqvV8Q49nU1ZybEh";
FirebaseData data;

// * Outros.
const int LED_PIN = 2;
const int SERVO_PIN = 33;
int pos = 0;
Servo servo;

void setup()
{
  // * Configuração de Outros.
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	servo.setPeriodHertz(50);    // standard 50 hz servo
	servo.attach(SERVO_PIN, 1000, 2000);
  Serial.begin(9600);
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

  if (Firebase.beginStream(data, "/portas/b9737ebe-42a9-11ee-be56-0242ac120002/isOpen"))
  {
    Serial.println("[+] Transmissão do Firebase Iniciada.");
  }
}

void loop()
{
  if (Firebase.ready())
  {
    if (Firebase.readStream(data) && data.to<bool>() )
    {
      Serial.println("acendeu");
      digitalWrite(LED_PIN, HIGH);
      servo.write(180); 
    }
    else
    {
      Serial.println("apago");
      digitalWrite(LED_PIN, LOW);
      servo.write(0);    
    }
  }
}