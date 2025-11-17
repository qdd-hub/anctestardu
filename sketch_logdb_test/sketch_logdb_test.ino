/*
  [통합 스케치]
  - DHT11 온습도 센서 값을 I2C LCD에 표시
  - 동시에 Wi-Fi를 통해 Python/PHP 웹 서버로 데이터를 전송
*/

// 1. 라이브러리 포함 (모두 합치기)
#include <ESP8266WiFi.h> // 또는 ESP32의 경우 <WiFi.h>
#include <HTTPClient.h>  // <-- Wi-Fi 전송용
#include <Wire.h>        // <-- LCD용
#include <LiquidCrystal_I2C.h> // <-- LCD용
#include <DHT.h>         // <-- 센서용

// 2. Wi-Fi 및 서버 정보 (이전 코드에서 가져옴)
const char* ssid = "503_5G";
const char* password = "sunmoon2024"; // 비밀번호 없으면 이 줄 주석 처리
const char* serverUrl = "http://127.0.0.1:5000/add_log"; // Python 서버 주소

// 3. 센서 및 LCD 설정 (새 코드에서 가져옴)
#define DHTPIN 7      // DHT11 센서 DATA 핀 (D7 아님, GPIO 7번)
#define DHTTYPE DHT11 // 사용하는 센서 타입

LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD 객체 생성
DHT dht(DHTPIN, DHTTYPE);           // DHT 객체 생성

// 4. setup() 함수 (합치기)
void setup() {
  // 시리얼 모니터 시작 (로그 출력을 위해)
  Serial.begin(115200); // ESP 보드는 115200 권장
  Serial.println("--- 통합 스케치 시작 ---");

  // LCD 시작
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  // Wi-Fi 연결 시작 (비밀번호 없으면 ssid만)
  WiFi.begin(ssid); 
  // WiFi.begin(ssid, password); // 비밀번호 있으면 이걸 사용
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(1500);

  // DHT 센서 시작
  dht.begin();
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Reading Sensor");
  delay(1000);
  lcd.clear();
}

// 5. loop() 함수 (합치기)
void loop() {
  // 2초마다 온습도 측정 (DHT11 최소 주기)
  delay(2000);

  // 온습도 값 읽기
  float h = dht.readHumidity();    // 습도(%)
  float t = dht.readTemperature(); // 온도(섭씨, °C)

  // 값 읽기 실패 시
  if (isnan(h) || isnan(t)) {
    Serial.println("DHT11 센서 읽기 실패!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error");
    return;
  }

  // --- 시리얼 모니터에 로그 출력 (원래 코드) ---
  Serial.print("습도: ");
  Serial.print(h);
  Serial.print(" %, ");
  Serial.print("온도: ");
  Serial.print(t);
  Serial.println(" °C");

  // --- LCD 화면에 온습도 표시 (원래 코드) ---
  lcd.clear();
  lcd.setCursor(0, 0); // 1번째 줄
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print(" C");
  lcd.setCursor(0, 1); // 2번째 줄
  lcd.print("Humi: ");
  lcd.print(h);
  lcd.print(" %");

  // --- DB 서버로 데이터 전송 (추가된 부분) ---
  // 1. 온도 값 전송
  sendDataToDB("temperature", String(t));
  
  // 2. 습도 값 전송 (서버 부담을 줄이기 위해 아주 잠깐 쉼)
  delay(100); 
  sendDataToDB("humidity", String(h));
}

// 6. 데이터를 DB로 전송하는 함수 (이전 코드를 함수로 만듦)
void sendDataToDB(String sensorType, String value) {
  
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverUrl); // HTTP 연결 시작
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // 폼 데이터 형식

    // 보낼 데이터 (예: sensor_type=temperature&value=25.5)
    String postData = "sensor_type=" + sensorType + "&value=" + value;

    // POST 요청 보내기
    int httpResponseCode = http.POST(postData);

    // 시리얼 모니터에 전송 결과 로그 출력
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("--- DB " + sensorType + " 전송 결과 ---");
      Serial.println(httpResponseCode); // 201 또는 200이 오면 성공
      Serial.println(response); // 서버(Python)가 보낸 응답
    } else {
      Serial.println("Error sending " + sensorType + " POST: " + String(httpResponseCode));
    }

    http.end(); // HTTP 연결 종료
    
  } else {
    Serial.println("WiFi Disconnected. Cannot send " + sensorType);
  }
}