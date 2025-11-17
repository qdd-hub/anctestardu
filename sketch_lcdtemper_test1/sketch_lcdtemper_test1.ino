/*
  온습도 센서(DHT11) 값을 I2C LCD에 표시하고,
  시리얼 모니터에 측정 로그를 출력하는 스케치입니다.
*/

// 1. 라이브러리 포함
#include <Wire.h>                 // I2C 통신을 위한 라이브러리
#include <LiquidCrystal_I2C.h>    // I2C LCD 라이브러리
#include <DHT.h>                  // DHT 센서 라이브러리

// 2. 핀 및 센서 타입 설정
#define DHTPIN 7       // DHT11 센서의 DATA 핀을 2번 핀에 연결
#define DHTTYPE DHT11  // 사용하는 센서 타입 (DHT11)

// 3. 객체 생성
// LCD 객체 생성 (I2C 주소, LCD 열, LCD 행)
// ※ LCD 주소는 0x27 또는 0x3F가 일반적입니다. 안되면 0x3F로 바꿔보세요.
LiquidCrystal_I2C lcd(0x27, 16, 2);  

// DHT 객체 생성
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // 시리얼 모니터 시작 (로그 출력을 위해)
  Serial.begin(9600);
  Serial.println("--- 온습도 측정 시작 ---");

  // LCD 시작
  lcd.begin(16 , 2);
  lcd.backlight(); // 백라이트 켜기
  lcd.clear();     // 화면 지우기

  // DHT 센서 시작
  dht.begin();
  
  // LCD 초기 메시지
  lcd.setCursor(0, 0); // 1번째 줄, 1번째 칸
  lcd.print("Reading Sensor");
  delay(1000);
  lcd.clear();
}

void loop() {
  // 2초마다 온습도 측정 (DHT11은 측정 주기가 2초 정도 필요)
  delay(2000);

  // 온습도 값 읽기
  float h = dht.readHumidity();    // 습도(%)
  float t = dht.readTemperature(); // 온도(섭씨, °C)

  // 값 읽기 실패 시 (인터넷 연결이 아닌 센서 연결 확인)
  if (isnan(h) || isnan(t)) {
    Serial.println("DHT11 센서 읽기 실패!");
    
    // LCD에 에러 표시
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error");
    return; // loop 함수를 처음부터 다시 시작
  }

  // --- 요청사항 1: 시리얼 모니터에 로그 출력 ---
  Serial.print("측정 시간: ");
  Serial.print(millis() / 1000); // 프로그램 시작 후 경과 시간(초)
  Serial.print("초, ");
  Serial.print("습도: ");
  Serial.print(h);
  Serial.print(" %, ");
  Serial.print("온도: ");
  Serial.print(t);
  Serial.println(" °C");

  // --- 요청사항 2: LCD 화면에 온습도 표시 ---
  
  lcd.clear(); // 이전 값 지우기

  // 1번째 줄: 온도 표시
  lcd.setCursor(0, 0); // 1번째 줄, 1번째 칸
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print(" C");

  // 2번째 줄: 습도 표시
  lcd.setCursor(0, 1); // 2번째 줄, 1번째 칸
  lcd.print("Humi: ");
  lcd.print(h);
  lcd.print(" %");

  
}