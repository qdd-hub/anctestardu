const int micL = A0;
const int micR = A1;

const int SAMPLE_DELAY = 2; // 2ms 샘플링 -> 약 500Hz

void setup() {
  Serial.begin(9600);
}

void loop() {
  int valL = analogRead(micL);
  int valR = analogRead(micR);

  // 시리얼 플로터용 형식: "valL,valR"
  Serial.print(valL);
  Serial.print(",");
  Serial.println(valR);

  delay(SAMPLE_DELAY);
}
