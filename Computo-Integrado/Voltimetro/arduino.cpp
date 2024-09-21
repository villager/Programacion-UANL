void setup(){
  Serial.begin(9600);
  pinMode(7, oUTPUT);
}

void loop(){
  float v = analogRead(0) * 0.0048;
  if (v >= 0.11) {
    if (v >= 1.0) {
      Serial.print(v);
      Serial.println();
      digitalWrite(7, LOW);    
    } else {
        Serial.print(v);
        Serial.println();
        digitalWrite(7, HIGH);
    }
  }
  delay(500);
}