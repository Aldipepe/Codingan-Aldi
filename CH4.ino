#define MQ4_PIN A0        // Pin sensor MQ-4 dihubungkan ke pin A0 Arduino
#define RL 10000.0        // Load resistance (RL) dalam ohm
#define A 2007.1          // Konstanta A dari datasheet MQ-4
#define B -2.186          // Konstanta B dari datasheet MQ-4

#define R0 19914.20       // Gantilah dengan nilai R0 hasil kalibrasi

const int numSamples = 10;  // Jumlah sampel untuk moving average
float readings[numSamples]; // Array untuk menyimpan nilai bacaan
int index = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("Monitoring Gas Metana (CH4) dengan MQ-4...");
    
    // Inisialisasi array bacaan dengan 0
    for (int i = 0; i < numSamples; i++) {
        readings[i] = 0.0;
    }
}

void loop() {
    int sensorValue = analogRead(MQ4_PIN);  // Membaca data dari sensor
    float voltage = sensorValue * (5.0 / 1023.0); // Konversi nilai ADC ke tegangan

    if (voltage == 0) {  // Deteksi kesalahan jika tegangan 0V
        Serial.println("Error: Tegangan sensor 0V, cek koneksi!");
        return;
    }

    // Menghitung nilai Rs (resistansi sensor)
    float Rs = ((5.0 - voltage) / voltage) * RL;
    float ratio = Rs / R0;  // Perbandingan Rs terhadap R0

    if (ratio <= 0) {  // Menghindari kesalahan matematika jika ratio <= 0
        Serial.println("Error: Nilai rasio tidak valid!");
        return;
    }

    float ppm = A * pow(ratio, B);  // Hitung kadar CH4 dalam PPM

    // **Moving Average untuk stabilisasi data**
    readings[index] = ppm;  // Simpan pembacaan baru dalam array
    index = (index + 1) % numSamples;  // Perbarui indeks (looping array)

    // Hitung rata-rata nilai dari semua sampel
    float avgPPM = 0.0;
    for (int i = 0; i < numSamples; i++) {
        avgPPM += readings[i];
    }
    avgPPM /= numSamples;

    // Tampilkan hasil akhir setelah stabilisasi
    Serial.print("Kadar Metana (CH4): ");
    Serial.print(avgPPM, 2);  // Tampilkan dengan 2 angka desimal
    Serial.println(" PPM");

    delay(1000);  // Delay untuk pembacaan berikutnya
}
