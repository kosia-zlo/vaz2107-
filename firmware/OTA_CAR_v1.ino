/*********************************************ОБЩАЯ ИНФОРМАЦИЯ***************************************

ПО машины VAZ 2107 "Студентка" 
  Версия                      : 1.1
  Сборка номер                : 2
  Сборка дата                 : 08.01.2019
  Сборка устройства           : Молоткин Константин и выпускник ..... 
  Программное Обеспечение     : Молоткин Константин 
  Курс                        : 4
  Учебное заведение           : Самарский авиационный техникум
  Группа                      : ВМ-54.
  Telegram                    : https://t.me/kosia_zlo 
  Канал                       : https://t.me/joinchat/AAAAAFk49unFlsLftfWZDA
  Последня версия ПО          : 
  Описание                    : 
  Преподаватель               : Митина Е.Н. 

*/



//************************************************БИБЛИОТЕКА***************************

#include <OneWire.h>
#include <IRremote.h>
#include <Servo2.h>









//********************************************************НАСТРОЙКИ**************************
#define OTLADKA_IR 0 // 0 для выключения. 1 для включения. РЕЖИМ ОТЛАДКИ ЧЕРЕЗ IR
#define BT         1 // 0 для выключения. 1 для включения. ВКЛЮЧИТЬ BLUETOOTH




//********************************************************ПОДКЛЮЧЁНИЯ***********************
int zagig = 7 ; // Зажигание 
int starter = 8 ; // Стартер 
int DverZad = 9; //  
int Ruch = 4 ; // Ручник 
int skor = 6; // Скорость 
int sgn = 3; // Сигнализация 
#define tahometr A1 // Датчик оборотов к аналогову входу1

IRrecv irrecv(A0); // указываем пин, к которому подключен IR приемник
OneWire  ds(2); 


//*******************************************************ИМЕНА ПОДКЛЮЧЕНИЙ*************************
decode_results results;
Servo2 myservo1;



//*******************************************************ПРОВЕРЯШКИ*********************
int val; // Значение BT 
int val2; // Значение BT 2
float N = 0.0; // Тахометр 
float oborots = 0.0; // Фактические обороты 
bool STATUS = false; // Статус системы 
boolean dverzad_fl = 0 ;// Флаг Открытия задних дверей
int but = 0; 
boolean butt = 0; // Проверяшка 
boolean butt2; // Проверяшка два 
boolean butt3; // Проверяшка три 
boolean gabar; // Габариты 
boolean bagagnik_up; // Багажник 
boolean climat_onik ; // Климат контроль 

void setup() 
{ 
pinMode(zagig, OUTPUT); 
pinMode(DverZad, OUTPUT) ;  
pinMode(Ruch, OUTPUT) ; 
pinMode(skor, OUTPUT) ; 
pinMode(tahometr, INPUT); // конфигурируем оборотов как вход
pinMode(sgn, INPUT_PULLUP); 
pinMode(13, OUTPUT); 
pinMode(12, OUTPUT);
pinMode(A0, INPUT); 
pinMode(A4, OUTPUT);// пин A0 будет входом 
irrecv.enableIRIn();  // запускаем прием инфракрасного сигнала
//myservo1.attach(5);
Serial.begin(9600); 

} 
void loop(){ 
if (OTLADKA_IR == 1){ 
ikremote();
}
if (BT == 1){
bluetooth();
//bluetooth_data(); 
}
} 
//____________________________________________________________________________________________________
//----------------------------------------------------IR REMOTE COMANDS-------------------------------
void ikremote(){

   if (irrecv.decode(&results)) // если данные пришли выполняем команды
  {
     if (results.value == 16724175) { 
     Serial.println("Функция доводчика активирована"),
     dovod();
     
   }
   if (results.value == 16761405) { 
     Serial.println("Функция автозапуск активирована"),
     zapusk();
   }
    if (results.value == 16718055) { 
     Serial.println("Функция климата активирована"),
     climat();
    }
      if (results.value == 16743045) { 
       
      centerzamok();
    }
     
       if (results.value == 16716015) { 
     Serial.println("Функция габаритов активирована"),
     gabarit();
    }
        if (results.value == 16736925) { 
     Serial.println("Проверка запущена"),
     proverka();
    }
       if (results.value == 16726215) { 
     Serial.println("Функция аварийной сигнализации активирована"),
     avar();
    }
       if (results.value == 16734885) { 
     Serial.println("Функция открытия багажника активирована"),
     bagup();
    }
      if (results.value == 16732845) { 
     Serial.println("Сигнализация активирована"),
     Ruch == 1,
     sgn == 1,
      Serial.println(Ruch),
      Serial.println(sgn);
      
    }
    // отправляем полученные данные на порт
     Serial.println(results.value);
    irrecv.resume(); 
  }
}

//----------------------------------------------------BLUETOOTH COMANDS-----------------------------— 
void bluetooth(){ 
if (Serial.available()){ 
val = Serial.read(); 
if (val == "dovod_on"){ 
dovod();} 
if ( val == "zapusk_on") 
{zapusk();} 
if ( val == "centerzamok_on") 
{centerzamok();} 
if ( val == "climat_on") 
{climat();} 
if ( val == "avar_on") 
{avar();} 
if ( val == "gabarit_on") 
{gabarit();} 
if ( val == "bagagnik_on") 
{bagup();} 
if (val == "proverka_on") 
{proverka();} 
} 
} 
//----------------------------------------------------BLUETOOTH DATA-----------------------------— 
//void bluetooth_data(){ 
//if (Serial.available()){ 
//val2 = Serial.read(); 
//
//} 
//}

//-------------------------------------------------------TEST SYSTEM------------------------— 
void proverka(){ 
//bluetooth();
Serial.println("Проверка bluetooth завершена"); 
dovod();
Serial.println("Проверка доводчика завершена"); 
digitalWrite(zagig, 1); // реле включено 
delay(3000); 
digitalWrite(zagig, 0); // реле выключено 
delay(3000); 
Serial.println("Проверка запуска завершена");
if( butt == 0) { 
centerzamok(); }
if( butt == 1 ) { 
centerzamok(); }
Serial.println("Проверка центрального замка завершена");
climat(); 
Serial.println("Проверка климат контроля завершена");
gabarit();
Serial.println("Проверка габаритов завершена");
bagup();
Serial.println("Проверка открытия багажника завершена");
avar();
Serial.println("Проверка 'аварийки' завершена");
}

//--------------------------------------------------ДОВОДЧИК СТЁКЛ------------------— 
void dovod() 
{ 
//boolean Ruch = digitalRead(4); 
//boolean sgn = digitalRead(8); 
if (Ruch == 1 && sgn == 1){  
digitalWrite(13, 1), 
Serial.print("Стёкла закрыты ") , 
delay(5000);} 
else { 
digitalWrite(13, 0);} 
} 
// —------------------------------------------------АВТОЗАПУСК------------------— 
void zapusk(){ 
  skor = 0; 
//boolean Ruch = digitalRead(4); 
//boolean skor = digitalRead(6); 
if( skor == 0 && Ruch == 1 ){ 
digitalWrite(zagig, 1), 
delay(5000); 
for ( int x = 0; x<4; x++){ // начало цикла троекратной попытки запуска 
digitalWrite (starter, 1); // включаем стартер
Serial.println("starter working"); 
delay (5000); // стартер крутит 5 сек 
digitalWrite (starter, 0); // выключаем стартер
Serial.println("starter no working");  
N = analogRead (tahometr); // считываем данные с датчика оборотов 
oborots = N*20; // вычисляем фактическое значение оборотов 
if (oborots>900){ // если обороты более 900 тогда 
STATUS=true; // присваем переменной СТАТУС значение ИСТИНА 
Serial.println("SYSTEM working"); 
break; // выходим из цикла for 
} 
else { 
delay(2000), 
Serial.print("SYSTEM pop" + x );} 
} 
} 

else { delay (2000); 
} 
} 
// —------------------------------------------------ЦЕНТРАЛЬНЫЙ ЗАМОК------------------— 
void centerzamok(){ 
if( butt == 0) {  
butt = 1 ,
Serial.println("Функция центрально замка активирована"),
Serial.println(butt),
digitalWrite(9, butt); 
} 
else{ 
Serial.println("Функция центрально замка деактивирована"),
butt = 0, 
Serial.println(butt),
digitalWrite(9, butt); 
} 
} 
//--------------------------------------------------------КЛИМАТ КОНТРОЛЬ------------------— 
 void climat(void){
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;

  if ( !ds.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }

  Serial.print("ROM =");
  for( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
  Serial.println();

  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);         // Read Scratchpad

  Serial.print("  Data = ");
  Serial.print(present, HEX);
  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");

//  if(celsius > 30){
//  {myservo1.write(30);} 
//  if(celsius < 30 && celsius > 25)
//  {myservo1.write(50);}
//  if(celsius < 25 && celsius > 20)
//  {myservo1.write(80);}
//  if(celsius < 20 && celsius > 10)
//  {myservo1.write(110);}
//  if(celsius < 10 && celsius > 0)
//  {myservo1.write(150);}
//  if(celsius <0 )
//  {myservo1.write(20);}
// }
 } 
//--------------------------------------------------------БАГАЖНИК------------------— 
 void bagup(){
   
  if(but ==0){
  but=!but;
  digitalWrite(10,but);
    Serial.println("Bag up");
    //myservo3bag.write(145);
    delay(5000);
  }
  if(but ==1){
    but=!but;
    digitalWrite(10,but);
    Serial.println("Bag down");
   // myservo3bag.write(45);
    delay(5000);
  }
 }
//--------------------------------------------------------ГАБАРИТЫ---------------------------------— 
void gabarit(){
if(Ruch == 1 && butt2 == 0) { 
butt2 = 1 , 
digitalWrite(A4, butt2);} 
if(Ruch == 1 && butt2==1){ 
digitalWrite(A4, 0);} } 
//--------------------------------------------------------АВАРИЙКА---------------------------------— 
void avar(){ 
  int i = 0;
  for (int i=0; i <= 10; i++){
digitalWrite(13,1);
digitalWrite(12,1);
  delay(1000);
  digitalWrite(13,0);
  digitalWrite(12,0);
    delay(1000);}
}
