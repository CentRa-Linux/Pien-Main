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

//カラーセンサー・色
enum cl {white, black, green, silver, red};
cl csrc;
cl cslc;

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

//モータードライバー前
int mdfstby;
int mdfr1;
int mdfr2;
int mdfrpwm;
int mdfl1;
int mdfl2;
int mdflpwm;

//モータードライバー後
int mdbstby;
int mdbr1;
int mdbr2;
int mdbrpwm;
int mdbl1;
int mdbl2;
int mdblpwm;

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

//赤検知
int thrcsr;
int thrcsg;
int thrcsb;

//緑検知
int thgcsr;
int thgcsg;
int thgcsb;

//白検知
int thwcsr;
int thwcsg;
int thwcsb;
int thwcsm;

//銀検知
int thscsr;
int thscsg;
int thscsb;

//黒検知
int thbcsr;
int thbcsg;
int thbcsb;
int thbcsm;

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

void csread() {
  csrrread();
  csrgread();
  csrbread();
  cslrread();
  cslgread();
  cslbread();

  //white
  if (csrr < thwcsr and csrg < thwcsg and csrb < thwcsb) {
    csrc = white;
  }
  if (cslr < thwcsr and cslg < thwcsg and cslb < thwcsb) {
    cslc = white;
  }

  //black
  if (csrr > thbcsr and csrg > thbcsg and csrb > thbcsb) {
    csrc = black;
  }
  if (cslr > thbcsr and cslg > thbcsg and cslb > thbcsb) {
    cslc = black;
  }

  //green
  if (csrr > thgcsr and csrg < thgcsg and csrb > thgcsb) {
    csrc = green;
  }
  if (cslr > thgcsr and cslg < thgcsg and cslb > thgcsb) {
    cslc = green;
  }

  //red
  if (csrr < thgcsr and csrg > thrcsg and csrb > thrcsb) {
    csrc = red;
  }
  if (cslr < thrcsr and cslg > thrcsg and cslb > thrcsb) {
    cslc = red;
  }
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
  if (digitalRead(tsfpin) == HIGH) {
    tsf = true;
  } else {
    tsf = false;
  }
}

void tsrread() {
  if (digitalRead(tsrpin) == HIGH) {
    tsr = true;
  } else {
    tsr = false;
  }
}

void tsbrread() {
  if (digitalRead(tsbrpin) == HIGH) {
    tsbr = true;
  } else {
    tsbr = false;
  }
}

void tsblread() {
  if (digitalRead(tsblpin) == HIGH) {
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
  csread();
  ptbread();

  //モノクロの計算
  csrm = (csrr + csrg + csrb)/3 ;
  cslm = (cslr + cslg + cslb)/3 ;

  //障害物があるか否か
  tsfread();
}

//ここまでリード関数

//ここから駆動関数

//モータードライバ、前、右
void mdfrd(int s) {
  digitalWrite(mdfstbypin, HIGH);
  if (s < 0) {
    digitalWrite(mdfr1pin, HIGH);
    digitalWrite(mdfr2pin, LOW);
  } else {
    digitalWrite(mdfr1pin, LOW);
    digitalWrite(mdfr2pin, HIGH);
  }
  analogWrite(mdfrpwmpin,abs(s));
}

//モータードライバ、前、左
void mdfld(int s) {
  digitalWrite(mdfstbypin, HIGH);
  if (s < 0) {
    digitalWrite(mdfl1pin, HIGH);
    digitalWrite(mdfl2pin, LOW);
  } else {
    digitalWrite(mdfl1pin, LOW);
    digitalWrite(mdfl2pin, HIGH);
  }
  analogWrite(mdflpwmpin,abs(s));
}

//モータードライバ、後、右
void mdbrd(int s) {
  digitalWrite(mdbstbypin, HIGH);
  if (s < 0) {
    digitalWrite(mdbr1pin, HIGH);
    digitalWrite(mdbr2pin, LOW);
  } else {
    digitalWrite(mdbr1pin, LOW);
    digitalWrite(mdbr2pin, HIGH);
  }
  analogWrite(mdbrpwmpin,abs(s));
}

//モータードライバ、後、左
void mdbld(int s) {
  digitalWrite(mdbstbypin, HIGH);
  if (s < 0) {
    digitalWrite(mdbl1pin, HIGH);
    digitalWrite(mdbl2pin, LOW);
  } else {
    digitalWrite(mdbl1pin, LOW);
    digitalWrite(mdbl2pin, HIGH);
  }
  analogWrite(mdblpwmpin,abs(s));
}

//ブレーキ
void mdb() {
  digitalWrite(mdfstbypin, HIGH);
  digitalWrite(mdfr1pin, HIGH);
  digitalWrite(mdfr2pin, HIGH);
  analogWrite(mdfrpwmpin, 0);

  digitalWrite(mdfl1pin, HIGH);
  digitalWrite(mdfl2pin, HIGH);
  analogWrite(mdflpwmpin, 0);

  digitalWrite(mdbstbypin, HIGH);
  digitalWrite(mdbr1pin, HIGH);
  digitalWrite(mdbr2pin, HIGH);
  analogWrite(mdbrpwmpin, 0);

  digitalWrite(mdbl1pin, HIGH);
  digitalWrite(mdbl2pin, HIGH);
  analogWrite(mdblpwmpin, 0);
}

//サーボモータ
void svb1d(int r) {
  svb1s.write(r);
}

void svb2d(int r) {
  svb2s.write(r);
}

void svb3d(int r) {
  svb3s.write(r);
}

void svb4d(int r) {
  svb4s.write(r);
}

void svrd(int r) {
  svfs.write(r);
}

//しきい値宣言
void threshold() {
  //赤検知
  thrcsr = 700;
  thrcsg = 900;
  thrcsb = 900;

  //緑検知
  thgcsr = 900;
  thgcsg = 700;
  thgcsb = 900;

  //白検知
  thwcsr = 700;
  thwcsg = 700;
  thwcsb = 700;
  thwcsm = 700;

  //銀検知
  thwcsr = 600;
  thwcsg = 600;
  thwcsb = 600;

  //黒検知
  thbcsr = 900;
  thbcsg = 900;
  thbcsb = 900;
  thbcsm = 900;
}

//ピンの設定
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

//ライントレース関数、連続で呼んでね
void linetrace() {
  int outmdfr;
  int outmdfl;
  int outmdbr;
  int outmdbl;
  lineread();

  outmdfr = (csrm - thwcsm)/1000*255;
  outmdfl = (cslm - thwcsm)/1000*255;
  outmdbr = (csrm - thwcsm)/1000*255;
  outmdbl = (cslm - thwcsm)/1000*255;

  mdfrd(outmdfr);
  mdfrd(outmdfl);
  mdfrd(outmdbr);
  mdfrd(outmdbl);
}

//徐行
void slowtrace() {
  int outmdfr;
  int outmdfl;
  int outmdbr;
  int outmdbl;
  lineread();

  outmdfr = (csrm - thwcsm)/1000*255/1.8;
  outmdfl = (cslm - thwcsm)/1000*255/1.8;
  outmdbr = (csrm - thwcsm)/1000*255/1.8;
  outmdbl = (cslm - thwcsm)/1000*255/1.8;

  mdfrd(outmdfr);
  mdfrd(outmdfl);
  mdfrd(outmdbr);
  mdfrd(outmdbl);
}

//逆走
void backtrace() {
  int outmdfr;
  int outmdfl;
  int outmdbr;
  int outmdbl;
  lineread();

  outmdfr = (csrm - thwcsm)/1000*255/1.8*-1;
  outmdfl = (cslm - thwcsm)/1000*255/1.8*-1;
  outmdbr = (csrm - thwcsm)/1000*255/1.8*-1;
  outmdbl = (cslm - thwcsm)/1000*255/1.8*-1;

  mdfrd(outmdfr);
  mdfrd(outmdfl);
  mdfrd(outmdbr);
  mdfrd(outmdbl);
}

//フェイルセーフ（ギャップ検索）
void gap() {
  
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