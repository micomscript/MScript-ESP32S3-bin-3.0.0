/*
   メモ
       ■シリアルポート設定
         通信速度：   115200
         データ長：   8 bit
         パリティ：   none
         ストップビット： 1bit
         フロー制御： none
         ローカルエコー：OFF
         送信遅延： 1ミリ秒／行

       ■端末設定
         改行コード：　受信CR  送信CR
*/
#include <stdio.h>
#include <string.h>
#include <FS.h>
#include <LittleFS.h>
#include <SPI.h>
//#include <TFT_eSPI.h>
#include "common.h"
#include "observer.h"
#include "fsystem.h"
#include "expression.h"
#include "logic.h"
#include "sentence.h"
#include "command.h"
#include "token.h"
#include "tokenizer.h"
#include "parser.h"
#include "varmap.h"

Parser parser;
//extern TFT_eSPI _tft;
extern MSFileSystem	_fileSystem;
extern int _bColor; //文字の背景色
int _system_mode;	// 0:Dialog Mode  1:Script Mode

void setup() {
//	Serial.begin(921600);
	Serial.begin(115200);
	Serial.println();
	_system_mode = 0;
	_fileSystem.initFSystem();
//	_tft.init();
//	_tft.setRotation(0);
//	_tft.setTextColor(0x0000, 0xffff, false);
//	_tft.fillScreen(_bColor);

	init_observer();

	Serial.println("\nMicomScript 1.0.0-beta\n");
	parser.execute_file("/init");
}

void loop() {
  parser.parse();
}
