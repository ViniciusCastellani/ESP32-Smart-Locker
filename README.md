# Smart Locker Project

This project showcases how to build a smart locker using an ESP32. The locker can connect to Wi-Fi and Firebase to control a servo motor and an LED based on the state of a remote database entry. This allows for secure and remote control of the locker.

## Features
- Connects to a Wi-Fi network.
- Uses Firebase Realtime Database for remote control.
- Controls a servo motor to lock or unlock the locker.
- Provides visual feedback with an LED.

## Components Needed
- ESP32 board
- Servo motor
- LED
- Resistor (220 ohms)
- Jumper wires
- Breadboard

## Circuit Connections
1. **Servo Motor:**
   - Connect the signal pin of the servo motor to pin `33` on the ESP32.
   - Connect the power (`VCC`) and ground (`GND`) pins of the servo motor to the ESP32's `3.3V` and `GND` pins, respectively.
2. **LED:**
   - Connect the positive leg (anode) of the LED to pin `2` on the ESP32 through a 220-ohm resistor.
   - Connect the negative leg (cathode) to `GND`.
3. **Power Supply:**
   - Ensure the ESP32 and servo motor are powered adequately.

## Code Explanation
The provided code connects the ESP32 to Wi-Fi and Firebase, reads the state of a specific database entry, and controls the servo motor and LED accordingly:

- **Wi-Fi:**
  - Connects to the network defined by `WIFI_SSID` and `WIFI_PASSWORD`.
- **Firebase:**
  - Connects to the database using `FIREBASE_URL` and `FIREBASE_SECRET`.
  - Monitors the path `/portas/b9737ebe-42a9-11ee-be56-0242ac120002/isOpen` for changes.
- **Servo Motor:**
  - Moves to 180 degrees when the locker is open and to 0 degrees when it is closed.
- **LED:**
  - Turns on when the locker is open and off when it is closed.

### Code
```cpp
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
```

## How to Use
1. Connect all components as described above.
2. Upload the code to your ESP32 board.
3. Power the ESP32 and ensure the Wi-Fi network is available.
4. Use the Firebase Realtime Database to control the locker state:
   - Set the value of `/portas/b9737ebe-42a9-11ee-be56-0242ac120002/isOpen` to `true` to open the locker.
   - Set the value to `false` to close the locker.

## Notes
- Ensure the servo motor is powered adequately to avoid instability.
- Use the Serial Monitor for debugging connection or Firebase issues.

## Future Improvements
- Add authentication to enhance Firebase security.
- Integrate a keypad or RFID for local control.
- Add a battery backup to keep the locker functional during power outages.

---
Enjoy building your smart locker!

