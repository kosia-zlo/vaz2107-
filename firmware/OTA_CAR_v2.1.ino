/*********************************************ОБЩАЯ ИНФОРМАЦИЯ***************************************

ПО машины VAZ 2107 "Студентка" 
  Версия                      : 2.1
  Сборка номер                : 1
  Сборка дата                 : 08.05.2019
  Сборка устройства           : Молоткин Константин и выпускник ..... 
  Программное Обеспечение     : Молоткин Константин 
  Курс                        : 4
  Учебное заведение           : Самарский авиационный техникум
  Группа                      : ВМ-54.
  Telegram                    : https://t.me/kosia_zlo 
  Канал                       : https://t.me/joinchat/AAAAAFk49unFlsLftfWZDA
  Последня версия ПО          : https://github.com/kosia-zlo/vaz2107-
  Описание                    : ПО для Arduino Uno встроенного в ВАЗ 2107 с управлением по Bluetooth
  Преподаватель               : Митина Е.Н. 

*/
//************************************************БИБЛИОТЕКА***************************
#include <OneWire.h>
#include <Servo2.h>
#include <EEPROM.h>
//********************************************************НАСТРОЙКИ**************************
#define BT         1 // 0 для выключения. 1 для включения. ВКЛЮЧИТЬ BLUETOOTH
#define PR         1 // 0 для выключения. 1 для включения. ВКЛЮЧИТЬ РЕЖИМ ПРОВЕРКИ
#define OBM        0 // 0 для выключения. 1 для включения. ВКЛЮЧИТЬ ОБМАНКУ
//********************************************************ПОДКЛЮЧЁНИЯ***********************
OneWire  ds(A5); // Подключение датчика температуры
int voltmetric = A0 ; // Подключение провода для измерения напряжения 
int zagig = 3 ; // Зажигание 
int starter = 4 ; // Стартер 
int DverZad = 5; // Задняя дверь
int bug = 6; // Багажник 
int Ruch = 7 ; // Ручник 
int skor = 8; // Скорость 
int sgn = 9; // Сигнализация
int gab = 11; // Габариты
int dov = 12; // Доводчик
int avar = 13; // Аварийка

#define tahometr A1 // Датчик оборотов к аналогову входу = 1;
//********************************************************ИМЕНА ПОДКЛЮЧЕНИЙ*************************
Servo2 servo;
//********************************************************ПРОВЕРЯШКИ*********************
int val; // Значение BT 
int but = 1; // Значение доводчика
int but_z = 1; // Значение Автостарта
int but_c = 1; // Значение Центрального замка
int but_g = 1; // Значение Габаритов
int u = 0; // переменная для числа пребразования 0-1023
float N = 0.0; // Тахометр 
float u_in = 0.0; // переменная для напряжения 0.0-5.0
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
pinMode(avar, OUTPUT); 
pinMode(gab, OUTPUT);
pinMode(bug, OUTPUT);
pinMode(dov, OUTPUT);
pinMode(A0,INPUT);// A0 - аналоговый вход
pinMode(A5, OUTPUT);// пин A0 будет входом 
servo.attach(A4);
Serial.begin(9600); }

void loop() {
 if (BT == 1){
bluetooth();  }
else {
Serial.println("BT ОТКЛЮЧЁН В НАСТРОЙКАХ");  
}
}

//----------------------------------------------------BLUETOOTH-----------------------------— 
void bluetooth(){ 
if (Serial.available()){ 
val = Serial.read();
Serial.println(val);
if (val == 'd') 
{dovod();} 
if (val == 'v') 
{volt;} 
if ( val == 'z')
{zapusk();} 
if ( val == 'c') 
{centerzamok();} 
if ( val == 'k') 
{ climat();}  
if ( val == 'a') 
{avar_on();} 
if ( val == 'g') 
{gabar();} 
if ( val == 'b') 
{bag();} 
if (val == 'p') 
{proverka();} 
} } 
//-------------------------------------------------------TEST SYSTEM------------------------— 
void proverka(){ 
  if(PR == 1){
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
Serial.println("Проверка центрального замка завершена"); 
climat();
Serial.println("Проверка определения температуры салона завершена");
bag();
Serial.println("Проверка открытия багажника завершена");  
gabar();
Serial.println("Проверка габаритных огнеей завершена");
avar_on();
Serial.println("Проверка аварийной сигнализации завершена");
volt();
Serial.println("Проверка напряжения завершена");
delay(1000);
Serial.println("ПРОВЕРКА СИСТЕМЫ ЗАВЕРШЕНА") ;
}
else{
(Serial.println("МОДУЛЬ ПРОВЕРКИ ОТКЛЮЧЁН"))
;}}
//------------------------------------------------------ДОВОДЧИК СТЁКЛ------------------— 
void dovod() { 
digitalWrite(dov, 1), 
delay(5000);
Serial.println("Cтёкла закрыты"),
digitalWrite(dov, 0); 
}
// —------------------------------------------------АВТОЗАПУСК-----------------
void zapusk(){ 
 switch(but_z){
  case 1 :  
  skor = 0; Ruch = 1;
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
if (oborots>800){ // если обороты более 800 тогда 
STATUS=true; // присваем переменной СТАТУС значение ИСТИНА 
Serial.println("Двигатель запущён"); 
but_z = 2;
break; } // выходим из цикла for  
else {  
Serial.print("Попытка запуска" + x );  
Serial.println("Двигатель не запущен");}}}
else {
Serial.println("Скорость или Ручник в неправильном положение");
}
break;
case 2:
digitalWrite(zagig, 0);
digitalWrite(starter, 0);
Serial.println("Двигатель остановлен");
but_z = 1;
break;}}
// —------------------------------------------------ЦЕНТРАЛЬНЫЙ ЗАМОК------------------— 
void centerzamok(){ 
 switch(but_c){ 
 case 1 : 
 Serial.println("Двери закрыты"),
 digitalWrite(DverZad, 1);
 but_c = 2; 
 break; 
 case 2 :
 Serial.println("Двери открыты"),
 digitalWrite(DverZad, 0); 
 but_c = 1;
 break; }}
//--------------------------------------------------------БАГАЖНИК------------------— 
 void bag(){
    digitalWrite(bug,1);
    servo.write(100); //ставим вал под 100
    digitalWrite(bug,0);
    servo.write(0); }//ставим вал под 0
 //--------------------------------------------------------ГАБАРИТЫ---------------------------------— 
void gabar(){
switch(but_g) {
  case 1: Serial.println("Габариты ВКЛ"); digitalWrite(gab, 1);  but_g = 2;  break;
  case 2: Serial.println("Габариты ВЫКЛ"); digitalWrite(gab, 0); but_g = 1; break;}}
//--------------------------------------------------------АВАРИЙКА---------------------------------— 
void avar_on(){ 
  int i = 0;
  for (int i=0; i <= 10; i++){
digitalWrite(13,1); delay(1000); digitalWrite(13,0); delay(1000);}}
//--------------------------------------------------------ВОЛЬТМЕТР---------------------------------— 
void volt() {
  u = analogRead(A0); // измерение 
  Serial.println("U1 = ");
  Serial.println(u);
  u_in = (u * 4.77) / 1023; // пересчет измерения в вольты
  Serial.println("U2 = ");
  Serial.print(u_in); // вывод измерения на экран
  Serial.print(" V");
  delay(1500);}
//--------------------------------------------------------КЛИМАТ КОНТРОЛЬ------------------— 
 void climat(void){
  byte i; byte present = 0; byte type_s; byte data[12]; byte addr[8]; float celsius;
  if ( !ds.search(addr)) {
    Serial.println("Нет больше адресов.");Serial.println();
    ds.reset_search(); delay(250); return;}
  Serial.print("ROM =");   // Вывод адреса 
  for( i = 0; i < 8; i++) { Serial.write(' '); Serial.print(addr[i], HEX); } 
  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!"); return;}
  Serial.println();
    switch (addr[0]) { // Определяем модель датчика температуры 
    case 0x28: Serial.println("  Chip = DS18B20"); type_s = 0; break; // выводим название датчика Chip = DS18B20 - мой датчик 
    case 0x10: Serial.println("  Chip = DS18S20"); type_s = 1; break; // выводим название датчика Chip = DS18S20 или старые модели 
    case 0x22: Serial.println("  Chip = DS1822"); type_s = 0; break;  // выводим название датчика Chip = DS1822
    default: Serial.println("Датчик не из семейства DS18x20.");// Если датчик не относится к семейству DS18x20
      return; } 
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        
  delay(1000);     // не ставить задержку меньше 750
  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);         
  for ( i = 0; i < 9; i++) {        
    data[i] = ds.read();}
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; 
    if (data[7] == 0x10) { 
      raw = (raw & 0xFFF0) + 12 - data[6]; }} else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw & ~7;  
    else if (cfg == 0x20) raw = raw & ~3; 
    else if (cfg == 0x40) raw = raw & ~1;}
  celsius = (float)raw / 16.0; 
  Serial.print("  Температура = "); Serial.print(celsius); Serial.print(" C ");
 }
