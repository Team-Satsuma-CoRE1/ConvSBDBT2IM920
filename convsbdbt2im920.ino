#include <SoftwareSerial.h>

#define SBDBTRX 12 //デジタル12番ピンはソフトウェアシリアルRX
#define SBDBTTX 11 //デジタル11番ピンはソフトウェアシリアルTX

SoftwareSerial SBDBTSerial(SBDBTRX, SBDBTTX);

unsigned char c[8]; //DualShockからの値を格納する
unsigned long chksum; //checksumの値を格納する

char str[32]={0}; //IM920で送信する値を格納する

void setup() {
    // put your setup code here, to run once:
    c[0] = 0x80; //SBDBTからのシリアル信号の１個目は固定。
    SBDBTSerial.begin(2400);//SBDBTとArduinoは2400bps
    Serial.begin(115200);//シリアルモニター表示
    Serial.setTimeout(1); //timeOut 1ms
}

void loop() {
  static unsigned long lastTime = 0; // 最後にデータを処理した時間を記録
  int i;
  unsigned long currentTime = millis(); // 現在の時間を取得
  
  // 150ms毎にデータが利用可能か確認し、利用可能なら処理
  if (currentTime - lastTime >= 150) {
    if (SBDBTSerial.available() >= 8) { //8byte以上あるかチェック
      if (SBDBTSerial.read() == 0x80) { //１byte読み込んで0x80のスタートビットかチェック
        for (chksum = c[0], i = 1; i < 8; i++) { //スタートビットは読み込み済みなので、次の７個のデータを読み込む。
          c[i] = SBDBTSerial.read();
          if (i < 7) chksum += c[i];
        }
        sprintf(str, "%02x,%02x,%02x,%02x,%02x,%02x,%02x\r\n", c[0], c[1], c[2], c[3], c[4], c[5], c[6]);
        Serial.print(str);
        lastTime = currentTime; // 最後にデータを処理した時間を更新
      }
    }
  }
}

