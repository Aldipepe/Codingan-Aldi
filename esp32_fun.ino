#include <WiFi.h>
#include <HTTPClient.h>

// SSID dan password WiFi
const char* ssid = "vivo 1910";        // Ganti dengan SSID WiFi Anda
const char* password = "aldi7179";     // Ganti dengan password WiFi Anda

// URL Google Apps Script Web App
const char* scriptUrl = "https://script.google.com/macros/s/AKfycbzKIyCmcIxaG60dYoSJeirfG8wJXimZrJ-LlRIf3cZAH0WdYNMdWR3z4MciAB3DMt1M4w/exec";  // Ganti dengan URL Web App Anda

void setup() {
  // Inisialisasi komunikasi Serial
  Serial.begin(9600);
  Serial1.begin(115200, SERIAL_8N1, 16, 17);  // GPIO16 (RX), GPIO17 (TX)

  // Hubungkan ke WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi!");
}

void loop() {
  if (Serial1.available()) {
    String data = Serial1.readStringUntil('\n');  // Baca data dari Arduino Mega

    // Pisahkan data untuk Google Sheets
    String t = data.substring(0, data.indexOf(','));
    data.remove(0, data.indexOf(',') + 1);
    String h = data.substring(0, data.indexOf(','));
    data.remove(0, data.indexOf(',') + 1);
    String mq4Value = data.substring(0, data.indexOf(','));
    data.remove(0, data.indexOf(',') + 1);
    String mq135Value = data.substring(0, data.indexOf(','));
    data.remove(0, data.indexOf(',') + 1);
    String mq136Value = data.substring(0, data.indexOf(','));
    data.remove(0, data.indexOf(',') + 1);
    String mq1352Value = data.substring(0, data.indexOf(','));
    data.remove(0, data.indexOf(',') + 1);
    String pH = data.substring(0, data.indexOf(','));
    data.remove(0, data.indexOf(',') + 1);
    String redValue = data.substring(0, data.indexOf(','));
    data.remove(0, data.indexOf(',') + 1);
    String greenValue = data.substring(0, data.indexOf(','));
    data.remove(0, data.indexOf(',') + 1);
    String blueValue = data;

    // Buat HTTP request ke Google Apps Script
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      // Buat URL dengan parameter untuk Google Apps Script
      String url = String(scriptUrl) + "?temperature=" + t +
                   "&humidity=" + h +
                   "&mq4=" + mq4Value +
                   "&mq135=" + mq135Value +
                   "&mq136=" + mq136Value +
                   "&mq1352=" + mq1352Value +
                   "&ph=" + pH +
                   "&red =" + redValue +
                   "&green =" + greenValue +
                   "&blue =" + blueValue;

      Serial.println("Sending Data:");
Serial.println("Temperature: " + t);
Serial.println("Humidity: " + h);
Serial.println("MQ4 Value: " + mq4Value);
Serial.println("MQ135 Value: " + mq135Value);
Serial.println("MQ136 Value: " + mq136Value);
Serial.println("MQ1352 Value: " + mq1352Value);
Serial.println("pH: " + pH);
Serial.println("Red: " + redValue);
Serial.println("Green: " + greenValue);
Serial.println("Blue: " + blueValue);


      http.begin(url);
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("Response: " + response);
      } else {
        Serial.println("Error on sending GET request: " + String(httpResponseCode));
      }

      http.end();  // Tutup koneksi
    }

    delay(2000);  // Tunggu sebelum loop berikutnya
  }
}
