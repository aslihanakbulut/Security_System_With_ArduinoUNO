#include <LiquidCrystal_I2C.h>  //LCD ekran için kutuphane başlatilir
#include <Wire.h>   //LCD ekran için kutuphane başlatilir
#include <SPI.h>     //SPI kütüphanemizi tanımlıyoruz.
#include <MFRC522.h>  // RFID modulu için kutuphane başlatilir
#include <HX711.h>  // agirlik sensoru için kutuphane
HX711 scale; // kutuphaneden nesne olustur

int RST_PIN = 9; //RC522 modülü reset pinini tanımlıyoruz.
int SS_PIN = 10; //RC522 modülü chip select pinini tanımlıyoruz.
int LDRPin = A2; // LDR nin bağlı olduğu analog pin numarasını bir değişkene ata
int lazerPin = 5; // Lazerin bağlı olduğu pinin numarasını lazerPin adında bir değişkene ata
int led = 6; // Ledin bağlı olduğu pinin numarasını led adında bir değişkene ata
int buzzerPin = 7; // Buzzerın bağlı olduğu pinin numarasını buzzerPin adında bir değişkene ata
int LDR2Pin = A3; // LDR2 nin bağlı olduğu analog pin numarasını bir değişkene ata
int giris_Sensor = 8; // Giriş için kizilotesi nin bağlı olduğu pinin numarasını giris_Sensor adında bir değişkene ata
int cikis_Sensor = 2; // Cıkış için kizilotesi nin bağlı olduğu pinin numarasını cikis_Sensor adında bir değişkene ata
int giris_kisi = 0; // Başlangıç için giris_kisi sayisi 0 olsun.
int cikis_kisi = 0; // Başlangıç için cikis_kisi sayisi 0 olsun.
int pirPin = 4; //Hareketin bağlı olduğu pinin numarasını pirPin adında bir değişkene ata
int hareket; // Hareket sensöründen okunan digital verinin atanacağı değişken
int mod = 0; // RFID modülüyle baslangıc olarak baslayacak mod belirtilir. 0 olması gece modunu işaret etmektedir.
int alarms = 0; // Alarm calma durumunun kontrolünü sağlamak için tanımlanmış bir değişkendir.
float agirlik; // En basta okunan ağırlık değerinin tutulacağı değişkendir.
float agirlik2; // Sürekli olarak okunan ağırlık değerinin tutulacağı değişkendir.

LiquidCrystal_I2C lcdekranim(0x3f, 16, 2); //16 sutun 2 satirdan olusan 0x3f modelinde LCD ekran tanimla
MFRC522 rfid(SS_PIN, RST_PIN); //rfid tanimla
 // mod degistirmek için gerekli olan kart ID'leri tanimlanir 
byte ID[4] = {
  23,
  211,
  192,
  181
}; //mod 0.

byte ID2[4] = {
  112,
  165,
  49,
  27
}; //mod 2.

byte ID3[4] = {
  195,
  69,
  162,
  167
}; //mod 1.

void setup() {
  pinMode(led, OUTPUT); // Ledin bağlı olduğu pini çıkış pini olarak işaretle
  pinMode(LDRPin, INPUT); // LDR nin bağlı olduğu pini giriş pini olarak işaretle
  pinMode(buzzerPin, OUTPUT); // Buzzerın bağlı olduğu pini çıkış pini olarak işaretle
  pinMode(lazerPin, OUTPUT); // Lazerin bağlı olduğu pini çıkış pini olarak işaretle
  pinMode(giris_Sensor, INPUT); // Giriş için olan kızılötesi nin bağlı olduğu pini giriş pini olarak işaretle
  pinMode(cikis_Sensor, INPUT); // Çıkış için olan kızılötesi nin bağlı olduğu pini giriş pini olarak işaretle
  pinMode(pirPin, INPUT); // Hareket sensörünün bağlı olduğu pini giriş olarak işaretle
  pinMode(LDR2Pin, INPUT); // LDR2 nin bağlı olduğu pini giriş pini olarak işaretle
  lcdekranim.init(); //LCD ekrani baslat
  lcdekranim.backlight(); // arka plan isigini ac
  lcdekranim.setCursor(0, 0); // setCursor ile yazinin baslayacagi satir ve sutun belirlenir
  lcdekranim.print("Hosgeldiniz"); // LCD ekrana "Hosgeldiniz" yazdırıyoruz.
  delay(1000); // LCD ekranda 1 saniye boyunca "Hosgeldiniz" yazıyor.
  lcdekranim.clear(); // LCD ekranı temizliyoruz.
  Serial.begin(9600); // Seri haberleşmeyi başlatıyoruz.
  scale.begin(3, 2);  // agirlik sensorunun DT ve SCK pinleri tanimlanir, baslatilir  
  scale.set_offset(591250); // ağirlik sensoru boş iken alinan deger
  scale.set_scale(234.13f);//kalibrasyon degeri
  Serial.begin(9600);// Seri haberleşmeyi başlatıyoruz.
  SPI.begin(); //SPI iletişimini başlatıyoruz.
  rfid.PCD_Init(); //RC522 modülünü başlat
  agirlik = scale.get_units(5), 1; // Ağırlık sensöründen okunan 5 değerin ortalamasını agirlik değişkenine ata
}
void loop() {
  hareket = digitalRead(pirPin); //Hareket sensöründen okunan digital degeri hareket değişkenine ata
  int isik = analogRead(LDRPin); //LDR sensöründen okunan analog degeri isik değişkenine ata
  int isik2 = analogRead(LDR2Pin); //LDR2 sensöründen okunan analog degeri isik2 değişkenine ata
  int bilgi_d = digitalRead(giris_Sensor); // Giriş kızılötesi sensöründen okunan digital degeri bilgi_d değişkenine ata
  int bilgi_a = analogRead(A0); // Giriş kızılötesi sensöründen okunan analogl degeri bilgi_a değişkenine ata
  int bilgi_d2 = digitalRead(cikis_Sensor); // Çıkış kızılötesi sensöründen okunan digital degeri bilgi_d2 değişkenine ata
  int bilgi_a2 = analogRead(A1); // Giriş kızılötesi sensöründen okunan analogl degeri bilgi_a2 değişkenine ata
  if (!rfid.PICC_IsNewCardPresent()) //Yeni kartın okunmasını bekliyoruz.
    return;
  if (!rfid.PICC_ReadCardSerial()) //Kart okunmadığı zaman bekliyoruz.
    return;
  if (rfid.uid.uidByte[0] == ID[0] && //Okunan kart ID'si ile ID değişkenini karşılaştırıyoruz.
      rfid.uid.uidByte[1] == ID[1] &&
      rfid.uid.uidByte[2] == ID[2] &&
      rfid.uid.uidByte[3] == ID[3]) {
    Serial.println("mod 0");
    mod = 0; // Gece modu
    ekranaYazdir();
  } 
  else if (rfid.uid.uidByte[0] == ID2[0] && //Okunan kart ID'si ile ID2 değişkenini karşılaştırıyoruz.
             rfid.uid.uidByte[1] == ID2[1] &&
             rfid.uid.uidByte[2] == ID2[2] &&
             rfid.uid.uidByte[3] == ID2[3]) {
    Serial.println("mod 2");
    mod = 2; // Gündüz modu
    ekranaYazdir();
  } 
  else if (rfid.uid.uidByte[0] == ID3[0] && //Okunan kart ID'si ile ID3 değişkenini karşılaştırıyoruz.
             rfid.uid.uidByte[1] == ID3[1] &&
             rfid.uid.uidByte[2] == ID3[2] &&
             rfid.uid.uidByte[3] == ID3[3]) {
    Serial.println("Devre dışı bırakıldı.");
    Serial.println("mod 1");
    mod = 1; // Alarm devre dışı bırakıldı.
    ekranaYazdir();
  }
  else { //Yetkisiz giriş
    Serial.println("Yetkisiz Kart");
    digitalWrite(led, LOW);
    ekranaYazdir();
  }
  // Gece modu evresi
  while (mod == 0 && !rfid.PICC_IsNewCardPresent() ) {
    Serial.println("Gece modu");
    hareket = digitalRead(pirPin); //Hareket sensöründen okunan digital degeri hareket değişkenine ata
    digitalWrite(lazerPin, HIGH); // lazerleri sürekli yak
    int isik = analogRead(LDRPin); //LDR sensöründen okunan analog degeri isik değişkenine ata
    int isik2 = analogRead(LDR2Pin); //LDR2 sensöründen okunan analog degeri isik değişkenine ata
    agirlik2 = scale.get_units(); // Ağırlık sensöründen sürekli olarak okunan değeri agirlik2 değişkenine ata
    
    /* Eger LDR ve LDR2 nin üzerine düşen lazer kesilirse ya da sürekli olarak okunan agirlik2 degeri basta okunan agirlik - 5.0 degerinden küçükse
       ya da sürekli olarak okunana agirlik2 degeri basta okunan agirlik + 5.0 degerinden büyükse
       ya da hareket sensörü hareket algıladığı zaman alarm aktifleşir */
    if ( !(isik < 100 && isik2 < 100) || (agirlik2 < (agirlik - 5.0)) || (agirlik2 > (agirlik + 5.0)) || hareket == HIGH )
    {
      alarms = 1; // alarms değişkenini 1 yapar

    } if (alarms == 1) { // alarms değişkeni 1 değerine eşitse
      alarm(); // alarm aktifleşir
    }
  }
  // Mod 1 durumunda bulunuyorsak ve herhangi bir kart okunuyorsa alarm susturulur.
  while (mod == 1 && !rfid.PICC_IsNewCardPresent()) {
    alarmSustur();

  }

  /*while (mod == 2 && alarms == 1 && !rfid.PICC_IsNewCardPresent() ){
        alarm();

    }*/

  // Gündüz modu evresi
  while (mod == 2 && !rfid.PICC_IsNewCardPresent() && alarms == 0) {
    agirlik2 = scale.get_units(); // Sürekli olarak ağırlık sensöründen değer alınır ve agirlik2 değişkenine atılır.
    digitalWrite(lazerPin, LOW); // Gündüz modunda lazerler söndürülür
    int bilgi_d = digitalRead(giris_Sensor); // Giriş kızılötesi sensöründen okunan digital degeri bilgi_d değişkenine ata
    int bilgi_a = analogRead(A0); // Giriş kızılötesi sensöründen okunan analogl degeri bilgi_a değişkenine ata
    int bilgi_d2 = digitalRead(cikis_Sensor); // Çıkış kızılötesi sensöründen okunan digital degeri bilgi_d2 değişkenine ata
    int bilgi_a2 = analogRead(A1); // Giriş kızılötesi sensöründen okunan analogl degeri bilgi_a2 değişkenine ata
    
    //Serial.println(bilgi_d);
    //Serial.println(bilgi_d2);
    //Serial.println(bilgi_a);
    Serial.println(bilgi_a2);
    // Giriş kızılötesi sensör nesne algıladığı zaman giris_kisi sayısını 1 arttır.
    if (bilgi_a < 500) {
      giris_kisi = giris_kisi + 1;
      delay(100);
    }
  /*  else if (bilgi_a > 100 && bilgi_a < 200) {
      giris_kisi = giris_kisi;
    }*/
    
    // Çıkış kızılötesi sensör nesne algıladığı zaman giris_kisi sayisi cikis_kisi sayisindan fazla ise cikis_kisi sayisini 1 arttır.
     // Çünkü içeride olmayan biri dışarı çıkamaz. İçerideki kişi sayısının negatif olmasının önüne geçilmiştir.
     if (bilgi_a2 < 500) {
      if (giris_kisi > cikis_kisi) {
        cikis_kisi = cikis_kisi + 1;
        delay(100);
      }
    }
    else if (bilgi_a2 > 100 && bilgi_a2 < 200) {
      cikis_kisi = cikis_kisi;
    }
        
    int kisi = giris_kisi - cikis_kisi;    // Kisi sayisi giris_kisi - cikis_kisi şeklinde hesaplanır.

    
    if (kisi < 10) {  // LCD ekran için kisi sayisi 10 dan az ise ona göre konum belirtilir.
      lcdekranim.setCursor(8, 1); // setCursor ile yazinin baslayacagi satir ve sutun belirlenir
      lcdekranim.print(kisi);
      lcdekranim.setCursor(9, 1);// setCursor ile yazinin baslayacagi satir ve sutun belirlenir
      lcdekranim.print(" ");
      lcdekranim.setCursor(1, 0); // setCursor ile yazinin baslayacagi satir ve sutun belirlenir
      lcdekranim.print("kisi sayisi");
    }
    // LCD ekran için kisi sayisi 10 a eşit veya fazla ise ona göre konum belirtilir.
    else {

      lcdekranim.setCursor(8, 1); // setCursor ile yazinin baslayacagi satir ve sutun belirlenir
      lcdekranim.print(kisi);
      lcdekranim.setCursor(1, 0); // setCursor ile yazinin baslayacagi satir ve sutun belirlenir
      lcdekranim.print("kisi sayisi");
    }
    // Gündüz modunda ağırlık sensörünün algıladığı değerde -5 veya +5 aralığında bir değişim olduğu zaman alarm aktifleşir.
    if ((agirlik2 < (agirlik - 5.0)) || (agirlik2 > (agirlik + 5.0))) {
      alarm();
    }
  }
  rfid.PICC_HaltA(); // yeni kart okuyana kadar kapat
}

// RFID modulünde okutulan kartın ID numarasını ekrana yazdırır.
void ekranaYazdir() {
  Serial.print("ID Numarasi: ");
  for (int sayac = 0; sayac < 4; sayac++) {
    Serial.print(rfid.uid.uidByte[sayac]);
    Serial.print(" ");
  }
  Serial.println("");
}

  // Alarm aktifleşince; Buzzer aktifleştirilir.Ledler yanıp sönmeye başlar.
void alarm() {
  buzzerSes();
  digitalWrite(led, HIGH);
  delay(500);  // ledin yanip sonmesi icin bekle
  digitalWrite(led, LOW);
  delay(100);
}
  // Buzzer ayarları;Buzzer tone ayarları yapılır.
void buzzerSes() {
  tone(buzzerPin, 20);
  delay(100);
  tone(buzzerPin, 15);
}
  // Alarmı devre dışı bırakılır; Buzzer susturulur, alarms değişkeni 0 yapılır. Agirlik sensöründen tekrar değer alınır ve agirlik değişkenine atanır.
void alarmSustur() {
  noTone(buzzerPin);
  alarms = 0;
  agirlik = scale.get_units(5), 1;

}
