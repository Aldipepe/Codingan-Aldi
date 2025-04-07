#include <DHT.h>

// Definisi untuk sensor DHT22
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Definisi untuk sensor warna TCS3200
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

// Definisi untuk sensor GAS
int mq4Pin = A0;
int mq135Pin = A2;
int mq136Pin = A3;
int mq1352Pin = A6;
int phPin = A9;

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

float calibrationOffset = 0.00; // Anda dapat menyesuaikan nilai ini sesuai kalibrasi sensor pH Anda

void setup() {
  // Inisialisasi komunikasi serial
  Serial.begin(9600);
  
  // Inisialisasi DHT22
  dht.begin();

  // Inisialisasi komunikasi dengan ESP32
  Serial1.begin(115200);    // Komunikasi dengan ESP32
  
  // Inisialisasi pin TCS3200
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  Serial.begin(9600);

  // Setting frekuensi prescale untuk TCS3200
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
}

void loop() {
  // Membaca data dari sensor DHT22
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Cek jika pembacaan gagal
  if (isnan(h) || isnan(t)) {
    Serial.println("Gagal membaca dari DHT sensor!");
  } else {
    Serial.print("Kelembapan: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Suhu: ");
    Serial.print(t);
    Serial.println(" *C");
  }

  // Baca warna merah
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  redFrequency = pulseIn(sensorOut, LOW);
  
  // Baca warna hijau
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  greenFrequency = pulseIn(sensorOut, LOW);
  
  // Baca warna biru
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  blueFrequency = pulseIn(sensorOut, LOW);

  // Konversi ke nilai RGB (disesuaikan dengan karakteristik sensor)
  int redValue = map(redFrequency, 20, 500, 255, 0);
  int greenValue = map(greenFrequency, 20, 500, 255, 0);
  int blueValue = map(blueFrequency, 20, 500, 255, 0);

  // Pastikan nilai RGB tetap dalam rentang 0-255
  redValue = constrain(redValue, 0, 255);
  greenValue = constrain(greenValue, 0, 255);
  blueValue = constrain(blueValue, 0, 255);

  // Cetak hasil
  Serial.print("RGB: (");
  Serial.print(redValue);
  Serial.print(", ");
  Serial.print(greenValue);
  Serial.print(", ");
  Serial.print(blueValue);
  Serial.println(")");

  // Membaca data dari sensor MQ-4
  int mq4Value = analogRead(mq4Pin);
  Serial.print("Gas NH3 value: ");
  Serial.println(mq4Value);

   // Membaca data dari sensor MQ-135
  int mq135Value = analogRead(mq135Pin);
  Serial.print("Gas CO2 value: ");
  Serial.println(mq135Value);

    // Membaca data dari sensor MQ-136
  int mq136Value = analogRead(mq136Pin);
  Serial.print("Gas H2S value: ");
  Serial.println(mq136Value);

     // Membaca data dari sensor MQ-1352
  int mq1352Value = analogRead(mq1352Pin);
  Serial.print("Gas CH4 value: ");
  Serial.println(mq1352Value);

  // Membaca data dari sensor pH
  int phValue = analogRead(phPin);
  float voltage = phValue * (5.0 / 1023.0);
  float pH = 3.5 * voltage + calibrationOffset;
  Serial.print("pH value: ");
  Serial.println(pH);

  // Buat format data untuk dikirim ke ESP32
  String data = String(t) + "," + String(h) + "," + 
                String(mq4Value) + "," + String(mq135Value) + "," + 
                String(mq136Value) + "," + String(mq1352Value) + "," +
                String(redValue) + "," + String(greenValue) + "," + String(blueValue) + "," + 
                String(pH);

  // Kirim data ke ESP32 melalui Serial1
  Serial1.println(data);

  // Delay 2 detik sebelum membaca lagi
  delay(2000);
}
