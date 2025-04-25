#define SensorPin A0
#define ArrayLength 40

float voltage, pHValue;
int analogBuffer[ArrayLength];

void setup() {
  Serial.begin(9600);
  pinMode(SensorPin, INPUT);
}

void loop() {
  static unsigned long samplingTime = millis();
  if (millis() - samplingTime > 1000U) {
    samplingTime = millis();

    // Ambil rata-rata pembacaan analog
    for (int i = 0; i < ArrayLength; i++) {
      analogBuffer[i] = analogRead(SensorPin);
      delay(5);
    }

    float avgValue = 0;
    for (int i = 0; i < ArrayLength; i++) {
      avgValue += analogBuffer[i];
    }
    avgValue /= ArrayLength;

    voltage = avgValue * (5.0 / 1023.0);

    // Gunakan rumus hasil kalibrasi
    pHValue = -8.222 * voltage + 34.345;

    Serial.print("Voltage: ");
    Serial.print(voltage, 3);
    Serial.print(" V | pH Value: ");
    Serial.println(pHValue, 2);
  }
}
