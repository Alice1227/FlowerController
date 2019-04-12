#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11); // RX | TX

#include <Servo.h> 
Servo myservo;

#include <BlynkSimpleStream.h>
char auth[] = "70a7d9750f004c39872aa7c22372a9b6";

int ledPin = 13;//led燈腳位
int motoPin = 6;//伺服馬達腳位
int RSTPin = A1;//光敏電阻腳位
int delayTime = 200;//摩斯電碼延遲時間
bool working = false;//摩斯電碼工作狀態
int pos = 0;//伺服馬達角度變數

WidgetTerminal terminal1(V1);
WidgetTerminal terminal2(V2);

void setup() {
  Serial.begin(9600);
  Blynk.begin(Serial, auth);
  BTSerial.begin(9600);

  terminal1.clear();
  terminal2.clear();
  
  pinMode(ledPin, OUTPUT);//led燈值接線口
  pinMode(RSTPin,INPUT);//光敏電阻值接線口
  myservo.attach(motoPin);//馬達訊號接線口
  myservo.write(0);//啟動角度0
  delay(3000);
}

void loop() {
  Blynk.run();

  int v = analogRead(RSTPin); //讀光敏電阻值
  //Serial.println(v);
  
  //花朵根據光線轉動
  if (v > 250) {
    while(pos <= 180) {
      myservo.write(pos); 
      pos+=5;
      delay(100);
    }
  } else {
    while(pos >= 0){
      myservo.write(pos); 
      pos-=5;
      delay(100);
    }
  }
}

BLYNK_WRITE(V1) {
  String morse_code = param.asStr();//讀訊息輸入值
  morse_code.toLowerCase();
  if(morse_code.length() > 0) {
    code(morse_code);//轉譯成摩斯電碼
  }
}

BLYNK_WRITE(V2) {
  String message = param.asStr();//讀心情日記輸入值
  if(message.length() > 0) {
    char msg[100];
    message.toCharArray(msg, 100);
    BTSerial.write(msg);//透過藍芽傳輸給對方
    terminal2.flush();
  }
}

void code (String str) {
  if (working) {
    return;
  } else {
    working = true;
    for (int i = 0; i < str.length(); i++) {
      //電碼對照表
      switch (str[i]) {
        case 'a':
          dot();
          dash();
          break;
        case 'b':
          dash();
          dot();
          dot();
          dot();
          break;
        case 'c':
          dash();
          dot();
          dash();
          dot();
          break;
        case 'd':
          dash();
          dot();
          dot();
          break;
        case 'e':
          dot();
          break;
        case 'f':
          dot();
          dot();
          dash();
          dot();
          break;
        case 'g':
          dash();
          dash();
          dot();
          break;
        case 'h':
          dot();
          dot();
          dot();
          dot();
          break;
        case 'i':
          dot();
          dot();
          break;
        case 'j':
          dot();
          dash();
          dash();
          dash();
          break;
        case 'k':
          dash();
          dot();
          dash();
          break;
        case 'l':
          dot();
          dash();
          dot();
          dot();
          break;
        case 'm':
          dash();
          dash();
          break;
        case 'n':
          dash();
          dot();
          break;
        case 'o':
          dash();
          dash();
          dash();
          break;
        case 'p':
          dot();
          dash();
          dash();
          dot();
          break;
        case 'q':
          dash();
          dash();
          dot();
          dash();
          break;
        case 'r':
          dot();
          dash();
          dot();
          break;
        case 's':
          dot();
          dot();
          dot();
          break;
        case 't':
          dash();
          break;
        case 'u':
          dot();
          dot();
          dash();
          break;
        case 'v':
          dot();
          dot();
          dot();
          dash();
          break;
        case 'w':
          dot();
          dash();
          dash();
          break;
        case 'x':
          dash();
          dot();
          dot();
          dash();
          break;
        case 'y':
          dash();
          dot();
          dash();
          dash();
          break;
        case 'z':
          dash();
          dash();
          dot();
          dot();
          break;
        case ' ': //空格
          wordWait();
          break;
        default:
          break;
      }
      characterWait();
    }
    working = false;
  }
}

//短訊號"."
void dot(){
  digitalWrite(ledPin, HIGH);
  delay(delayTime);
  digitalWrite(ledPin, LOW);
  delay(delayTime); 
}
//長訊號"–"
void dash(){
  digitalWrite(ledPin, HIGH); 
  delay(delayTime*3);
  digitalWrite(ledPin, LOW);
  delay(delayTime); 
}

// 字母間delay
void characterWait(){
  delay(delayTime * 3);
}

//單字間delay(=空格)
void wordWait(){
  delay(delayTime * 6);
}
