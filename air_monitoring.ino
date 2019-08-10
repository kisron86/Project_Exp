/*
 * SISTEM MONITORING KEBERSIHAN UDARA
 * CO2
 * SO2
 * CO
 * NO2
 * TEMP
 * HUMIDITY
 * AIR PRESS
 * PM2.5 
 * PM10
 */
#include <SoftwareSerial.h>
#include <DHT.h>

#define DHTPIN 9
#define DHTTYPE DHT11   // DHT 11

SoftwareSerial SIM800L(7, 8); //(SIM800L_TX_PIN,SIM800L_RX_PIN)

int i, n = 1;

const int CO = A0;
const int CO2 = A1; //GAS sensor output pin to Arduino analog A0 pin
const int SO2 = A2;
const int NO2 = A3;
const int PM = A3;
const int TEMP = A4;
const int AIR_PRESS = A5;

unsigned int gas, data_gas, gg;
DHT dht(DHTPIN, DHTTYPE);

int d1 = 1;
int d2 = 2;
int d3 = 3;
int d4 = 4;
int d5 = 5;
int d6 = 6;
int d7 = 7;
int d8 = 8;
int d9 = 9;

void hum()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
  delay(500);
}

void setup() {
  delay(10000);
  dht.begin();
  SIM800L.begin(9600);
  Serial.begin(9600);
  SIM800L.flush();
  Serial.flush();
  for (i = 0; i < 2; i++) {
    SIM800L.println("AT+CGATT?");
    delay(100);
    SIM800L.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
    delay(2000);
    SIM800L.println("AT+SAPBR=3,1,\"APN\",\"internet\""); //set APN : -XL=www.xlgprs.net -INDOSAT=indosatgprs -TELKOMSEL=internet -TRI=3gprs
    delay(2000);
    SIM800L.println("AT+SAPBR=1,1");
    delay(500);
  }
  delay(1000);
  
}

void kirim_gprs() {

  SIM800L.println("AT+HTTPINIT"); //inisialisasi http
  delay(2000); //2000

  SIM800L.print("AT+HTTPPARA=\"URL\",\"http://66.42.57.140/air_monitoring/index.php?data1="); //set parameter nilai http
  SIM800L.print(gg);
  SIM800L.print("&data2=");
  SIM800L.print(d2);
  SIM800L.print("&data3=");
  SIM800L.print(d3);
  SIM800L.print("&data4=");
  SIM800L.print(d4);
  SIM800L.print("&data5=");
  SIM800L.print(d5);
  SIM800L.print("&data6=");
  SIM800L.print(d6);
  SIM800L.print("&data7=");
  SIM800L.print(d7);
  SIM800L.print("&data8=");
  SIM800L.print(d8);
  SIM800L.print("&data9=");
  SIM800L.print(d9);
  SIM800L.println("\"");

  Serial.print(d1);
  Serial.print("&data2=");
  Serial.print(d2);
  Serial.print("&data3=");
  Serial.print(d3);
  Serial.print("&data4=");
  Serial.print(d4);
  Serial.print("&data5=");
  Serial.print(d5);
  Serial.print("&data6=");
  Serial.print(d6);
  Serial.print("&data7=");
  Serial.print(d7);
  Serial.print("&data8=");
  Serial.print(d8);
  Serial.print("&data9=");
  Serial.print(d9);
  Serial.println("\"");
  delay(200); //2000

  SIM800L.println("AT+HTTPACTION=1"); //set perintah http (0=GET, 1=POST, 2=HEAD)
  delay(200); //6000

  SIM800L.println("AT+HTTPREAD"); //membaca respon server
  delay(200); //1000

  SIM800L.println("");
  SIM800L.println("AT+HTTPTERM");
  delay(200); //300

  SIM800L.println("");
  Serial.println("Data berhasil ditambahkan ke database.");
  delay(10000);
}

void loop() {

  data_gas = analogRead(CO2);
  gas = data_gas*0.11;//data_gas/870*100;
  gg = gas;
  Serial.print(gas);
  Serial.println(" %");
  delay(150);
    kirim_gprs();
  }

void loop9(){
  //hum();
  kirim_gprs();
}
