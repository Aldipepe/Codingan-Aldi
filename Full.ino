// Definisi pin dan konstanta untuk sensor CH4 (MQ-4)
#define MQ4_PIN A0
#define MQ4_RL 10000.0
#define MQ4_A 2007.1
#define MQ4_B -2.186
#define MQ4_R0 1014.20

// Definisi pin dan konstanta untuk sensor CO2 (MQ-135)
#define MQ135_CO2_PIN A2
#define MQ135_CO2_RL 10000.0
#define MQ135_CO2_A 110.47
#define MQ135_CO2_B -2.862
#define MQ135_CO2_R0 11500.0

// Definisi pin dan konstanta untuk sensor H2S (MQ-136)
#define MQ136_PIN A3
#define MQ136_RL 10000.0
#define MQ136_A 100.0
#define MQ136_B -2.3
#define MQ136_R0 1533.80

// Definisi pin dan konstanta untuk sensor NH3 (MQ-135)
#define MQ135_NH3_PIN A6
#define MQ135_NH3_RL 10000.0
#define MQ135_NH3_A 0.9
#define MQ135_NH3_B -2.6
#define MQ135_NH3_R0 10200.0

const int numSamples = 10;  // Jumlah sampel untuk moving average
float readingsCH4[numSamples]; // Array untuk menyimpan nilai bacaan CH4
float readingsCO2[numSamples]; // Array untuk menyimpan nilai bacaan CO2
float readingsH2S[numSamples]; // Array untuk menyimpan nilai bacaan H2S
float readingsNH3[numSamples]; // Array untuk menyimpan nilai bacaan NH3
int indexCH4 = 0;
int indexCO2 = 0;
int indexH2S = 0;
int indexNH3 = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("Monitoring Gas dengan berbagai sensor...");

    // Inisialisasi array bacaan dengan 0
    for (int i = 0; i < numSamples; i++) {
        readingsCH4[i] = 0.0;
        readingsCO2[i] = 0.0;
        readingsH2S[i] = 0.0;
        readingsNH3[i] = 0.0;
    }
}

void loop() {
    // Baca sensor CH4
    float ppmCH4 = readSensor(MQ4_PIN, MQ4_RL, MQ4_A, MQ4_B, MQ4_R0, readingsCH4, &indexCH4);
    Serial.print("Kadar Metana (CH4): ");
    Serial.print(ppmCH4, 2);
    Serial.println(" PPM");

    // Baca sensor CO2
    float ppmCO2 = readSensor(MQ135_CO2_PIN, MQ135_CO2_RL, MQ135_CO2_A, MQ135_CO2_B, MQ135_CO2_R0, readingsCO2, &indexCO2);
    Serial.print("Kadar CO2: ");
    Serial.print(ppmCO2, 2);
    Serial.println(" PPM");

    // Baca sensor H2S
    float ppmH2S = readSensor(MQ136_PIN, MQ136_RL, MQ136_A, MQ136_B, MQ136_R0, readingsH2S, &indexH2S);
    Serial.print("Kadar Hidrogen Sulfida (H2S): ");
    Serial.print(ppmH2S, 2);
    Serial.println(" PPM");

    // Baca sensor NH3
    float ppmNH3 = readSensor(MQ135_NH3_PIN, MQ135_NH3_RL, MQ135_NH3_A, MQ135_NH3_B, MQ135_NH3_R0, readingsNH3, &indexNH3);
    Serial.print("Kadar NH3: ");
    Serial.print(ppmNH3, 2);
    Serial.println(" PPM");

    delay(1000);  // Delay untuk pembacaan berikutnya
}

// Fungsi untuk membaca sensor dengan moving average
float readSensor(int pin, float RL, float A, float B, float R0, float* readings, int* index) {
    int sensorValue = analogRead(pin);  // Membaca data dari sensor
    float voltage = sensorValue * (5.0 / 1023.0); // Konversi nilai ADC ke tegangan

    if (voltage == 0) {  // Deteksi kesalahan jika tegangan 0V
        Serial.println("Error: Tegangan sensor 0V, cek koneksi!");
        return 0.0;
    }

    // Menghitung nilai Rs (resistansi sensor)
    float Rs = ((5.0 - voltage) / voltage) * RL;
    float ratio = Rs / R0;  // Perbandingan Rs terhadap R0

    if (ratio <= 0) {  // Menghindari kesalahan matematika jika ratio <= 0
        Serial.println("Error: Nilai rasio tidak valid!");
        return 0.0;
    }

    float ppm = A * pow(ratio, B);  // Hitung kadar gas dalam PPM

    // **Moving Average untuk stabilisasi data**
    readings[*index] = ppm;  // Simpan pembacaan baru dalam array
    *index = (*index + 1) % numSamples;  // Perbarui indeks (looping array)

    // Hitung rata-rata nilai dari semua sampel
    float avgPPM = 0.0;
    for (int i = 0; i < numSamples; i++) {
        avgPPM += readings[i];
    }
    avgPPM /= numSamples;

    return avgPPM;
}

// Fungsi untuk membaca sensor tanpa moving average
float readSensorSimple(int pin, float RL, float A, float B, float R0) {
    int sensorValue = analogRead(pin);
    float voltage = sensorValue * (5.0 / 1023.0);
    float Rs = ((5.0 - voltage) / voltage) * RL;
    float ratio = Rs / R0;

    if (ratio <= 0) {
        Serial.println("Error: Nilai rasio tidak valid!");
        return 0.0;
    }

    float ppm = pow(10, (log10(ratio) - log10(A)) / B);
    return ppm;
}
