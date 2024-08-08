#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <LiquidCrystal_I2C.h>

const char* ssid      = "ISI SSID WiFi";
const char* password  = "ISI PASS WiFi";
const char* ntpServer = "id.pool.ntp.org";
const int   timezone  = 25200;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, timezone, 60000);
LiquidCrystal_I2C lcd(0x27, 16, 2);
String formatUpdate;
String daystamp;
String timestamp;
int jam;
int menit;
int detik;
int hari; 
int tanggal; 
int bulan;  
int tahun; 

void wifi(){
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Berhasil Terkoneksi dengan WiFi");
  Serial.print("Ip-Address : ");
  Serial.println(WiFi.localIP());
  lcd.setCursor(0,0);
  lcd.print("Terkoneksi!");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  delay(2000);
  lcd.clear();
}

void setup(){
  Serial.begin(115200);
  wifi();
  timeClient.begin();
  lcd.begin();
}

void loop(){
  delay(1000);
  timeClient.update();
  while(!timeClient.update()){
    timeClient.forceUpdate();
  }
  delay(1000);
  extractInfoNTP();
  convertionIndividual();
  SerialOut();
  ScreenOut();
}

void extractInfoNTP(){
  formatUpdate = timeClient.getFormattedTime();
  int splitT   = formatUpdate.indexOf("T");
  daystamp     = formatUpdate.substring(0, splitT);
  timestamp    = formatUpdate.substring(splitT+1, formatUpdate.length()-1);
  delay(1000);
}

void convertionIndividual(){
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime(&epochTime);
  jam = ptm->tm_hour;
  menit = ptm->tm_min;
  detik = ptm->tm_sec;
  hari = ptm->tm_wday; 
  tanggal = ptm->tm_mday;
  bulan = ptm->tm_mon + 1; 
  tahun = ptm->tm_year + 1900;
}

void SerialOut(){
  Serial.print("Waktu saat ini:");
  Serial.println(timestamp);
  Serial.print("Hari saat ini: ");
  Serial.println(daystamp);
}

void ScreenOut(){
  lcd.setCursor(0,0);
  lcd.print("Waktu Saat ini:");
  lcd.setCursor(0,1);
  lcd.print(jam);
  lcd.print(":");
  lcd.print(menit);
  lcd.print(":");
  lcd.print(detik);
}
