#define MQ135_PIN A0
#define RL 10000.0
#define A 110.47
#define B -2.862

#define R0 20000.0  // Ganti dengan nilai R0 hasil kalibrasi

void setup() {
    Serial.begin(9600);
    Serial.println("Monitoring CO2 dengan MQ-135...");
}

void loop() {
    int sensorValue = analogRead(MQ135_PIN);
    float voltage = sensorValue * (5.0 / 1023.0);
    float Rs = ((5.0 - voltage) / voltage) * RL;
    float ratio = Rs / R0;

    float ppm = pow(10, (log10(ratio) - log10(A)) / B);

    Serial.print("Kadar CO2: ");
    Serial.print(ppm);
    Serial.println(" PPM");

    delay(1000);
}
