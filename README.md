# Security_System_With_ArduinoUNO

## PROJE AMACI 
Projede amaç hareket (PIR), kızılötesi (IR), lazer, ışık (LDR) ve ağırlık sensörlerini kullanarak alarm veren bir 
sistem oluşturmaktır. Müzelerde bulunan değerli eşyaların çalınmasına karşın ağırlık sensörü 
kullanılmaktadır. Nesneler yerinden hareket ettirildiğinde alarm çalışacak şeklinde bir sistem 
oluşturulmakta ve müzenin kapalı olduğu vakitlerde de ağırlık, hareket, lazer ve ışık sensörleri kullanılarak 
ortamda herhangi bir hareketlilik olup olmadığı kontrol edilmektedir. Herhangi bir şey algılanırsa alarm 
sistemi devreye girmektedir. Alarm sistemini başlatmak ve susturmak için RFID modülü kullanılır. Ek olarak 
ortamda bulunan insan sayısını algılayabilmek için de IR (kızılötesi) sensörünün kullanılması 
planlanmaktadır. Bu şekilde içeri giren çıkan insanların sayısı belirlenmiş olur. Böylelikle kapanış 
vakitlerinde içeride birilerinin kalıp kalmadığı bilinerek de hırsızlara karşı önlem alınabilir

## DONANIM SİSTEMİNİN ÇALIŞMA MANTIĞI
Güvenlik Sisteminde RFID ile mod değişimi yapılmaktadır. Bunun için iki kart, bir anahtarlık kullanılmıştır. 
Okunan UID numarasına göre modlar arası geçiş yapılarak farklı sensörler aktifleştirilir veya alarm sistemi 
devre dışı bırakılır. Sistemin gündüz modunda ağırlık sensörü ve kızılötesi sensörleri çalışmaktadır. Ağırlık 
sensörünün üstüne koyulan cismin ağırlığı hesaplanarak cismin bulunduğu yerden hareket ettirilip 
ettirilmediği kontrol edilir ve durumda değişiklik algılanırsa alarm aktifleştirilir. Ağırlık sensörünün yapısında
yay elemanı olarak adlandırılan metal şase ile strain gaugelerle kurulmuş bir wheatstone köprüsünden
oluşur. Yay elemanı üzerine bir fiziksel kuvvet uygulandığında bu bir değişikliğe sebep olur ,yay elemanında 
meydana gelen bu değişiklik wheatstone köprüsü üzerinden bir elektrik sinyali olarak çıktılanır. Elde edilen 
bu sinyal mikroişlemci tabanlı bir göstergede işlenir ve ağırlık bilgisi olarak kaydedilir.
Giriş ve çıkış kapılarındaki kızılötesi sensörlerden alınan analog değere göre kapılardan geçiş yapan kişi sayısı 
hesaplanır ve LCD ekranda görüntülenir. Kapılardan geçiş yapılmadığında okunan analog değer kapılardan geçiş 
yapıldığı durumda düşer ve bu değişime bağlı olarak kişi sayısının tutulduğu değişken üzerinde işlem yapılır.
Gece modunda ise ağırlık sensörüne ek olarak hareket sensörü, lazer ve LDR sensörü kullanılır. Hareket sensörü 
pasif kızılötesi sensördür, odada sıcak bir canlı yani hareket algılandığında kızılötesi ışın seviyesinde değişim 
meydana gelir ve alarm aktifleşir.
LDR (ışık) sensörüne gelen ışık lazer tarafından sağlanır lazer ile LDR arasındaki bağlantı kesildiğinde alarm 
aktifleşir. LDR üzerine düşen ışık miktarı azaldığında direnci azalır ve okunan analog değer artar. Okunan analog 
değerin artması durumunda alarm çalışmaktadır
