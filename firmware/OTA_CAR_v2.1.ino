/*********************************************ОБЩАЯ ИНФОРМАЦИЯ***************************************

ПО машины VAZ 2107 "Студентка" 
  Версия                      : 2.1
  Сборка номер                : 1
  Сборка дата                 : 22.04.2019
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
#include <Servo2.h>
#include <EEPROM.h>


//********************************************************НАСТРОЙКИ**************************
#define BT         1 // 0 для выключения. 1 для включения. ВКЛЮЧИТЬ BLUETOOTH
#define PR         1 // 0 для выключения. 1 для включения. ВКЛЮЧИТЬ РЕЖИМ ПРОВЕРКИ
#define OBM        1 // 0 для выключения. 1 для включения. ВКЛЮЧИТЬ ОБМАНКУ



//********************************************************ПОДКЛЮЧЁНИЯ***********************
OneWire  ds(2); // Подключение датчика температуры
int zagig = 3 ; // Зажигание 
int starter = 4 ; // Стартер 
int DverZad = 5; //
int bug = 6; // Багажник 
int Ruch = 7 ; // Ручник 
int skor = 8; // Скорость 
int sgn = 9; // Сигнализация
int gab = 10; // Габариты
int avar = 11; // Аварийка
#define tahometr A1 // Датчик оборотов к аналогову входу = 1;


//********************************************************ИМЕНА ПОДКЛЮЧЕНИЙ*************************
Servo2 servo;



//********************************************************ПРОВЕРЯШКИ*********************
int val; // Значение BT 
int but = 1; // Значение доводчика
int but_z = 1; // Значение Автостарта
int but_c = 1; // Значение Центрального замка
int but_g = 1; // Значение Габаритов
float N = 0.0; // Тахометр 
float oborots = 0.0; // Фактические обороты 
bool STATUS = false; // Статус системы 
/* ------------------------------------------------------------------------------------ */
void setup() {
pinMode(zagig, OUTPUT); 
pinMode(starter, OUTPUT);
pinMode(DverZad, OUTPUT) ;  
pinMode(Ruch, INPUT) ; 
pinMode(skor, INPUT) ; 
pinMode(tahometr, INPUT); // конфигурируем оборотов как вход
pinMode(sgn, INPUT); 
pinMode(13, OUTPUT); 
pinMode(12, OUTPUT);
pinMode(A0, INPUT); 
pinMode(A4, OUTPUT);// пин A0 будет входом 
servo.attach(12);
Serial.begin(9600);  
}

void loop() {
 if (BT == 1){
bluetooth();
//bluetooth_data(); 
}
}
void bluetooth(){ 
if (Serial.available()){ 
val = Serial.read(); 
if (val == 'd') 
{dovod();} 
if (val == 't') 
{;} 
if ( val == 'z')
{zapusk();} 
if ( val == 'c') 
{centerzamok();} 
if ( val == 'k') 
{klimat;} 
if ( val == 'a') 
{avar_on;} 
if ( val == 'g') 
{gabar;} 
if ( val == 'b') 
{bag;} 
if (val == 'p') 
{proverka();} 
} } 
//----------------------------------------------------BLUETOOTH DATA-----------------------------— 
//void bluetooth_data(){ 
//if (Serial.available()){ 
//val2 = Serial.read(); } 
//}
//-------------------------------------------------------TEST SYSTEM------------------------— 
void proverka(){ 
  if(PR == 1){
bluetooth();
Serial.println("Проверка bluetooth завершена"); 
//bluetooth();
dovod();
delay(500);
dovod();
delay(500);
Serial.println("Проверка доводчиков завершена");
zapusk();
delay(500);
zapusk();
delay(500);
Serial.println("Проверка автозапуска завершена"); 
centerzamok();
delay(500);
centerzamok();
delay(500);
Serial.println("Проверка центрального замка завершена"); 
klimat();
Serial.println("Проверка климата завершена");
bag();
Serial.println("Проверка багажника завершена");  
gabar();
Serial.println("Проверка габаритов завершена");
avar_on();
Serial.println("Проверка аварийки завершена"); 
}
}
//------------------------------------------------------ДОВОДЧИК СТЁКЛ------------------— 
void dovod() { 
  switch(but){ 
  case 1:
//boolean Ruch = digitalRead(4); boolean sgn = digitalRead(8);
Serial.println("Доводчик стёкл включён");
Serial.println("dov_on");
if(OBM == 1){
N = analogRead (tahometr); // считываем данные с датчика оборотов 
oborots = N*20/1000;} // вычисляем фактическое значение оборотов 
if(OBM == 0){
N = analogRead (tahometr); // считываем данные с датчика оборотов 
oborots = N*20;}
if (oborots<100){ // если обороты равны 0
if (Ruch == 1 && sgn == 1){  
digitalWrite(13, 1), 
Serial.println("Cтёкла закрыты"),
delay(5000);} }
but = 2;
break;
case 2:
Serial.println("Доводчик стёкл выключён");
Serial.println("dov_off");   
but = 1;
break;
}
}
// —------------------------------------------------АВТОЗАПУСК-----------------
void zapusk(){ 
 switch(but_z){
  case 1 : 
  skor = 0; 
  Ruch = 1;
//boolean Ruch = digitalRead(4); 
//boolean skor = digitalRead(6); 
if( skor == 0 && Ruch == 1 ){ 
digitalWrite(zagig, 1), 
delay(5000); // ждём работы бензонасоса.
for ( int x = 0; x<4; x++){ // начало цикла троекратной попытки запуска 
digitalWrite (starter, 1); // включаем стартер
Serial.println("Стартер работает"); 
delay (5000); // стартер крутит 5 сек 
digitalWrite (starter, 0); // выключаем стартер
Serial.println("Стартер отключён"); 
N = analogRead (tahometr); // считываем данные с датчика оборотов 
oborots = N*20; // вычисляем фактическое значение оборотов 
Serial.println(N);
Serial.println(oborots);
if (oborots>900){ // если обороты более 900 тогда 
STATUS=true; // присваем переменной СТАТУС значение ИСТИНА 
Serial.println("Двигатель запущён"); 
but_z = 2;
break; // выходим из цикла for 

} 
else { 
delay(2000), 
Serial.print("SYSTEM pop" + x );} 
} 
}
break;
case 2:
digitalWrite(zagig, 0);
Serial.println("Двигатель остановлен");
Serial.println(oborots);
but_z = 1;
break;
}
}
// —------------------------------------------------ЦЕНТРАЛЬНЫЙ ЗАМОК------------------— 
void centerzamok(){ 
 switch(but_c){ 
 case 1 : 
 
 Serial.println("Функция центрально замка активирована"),
 digitalWrite(DverZad, 1);
 but_c = 2; 
 break; 
 case 2 :
 Serial.println("Функция центрально замка деактивирована"),
 digitalWrite(DverZad, 0); 
 but_c = 1;
 break; 
 }
}
//--------------------------------------------------КЛИМАТ КОНТРОЛЬ------------------— 
void klimat(void){
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
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");
}
//--------------------------------------------------------БАГАЖНИК------------------— 
 void bag(){
    digitalWrite(bug,1);
    servo.write(100); //ставим вал под 0
    Serial.println("Замок открыт");
    delay(5000);
    digitalWrite(bug,0);
    servo.write(0); //ставим вал под 0
  }
 //--------------------------------------------------------ГАБАРИТЫ---------------------------------— 
void gabar(){
switch(but_g) {
  case 1:
 but_g = 2;  
digitalWrite(gab, 1); 
break;
 case 2:
 but_g = 1;  
digitalWrite(gab, 0); 
break;
}
}
//--------------------------------------------------------АВАРИЙКА---------------------------------— 
void avar_on(){ 
  int i = 0;
  for (int i=0; i <= 10; i++){
digitalWrite(13,1);
  delay(1000);
  digitalWrite(13,0);
    delay(1000);}
}
