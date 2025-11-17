const int micPrimary = A0;   // 주경로 마이크
const int micReference = A1; // 보조경로 마이크

const int SAMPLE_DELAY = 2; // 2ms 샘플링 ~500Hz

void setup() {
  Serial.begin(115200); // PC와 시리얼 통신
}

void loop() {
  int valPrimary = analogRead(micPrimary);
  int valRef     = analogRead(micReference);

  // 시리얼로 "주경로,보조경로" 전송
  Serial.print(valPrimary);
  Serial.print(",");
  Serial.println(valRef);

  delay(SAMPLE_DELAY);
}
