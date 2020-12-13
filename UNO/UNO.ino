#include <Servo.h>

//Servo myservo3;
//Servo myservo23;


void trigger1_f(uint8_t trigger1);
void trigger2_f(uint8_t trigger2);

unsigned long currentMillis;
unsigned long millis_onMotor_button_start = 0;

uint8_t turn_motor = 0;
uint8_t mode_speed = 0;

Servo myservo3;
Servo myservo4;
Servo myservo5;
Servo myservo6;

int8_t selectHead;
int8_t selectHeadselect = 0;

#include <Ethernet.h>
#include <EthernetUdp.h>

// #include "navatar/mavlink.h"
// #include "navatar/mavlink_msg_navatar.h"
// #include "navatar/mavlink_msg_axis_configured.h"
// #include "navatar/mavlink_msg_button_configured.h"
// #include "navatar/mavlink_msg_connected_game_pad.h"
// #include "navatar/mavlink_msg_gamepad_axis_event.h"
// #include "navatar/mavlink_msg_gamepad_button_press_event.h"
// #include "navatar/mavlink_msg_gamepad_button_release_event.h"

// mavlink_gamepad_button_press_event_t            smth;
// mavlink_gamepad_button_release_event_t          smth2;
// mavlink_gamepad_axis_event_t                    smth3;

uint8_t forward_4 = 0; //вперёд два колеса 1-вперёд 0-стоп
uint8_t back_4 = 0; //назад два колеса  1-назад 0-стоп
void MCP41010Write(byte value);

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

IPAddress ip(224, 224, 224, 1);
unsigned int localPort = 7878;      // local port to listen on
byte packetBuffer[64];  // buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged";        // a string to send back
EthernetUDP Udp;

#include <SPI.h>  // подключаем библиотеку SPI

  uint8_t onDriveMotor = 0;

  uint8_t trigger1 = 0;
  uint8_t pot_trigger1 = 0;

  uint8_t trigger2 = 0;
  uint8_t pot_trigger2 = 0;
  String inString;

  
  //uint8_t c = 0;
  //int b1 = 0;

void setup() {
  // start the Ethernet
  Ethernet.begin(mac, ip);
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }
  // start UDP
  Udp.begin(localPort);
  //myservo3.attach(3);
  //myservo23.attach(23);
  
  myservo3.attach(3);
  myservo4.attach(4);
  myservo5.attach(5);
  myservo6.attach(6);

   SPI.begin(); // инициализация SPI
   
    pinMode(25, OUTPUT); //relay driver_motor
    digitalWrite(25, 1);
    pinMode(26, OUTPUT); //motor_left_down
    digitalWrite(26, 1);
    pinMode(27, OUTPUT); //motor_right_down
    digitalWrite(27, 1);
    pinMode(49, OUTPUT); //motor_left
    pinMode(48, OUTPUT); //motor_right


    pinMode(10, OUTPUT);
    pinMode(53, OUTPUT); 
     digitalWrite(48,HIGH);
     digitalWrite(49,HIGH);
     digitalWrite(10,HIGH);
     


    
    digitalWrite(49,LOW);
    
      for (int level = 255; level > 0; level--)
         {
            MCP41010Write(level);
         }
    digitalWrite(49,HIGH);

    digitalWrite(48,LOW);
       for (int level = 255; level > 0; level--)
       {
          MCP41010Write(level);
       }
    digitalWrite(48, HIGH);

}

void loop() {
    digitalWrite(10,HIGH);
    

    currentMillis = millis();
    //Serial.println("работает");
    int packetSize = Udp.parsePacket();

    if (packetSize) {

    Udp.read(packetBuffer, 64);

    


    for(int i = 0; packetSize > i; i++){

      inString+=(char)packetBuffer[i];
      //Serial.print((char)packetBuffer[i]); 
      //Serial.print(","); 

    }
    // Serial.print(inString);
    // Serial.println(); 

    String sa[8]; 
    int r=0, t=0;
    
          for (int i=0; i < inString.length(); i++)
          { 
            if((char)packetBuffer[i] == ',') 
              { 
                sa[t] = inString.substring(r, i); 
                r=(i+1); 
                t++; 
              }
          }



    Serial.print(sa[0].toInt()); //dPad
    Serial.print("  ");
    Serial.print(sa[1].toInt()); //dPad
     Serial.print("  ");
    Serial.print(sa[2].toInt()); //button
     Serial.print("  ");
    Serial.print(sa[3].toInt()); //rxStyck
     Serial.print("  ");
    Serial.print(sa[4].toInt()); //ryStyck
     Serial.print("  ");
    Serial.print(sa[5].toInt()); //lxStyck
     Serial.print("  ");
    Serial.print(sa[6].toInt()); //lyStyck
     Serial.print("  ");
    Serial.print(sa[7].toInt()); //rTrigger
     Serial.print("  ");
    Serial.print(sa[8].toInt()); //rTrigger


    Serial.println();

  inString = "";

    // uint16_t asimut  = ((byte)packetBuffer[9] << 8) | (byte)packetBuffer[10]; //Compass
    // int8_t head_z  = packetBuffer[11]; //slop_left_rigt
    // int16_t head_y  = ((byte)packetBuffer[12] << 8) | (byte)packetBuffer[13]; //slop_ud_dowm






          if(sa[1].toInt()  == 99) digitalWrite(25, 0); //on relay motor driver
          if(sa[1].toInt()  == 100) digitalWrite(25, 1); //off relay motor driver

          // if((uint8_t)packetBuffer[2] == 12 ) {  //вперёд trigger
          //   digitalWrite(26, 1);
          //   digitalWrite(27, 1);
          //   turn_motor = 0;
          // }
          
          // if((uint8_t)packetBuffer[2] == 13 ) {  //назад trigger
          //   digitalWrite(26, 0);
          //   digitalWrite(27, 0);
          //   turn_motor = 0;
          // } 

          // if((uint8_t)packetBuffer[2] == 15 ) { //разворот влево-вправо
          //     digitalWrite(26, 0);
          //     digitalWrite(27, 1);
          //     turn_motor = 1;
          // }

          // if((uint8_t)packetBuffer[2] == 14 ) { //разворот вправо-влево
          //     digitalWrite(26, 1);
          //     digitalWrite(27, 0);
          //     turn_motor = 1;
          // }

          // if((uint8_t)packetBuffer[2] == 4 ) { //впёрёд-назад trigger one
          //     digitalWrite(26, 1);
          //     digitalWrite(27, 1);
          //     turn_motor = 2;
          // }  

          // if((uint8_t)packetBuffer[2] == 8 ) { //режим скорости
          //     mode_speed = 0;
          // }
          
          // if((uint8_t)packetBuffer[2] == 9 ) { //режим скорости
          //     mode_speed = 1;
          // }    

          // if((uint8_t)packetBuffer[2] == 7 && turn_motor == 0 ) { digitalWrite(25, 0); trigger2_f((uint8_t)packetBuffer[1]) ; millis_onMotor_button_start = currentMillis; Serial.println((uint8_t)packetBuffer[1]); } //left motor
          // if((uint8_t)packetBuffer[2] == 6 && turn_motor == 0 ) { digitalWrite(25, 0); trigger1_f((uint8_t)packetBuffer[1]) ; millis_onMotor_button_start = currentMillis; Serial.println((uint8_t)packetBuffer[1]); } //right motor


          // if((uint8_t)packetBuffer[2] == 6 && turn_motor == 1 ) { 
          //   digitalWrite(25, 0);
          //   digitalWrite(26, 0);
          //   digitalWrite(27, 1);
          //   trigger1_f((uint8_t)packetBuffer[1]);
          //   trigger2_f((uint8_t)packetBuffer[1]);
          //   millis_onMotor_button_start = currentMillis; 
          // } 

          // if((uint8_t)packetBuffer[2] == 7 && turn_motor == 1 ) { 
          //   digitalWrite(25, 0);
          //   digitalWrite(26, 1);
          //   digitalWrite(27, 0);
          //   trigger1_f((uint8_t)packetBuffer[1]);
          //   trigger2_f((uint8_t)packetBuffer[1]);
          //   millis_onMotor_button_start = currentMillis; 
          // } 
          

          // if((uint8_t)packetBuffer[2] == 6 && turn_motor == 2 ) { 
          //   digitalWrite(25, 0); digitalWrite(26, 1);digitalWrite(27, 1); trigger1_f((uint8_t)packetBuffer[1]); trigger2_f((uint8_t)packetBuffer[1]); millis_onMotor_button_start = currentMillis; 
          // } //left motor

          // if((uint8_t)packetBuffer[2] == 7 && turn_motor == 2 ) { 
          //   digitalWrite(25, 0); digitalWrite(26, 0);digitalWrite(27, 0); trigger1_f(packetBuffer[1]); trigger2_f((uint8_t)packetBuffer[1]); millis_onMotor_button_start = currentMillis; } //right motor
          

          // Serial.println(trigger2);


          // if((uint8_t)packetBuffer[2] == 6 ) { trigger1_f(0);   } //left motor
          // if((uint8_t)packetBuffer[2] == 7 ) { trigger2_f(0);   } //right motor
          // if((uint8_t)packetBuffer[2] == 6 && turn_motor == 2 ) { trigger1_f(0); trigger2_f(0);  } //left motor
          // if((uint8_t)packetBuffer[2] == 7 && turn_motor == 2 ) { trigger1_f(0); trigger2_f(0);  } //right motor
          // if((uint8_t)packetBuffer[2] == 6 &&  turn_motor == 1) { trigger1_f(0); trigger2_f(0); } //left motor
          // if((uint8_t)packetBuffer[2] == 7 &&  turn_motor == 1) { trigger1_f(0); trigger2_f(0);  } //right motor
          // Serial.println((uint8_t)packetBuffer[2]);

          
          selectHead =  sa[1].toInt();
          //Serial.println(selectHead);

          if (selectHead == 4 || selectHead == 8)
          {
            selectHeadselect = 0;
          };

          if (selectHead == 108 || selectHead == 9)
          {
            selectHeadselect = 1;
          };

          if(selectHeadselect == 0){
            //myservo6.write(90 + sa[2].toInt());  //y
            myservo3.write(90 + sa[3].toInt());  //y
            myservo4.write(90 + sa[2].toInt());  //z
            myservo5.write(90 + sa[4].toInt());  //x
          };

          if(selectHeadselect == 1){

          uint16_t asimut  = ((byte)packetBuffer[9] << 8) | (byte)packetBuffer[10]; //Compass
          //int8_t head_z  = packetBuffer[11]; //slop_left_rigt
          int16_t head_y  = ((byte)packetBuffer[12] << 8) | (byte)packetBuffer[13]; //slop_ud_dowm

            if(head_y < 0)
           { myservo3.write(180 - (180+ head_y)); myservo6.write(180 - (180 + ~head_y)); }

            myservo4.write(90 + ~(int8_t)packetBuffer[11]);  

            //myservo5.write(asimut);
            //Serial.println(asimut);

            myservo5.writeMicroseconds(3000-(500 + (asimut * 5.5555)));

            }

    };
     

    

    // myservo5.writeMicroseconds(500);
    // delay(4000);
    // myservo5.writeMicroseconds(2500);
    // delay(4000);



        if(currentMillis - millis_onMotor_button_start >= 30000){
             digitalWrite(25, 1);
             //millis_onMotor_button_start = currentMillis;
        };



           
}

    

      //b1 = trigger1;
      //Serial.println(trigger1);

      void trigger1_f(uint8_t trigger1) { //right motor
      digitalWrite(10,HIGH);
      

      if (mode_speed == 0) trigger1 = trigger1 / 6;
      digitalWrite(49,LOW);
           if(trigger1 != pot_trigger1 && trigger1 > pot_trigger1){
           for (int level = pot_trigger1; level <= trigger1; level++)
                 {
                    MCP41010Write(level);
                 }
           }

            if(trigger1 != pot_trigger1 && trigger1 < pot_trigger1){
            for (int level = pot_trigger1; level >= trigger1; level--)
                 {
                    MCP41010Write(level);
                 }
            }

          digitalWrite(49,HIGH);
          digitalWrite(10,LOW);
          
          pot_trigger1 = trigger1;
      }

      void trigger2_f(uint8_t trigger2) { //left motor
        if (mode_speed == 0) trigger2 = trigger2 / 7;
        digitalWrite(10,HIGH);
        
        digitalWrite(48,LOW);
            if(trigger2 != pot_trigger2 && trigger2 > pot_trigger2){
            for (int level = pot_trigger2; level <= trigger2; level++)
                  {
                      MCP41010Write(level);
                  }
            }

              if(trigger2 != pot_trigger2 && trigger2 < pot_trigger2){
              for (int level = pot_trigger2; level >= trigger2; level--)
                  {
                      MCP41010Write(level);
                  }
              }
        digitalWrite(48,HIGH);
        digitalWrite(10,LOW);
        
        pot_trigger2 = trigger2;
      }


void MCP41010Write(byte value)
{
  //digitalWrite(CS,LOW); // выбор микросхемы
   SPI.transfer(B00010001); // командный байт
   SPI.transfer(value); // байт данных 
   //digitalWrite(CS,HIGH); // снимаем выбор с микросхемы
}
