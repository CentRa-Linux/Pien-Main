#include <Arduino.h>
#include <Servo.h>

/*
＿人人人人＿
＞　Pien　＜
￣Y^Y^Y^Y^￣
*/

//サーボモーター
Servo svb1s;
Servo svb2s;
Servo svb3s;
Servo svb4s;
Servo svfs;

//ここからDefine

//カラーセンサー・右
#define csrrpin A3
#define csrgpin A4
#define csrbpin A5

//カラーセンサー・左
#define cslrpin A6
#define cslgpin A7
#define cslbpin A8

//フォトトランジスタ・機体裏、右、左
#define ptbpin A0
#define ptrpin A1
#define ptlpin A2

//タッチセンサー・前、後
#define tsfpin 32
#define tsrpin 33

//タッチセンサー・バケット右、左
#define tsbrpin 34
#define tsblpin 35

//超音波センサー右・Output、PulseIn
#define ssropin 36
#define ssrppin 37

//超音波センサー左・Output、PulseIn
#define sslopin 38
#define sslppin 39

//モータードライバー前
#define mdfstbypin 22
#define mdfr1pin 23
#define mdfr2pin 24
#define mdfrpwmpin 2
#define mdfl1pin 25
#define mdfl2pin 26
#define mdflpwmpin 3

//モータードライバー後ろ
#define mdbstbypin 27
#define mdbr1pin 28
#define mdbr2pin 29
#define mdbrpwmpin 4
#define mdbl1pin 30
#define mdbl2pin 31
#define mdblpwmpin 5

//サーボモーター
#define svb1pin 6
#define svb2pin 7
#define svb3pin 8
#define svb4pin 9
#define svfpin 10

//音速
#define sonicspeed 340

//ここまでDefine

//ここから変数定義
//ここから部品系

//カラーセンサー・右
int csrr;
int csrg;
int csrb;

//カラーセンサー・左
int cslr;
int cslg;
int cslb;

//カラーセンサー・モノクロ
int csrm;
int cslm;

//フォトトランジスタ
int ptb;
int ptr;
int ptl;

//タッチセンサー
bool tsf;
bool tsr;
bool tsbr;
bool tsbl;

//超音波センサー
float ssr;
float ssl;

//モータードライバー
int mdfstby;
int mdfr1;
int mdfr2;
int mdfrpwm;
int mdfl1;
int mdfl2;
int mdflpwm;

//サーボモーター
int svb1;
int svb2;
int svb3;
int svb4;
int svf;

//ここまで部品系

//ここからしきい値系
//しきい値はプログラム内で動的に変更できるように変数でおく
//setupで定義しよう

//緑検知
int thrcsr;
int thrcsg;
int thrcsb;

//白検知
int thwcsr;
int thwcsg;
int thwcsb;

//銀検知
int thscsr;
int thscsg;
int thscsb;

//黒検知
int thbcsr;
int thbcsg;
int thbcsb;

//ここまでしきい値系
//ここまで変数定義

//センサー系のリード関数

//カラーセンサー
void csrrread() {
  csrr = analogRead(csrrpin);
}

void csrgread() {
  csrg = analogRead(csrgpin);
}

void csrbread() {
  csrb = analogRead(csrbpin);
}

void cslrread() {
  cslr = analogRead(cslrpin);
}

void cslgread() {
  cslg = analogRead(cslgpin);
}

void cslbread() {
  cslb = analogRead(cslbpin);
}

//フォトトランジスタ
void ptbread() {
  ptb = analogRead(ptbpin);
}

void ptrread() {
  ptr = analogRead(ptrpin);
}

void ptlread() {
  ptl = analogRead(ptlpin);
}

//タッチセンサー

void tsfread() {
  if (digitalRead(tsfpin) == 0) {
    tsf = true;
  } else {
    tsf = false;
  }
}

void tsrread() {
  if (digitalRead(tsrpin) == 0) {
    tsr = true;
  } else {
    tsr = false;
  }
}

void tsbrread() {
  if (digitalRead(tsbrpin) == 0) {
    tsbr = true;
  } else {
    tsbr = false;
  }
}

void tsblread() {
  if (digitalRead(tsblpin) == 0) {
    tsbl = true;
  } else {
    tsbl = false;
  }
}

//超音波センサー

void ssrread() {
  digitalWrite(ssropin,LOW); 
  delayMicroseconds(2); 
  digitalWrite(ssropin,HIGH); //超音波を出力
  delayMicroseconds( 10 ); //
  digitalWrite(ssropin,LOW);
  ssr = pulseIn(ssrppin,HIGH)*sonicspeed/5000;//読んで→音速かけて→いい感じにcmに変換
}

void sslread() {
  digitalWrite(sslopin,LOW); 
  delayMicroseconds(2); 
  digitalWrite(sslopin,HIGH); //超音波を出力
  delayMicroseconds( 10 ); //
  digitalWrite(sslopin,LOW);
  ssl = pulseIn(sslppin,HIGH)*sonicspeed/5000;//読んで→音速かけて→いい感じにcmに変換
}

//↑までをまとめると

void lineread() {
  //ラインを読んで
  csrrread();
  csrgread();
  csrbread();
  cslrread();
  cslgread();
  cslbread();
  ptbread();

  //モノクロの計算
  csrm = (csrr + csrg + csrb)/3 ;
  csrm = (csrr + csrg + csrb)/3 ;
}

//ここまでリード関数

//しきい値宣言

void threshold() {
  //緑検知
  thrcsr = 900;
  thrcsg = 700;
  thrcsb = 900;

  //白検知
  thwcsr = 700;
  thwcsg = 700;
  thwcsb = 700;

  //銀検知
  thwcsr = 600;
  thwcsg = 600;
  thwcsb = 600;

  //黒検知
  thbcsr = 900;
  thbcsg = 900;
  thbcsb = 900;
}

void pinset() {
  //タッチセンサー
  pinMode(tsfpin,INPUT);
  pinMode(tsrpin,INPUT);
  pinMode(tsbrpin,INPUT);
  pinMode(tsblpin,INPUT);
  
  //超音波センサー
  pinMode(ssropin,OUTPUT);
  pinMode(ssrppin,INPUT);
  pinMode(sslopin,OUTPUT);
  pinMode(sslppin,INPUT);

  //モータードライバ
  pinMode(mdfstbypin,OUTPUT);
  pinMode(mdfr1pin,OUTPUT);
  pinMode(mdfr2pin,OUTPUT);
  pinMode(mdfl1pin,OUTPUT);
  pinMode(mdfl2pin,OUTPUT);

  pinMode(mdbstbypin,OUTPUT);
  pinMode(mdbr1pin,OUTPUT);
  pinMode(mdbr2pin,OUTPUT);
  pinMode(mdbl1pin,OUTPUT);
  pinMode(mdbl2pin,OUTPUT);

  //ついでにサーボアタッチ
  svb1s.attach(svb1pin);
  svb2s.attach(svb2pin);
  svb3s.attach(svb3pin);
  svb4s.attach(svb4pin);
  svfs.attach(svfpin);
}

void line_sequence() {

}

void rescue_sequence() {

}

void setup() {
  // put your setup code here, to run once:

  threshold();//しきい値設定

  pinset();//ピンモードの設定

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
}