int relayPin = 7;

void setup() {
  pinMode(relayPin,OUTPUT);
  digitalWrite(relayPin, LOW);
  Serial.begin(9600);
  Serial.println("PDLC 제어 준비 완료. 1=ON / 0=OFF 입력");
}

void loop() {
   if (Serial.available() > 0) {
    char cmd = Serial.read();

    if (cmd == '1') {
      digitalWrite(relayPin, HIGH); // 릴레이 ON
      Serial.println("PDLC ON");
    }
    else if (cmd == '0') {
      digitalWrite(relayPin, LOW); // 릴레이 OFF
      Serial.println("PDLC OFF");
    }
  }
}
