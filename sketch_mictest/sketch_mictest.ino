const int sensorPin1 = A0; // 첫 번째 마이크

void setup() {
  Serial.begin(115200); 
}

void loop() {
  int sensorValue1 = analogRead(sensorPin1);
  
  // ★★★
  // 값을 출력한 뒤 "줄바꿈"을 해야 
  // 시리얼 플로터가 그래프를 그릴 수 있습니다.
  Serial.println(sensorValue1); 
  // ★★★
  
  // (공백을 출력하는 Serial.print(" "); 는 제거합니다)
}