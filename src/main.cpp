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
int thrrcsr;
int thrrcsg;
int thrrcsb;
int thrlcsr;
int thrlcsg;
int thrlcsb;

//緑検知
int thgrcsr;
int thgrcsg;
int thgrcsb;
int thglcsr;
int thglcsg;
int thglcsb;

//白検知
int thwrcsr;
int thwrcsg;
int thwrcsb;
int thwrcsm;
int thwlcsr;
int thwlcsg;
int thwlcsb;
int thwlcsm;

//銀検知
int thsrcsr;
int thsrcsg;
int thsrcsb;
int thslcsr;
int thslcsg;
int thslcsb;

//黒検知
int thbrcsr;
int thbrcsg;
int thbrcsb;
int thbrcsm;
int thblcsr;
int thblcsg;
int thblcsb;
int thblcsm;

//補正用
int thcrcsr;
int thcrcsg;
int thcrcsb;
int thclcsr;
int thclcsg;
int thclcsb;

//カラーカウンタ
int ccbrcs;
int ccblcs;
int ccwrcs;
int ccwlcs;
int ccgrcs;
int ccglcs;
int ccrrcs;
int ccrlcs;

int ccbrcse;
int ccblcse;
int ccwrcse;
int ccwlcse;
int ccgrcse;
int ccglcse;
int ccrrcse;
int ccrlcse;

//ここまでしきい値系

//タイマー
unsigned long timer;
unsigned long timerb;

//ここまで変数定義

//センサー系のリード関数

//カラーセンサー
void csrrread() {
  csrr = (analogRead(csrrpin) - thcrcsr)*(1023/(1023 - thcrcsr));
}

void csrgread() {
  csrg = (analogRead(csrgpin) - thcrcsg)*(1023/(1023 - thcrcsg));
}

void csrbread() {
  csrb = (analogRead(csrbpin) - thcrcsb)*(1023/(1023 - thcrcsb));
}

void cslrread() {
  cslr = (analogRead(cslrpin) - thclcsr)*(1023/(1023 - thclcsr));
}

void cslgread() {
  cslg = (analogRead(cslgpin) - thclcsg)*(1023/(1023 - thclcsg));
}

void cslbread() {
  cslb = (analogRead(cslbpin) - thclcsb)*(1023/(1023 - thclcsb));
}

void csread() {
  csrrread();
  csrgread();
  csrbread();
  cslrread();
  cslgread();
  cslbread();

  //white
  if (csrr < thwrcsr and csrg < thwrcsg and csrb < thwrcsb) {
    csrc = white;
  }
  if (cslr < thwlcsr and cslg < thwlcsg and cslb < thwlcsb) {
    cslc = white;
  }

  //black
  if (csrr > thbrcsr and csrg > thbrcsg and csrb > thbrcsb) {
    csrc = black;
  }
  if (cslr > thblcsr and cslg > thblcsg and cslb > thblcsb) {
    cslc = black;
  }

  //green
  if (csrr > thgrcsr and csrg < thgrcsg and csrb > thgrcsb) {
    csrc = green;
  }
  if (cslr > thglcsr and cslg < thglcsg and cslb > thglcsb) {
    cslc = green;
  }

  //red
  if (csrr < thgrcsr and csrg > thrrcsg and csrb > thrrcsb) {
    csrc = red;
  }
  if (cslr < thrlcsr and cslg > thrlcsg and cslb > thrlcsb) {
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

  //超音波センサ
  ssrread();
  sslread();
}

//カラーカウンタ、黒
void ccbrread() {
  if (csrc == black) {
    ccbrcs = ccbrcs + 1;
  } else {
    ccbrcs = ccbrcs - 4;
    if (ccbrcs < 0) {
      ccbrcs = 0;
    }
  }
  
  if (ccbrcs > 30) {
    ccbrcse = 1;
  } else {
    ccbrcse = 0;
  }
}

void ccblread() {
  if (cslc == black) {
    ccblcs = ccblcs + 1;
  } else {
    ccblcs = ccblcs - 4;
    if (ccblcs < 0) {
      ccblcs = 0;
    }
  }
  
  if (ccblcs > 30) {
    ccblcse = 1;
  } else {
    ccblcse = 0;
  }
}

//カラーカウンタ、白
void ccwrread() {
  if (csrc == white) {
    ccwrcs = ccwrcs + 1;
  } else {
    ccwrcs = ccwrcs - 4;
    if (ccwrcs < 0) {
      ccwrcs = 0;
    }
  }
  
  if (ccwrcs > 30) {
    ccwrcse = 1;
  } else {
    ccwrcse = 0;
  }
}

void ccwlread() {
  if (cslc == white) {
    ccwlcs = ccwlcs + 1;
  } else {
    ccwlcs = ccwlcs - 4;
    if (ccwlcs < 0) {
      ccwlcs = 0;
    }
  }
  
  if (ccwlcs > 30) {
    ccwlcse = 1;
  } else {
    ccwlcse = 0;
  }
}

//カラーカウンタ、緑
void ccgrread() {
  if (csrc == green) {
    ccgrcs = ccgrcs + 1;
  } else {
    ccgrcs = ccgrcs - 4;
    if (ccgrcs < 0) {
      ccgrcs = 0;
    }
  }
  
  if (ccgrcs > 30) {
    ccgrcse = 1;
  } else {
    ccgrcse = 0;
  }
}

void ccglread() {
  if (cslc == green) {
    ccglcs = ccglcs + 1;
  } else {
    ccglcs = ccglcs - 4;
    if (ccglcs < 0) {
      ccglcs = 0;
    }
  }
  
  if (ccglcs > 30) {
    ccglcse = 1;
  } else {
    ccglcse = 0;
  }
}

//カラーカウンタ、赤
void ccrrread() {
  if (csrc == red) {
    ccrrcs = ccrrcs + 1;
  } else {
    ccrrcs = ccrrcs - 4;
    if (ccrrcs < 0) {
      ccrrcs = 0;
    }
  }
  
  if (ccrrcs > 30) {
    ccrrcse = 1;
  } else {
    ccrrcse = 0;
  }
}

void ccrlread() {
  if (cslc == red) {
    ccrlcs = ccrlcs + 1;
  } else {
    ccrlcs = ccrlcs - 4;
    if (ccrlcs < 0) {
      ccrlcs = 0;
    }
  }
  
  if (ccrlcs > 30) {
    ccrlcse = 1;
  } else {
    ccrlcse = 0;
  }
}



//ここまでリード関数

//ここから駆動関数

//モータードライバ、前、右
void mdfrd(int s) {
  digitalWrite(mdfstbypin, HIGH);
  if (s < 0) {
    digitalWrite(mdfr1pin, LOW);
    digitalWrite(mdfr2pin, HIGH);
  } else {
    digitalWrite(mdfr1pin, HIGH);
    digitalWrite(mdfr2pin, LOW);
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
    digitalWrite(mdbl1pin, LOW);
    digitalWrite(mdbl2pin, HIGH);
  } else {
    digitalWrite(mdbl1pin, HIGH);
    digitalWrite(mdbl2pin, LOW);
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
  thrrcsr = 700;
  thrrcsg = 900;
  thrrcsb = 900;
  thrlcsr = 700;
  thrlcsg = 900;
  thrlcsb = 900;

  //緑検知
  thgrcsr = 850;
  thgrcsg = 700;
  thgrcsb = 880;
  thglcsr = 850;
  thglcsg = 700;
  thglcsb = 880;

  //白検知
  thwrcsr = 640;
  thwrcsg = 500;
  thwrcsb = 410;
  thwrcsm = 516;
  thwlcsr = 640;
  thwlcsg = 500;
  thwlcsb = 410;
  thwlcsm = 516;

  //黒検知
  thbrcsr = 880;
  thbrcsg = 880;
  thbrcsb = 880;
  thbrcsm = 880;
  thblcsr = 880;
  thblcsg = 880;
  thblcsb = 880;
  thblcsm = 880;

  //補正用
  thcrcsr = 700;
  thcrcsg = 700;
  thcrcsb = 700;
  thclcsr = 700;
  thclcsg = 700;
  thclcsb = 700;
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

  //led
  pinMode(LED_BUILTIN, OUTPUT);

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

  outmdfr = csrm/1023*255;
  outmdfl = cslm/1023*255;
  outmdbr = csrm/1023*255;
  outmdbl = cslm/1023*255;

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

  outmdfr = csrm/1000*255/1.8;
  outmdfl = cslm/1000*255/1.8;
  outmdbr = csrm/1000*255/1.8;
  outmdbl = cslm/1000*255/1.8;

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

  outmdfr = csrm/1000*255/1.8*-1;
  outmdfl = cslm/1000*255/1.8*-1;
  outmdbr = csrm/1000*255/1.8*-1;
  outmdbl = cslm/1000*255/1.8*-1;

  mdfrd(outmdfr);
  mdfrd(outmdfl);
  mdfrd(outmdbr);
  mdfrd(outmdbl);
}

//フェイルセーフ（ギャップ検索）
void failsafe() {
    //120度回す
    timer = millis();
    timerb = millis();
    while (timerb < timer + 1000) {
      timerb = millis();
      mdfrd(255);
      mdfld(-255);
      mdfrd(255);
      mdfld(-255);
      Serial.println("rotating 120 degrees to right");
    }

    //ちょいブレーキ
    mdb();
    delay(500);

    //逆にも
    timer = millis();
    timerb = millis();
    while (timerb < timer + 1000) {
      timerb = millis();
      mdfrd(-125);
      mdfld(125);
      mdfrd(-125);
      mdfld(125);
      Serial.println("rotating 120 degrees to left");
    }

  //ちょいブレーキ
  mdb();
  delay(500);

  //戻す
  timer = millis();
  timerb = millis();
  while (timerb < timer + 500) {
    timerb = millis();
    mdfrd(125);
    mdfld(-125);
    mdfrd(125);
    mdfld(-125);
    Serial.println("rotating 60 degrees to right");
  }

  //ちょいブレーキ
  mdb();
  delay(500);

  //ちょっと前へ
  timer = millis();
  timerb = millis();
  while (timerb < timer + 1500) {
    timerb = millis();
    mdfrd(125);
    mdfld(125);
    mdfrd(125);
    mdfld(125);
    Serial.println("moving forward");
  }

  //ちょいブレーキ
  mdb();
  delay(500);
}

void line_sequence() {

}

void rescue_sequence() {

}

void initf() {
  Serial.begin(9600);

  threshold();//しきい値設定

  pinset();//ピンモードの設定
}

void test() {
  lineread();
  Serial.print(csrc);
  Serial.print(", ");
  Serial.print(cslc);
  Serial.print(", ");
  Serial.print(csrr);
  Serial.print(", ");
  Serial.print(csrg);
  Serial.print(", ");
  Serial.print(csrb);
  Serial.print(", ");
  Serial.print(cslr);
  Serial.print(", ");
  Serial.print(cslg);
  Serial.print(", ");
  Serial.print(cslb);
  Serial.println("");
}

void test2() {
  timer = millis();
  timerb = millis();
  while (timerb < timer+1000) {
    timerb = millis();
    Serial.println("hoge!");
  }
  for (int i = 0; i < 100; i++){
    Serial.println("fuga!");
  }

}

void setup() {
  // put your setup code here, to run once:
  initf();
}

void loop() {
  // put your main code here, to run repeatedly:
  mdfrd(255);
  mdfld(255);
  mdbrd(255);
  mdbld(255);
}