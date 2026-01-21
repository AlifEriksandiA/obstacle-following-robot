# Smart Object Follower Robot dengan Auto-Scan 🤖👀

Proyek ini adalah kode program untuk robot beroda yang dapat mengikuti objek di depannya secara otomatis menggunakan sensor Ultrasonik. 

Keunggulan utama dari versi ini adalah fitur **Smart Scanning**: Jika objek bergerak terlalu cepat dan hilang dari pandangan sensor, robot tidak akan diam saja, melainkan akan menengok ke kanan dan ke kiri untuk mencari kembali targetnya.

## 🌟 Fitur Unggulan

* **🏃 Real-time Following**: Robot akan maju jika objek menjauh (jarak 20-100cm) dan mundur jika objek terlalu dekat (<10cm).
* **🔍 Auto-Scan Logic**: Mendeteksi perubahan jarak yang drastis (objek "lompat" atau hilang tiba-tiba). Robot akan berhenti, menengok kanan-kiri, dan berbelok ke arah di mana objek terdeteksi kembali.
* **🛑 Safety Distance**: Robot akan menjaga jarak aman sekitar 10-20cm dari objek untuk mencegah tabrakan.
* **⚡ PWM Speed Control**: Menggunakan sinyal PWM pada pin Enable (ENA/ENB) untuk mengatur kecepatan motor agar pergerakan lebih halus.

## 🛠️ Komponen Hardware

* **Microcontroller**: Arduino Uno / Nano / Mega.
* **Sensor**: Modul Ultrasonik HC-SR04.
* **Motor Driver**: L298N Dual H-Bridge.
* **Actuators**: 2x DC Gearbox Motor + Roda.
* **Power**: Baterai (Disarankan Li-Ion 18650 2x atau 3x seri) untuk menyuplai L298N.

## 🔌 Konfigurasi Pin (Wiring Diagram)

Berikut adalah sambungan kabel berdasarkan kode program:

| Komponen | Pin Modul | Pin Arduino | Keterangan |
| :--- | :--- | :--- | :--- |
| **Ultrasonic** | Trig | **A1** | Trigger Pulse |
| | Echo | **A0** | Echo Pulse |
| **L298N Driver** | ENA | **5** | Speed Motor A (PWM) |
| | ENB | **6** | Speed Motor B (PWM) |
| | IN1 | **8** | Input Motor A |
| | IN2 | **9** | Input Motor A |
| | IN3 | **10** | Input Motor B |
| | IN4 | **11** | Input Motor B |

*> **Catatan:** Pastikan *jumper* pada ENA dan ENB di driver L298N dilepas agar kita bisa mengontrol kecepatan lewat coding.*

## ⚙️ Logika Algoritma

Robot bekerja berdasarkan **State Machine** sederhana di dalam `void loop()`:

1.  **Pengukuran Jarak**: Robot terus mengukur jarak objek di depan.
2.  **Deteksi Anomali (Smart Feature)**:
    * Sistem menghitung selisih jarak saat ini dengan jarak sebelumnya (`delta`).
    * Jika `delta > 20 cm` (objek tiba-tiba hilang/jauh), robot masuk mode **SCANNING**.
    * **Mode Scanning**: Robot menengok Kanan -> Ukur -> Tengah -> Kiri -> Ukur -> Tengah. Kemudian robot berbelok ke arah jarak terpendek yang ditemukan.
3.  **Mode Normal (Following)**:
    * **Jarak > 20cm**: Maju mendekati objek.
    * **Jarak 10-20cm**: Berhenti (Posisi Ideal).
    * **Jarak < 10cm**: Mundur (Terlalu dekat).

## 🚀 Cara Instalasi & Upload

1.  Rakit robot sesuai diagram pin di atas.
2.  Buka **Arduino IDE**.
3.  Copy-paste kode program ke sketch baru.
4.  Hubungkan Arduino ke PC via USB.
5.  Pilih Board (misal: Arduino Uno) dan Port yang sesuai.
6.  Klik **Upload**.

## ⚠️ Troubleshooting (Masalah Umum)

* **Robot berputar di tempat**: Kemungkinan kabel motor A atau B terbalik polaritasnya (+/-). Tukar posisi kabel salah satu motor di terminal L298N.
* **Robot tidak bergerak**:
    * Cek tegangan baterai (Motor driver butuh minimal 6V-12V yang stabil).
    * Pastikan pin ENA/ENB terhubung ke pin Arduino yang support PWM (ada tanda `~` di board, pin 5 & 6 cocok).
* **Jarak tidak akurat**: Pastikan sensor HC-SR04 menghadap lurus ke depan dan tidak terhalang kabel robot sendiri.

## 👤 Author

1. Alif Eriksandi Agustino
2. Ahmad Husein Rafi
3. Auzan Pradipa Bhamakerti Ediono
4. I Putu Arya Adhi Widyatma
5. Jonathan Arya Priguna
6. Muhammad Farkhan Fadillah

* Teknik Komputer - Universitas Brawijaya
