# 📞 SIM800C GSM ile Otomatik Sesli Arama Sistemi

<p align="center">
  <a href="#donanimlar"><img src="https://img.shields.io/badge/Donan%C4%B1m-Deneyap%20Kart%201A-24292e?style=flat-square&logo=espressif" alt="Donanım" /></a>
  <a href="#donanimlar"><img src="https://img.shields.io/badge/GSM%20Mod%C3%BCl%C3%BC-SIM800C%20Voice%20Call-24292e?style=flat-square" alt="GSM" /></a>
  <a href="#arama-mantigi"><img src="https://img.shields.io/badge/Protokol-Hayes%20ATD%20Dialing-24292e?style=flat-square" alt="ATD" /></a>
  <a href="#donanimlar"><img src="https://img.shields.io/badge/Besleme-3.7V%202000mAh%20LiPo-24292e?style=flat-square" alt="Batarya" /></a>
  <a href="#lisans"><img src="https://img.shields.io/badge/Lisans-MIT-24292e?style=flat-square" alt="Lisans" /></a>
</p>

> **Acil durum uyarıları, güvenlik ve saha otomasyonları için;** Deneyap Kart 1A ve **SIM800C GSM modülü** kullanılarak internete ya da Wi-Fi ağına gerek kalmadan, doğrudan hücresel şebeke üzerinden belirlenen telefon numarasını **otomatik olarak sesli arayan (çaldıran)** otonom çağrı sistemi.

---

<a id="icindekiler"></a>
## 📑 İçindekiler

1. [Proje Özeti ve Temel Özellikler](#proje-ozeti)
2. [Devre Bağlantı Şeması](#devre-semasi)
3. [Kullanılan Donanımlar ve Modüller](#donanimlar)
4. [Pin Haritası ve Bağlantı Tablosu](#pin-haritasi)
5. [AT Komutları ile Sesli Arama Mantığı](#arama-mantigi)
6. [Kurulum ve Çalıştırma Rehberi](#kurulum-ve-calistirma)
7. [Karşılaşılabilecek Sorunlar ve Çözümleri](#sorunlar-ve-cozumleri)
8. [Geliştirici](#gelistirici)
9. [Lisans](#lisans)

---

<a id="proje-ozeti"></a>
## 🎯 1. Proje Özeti ve Temel Özellikler

Yangın alarmları, izinsiz giriş tespiti veya kritik fabrika altyapılarında SMS bazen fark edilmeyebilir; ancak doğrudan telefonun çalması en yüksek dikkat uyandırma yöntemidir:

- **Doğrudan Sesli Çağrı (Voice Call):** Sistem `ATD+90xxxxxxxxxx;` komutu ile hedef telefon numarasını anında çaldırır.
- **Sıfır İnternet İhtiyacı:** Yalnızca standart 2G hücresel ses şebekesini kullanır; Wi-Fi veya mobil veri kotası gerektirmez.
- **Kararlı LiPo Besleme Mimarisi:** Modül şebekeye bağlanıp çağrı başlatırken anlık 2 Amper akım çeker. Sistemde kullanılan **3.7V 2000mAh LiPo Batarya**, kartın voltaj düşümü yaşamasını ve kapanmasını engeller.
- **Çift Yönlü Seri Köprü (`updateSerial`):** Arduino Seri Monitörü üzerinden SIM800C ile interaktif konuşulabilir ve gelen yanıtlar canlı takip edilebilir.

---

<a id="devre-semasi"></a>
## 🔌 2. Devre Bağlantı Şeması

> 💡 **Breadboard Bağlantısı:** Deneyap Kart 1A, SIM800C modülü ve harici 3.7V LiPo bataryanın fiziksel breadboard bağlantı şeması aşağıda gösterilmiştir:

<p align="center">
  <!-- Devre şeması fotoğrafını GitHub editörüne sürükle-bırak yaptığında oluşan link buraya yapıştırılır -->
<img width="1060" height="451" alt="Ekran görüntüsü 2026-10-12 202846" src="https://github.com/user-attachments/assets/0d2ba5f8-168f-4b04-b01f-dd0bf55b9424" />
  <br/><em>Deneyap Kart 1A, SIM800C GSM Modülü, 3.7V 2000mAh LiPo Batarya ve Harici GSM Anteni</em>
</p>

---

<a id="donanimlar"></a>
## 🧰 3. Kullanılan Donanımlar ve Modüller

| Bileşen | Model / Açıklama | Görev |
| :--- | :--- | :--- |
| **Geliştirme Kartı** | Deneyap Kart 1A (ESP32) | SoftwareSerial üzerinden AT komutlarını yönetme |
| **GSM Modülü** | SIM800C GSM/GPRS Modülü | Hücresel şebekeye bağlanarak sesli arama başlatma |
| **Güç Kaynağı** | 3.7V 2000mAh LiPo Batarya | SIM800C'nin anlık 2A arama akımını sağlama |
| **Anten** | Harici GSM Yay Anteni | Şebeke sinyal kalitesini güçlendirme |
| **SIM Kart** | Standart 2G Destekli Hat | Arama yapabilen aktif hücresel hat |

---

<a id="pin-haritasi"></a>
## 🗺️ 4. Pin Haritası ve Bağlantı Tablosu

```text
+------------------------------------+              +-----------------------------+
|          Deneyap Kart 1A           |              |       SIM800C GSM Modülü    |
|                                    |              |                             |
|  D8 (SoftwareSerial RX)            |<------------>| TXD (Veri Gönderme)         |
|  D14 (SoftwareSerial TX)           |------------->| RXD (Veri Alma)             |
|  Ortak GND                         |<------------>| GND (Toprak)                |
+------------------------------------+              +-----------------------------+
                                                    +-----------------------------+
                                                    |   3.7V 2000mAh LiPo Batarya |
                                                    |   Kırmızı (+) -> SIM800C VBAT
                                                    |   Siyah   (-) -> Ortak GND  |
                                                    +-----------------------------+
```

> ⚠️ **Kritik Donanım İpuçları:**
> 1. **Ortak Toprak (Common GND):** Deneyap Kart'ın GND pini ile LiPo bataryanın eksi kutbu mutlaka aynı breadboard hattında birleştirilmelidir.
> 2. **Harici Anten:** SIM800C modülü üzerinde anten takılı olmadan baz istasyonuna bağlanamaz ya da arama sırasında şebekeden düşer.

---

<a id="arama-mantigi"></a>
## 📡 5. AT Komutları ile Sesli Arama Mantığı

SIM800C modülünde sesli arama yapmak için **Hayes AT komut seti** kullanılır:

1. **`AT`** ➔ Modülün bağlantısını test eder (`OK` yanıtı beklenir).
2. **`ATD+905XXXXXXXXX;`** ➔ Belirtilen numarayı sesli olarak arar.

> 🔔 **ÖNEMLİ KURAL (Sondaki Noktalı Virgül):**  
> `ATD` komutunun sonuna **noktalı virgül (`;`)** konulması zorunludur! Noktalı virgül modüle **"Sesli Çağrı (Voice Call)"** başlatmasını bildirir. Noktalı virgül konulmazsa modül veri (faks/data) araması yapmaya çalışır ve başarısız olur.

3. **`ATH`** ➔ Çağrıyı sonlandırır (telefonu kapatır).
4. **`ATA`** ➔ Gelen bir çağrıyı yanıtlar (hattı açar).

---

<a id="kurulum-ve-calistirma"></a>
## 🚀 6. Kurulum ve Çalıştırma Rehberi

### 1. SIM Kart Hazırlığı
- SIM kartınızı önce normal bir akıllı telefona takıp **PIN Kodu Kilidini KAPATIN**.
- Kartta arama yapabilecek aktif bir bakiye / tarife olduğunu doğrulayın.

### 2. Koddaki Telefon Numarasını Güncelleme
`SIM800C_Arama.ino` dosyasını açıp aranacak numarayı kendi telefon numaranızla değiştirin:
```cpp
mySerial.println("ATD+905XXXXXXXXX;"); // Sondaki noktalı virgülü unutmayın!
```

### 3. Kodu Yükleme
- Arduino IDE'de kart olarak **Deneyap Kart 1A** seçin.
- `SoftwareSerial` kütüphanesinin kurulu olduğundan emin olun.
- Kodu karta yükleyin ve Seri Monitörü **9600 baud** hızında açın.
- Sistem açıldıktan yaklaşık 3–5 saniye sonra telefonunuz çalmaya başlayacaktır!

---

<a id="sorunlar-ve-cozumleri"></a>
## ⚠️ 7. Karşılaşılabilecek Sorunlar ve Çözümleri

### 1. Arama Yapmıyor / Seri Portta "NO CARRIER" Yazıyor
- **Neden:** Numaranın sonuna noktalı virgül (`;`) konulmamış olabilir veya sinyal seviyesi yetersizdir.
- **Çözüm:** `ATD+90...;` şeklinde noktalı virgülü kontrol edin ve harici GSM anteninin takılı olduğundan emin olun.

### 2. Arama Başladığı Anda Modül Kapanıyor / Yeniden Başlıyor
- **Neden:** Arama esnasında modül anlık yüksek akım (2A) çeker. Deneyap Kart'ın 3.3V pini yetersiz kalır.
- **Çözüm:** Şemada gösterildiği gibi modülü doğrudan **3.7V LiPo Batarya** ile besleyin ve ortak GND hattını kontrol edin.

---

<a id="gelistirici"></a>
## 👩‍💻 8. Geliştirici

**Kevser ÜSTÜN**  
🎓 Giresun Üniversitesi — Bilgisayar Mühendisliği  
🔗 [GitHub Profilim](https://github.com/kvsrustun)

---

<a id="lisans"></a>
## 📄 9. Lisans

Bu proje [MIT Lisansı](LICENSE) kapsamında açık kaynak olarak sunulmuştur.

