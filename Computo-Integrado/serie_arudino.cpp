const int buttonPin = 8;  // Pin del primer botón
const int accelButtonPin = 9;  // Pin del segundo botón

bool sequenceActive = false;  // Estado de la secuencia
unsigned long lastUpdateTime = 0;
unsigned long updateInterval = 3000;  // Intervalo inicial entre LEDs en ms
const unsigned long minInterval = 10;  // Intervalo mínimo en ms
const unsigned long initialInterval = 3000; // Intervalo inicial de reinicio
int ledIndex = -1;
int direction = 1;
int LEDS[] = {5,4,3,2,1,0};

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(accelButtonPin, INPUT);
  for (int i = 0; i < int(sizeof(LEDS)); i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
}
void turnOffLights() {
  for (int i = 0; i < int(sizeof(LEDS)); i++){
    digitalWrite(i, LOW);
  }
}
void loop() {
  bool buttonState = digitalRead(buttonPin);
  bool accelButtonState = digitalRead(accelButtonPin);

  static bool lastButtonState = LOW;
  if (buttonState != lastButtonState && buttonState == HIGH) {
    sequenceActive = !sequenceActive;
    if (!sequenceActive) {
      turnOffLights();
      ledIndex = -1;
      updateInterval = initialInterval;  // Reinicia el intervalo al valor inicial
    } else {
      ledIndex = 5;
      direction = 1;
      lastUpdateTime = millis();
      updateInterval = initialInterval;  // Reinicia el intervalo al valor inicial
    }
  }
  lastButtonState = buttonState;

  // Manejo del segundo botón (acelerar la secuencia)
  static bool lastAccelButtonState = LOW;
  if (accelButtonState != lastAccelButtonState && accelButtonState == HIGH) {
    // Reducir el intervalo en un 10% como si hubiera habido un rebote
    updateInterval = max(minInterval, (unsigned long)(updateInterval * 0.9));
  }
  lastAccelButtonState = accelButtonState;

  // Reinicio del intervalo si llega a 10 ms o menos
  if (updateInterval <= minInterval) {
    updateInterval = initialInterval;
  }

  if (sequenceActive) {
    unsigned long currentTime = millis();
    if (currentTime - lastUpdateTime >= updateInterval) {
      // Apaga el LED actual
      digitalWrite(ledIndex, LOW);

      // Avanza al siguiente LED en la dirección actual
      ledIndex += direction;
      if (ledIndex > 5 || ledIndex < 0) {
        direction = -direction;
        ledIndex += direction;
      }

      // Enciende el LED actual
      digitalWrite(ledIndex, HIGH);

      lastUpdateTime = currentTime;
    }
  }
}
