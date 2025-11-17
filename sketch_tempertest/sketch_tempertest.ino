#include "DHT.h"

#define DHTPIN A0       // DHT 센서가 연결된 핀 번호
#define DHTTYPE DHT11  // 사용 중인 DHT 센서의 종류(DHT11)

DHT dht(DHTPIN, DHTTYPE); // DHT 센서 객체 생성

void setup() {
  Serial.begin(9600);  // 시리얼 통신 시작
  dht.begin();         // DHT 센서 초기화
}

void loop() {
  float humidity = dht.readHumidity();       // 습도 값 읽기
  float temperature = dht.readTemperature(); // 온도 값 읽기
 
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!"); // 읽기 실패 시 에러 메시지 출력
    return;
  }

  // 온도와 습도 값을 시리얼 모니터에 출력
  Serial.print("Temperature: ");
  Serial.print((int)temperature);
  Serial.print(" *C, ");
  Serial.print("Humidity: ");
  Serial.print((int)humidity);
  Serial.println(" %");

  delay(2000); // 2초 대기 후 다시 측정
}