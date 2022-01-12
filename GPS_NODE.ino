/* https://randomnerdtutorials.com/guide-to-neo-6m-gps-module-with-arduino/ */
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <RH_RF95.h>
#include <Wire.h>
#include <SPI.h>
#include <LowPower.h>

// for  Dragino Shield + UNO
#define RFM95_CS 10           // SS pin D10
#define RFM95_RST 4
#define RFM95_INT 2            //(interrupt 0 pin D2)
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT); // RH_RF95 rf95(10, 2); // Dragino with RFM95
#define RF95_FREQ 905.1    //頻率915.0MHz


// The TinyGPS++ object
TinyGPSPlus gps;

//pin and baud rate define 
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
const int ledPin = 7;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

//satellites found 
bool findFlag = false;

int counter=0;

//broadcast content
double latCur = 0;
double lngCur = 0;
String monCur = "";
String dayCur = "";
String hourCur = "";
String minCur = "";
String secCur = "";
String dateCur = "";
String timeCur = "";
const int digit = 6;
const String device = "001";

// for sleep mode
int sleepTimes = 0;
const int totalSleepTimes = 2;
bool sleepFlag = false;

uint8_t MSG[12];
uint32_t CK_A, CK_B;

void deepSleepMode(int sleepTimes){
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
  if(sleepTimes == totalSleepTimes){
    Serial.println("Arduino wakes up");
  }
}

void setup(){
  Serial.begin(9600);
  ss.begin(GPSBaud);
  pinMode(ledPin, OUTPUT);
  
  if (!rf95.init()){
    Serial.println("init failed");   
  }
  else{
    //    
  }
}

void loop(){

  if(sleepFlag == false){
    ss.begin(GPSBaud);
    //powerSet(0x00);
    //delay(1000);
    findFlag = false;
    while (findFlag == false && sleepFlag == false && ss.available() > 0){
      gps.encode(ss.read());
      if (gps.location.isUpdated()){
        if(gps.satellites.value() > 0)
          findFlag = true;
        else
          findFlag = false;
      }

      if (findFlag == true){
        //Serial.print("Lat= "); 
        Serial.print(gps.location.lat(), digit);
        
        //Serial.print(" Log= "); 
        Serial.print(" ");
        Serial.print(gps.location.lng(), digit);
        
        //Serial.print(" Date= "); 
        Serial.print(" ");
        Serial.print(gps.date.year());
        Serial.print(gps.date.month());
        Serial.print(gps.date.day());
        
        //Serial.print(" Time= ");
        Serial.print(" "); 
        int GMT = (int)((gps.location.lng() + 7.5)/15);
        Serial.print(gps.time.hour()+GMT);
        Serial.print(gps.time.minute());
        Serial.println(gps.time.second());
        
        latCur = gps.location.lat();
        lngCur = gps.location.lng();
        monCur = (gps.date.month() < 10)?("0" + String(gps.date.month())):String(gps.date.month());
        dayCur = (gps.date.day() < 10)?("0" + String(gps.date.day())):String(gps.date.day());
        hourCur = (gps.time.hour()+GMT < 10)?("0" + String(GMT+gps.time.hour())):String(GMT+gps.time.hour());
        minCur = (gps.time.minute() < 10)?("0" + String(gps.time.minute())):String(gps.time.minute());
        secCur = (gps.time.second() < 10)?("0" + String(gps.time.second())):String(gps.time.second());
        dateCur = String(gps.date.year()) + monCur + dayCur;
        timeCur = hourCur + minCur + secCur;
  
        uint8_t data[48];
        //String total_countString = String(latCur,digit);
        String total_countString = dateCur + " " + timeCur + " " + device + " " + String(latCur,digit)+ " " + String(lngCur,digit);
        for (int i=0 ; i< 48; i++){
            data[i] = total_countString.charAt(i);
        }
  
        rf95.send(data, sizeof(data));
        rf95.waitPacketSent();
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
  
        if (rf95.waitAvailableTimeout(3000)){    
          if (rf95.recv(buf, &len)){
              Serial.print("got reply: ");
              Serial.println((char*)buf);

              
           
              String lineOut= (char*)buf;
              lineOut= lineOut+ "                         ";
              String Line0  = lineOut.substring(0,15);
              String Line1  = "Rx:"+lineOut.substring(19,42);     
              int rssiRev = rf95.lastRssi();    
              
          }
          else{
            Serial.println("recv failed");
          }
        }
        else{
          Serial.println("No reply, is rf95_server running?");    
        }
        
        delay(4000);
        counter++;
        sleepFlag = true;
      }
    }
  }

  else{
      //powerSet();
      ss.end();
      while(sleepTimes<=totalSleepTimes){
          if(sleepTimes == 0){
            Serial.println("Arduino Sleep Mode");
            delay(500);
          }
          deepSleepMode(sleepTimes);
          sleepTimes++;
      }
      
      sleepTimes = 0;
      sleepFlag = false;
  }
}
