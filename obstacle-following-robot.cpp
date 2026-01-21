/**
 * Robot Pengikut Objek (Object Follower) dengan Ultrasonic
 * Refactored Version
 */

// ==========================================
// 1. KONFIGURASI PIN & KONSTANTA
// ==========================================

// Pin Ultrasonic
const int PIN_ECHO = A0;
const int PIN_TRIG = A1;

// Pin Motor Driver (L298N)
const int PIN_ENA = 5;
const int PIN_ENB = 6;
const int PIN_IN1 = 8;
const int PIN_IN2 = 9;
const int PIN_IN3 = 10;
const int PIN_IN4 = 11;

// Konfigurasi Pergerakan
const int SPEED_NORMAL = 120;     // Kecepatan motor (0-255)
const int DIST_MIN     = 10;      // Jarak terlalu dekat (mundur)
const int DIST_STOP    = 20;      // Jarak ideal (berhenti)
const int DIST_MAX     = 100;     // Jarak maksimum (diabaikan)
const int JUMP_LIMIT   = 20;      // Batas perubahan jarak tiba-tiba (deteksi objek hilang)

// Variabel Global
int prevDistance = 0;

// ==========================================
// 2. SETUP
// ==========================================
void setup() {
  Serial.begin(9600);

  // Setup Pin Sensor
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // Setup Pin Motor
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  stopCar();
  Serial.println("Sistem Robot Siap.");
}

// ==========================================
// 3. MAIN LOOP
// ==========================================
void loop() {
  int frontDistance = measureDistance();
  
  // Debugging jarak
  // Serial.print("Jarak: "); Serial.println(frontDistance);

  // Hitung perubahan jarak drastis
  int delta = abs(frontDistance - prevDistance);

  // LOGIKA 1: Deteksi jika objek tiba-tiba menghilang/lompat jauh
  if (delta > JUMP_LIMIT && frontDistance > DIST_STOP) {
    handleObjectLoss(); // Lakukan scanning kiri/kanan
  } 
  // LOGIKA 2: Pergerakan Normal (Follower)
  else {
    if (frontDistance > DIST_STOP && frontDistance <= DIST_MAX) {
      forward();
    } 
    else if (frontDistance >= DIST_MIN && frontDistance <= DIST_STOP) {
      stopCar();
    } 
    else if (frontDistance < DIST_MIN) {
      back();
    }
  }

  // Simpan jarak saat ini untuk loop berikutnya
  prevDistance = frontDistance;
  delay(100); // Sedikit delay untuk stabilitas pembacaan
}

// ==========================================
// 4. FUNGSI LOGIKA KHUSUS
// ==========================================

/**
 * Fungsi untuk mencari objek jika tiba-tiba hilang
 * Robot akan menengok ke kanan dan kiri, lalu memilih arah terdekat.
 */
void handleObjectLoss() {
  stopCar();
  Serial.println(">> Objek hilang tiba-tiba. Memulai scanning...");

  int distRight, distLeft;

  // 1. Cek Kanan
  right(); delay(150);
  stopCar(); delay(300);
  distRight = measureDistance();
  Serial.print("   Kanan: "); Serial.println(distRight);

  // Balik ke Tengah
  left(); delay(150);
  stopCar(); delay(300);

  // 2. Cek Kiri
  left(); delay(150);
  stopCar(); delay(300);
  distLeft = measureDistance();
  Serial.print("   Kiri: "); Serial.println(distLeft);

  // Balik ke Tengah
  right(); delay(150);
  stopCar(); delay(300);

  // 3. Putuskan Arah
  if (distRight < distLeft && distRight < DIST_MAX) {
    Serial.println(">> Kanan lebih dekat. Belok Kanan.");
    right(); delay(200);
    forward();
  } 
  else if (distLeft < distRight && distLeft < DIST_MAX) {
    Serial.println(">> Kiri lebih dekat. Belok Kiri.");
    left(); delay(200);
    forward();
  } 
  else {
    Serial.println(">> Tidak ditemukan di kedua sisi. Berhenti.");
    stopCar();
  }
  
  // Reset prevDistance agar tidak trigger scanning lagi di loop berikutnya
  prevDistance = measureDistance();
}

/**
 * Mengukur jarak menggunakan sensor Ultrasonic
 * Return: Jarak dalam cm
 */
int measureDistance() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  long duration = pulseIn(PIN_ECHO, HIGH, 30000); // Timeout 30ms agar tidak hang
  
  if (duration == 0) return 999; // Anggap jauh sekali jika timeout
  return duration / 58;
}

// ==========================================
// 5. KONTROL MOTOR
// ==========================================

void setMotorSpeed(int speed) {
  analogWrite(PIN_ENA, speed);
  analogWrite(PIN_ENB, speed);
}

void forward() {
  setMotorSpeed(SPEED_NORMAL);
  digitalWrite(PIN_IN1, HIGH); digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, HIGH); digitalWrite(PIN_IN4, LOW);
  Serial.println("Status: Maju");
}

void back() {
  setMotorSpeed(SPEED_NORMAL);
  digitalWrite(PIN_IN1, LOW);  digitalWrite(PIN_IN2, HIGH);
  digitalWrite(PIN_IN3, LOW);  digitalWrite(PIN_IN4, HIGH);
  Serial.println("Status: Mundur");
}

void left() {
  setMotorSpeed(SPEED_NORMAL);
  digitalWrite(PIN_IN1, HIGH); digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);  digitalWrite(PIN_IN4, HIGH);
  Serial.println("Status: Kiri");
}

void right() {
  setMotorSpeed(SPEED_NORMAL);
  digitalWrite(PIN_IN1, LOW);  digitalWrite(PIN_IN2, HIGH);
  digitalWrite(PIN_IN3, HIGH); digitalWrite(PIN_IN4, LOW);
  Serial.println("Status: Kanan");
}

void stopCar() {
  digitalWrite(PIN_ENA, LOW);
  digitalWrite(PIN_ENB, LOW);
  // Mematikan logic pins juga praktik yang baik untuk pengereman
  digitalWrite(PIN_IN1, LOW); digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW); digitalWrite(PIN_IN4, LOW);
  Serial.println("Status: Stop");
}
