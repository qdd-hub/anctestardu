const int MIC_PIN = A0;     // 마이크 입력
const int VIB_PIN = 9;      // 진동자 출력 (PWM 가능 핀)

void setup() {
  Serial.begin(115200);     // PC와 통신속도
  pinMode(MIC_PIN, INPUT);
  pinMode(VIB_PIN, OUTPUT);
}

void loop() {
  // 1) 마이크 값 읽기 (0~1023)
  int micValue = analogRead(MIC_PIN);

  // 2) PC로 전송 ("mic:123\n" 형식)
  Serial.print("mic:");
  Serial.println(micValue);

  // 3) PC에서 역파형 값(y)을 수신
  if (Serial.available() > 0) {
    int yValue = Serial.parseInt();  // -255 ~ +255 형태

    // PWM 출력은 0~255라서 127 기준으로 오프셋 필요
    int pwmValue = constrain(yValue + 127, 0, 255);
    analogWrite(VIB_PIN, pwmValue);
  }
}
