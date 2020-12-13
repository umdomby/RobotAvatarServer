#include <Servo.h>


void trigger1_f(uint8_t trigger1);
void trigger2_f(uint8_t trigger2);
void speedNull();


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
int8_t selectMove = 0;

int8_t selectMoveMode = 0;

#include <Ethernet.h>
#include <EthernetUdp.h>


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


    //digitalWrite(48,HIGH);
    //digitalWrite(49,HIGH);

    
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
    //Serial.print(inString);
    //Serial.println(); 

    String sa[32]; 
    int r=0, t=0;
    
          for (int i=0; i < inString.length(); i++)
          { 
            if((char)packetBuffer[i] == ',') 
              { 
                sa[t] = inString.substring(r, i);
                Serial.print(t);
                Serial.print("=");  
                Serial.print(sa[t]);
                Serial.print("  ");
                r=(i+1); 
                t++; 
              }
          }


    Serial.println();



    //Serial.print(sa[0].toInt()); //dPad
    // Serial.print("  ");
    // Serial.print(sa[1].toInt()); //dPad
    //  Serial.print("  ");
    // Serial.print(sa[2].toInt()); //button
    //  Serial.print("  ");
    // Serial.print(sa[3].toInt()); //rxStyck
    //  Serial.print("  ");
    // Serial.print(sa[4].toInt()); //ryStyck
    //  Serial.print("  ");
    // Serial.print(sa[5].toInt()); //lxStyck
    //  Serial.print("  ");
    // Serial.print(sa[6].toInt()); //lyStyck
    //  Serial.print("  ");
    // Serial.print(sa[7].toInt()); //rTrigger
    //  Serial.print("  ");
    // Serial.print(sa[8].toInt()); //rTrigger

    // Serial.println();

  inString = "";

    // uint16_t asimut  = ((byte)packetBuffer[9] << 8) | (byte)packetBuffer[10]; //Compass
    // int8_t head_z  = packetBuffer[11]; //slop_left_rigt
    // int16_t head_y  = ((byte)packetBuffer[12] << 8) | (byte)packetBuffer[13]; //slop_ud_dowm

          // if(sa[1].toInt()  == 99) digitalWrite(25, 0); //on relay motor driver
          // if(sa[1].toInt()  == 100) digitalWrite(25, 1); //off relay motor driver

          
          selectHead =  sa[1].toInt();
          // //Serial.println(selectHead);

          if (sa[1].toInt() == 4)
          {
            selectHeadselect = 0;
          };

          if (sa[1].toInt() == 108)
          {
            selectHeadselect = 1;
          };

          if (sa[1].toInt() == 96)
          {
            selectMove = 0;
          };


          if (sa[1].toInt() == 107)
          {
            selectMove = 0;
            digitalWrite(26, 1);
            digitalWrite(27, 1);
            digitalWrite(25, 1); 
            trigger1_f(0);
            trigger2_f(0);
          };

          if (sa[1].toInt() == 97)
          {
            selectMove = 1;
          };


          if(selectHeadselect == 0){
            //myservo6.write(90 + sa[2].toInt());  //y
            myservo3.write(90 + sa[3].toInt());  //y
            myservo4.write(90 + sa[2].toInt());  //z
            myservo5.write(90 + sa[4].toInt());  //x
          };

          if(selectHeadselect == 1){

          //uint16_t asimut  = ((byte)packetBuffer[9] << 8) | (byte)packetBuffer[10]; //Compass
          //int8_t head_z  = packetBuffer[11]; //slop_left_rigt
          //int16_t head_y  = ((byte)packetBuffer[12] << 8) | (byte)packetBuffer[13]; //slop_ud_dowm



            myservo3.write(180 - (180 + sa[11].toInt())); 
            //myservo6.write(180 - (180 + ~head_y)); }
            myservo5.write(90 + ~sa[10].toInt());  

            //myservo5.write(asimut);
            //Serial.println(asimut);

            myservo4.writeMicroseconds(3000-(500 + (sa[9].toInt() * 5.5555)));
            
          }

            
          if (selectMove == 0){

                  //movement
                  if(sa[0].toInt() == 0 ) {  //вперёд trigger
                    digitalWrite(26, 1);
                    digitalWrite(27, 1);
                    turn_motor = 0;
                  }
                  
                  if(sa[0].toInt() == 3 ) {  //назад trigger
                    digitalWrite(26, 0);
                    digitalWrite(27, 0);
                    turn_motor = 0;
                  } 

                  if(sa[0].toInt() == 1 ) { //разворот влево-вправо
                      digitalWrite(26, 0);
                      digitalWrite(27, 1);
                      turn_motor = 1;
                  }

                  // if(sa[0].toInt() == 2 ) { //разворот вправо-влево
                  //     digitalWrite(26, 1);
                  //     digitalWrite(27, 0);
                  //     turn_motor = 1;
                  // }

                  if(sa[0].toInt() == 2) { //впёрёд-назад trigger one
                      digitalWrite(26, 1);
                      digitalWrite(27, 1);
                      turn_motor = 2;
                  }  

                  // if((uint8_t)packetBuffer[2] == 8 ) { //режим скорости
                  //     mode_speed = 0;
                  // }
                  
                  // if((uint8_t)packetBuffer[2] == 9 ) { //режим скорости
                  //     mode_speed = 1;
                  // }    

                  if(sa[6].toInt() > 0 && turn_motor == 0 ) 
                  { digitalWrite(25, 0); trigger1_f(sa[6].toInt()); millis_onMotor_button_start = currentMillis; } //left motor
                  if(sa[7].toInt() > 0 && turn_motor == 0 ) 
                  { digitalWrite(25, 0); trigger2_f(sa[7].toInt()); millis_onMotor_button_start = currentMillis; } //right motor


                  if(sa[6].toInt() > 0  && turn_motor == 1 ) { 
                    digitalWrite(25, 0);
                    digitalWrite(26, 0);
                    digitalWrite(27, 1);
                    trigger1_f(sa[6].toInt());
                    trigger2_f(sa[6].toInt());
                    millis_onMotor_button_start = currentMillis; 
                  } 

                  if(sa[7].toInt() > 0  && turn_motor == 1 ) { 
                    digitalWrite(25, 0);
                    digitalWrite(26, 1);
                    digitalWrite(27, 0);
                    trigger1_f(sa[7].toInt());
                    trigger2_f(sa[7].toInt());
                    millis_onMotor_button_start = currentMillis; 
                  } 


                  if(sa[6].toInt() > 0 && turn_motor == 2 ) { 
                    digitalWrite(25, 0); digitalWrite(26, 1);digitalWrite(27, 1); trigger1_f(sa[6].toInt()); trigger2_f(sa[6].toInt()); millis_onMotor_button_start = currentMillis; 
                  } //left motor

                  if(sa[7].toInt() > 0 && turn_motor == 2 ) { 
                    digitalWrite(25, 0); digitalWrite(26, 0);digitalWrite(27, 0); trigger1_f(sa[7].toInt()); trigger2_f(sa[7].toInt()); millis_onMotor_button_start = currentMillis; } //right motor

                  //Test
                  // digitalWrite(26, 1);
                  // digitalWrite(27, 1);
                  // trigger2_f(sa[6].toInt());
                  // trigger1_f(sa[7].toInt());
          }

          if (selectMove == 1){
          
             if(sa[12].equals("вперед")){

                    digitalWrite(26, 1);
                    digitalWrite(27, 1);
                    digitalWrite(25, 0); 
                    trigger1_f(255); millis_onMotor_button_start = currentMillis;
                    trigger2_f(255); millis_onMotor_button_start = currentMillis;
            
             }

             if(sa[12].equals("назад")){
                 
                    digitalWrite(26, 0);
                    digitalWrite(27, 0);
                    digitalWrite(25, 0); 
                    trigger1_f(255); millis_onMotor_button_start = currentMillis;
                    trigger2_f(255); millis_onMotor_button_start = currentMillis;
             }

             if(sa[12].equals("влево") || sa[12].equals("в лево") || sa[12].equals("в лего") || sa[12].equals("лего")){
                    
                    digitalWrite(26, 1);
                    digitalWrite(27, 0);
                    digitalWrite(25, 0); 
                    trigger1_f(255); millis_onMotor_button_start = currentMillis;
                    trigger2_f(255); millis_onMotor_button_start = currentMillis;
             }

             if(sa[12].equals("вправо") || sa[12].equals("справа") || sa[12].equals("в право") || sa[12].equals("в права") || sa[12].equals("права") || sa[12].equals("трава")){
                    
                    digitalWrite(26, 0);
                    digitalWrite(27, 1);
                    digitalWrite(25, 0); 
                    trigger1_f(255); millis_onMotor_button_start = currentMillis;
                    trigger2_f(255); millis_onMotor_button_start = currentMillis;
             }

              if(sa[12].equals("стоп") || sa[12].equals("сто") || sa[12].equals("топ")){
                    
                    digitalWrite(26, 1);
                    digitalWrite(27, 1);
                    digitalWrite(25, 1); 
                    trigger1_f(0);
                    trigger2_f(0);
             }

          }

    };
     

    

    // myservo5.writeMicroseconds(500);
    // delay(4000);
    // myservo5.writeMicroseconds(2500);
    // delay(4000);



        if(currentMillis - millis_onMotor_button_start >= 10000){
             digitalWrite(25, 1);
             //millis_onMotor_button_start = currentMillis;
        };



           
}

      //b1 = trigger1;
      //Serial.println(trigger1);

      void trigger1_f(uint8_t trigger1) { //right motor
      if (mode_speed == 0) trigger1 = trigger1 / 4;
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
          pot_trigger1 = trigger1;
      }

      void trigger2_f(uint8_t trigger2) { //left motor
        if (mode_speed == 0) trigger2 = trigger2 / 6;
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
        pot_trigger2 = trigger2;
      }


void MCP41010Write(byte value)
{
  //digitalWrite(CS,LOW); // выбор микросхемы
   SPI.transfer(B00010001); // командный байт
   SPI.transfer(value); // байт данных 
   //digitalWrite(CS,HIGH); // снимаем выбор с микросхемы
}