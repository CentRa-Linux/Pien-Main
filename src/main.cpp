#include <Arduino.h>
/*
＿人人人人＿
＞　Pien　＜
￣Y^Y^Y^Y^￣
*/

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

//フォトトランジスタ
int ptb;
int ptr;
int ptl;

//タッチセンサー
int tsf;
int tsb;
int tsbr;
int tsbl;

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

//ここまで部品系

//ここからしきい値系
//しきい値はプログラム内で動的に変更できるように変数でおく



void line_sequence() {

}

void rescue_sequence() {

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
}