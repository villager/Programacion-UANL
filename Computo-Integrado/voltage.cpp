
#define ANALOGPILA 0 
#define PIN_OUTPUT 7

int analogValor = 0;
float voltaje = 0;
int ledDelay = 800;
 
float maximo = 1.6;
float medio = 1.4;
float minimo = 0.3;
 
void setup() {
  Serial.begin(9600);
  pinMode(PIN_OUTPUT, OUTPUT);
 
}
 
void loop() {
  analogValor = analogRead(ANALOGPILA);
 
  voltaje = 0.0048 * analogValor;
  Serial.print("Voltaje: ");
  Serial.println(voltaje);
 
  if (voltaje >= maximo)
  {
    delay(ledDelay);
    digitalWrite(PIN_OUTPUT, LOW);
  }
  else if (voltaje < maximo && voltaje > medio)
  {
    digitalWrite(PIN_OUTPUT, LOW);
    delay(ledDelay);
  }
  else if (voltaje < medio && voltaje > minimo)
  {
    digitalWrite(LEDROJO, HIGH);
    delay(ledDelay);
    digitalWrite(PIN_OUTPUT, HIGH);
    digitalWrite(LEDROJO, LOW);
  }
  digitalWrite(PIN_OUTPUT, HIGH);
}