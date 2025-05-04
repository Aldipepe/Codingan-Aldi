#include <WiFi.h>
#include <HTTPClient.h>

// SSID dan password WiFi
const char* ssid = "OPPO Reno8 T";        // Ganti dengan SSID WiFi Anda
const char* password = "Aldi1409";     // Ganti dengan password WiFi Anda

// URL Google Apps Script Web App
const char* scriptUrl = "https://script.google.com/macros/s/AKfycbzKPBRLbe0rSFFmYfdFZNmWz6oHt-njEaF1dZbnz5juciPizZ3YCDaYz50DVP_46p3u5Q/exec";  // Ganti dengan URL Web App Anda

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
    String ppmCH4 = data.substring(0, data.indexOf(','));
    data.remove(0, data.indexOf(',') + 1);
    String ppmCO2 = data.substring(0, data.indexOf(','));
    data.remove(0, data.indexOf(',') + 1);
    String ppmH2S = data.substring(0, data.indexOf(','));
    data.remove(0, data.indexOf(',') + 1);
    String ppmNH3 = data.substring(0, data.indexOf(','));
    data.remove(0, data.indexOf(',') + 1);
    String redValue = data.substring(0, data.indexOf(','));
    data.remove(0, data.indexOf(',') + 1);
    String greenValue = data.substring(0, data.indexOf(','));
    data.remove(0, data.indexOf(',') + 1);
    String blueValue = data.substring(0, data.indexOf(','));
    data.remove(0, data.indexOf(',') + 1);
    String pHValue = data;

    // Buat HTTP request ke Google Apps Script
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      // Buat URL dengan parameter untuk Google Apps Script
      String url = String(scriptUrl) + "?temperature=" + t +
                   "&humidity=" + h +
                   "&ppmCH4=" + ppmCH4 +
                   "&ppmCO2=" + ppmCO2 +
                   "&ppmH2S=" + ppmH2S +
                   "&ppmNH3=" + ppmNH3 +
                   "&redValue=" + redValue +
                   "&greenValue=" + greenValue +
                   "&blueValue=" + blueValue +
                   "&pHValue=" + pHValue;

      Serial.println("Sending Data:");
      Serial.println("Temperature: " + t);
      Serial.println("Humidity: " + h);
      Serial.println("CH4 (ppm): " + ppmCH4);
      Serial.println("CO2 (ppm): " + ppmCO2);
      Serial.println("H2S (ppm): " + ppmH2S);
      Serial.println("NH3 (ppm): " + ppmNH3);
      Serial.println("Red: " + redValue);
      Serial.println("Green: " + greenValue);
      Serial.println("Blue: " + blueValue);
      Serial.println("pH: " + pHValue);

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