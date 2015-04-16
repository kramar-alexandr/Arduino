int a=0;
long midBtnVal = 0;
int midCnt = 0;
int maxMidCnt = 30;
int btnVal;
int dealCounter = 0;
boolean btn1,btn2,btn3,btn4,btn5,zerroflag;
int analogVal;

void setup ()
{

Serial.begin(9600);
//pinMode (A3, INPUT);
zerroflag = true;
}

void loop ()

{
  analogVal = analogRead (3);
  if(analogVal>200){
    midCnt++;
    midBtnVal = midBtnVal + analogVal;
    
    if(midCnt>=maxMidCnt & analogVal>100){
      btnVal = midBtnVal/midCnt;
      midBtnVal = 0;
      midCnt = 0;
    }
    }else {
      btnVal = 0;
      midBtnVal = 0;
      midCnt = 0;
    }

  
  Serial.print(analogVal);
  Serial.print("   ");
  Serial.println(btnVal);
  /*
  btn1 = false;
  btn2 = false;
  btn3 = false;
  btn4 = false;
  btn5 = false;
  
  if(btnVal<10){
    zerroflag = true;
  }
  
  
  
  if(btnVal>1015 & btnVal<1025 & zerroflag){
    btn1 = true;
    zerroflag = false;
  }
  if(btnVal>961 & btnVal<981 & zerroflag){
    btn2 = true;
    zerroflag = false;
  }
  if(btnVal>914 & btnVal<934 & zerroflag){
    btn3 = true;
    zerroflag = false;
  }
  if(btnVal>871 & btnVal<891 & zerroflag){
    btn4 = true;
    zerroflag = false;
  }
  if(btnVal>832 & btnVal<852 & zerroflag){
    btn5 = true;
    zerroflag = false;
  }
  
  if(btn1){
    Serial.println("Button 1");
  }
  if(btn2){
    Serial.println("Button 2");
  }
  if(btn3){
    Serial.println("Button 3");
  }
  if(btn4){
    Serial.println("Button 4");
  }
  if(btn5){
    Serial.println("Button 5");
  }
*/
}
