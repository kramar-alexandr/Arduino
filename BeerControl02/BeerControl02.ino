#include <OneWire.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>//LCD

#define ONE_WIRE_BUS 2 //Termometers

#define RELAY1 3 //Relay 1
#define RELAY2 4 //Relay 2
#define RELAY3 5 //Relay 3
#define RELAY4 6 //Relay 4
#define RELAY5 7 //Relay 5
#define RELAY6 8 //Relay 6


OneWire ds(ONE_WIRE_BUS);//Termo
byte Thermometer1[] = { 
  0x28, 0xFF, 0xBC, 0x03, 0x42, 0x04, 0x00, 0xEB };  // адрес датчика DS18B20 28 FF BC 03 42 04 00 EB
byte Thermometer2[] = { 
  0x28, 0xFF, 0x39, 0x6A, 0x56, 0x14, 0x03, 0x2B };  // адрес датчика DS18B20 28 FF 39 6A 56 14 03 2B
float T1,T2;
unsigned long TempCounter = 0;
unsigned long oldTempCounter = 0;
int TempTime = 750;
boolean GetTempRequest=true;
  
//Buttons controll block begin
int a=0;
long midBtnVal = 0;
int midCnt = 0;
int maxMidCnt = 30;
int btnVal;
int dealCounter = 0;
boolean btn1,btn2,btn3,btn4,btn5,btn6,btn7,btn8,btn9,btn10,zerroflag;
int analogVal,analogSensor;
boolean rel1,rel2,rel3,rel4;
boolean man1,man2,man3,man4;
int cntmode;/*0 - select prg P; 1 - chng prg temp T; 2 - cnhg prg time M; 3 - on/off heating H ; 4 - pomp pause seconds Pmp */
//End Buttons block

//LCD begin
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int lcdCounter=0;
int lcdF = 500;
int displ = 1;
//LCD end

//Pomp cntrol
int pausetime = 5;

//Sound
int soundchanel = 7;
unsigned long lastbeep,beeptime;
boolean beepflag,playbeep;
int signaltime;

//tempcontrol
unsigned long tmills = 0;
unsigned long oldtmills = 0;
unsigned long tmills1 = 0;
unsigned long oldtmills1 = 0;
unsigned long lastsensortime = 0;
long tseconds = 0;
int tminutes = 0;
int program = 0;
int prgcnt = 6;
float totemp[] = {75,65,67,72,78,100,0};
int progtime[] = {1,50,30,15,5,100,1};
int pauseflag[] = {1,0,0,0,1,0};
int remtminutes = 0;
boolean oncontrol;
float heatspeed1,heatspeed2,oldT1,oldT2;



void Beep(){
  if((millis()-beeptime)>100){
     beepflag = !beepflag;
     beeptime = millis();
     
     if(playbeep){
      if(beepflag){
        tone (soundchanel, 500);
      }else{
        tone(soundchanel, 1000);
      }
      }else{
         noTone(soundchanel);
      }
    
      if((millis()-lastbeep)>signaltime*1000){
        playbeep = false;
      }  
     
  }
}

void BeepProc(int time){
  
  playbeep = true;
  lastbeep = millis();
  signaltime = time;
}

void printdisp1(){
  lcd.setCursor(8,0);
    lcd.print("CT");
    lcd.setCursor(11,0);
    lcd.print(T1);
    lcd.setCursor(8,1);
    lcd.print("PT");
    lcd.setCursor(11,1);
    lcd.print(totemp[program]);
    //lcd.print(T2);
    
    lcd.setCursor(0,0);
    lcd.print("P");
    lcd.setCursor(1,0);
    lcd.print(program);
    lcd.setCursor(3,0);
    lcd.print(remtminutes);
    if(remtminutes<10){
      lcd.setCursor(4,0);
      lcd.print(" ");
    }
        
    lcd.setCursor(0,1);
    lcd.print("M");
    lcd.setCursor(1,1);
    lcd.print(progtime[program]);
    if(progtime[program]<10){
      lcd.setCursor(2,1);
      lcd.print(" ");
    }
    
    lcd.setCursor(4,1);
    switch(cntmode){
      case 0:lcd.print("Prg");
      break;
      case 1:lcd.print("Tmp");
      break;
      case 2:lcd.print("Min");
      break;
      case 3:lcd.print("Htn");
      break;
      case 4:lcd.print("Pmp");
      break;
    }  
}

void printdisp2(){
    lcd.setCursor(0,0);
    lcd.print("HS");
    lcd.setCursor(3,0);
    lcd.print(heatspeed1);
    lcd.setCursor(0,1);
    lcd.print("HS");
    lcd.setCursor(3,1);
    lcd.print(heatspeed2);
    
    
    lcd.setCursor(8,0);
    lcd.print("T1");
    lcd.setCursor(11,0);
    lcd.print(T1);
    lcd.setCursor(8,1);
    lcd.print("T2");
    lcd.setCursor(11,1);
    lcd.print(T2);    
}

void printdisp3(){
    lcd.setCursor(0,0);
    lcd.print("PMP");
    lcd.setCursor(4,0);
    lcd.print(pausetime);
    
}

void LCDPrintInfo(){
  if(lcdCounter==0){
    //lcd.clear();
    switch(displ){
      case 1: printdisp1();
      break;
      case 2: printdisp2();
      break;
      case 3: printdisp2();
      break;
    }
    
  }
  
  lcdCounter++;
  if(lcdCounter>lcdF){
    lcdCounter=0;
  }
}


void PressBtn1(){
    //Serial.println("Button 1");
    if(rel1){
      digitalWrite(RELAY1, HIGH);
      rel1 = false;
      man1 = false;
    }else{
      digitalWrite(RELAY1, LOW);
      rel1 = true;
      man1 = true;
    }

}

void PressBtn2(){
   // Serial.println("Button 2");
   displ++;
   lcd.clear();
   if(displ>3){
     displ = 1;
   }
}

void PressBtn3(){
  
  cntmode++;
  if(cntmode>4){
    cntmode = 0;
  }

}

void changeProgram(int to){
  if(program+to<prgcnt and program+to>=0){
  program = program+to;
  lcd.setCursor(0,0);
  lcd.print("   ");
  }
}
void changeProgramTemp(int to){
    totemp[program] = totemp[program] + to;
}
void changeProgramTime(int to){
  progtime[program] = progtime[program] + to;
}
void changeHeating(int to){
  if(to==1){
    if(man2){
      if(rel2){
        digitalWrite(RELAY2, HIGH);
        rel2 = false;
      }
      man2 = false;
    }else{
      man2 = !man2;
    }
  }
  if(to==2){
     if(man3){
      if(rel3){
        digitalWrite(RELAY3, HIGH);
        rel3 = false;
      }
      man3 = false;
    }else{
      man3 = !man3;
    }
  }
}

void changePompPause(int sec){
  pausetime = pausetime + sec;
  if(pausetime<0){
     pausetime = 0;
  }
}


void PressBtn4(){
  switch(cntmode){
    case 0:changeProgram(1);
    break;
    case 1:changeProgramTemp(1);
    break;
    case 2:changeProgramTime(5);
    break;
    case 3:changeHeating(1);
    break;
    case 4:changePompPause(1);
    break;
  }
}

void PressBtn5(){
  
  switch(cntmode){
    case 0:changeProgram(-1);
    break;
    case 1:changeProgramTemp(-1);
    break;
    case 2:changeProgramTime(-5);
    break;
    case 3:changeHeating(2);
    break;
    case 4:changePompPause(-1);
    break;
  }

}

void PressBtn6(){
  if(pauseflag[program]==1){
    pauseflag[program]=1;
  }else{
    pauseflag[program]=0;
  }
}

void PressBtn7(){
}
void PressBtn8(){
}
void PressBtn9(){
}
void PressBtn10(){
}


void GetTemperature() {//                                          TEMPERATURE
  byte data[2];
  byte data1[2];
  
  if(GetTempRequest){
    GetTempRequest = false;
    data[2];
    ds.reset(); 
    ds.select(Thermometer1);
    ds.write(0x44);
    data1[2];
    ds.reset(); 
    ds.select(Thermometer2);
    ds.write(0x44);
    TempCounter = 0;
  }
  
  if(GetTempRequest==false){
    TempCounter = millis() - oldTempCounter;
    if(TempCounter>=TempTime){
      TempCounter = millis();
      oldTempCounter = millis();
      GetTempRequest = true;
      ds.reset();
      ds.select(Thermometer1);
      ds.write(0xBE);
      data[0] = ds.read(); 
      data[1] = ds.read();
      int Temp = (data[1]<< 8)+data[0];
      T1 = (float)(Temp>>2)*0.25;

      ds.reset();
      ds.select(Thermometer2);
      ds.write(0xBE);
      data1[0] = ds.read(); 
      data1[1] = ds.read();
      Temp = (data1[1]<< 8)+data1[0];
      T2 = (float)(Temp>>2)*.25;
    }
  }
}

void ButtonsLoop(){//                                              BUTTONS
  analogVal = analogRead (3);
  analogSensor = analogRead (2);
    
  if((millis()-lastsensortime)>1000*pausetime){
    if(analogSensor>100){
      lastsensortime = millis();
      if(rel1){
        digitalWrite(RELAY1, HIGH);
        rel1 = false;
      }
    }else{
      if(man1 and !rel1){
        digitalWrite(RELAY1, LOW);
        rel1 = true;
      }
      lastsensortime = millis();
    }
  }
  
  
  if(analogVal>200){
  midCnt++;
  midBtnVal = midBtnVal + analogVal;  
  if(midCnt>=maxMidCnt & analogVal>200){
    btnVal = midBtnVal/midCnt;
    midBtnVal = 0;
    midCnt = 0;
  }
  }else {
    btnVal = 0;
    midBtnVal = 0;
    midCnt = 0;
  }
  
  btn1 = false;
  btn2 = false;
  btn3 = false;
  btn4 = false;
  btn5 = false;
  btn6 = false;
  btn7 = false;
  btn8 = false;
  btn9 = false;
  btn10 = false;
  
  if(btnVal<200){
    zerroflag = true;
  }
  if(btnVal>1007 & btnVal<1037 & zerroflag){
    btn1 = true;
    zerroflag = false;
  }
  if(btnVal>773 & btnVal<803 & zerroflag){
    btn2 = true;
    zerroflag = false;
  }
  if(btnVal>625 & btnVal<655 & zerroflag){
    btn3 = true;
    zerroflag = false;
  }
  if(btnVal>525 & btnVal<555 & zerroflag){
    btn4 = true;
    zerroflag = false;
  }
  if(btnVal>451 & btnVal<481 & zerroflag){
    btn5 = true;
    zerroflag = false;
  }
  if(btnVal>395 & btnVal<425 & zerroflag){
    btn6 = true;
    zerroflag = false;
  }
  if(btnVal>351 & btnVal<381 & zerroflag){
    btn7 = true;
    zerroflag = false;
  }
  if(btnVal>315 & btnVal<345 & zerroflag){
    btn8 = true;
    zerroflag = false;
  }
  if(btnVal>286 & btnVal<315 & zerroflag){
    btn9 = true;
    zerroflag = false;
  }
  if(btnVal>262 & btnVal<286 & zerroflag){
    btn10 = true;
    zerroflag = false;
  }
  
  
  
  if(btn1){
    PressBtn1();
  }
  if(btn2){
    PressBtn2();
  }
  if(btn3){
    PressBtn3();
  }
  if(btn4){
    PressBtn4();
  }
  if(btn5){
    PressBtn5();
  }
  if(btn6){
    PressBtn6();
  }
  if(btn7){
    PressBtn7();
  }
  if(btn8){
    PressBtn8();
  }
  if(btn9){
    PressBtn9();
  }
  if(btn10){
    PressBtn10();
  }
}

void setup() {
 
  //Buttons init
  pinMode (A3, INPUT);
  pinMode (A2, INPUT);
  zerroflag = true;
  oldTempCounter = millis();
  lastsensortime = millis();
  pausetime = 10;
  //Buttons init end
  
  //relay init
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);
  rel1 = false;
  rel2 = false;
  rel3 = false;
  rel4 = false;
  man1 = false;//pomp
  man2 = false;//heater1
  man3 = false;//heater2
  man4 = false;
  //digitalWrite(RELAY5, HIGH);
  //digitalWrite(RELAY6, HIGH);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  //pinMode(RELAY5, OUTPUT);
  //pinMode(RELAY6, OUTPUT);
  
  
  //LCD init
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  
  
  //temp
  oldtmills=millis();
  oncontrol = true;
  
  pinMode(soundchanel, OUTPUT); //объявляем пин как выход  sound
  
  //Serial.begin(9600);
}

void ControlTemp(){
  
  float temp = totemp[program];
  long maxmin = progtime[program];
  remtminutes = maxmin - tminutes;
  if(tmills>=1000){
    if(T1<=temp-0.5){
      tminutes = 0;
    }    
    tmills=0;
    oldtmills=millis();
    if(T1<temp){
      if(man2 and !rel2){
        digitalWrite(RELAY2, LOW);
        rel2 = true;
      }
      if(man3 and !rel3){
        digitalWrite(RELAY3, LOW);
        rel3 = true;
      }
    }
    if(T1>temp+0.5){
      if(rel2){
        digitalWrite(RELAY2, HIGH);
        rel2 = false;
      }
      if(rel3){
        digitalWrite(RELAY3, HIGH);
        rel3 = false;
      }
    }
    if(T2>temp-0.5){
      if(tseconds>=60){
        tminutes++;
        tseconds = 0;
        
        if(maxmin>0 and tminutes>=maxmin){
          if(pauseflag[program]==0){
            tminutes = 0;
            program++;
            BeepProc(10);
          }else {
            BeepProc(1);
          }
        }
      }
      tseconds++;
    }
  }
  tmills = millis() - oldtmills;
  
}

void loop() {
  GetTemperature();//UpdateTemp T1 T2
  Beep();
  if(tmills1>=60000){
    
    tmills1=0;
    oldtmills1=millis();
    heatspeed1 = T1-oldT1;
    oldT1 = T1;
    heatspeed2 = T2-oldT2;
    oldT2 = T2;
  }
  tmills1 = millis() - oldtmills1;
  
  ButtonsLoop();//ReadButtonsStatus and sensor water level
  LCDPrintInfo(); //LCD PritInfo
  if(oncontrol){
    ControlTemp();
  }
  if(program==prgcnt-1){
    oncontrol = false;
    digitalWrite(RELAY1, HIGH);
    digitalWrite(RELAY2, HIGH);
    digitalWrite(RELAY3, HIGH);
    digitalWrite(RELAY4, HIGH);
    rel1 = false;
    rel2 = false;
    rel3 = false;
    rel4 = false;
    man1 = false;//pomp
    man2 = false;//heater1
    man3 = false;//heater2
    man4 = false;
  }else{
    oncontrol = true;
  }
  
}
