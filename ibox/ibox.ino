
#include <XBee.h>
#include <SPI.h>
#include <Wire.h>

//Xbee programs
XBee xbee = XBee();
char condition = '0'; 
uint8_t msg;                                  // 0(Save) 1(Emergency) 3(unregistered)
uint8_t msg_type = 'R';
uint8_t payload[] = { msg_type, msg};
uint8_t regpayload[10];
char respon[10];
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

XBeeAddress64 addr64 = XBeeAddress64(0x00000000, 0x00000000);

ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

ZBTxRequest zbTxReg = ZBTxRequest(addr64, regpayload, sizeof(regpayload));
ZBTxStatusResponse txStatusReg = ZBTxStatusResponse();

const int resetButton = 2;                                // Reset Button
int lastState = HIGH;  
int currentState;     
unsigned long pressedTime  = 0;
bool isPressing = false;
bool isLongDetected = false;
#define buzzerPin 13

void setup() {
  Serial.begin(9600);
  pinMode(resetButton, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
 

}

void loop() {
  // put your main code here, to run repeatedly:
    SensorInfus();
}
int count;
void SensorInfus(){
      currentState = digitalRead(resetButton);
  if(lastState == HIGH && currentState == LOW) {        // button is pressed
    pressedTime = millis();
    isPressing = true;
    isLongDetected = false;
   
  } else if(lastState == LOW && currentState == HIGH) { // button is released
    isPressing = false;
     count ++;
    Serial.println ("jumlah count  = ");
    Serial.println (count);
  }

  if(isPressing == true && isLongDetected == false) {
    long pressDuration = millis() - pressedTime;
    
      //Serial1.println(pressDuration);
    if( pressDuration > 3000 ) {
      
      Serial.println ("GAGAL");
      xbeesend();
     // isLongDetected = true;
    }
  }
  lastState = currentState;
  }
  void xbeesend(){
   xbee.send(zbTx);
  if (xbee.readPacket(500)) {           
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);
      if (txStatus.getDeliveryStatus() == SUCCESS){
          delay(100);
      isLongDetected = true;
      }}} 
}
