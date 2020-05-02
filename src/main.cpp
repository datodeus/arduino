#include <Arduino.h>
#include <Print.h>
#include <dht11.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>


//MCU690614 - Infrared thermal
#define RXPIN 2 // pin used to receive from MCU90614
#define TXPIN 3 // // pin used to send to MCU90614
SoftwareSerial serial_thermal(RXPIN, TXPIN);
unsigned char Re_buf[11],counter=0;
unsigned char sign=0;
float TO=0,TA=0;
int i = 0;


struct IR_temp{
  float object_temp = 1000;
  float envi_temp = 10000;
};

struct DHT11{
  int humidity = 0;
  int temperature = 0;
};


void setup() {

  delay(100);


  //Serial setup for thermal
  pinMode(RXPIN, INPUT);
  pinMode(TXPIN, OUTPUT);

  Serial.begin(9600);
  Serial.print("Data frame is being sent");
  delay(100);

  serial_thermal.begin(9600);
  serial_thermal.write(0XA5);
  serial_thermal.write(0X15);
  serial_thermal.write(0XBA);
  Serial.println("Data frame sent");
  delay(1000);


}

DHT11 get_humid_temp(void){
  DHT11 humid_temp_data;

  return humid_temp_data;
}

IR_temp get_ir_temp(void){

  IR_temp ir_temperature;

  ir_temperature.object_temp = 20.0;
  ir_temperature.envi_temp = 30.0;


  unsigned char i=0,sum=0;
  delay (100);
  //while (serial_thermal.available()>0) {
  if   (serial_thermal.available()>0) {
    Re_buf[counter]=(unsigned char)serial_thermal.read();
    sign=0;
    for(i=0;i<8;i++){
      sum+=Re_buf[i];
        //Serial.print("sum:"); Serial.println(sum);
        //Serial.print("Re_buf[i]"); Serial.println(Re_buf[i]);
    }
    if(sum==Re_buf[i] ){        //Check frame header, frame tail
      ir_temperature.object_temp = (float)(Re_buf[4]<<8|Re_buf[5])/100;
      //TO=(float)(Re_buf[4]<<8|Re_buf[5])/100;
      //ir_temperature.object_temp = TO;
      Serial.print("TO:");
      Serial.println(ir_temperature.object_temp);

      ir_temperature.envi_temp = (float)(Re_buf[6]<<8|Re_buf[7])/100;
      //TA=(float)(Re_buf[6]<<8|Re_buf[7])/100;
      //ir_temperature.envi_temp = TA;
      Serial.print("TA:");
      Serial.println(ir_temperature.envi_temp);
      delay(10000);
    }

    if(counter==0&&Re_buf[0]!=0x5A){

      return ir_temperature;      // Check frame header

    }
    counter++;
    if(counter==9){                //Received data
      counter=0;                 //Reassignment, prepare for the next frame of data reception
      sign=1;
    }
  }


  return ir_temperature;
}

// the loop function runs over and over again forever
void loop() {

  IR_temp read_temperature;

  read_temperature = get_ir_temp();

  Serial.print("Object: "); Serial.println(read_temperature.object_temp);
  Serial.print("Envi: "); Serial.println(read_temperature.envi_temp);
  delay(100);

  i= i+1;
  Serial.println(i);
  delay(10);


}
