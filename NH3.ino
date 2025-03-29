#define MQ135_PIN A0  // Pin sensor gas MQ-135
#define RL 10000.0    // Resistor load 10kΩ

// Konstanta dari datasheet MQ-135 untuk NH3
#define A 0.9
#define B -2.6
#define R0 8200.0  // Harus dikalibrasi di udara bersih

void setup() {
    Serial.begin(9600);
    Serial.println("Monitoring Gas Amonia (NH3)...");
}

void loop() {
    int sensorValue = analogRead(MQ135_PIN);
    float voltage = sensorValue * (5.0 / 1023.0); // Konversi ke voltase
    float Rs = ((5.0 - voltage) / voltage) * RL; // Hitung Rs
    float ratio = Rs / R0;  // Bandingkan dengan udara bersih

    // Hitung PPM dengan persamaan logaritmik
    float ppm = pow(10, (log10(ratio) - log10(A)) / B);

    Serial.print("Kadar NH3: ");
    Serial.print(ppm);
    Serial.println(" PPM");

    delay(1000); // Update setiap 1 detik
}
