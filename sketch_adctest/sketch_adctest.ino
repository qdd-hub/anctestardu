#include <SPI.h>

const int CS_PIN = 10;

// ★ MCP3008에 맞는 SPI 설정을 미리 정의합니다.
// (속도 1MHz, MSB First, 모드 0)
SPISettings settingsMCP3008(1000000, MSBFIRST, SPI_MODE0);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH); // 칩 비활성화로 시작
}

int readMCP3008(int channel) {
  if (channel < 0 || channel > 7) return -1;
  
  byte start = 0x01; // 시작 비트
  byte config = 0x80 | (channel << 4); // single-ended, 채널 설정

  // ★ 1. SPI 통신 시작 (설정 적용)
  SPI.beginTransaction(settingsMCP3008);
  
  digitalWrite(CS_PIN, LOW); // 칩 활성화

  SPI.transfer(start);             // 1. 시작 비트 전송
  byte high = SPI.transfer(config); // 2. 설정 전송 (B9, B8 수신)
  byte low = SPI.transfer(0x00);    // 3. 더미 전송 (B7~B0 수신)
  
  digitalWrite(CS_PIN, HIGH); // 칩 비활성화

  // ★ 2. SPI 통신 종료
  SPI.endTransaction();

  // 10비트 값으로 조합
  int value = ((high & 0x03) << 8) | low;
  return value;
}

void loop() {
  // 마이크가 CH0에 연결되어 있으므로 CH0만 읽어봅니다.
  // (연결 안 된 CH1,2,3은 0 또는 불안정한 값이 뜨는 게 정상)
  int v = readMCP3008(0); 
  Serial.print("CH0 (마이크): ");
  Serial.println(v);
  
  delay(100);
}