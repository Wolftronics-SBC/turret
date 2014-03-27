#include <ax12.h>
#include <BioloidController.h>
//#include "poses.h"

#include <avr/pgmspace.h>

PROGMEM prog_uint16_t Center[] = {2, 2048, 2048};

BioloidController bioloid = BioloidController(1000000);

const int SERVOCOUNT = 2;
int id;
int pos;
boolean IDCheck;
boolean RunCheck;

int posX;
int posY;
boolean isReadSerial;

//char serialString[32];

void setup(){
   pinMode(0,OUTPUT);    
   //initialize variables 
   id = 1;
   pos = 0;
   IDCheck = 1;
  //open serial port
   Serial.begin(9600);
   delay (500);   
   Serial.println("Starting!!!");  
   
  posX = 0;
  posY = 0;
  isReadSerial = false;
}

void ReadSerial()
{
  int i=0;
  char mas[32];
  isReadSerial = false;
  memset(&mas,0,sizeof(mas));
  while(Serial.available()>0)
  {
   mas[i]=Serial.read();
   i++;
   isReadSerial = true;
  }
  if (isReadSerial)
  {
    char strX[5] = {mas[0], mas[1], mas[2], mas[3], '\0'};
    char strY[5] = {mas[4], mas[5], mas[6], mas[7], '\0'};
    posX = atoi( strX );
    posY = atoi( strY );
  }
}

void loop(){
  
  ReadSerial();
  delay(20);
  
  if (isReadSerial) {
    Serial.println("Rotation!!! X");
    Serial.println(posX);
    SetPosition(1, posX);
    
    //Serial.println("Rotation!!! Y");
    //Serial.println(posY);
    //SetPosition(2, posY);    
  }
}


void ScanServo(){
  id = 1;
  Serial.println("###########################");
  Serial.println("Starting Servo Scanning Test.");
  Serial.println("###########################");
      
  while (id <= SERVOCOUNT){
  pos =  ax12GetRegister(id, 36, 2);
  Serial.print("Servo ID: ");
  Serial.println(id);
  Serial.print("Servo Position: ");
  Serial.println(pos);
  
  if (pos <= 0){
  Serial.println("###########################");
  Serial.print("ERROR! Servo ID: ");
  Serial.print(id);
  Serial.println(" not found. Please check connection and verify correct ID is set.");
  Serial.println("###########################"); 
  IDCheck = 0;
  }
  
  id = (id++)%SERVOCOUNT;
  delay(1000);
  }
  if (IDCheck == 0){
    Serial.println("###########################");
    Serial.println("ERROR! Servo ID(s) are missing from Scan. Please check connection and verify correct ID is set.");
    Serial.println("###########################");  
  }
  else{
  Serial.println("All servo IDs present.");
  }
    if (RunCheck == 1){
    MenuOptions();
  }

}



void CheckVoltage(){  
   // wait, then check the voltage (LiPO safety)
  float voltage = (ax12GetRegister (1, AX_PRESENT_VOLTAGE, 1)) / 10.0;
  Serial.println("###########################");   
  Serial.print ("System Voltage: ");
  Serial.print (voltage);
  Serial.println (" volts.");
  if (voltage < 10.0){
    Serial.println("Voltage levels below 10v, please charge battery.");
    while(1);
  }  
  if (voltage > 10.0){
  Serial.println("Voltage levels nominal.");
  }
    if (RunCheck == 1){
      MenuOptions();
  }
      Serial.println("###########################"); 
}

void MoveCenter(){
    delay(100);                    // recommended pause
    bioloid.loadPose(Center);   // load the pose from FLASH, into the nextPose buffer
    bioloid.readPose();            // read in current servo positions to the curPose buffer
    Serial.println("###########################");
    Serial.println("Moving servos to centered position");
    Serial.println("###########################");    
    delay(1000);
    bioloid.interpolateSetup(1000); // setup for interpolation from current->next over 1/2 a second
    while(bioloid.interpolating > 0){  // do this while we have not reached our new pose
        bioloid.interpolateStep();     // move servos, if necessary. 
        delay(3);
    }
    if (RunCheck == 1){
      MenuOptions();
  }
}





void MoveTest(){
  Serial.println("###########################");
  Serial.println("Initializing Movement Sign Test");  
  Serial.println("###########################");
  delay(500);  
  id = 1;
  pos = 2048;
  while(id <= SERVOCOUNT){
  Serial.print("Moving Servo ID: ");
  Serial.println(id);  

  while(pos >= 1024){  
  SetPosition(id, pos);
  pos = pos--;
  delay(10);
  }

  while(pos <= 2048){  
  SetPosition(id, pos);
  pos = pos++;
  delay(10);
  }

  //iterate to next servo ID
  id = id++;

  }
    if (RunCheck == 1){
   MenuOptions();
  }
  
}



void MenuOptions(){
  
    Serial.println("###########################"); 
    Serial.println("Please enter option 1-5 to run individual tests again.");     
    Serial.println("1) Servo Scanning Test");        
    Serial.println("2) Move Servos to Center");    
    Serial.println("3) Relax Servos");            
    Serial.println("4) Perform Movement Sign Test");                
    Serial.println("5) Check System Voltage");   
    Serial.println("6) Perform LED Test");   
    Serial.println("7) Test");    
    Serial.println("###########################"); 
}

void RelaxServos(){
  id = 1;
  Serial.println("###########################");
  Serial.println("Relaxing Servos.");
  Serial.println("###########################");    
  while(id <= SERVOCOUNT){
    Relax(id);
    id = (id++)%SERVOCOUNT;
    delay(50);
  }
   if (RunCheck == 1){
      MenuOptions();
  }
}

void LEDTest(){
    id = 1;
  Serial.println("###########################");
  Serial.println("Running LED Test");
  Serial.println("###########################");    
  while(id <= SERVOCOUNT){
    ax12SetRegister(id, 25, 1);
    Serial.print("LED ON - Servo ID: ");
    Serial.println(id);
    delay(3000);
    ax12SetRegister(id, 25, 0);  
    Serial.print("LED OFF - Servo ID: ");
    Serial.println(id);    
    delay(3000);    
    id = id++;
  }
  
   if (RunCheck == 1){
      MenuOptions();
  }
}

void Test(){
    Serial.println("###########################");
    Serial.println("Test");  
    Serial.println("###########################");
    delay(500);  
    id = 1;
    pos = 2048;
    while(id <= SERVOCOUNT){
    Serial.print("Moving Servo ID: ");
    Serial.println(id);  
  
    while(pos >= 1024){  
    SetPosition(id, pos);
    pos = pos--;
    //delay(10);
    }
  
    while(pos <= 2048){  
    SetPosition(id, pos);
    pos = pos++;
    //delay(10);
    }
  
    //iterate to next servo ID
    id = id++;
  
    }
      if (RunCheck == 1){
     MenuOptions();
    }
}
    
