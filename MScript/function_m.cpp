////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
Begin license text.

Copyright 2023- MicomScript Project

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
associated documentation files (the “Software”), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial 
portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
IN THE SOFTWARE.

End license text.
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <SPI.h>
#include <Wire.h>
//#include <TFT_eSPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>
#include "common.h"
#include "lock.h"
#include "command.h"
#include "expression.h"
#include "logic.h"
#include "varmap.h"
#include "sentence.h"
#include "function.h"
#include "function_m.h"

extern Command	_command;

//TFT_eSPI _tft = TFT_eSPI();

//int _gColor = TFT_WHITE;
//int _bColor = TFT_BLACK;	//文字の背景色
//int textSize = 1;			//文字のサイズ
//------------------------------------------------------
extern VarMapStack _varMap;

/*　Input
Value *Input::evaluate(){
// for ESP32
	char buff[IN_BUFF_SIZE];
	_command.readLine(buff, IN_BUFF_SIZE, false);
	return new Text(buff);
}
*/

/*
Value *SetColor::evaluate(){
	int color,back;
	color = _list -> getElement(0) -> getInt();
	if(_list->size() == 2){
		back = _list -> getElement(1) -> getInt();
		_bColor = back;
	}
	_gColor = color;
	
	_tft.setTextColor(_gColor,_bColor);
	return NULL;
}

Value *TextSize::evaluate(){
	textSize = _list -> getElement(0) -> getInt();
	_tft.setTextSize(textSize);
	return NULL;
}

Value *ClearScreen::evaluate(){
	_tft.fillScreen(_bColor);
	return NULL;
}

Value *FillScreen::evaluate(){
	_tft.fillScreen(_gColor);
	return NULL;
}

Value *DrawLine::evaluate() {
	int x1,x2,y1,y2;
	x1 = _list-> getElement(0)->getInt();
	y1 = _list-> getElement(1)->getInt();
	x2 = _list-> getElement(2)->getInt();
	y2 = _list-> getElement(3)->getInt();

	_tft.drawLine(x1,y1,x2,y2,_gColor);
	return NULL;
}

Value *DrawPixel::evaluate() {
	int x,y;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	_tft.drawPixel(x,y,_gColor);
	return NULL;
}

Value *DrawString::evaluate() {
	int x,y;
	char *str;
	str = _list->getElement(0)->getText();
	x = _list-> getElement(1)->getInt();
	y = _list-> getElement(2)->getInt();
		
	_tft.drawString(str,x,y);
	return NULL;
}

Value *DrawRect::evaluate() {
	int x,y,w,h;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	w = _list-> getElement(2)->getInt();
	h = _list-> getElement(3)->getInt();
	_tft.drawRect(x,y,w,h,_gColor);
	return NULL;
}

Value *FillRect::evaluate() {
	int x,y,w,h;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	w = _list-> getElement(2)->getInt();
	h = _list-> getElement(3)->getInt();
	_tft.fillRect(x,y,w,h,_gColor);
	return NULL;
}

Value *DrawCircle::evaluate() {
	int x,y,r;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	r = _list-> getElement(2)->getInt();
	_tft.drawCircle(x,y,r,_gColor);
	return NULL;
}

Value *FillCircle::evaluate() {
	int x,y,r;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	r = _list-> getElement(2)->getInt();
	_tft.fillCircle(x,y,r,_gColor);
	return NULL;
}

Value *DrawTriangle::evaluate() {
	int x0,y0,x1,y1,x2,y2;
	x0 = _list-> getElement(0)->getInt();
	y0 = _list-> getElement(1)->getInt();
	x1 = _list-> getElement(2)->getInt();
	y1 = _list-> getElement(3)->getInt();
	x2 = _list-> getElement(4)->getInt();
	y2 = _list-> getElement(5)->getInt();
	_tft.drawTriangle(x0,y0,x1,y1,x2,y2,_gColor);
	return NULL;
}
Value *FillTriangle::evaluate() {
	int x0,y0,x1,y1,x2,y2;
	x0 = _list-> getElement(0)->getInt();
	y0 = _list-> getElement(1)->getInt();
	x1 = _list-> getElement(2)->getInt();
	y1 = _list-> getElement(3)->getInt();
	x2 = _list-> getElement(4)->getInt();
	y2 = _list-> getElement(5)->getInt();
	_tft.fillTriangle(x0,y0,x1,y1,x2,y2,_gColor);
	return NULL;
}
Value *DrawFastVLine::evaluate(){
	int x,y,v;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	v = _list-> getElement(2)->getInt();
	_tft.drawFastVLine(x,y,v,_gColor);
	return NULL;
}

Value *DrawFastHLine::evaluate(){
	int x,y,h;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	h = _list-> getElement(2)->getInt();
	_tft.drawFastHLine(x,y,h,_gColor);
	return NULL;
}
Value *DrawEllipse::evaluate(){
	int x,y,rx,ry;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	rx = _list-> getElement(2)->getInt();
	ry = _list-> getElement(3)->getInt();
	_tft.drawEllipse(x,y,rx,ry,_gColor);
	return NULL;
}
Value *FillEllipse::evaluate(){
	int x,y,rx,ry;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	rx = _list-> getElement(2)->getInt();
	ry = _list-> getElement(3)->getInt();
	_tft.fillEllipse(x,y,rx,ry,_gColor);
	return NULL;
}
Value *DrawRoundRect::evaluate(){
	int x,y,w,h,r;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	w = _list-> getElement(2)->getInt();
	h = _list-> getElement(3)->getInt();
	r = _list-> getElement(4)->getInt();
	_tft.drawRoundRect(x,y,w,h,r,_gColor);
	return NULL;
}
Value *FillRoundRect::evaluate(){
	int x,y,w,h,r;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	w = _list-> getElement(2)->getInt();
	h = _list-> getElement(3)->getInt();
	r = _list-> getElement(4)->getInt();
	_tft.fillRoundRect(x,y,w,h,r,_gColor);
	return NULL;
}
Value *DrawCircleHelper::evaluate(){
	int x,y,r,c;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	r = _list-> getElement(2)->getInt();
	c = _list-> getElement(3)->getInt();
	_tft.drawCircleHelper(x,y,r,c,_gColor);
	return NULL;
}
Value *FillCircleHelper::evaluate(){
	int x,y,r,c,d;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	r = _list-> getElement(2)->getInt();
	c = _list-> getElement(3)->getInt();
	d = _list-> getElement(4)->getInt();
	_tft.fillCircleHelper(x,y,r,c,d,_gColor);
	return NULL;
}
*/
// letter size: 8rowsx5colums
uint8_t _letters[][8] = {                                   //tatch
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Space
//{ 0x00, 0x02, 0x06, 0x06, 0x04, 0x04, 0x00, 0x04 }, // !
{ 0x00, 0x04, 0x0e, 0x0e, 0x04, 0x04, 0x00, 0x04 }, // !
{ 0x00, 0x0a, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00 }, // "
{ 0x00, 0x0a, 0x1f, 0x0a, 0x0a, 0x1f, 0x0a, 0x0a }, // #
{ 0x00, 0x0e, 0x15, 0x14, 0x0e, 0x05, 0x15, 0x0e }, // $
{ 0x00, 0x00, 0x09, 0x02, 0x04, 0x08, 0x12, 0x00 }, // %
{ 0x00, 0x0c, 0x12, 0x12, 0x0c, 0x0d, 0x12, 0x1d }, // &
{ 0x00, 0x08, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00 }, // '
{ 0x00, 0x0c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0c }, // (
{ 0x00, 0x18, 0x04, 0x04, 0x04, 0x04, 0x04, 0x18 }, // )
{ 0x00, 0x04, 0x15, 0x0e, 0x04, 0x0e, 0x15, 0x04 }, // *
{ 0x00, 0x04, 0x04, 0x04, 0x1f, 0x04, 0x04, 0x04 }, // +
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x08 }, // ,
{ 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00 }, // -
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c }, // .
{ 0x00, 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x00 }, // /
{ 0x00, 0x0e, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e }, // 0
{ 0x00, 0x04, 0x0c, 0x04, 0x04, 0x04, 0x04, 0x0e }, // 1
{ 0x00, 0x0e, 0x11, 0x02, 0x04, 0x08, 0x11, 0x1f }, // 2
{ 0x00, 0x0e, 0x11, 0x01, 0x06, 0x01, 0x11, 0x0e }, // 3
{ 0x00, 0x14, 0x14, 0x14, 0x14, 0x1f, 0x04, 0x04 }, // 4
{ 0x00, 0x1f, 0x10, 0x10, 0x1e, 0x01, 0x01, 0x1e }, // 5
{ 0x00, 0x0e, 0x11, 0x10, 0x1e, 0x11, 0x11, 0x0e }, // 6
{ 0x00, 0x1f, 0x11, 0x11, 0x02, 0x04, 0x04, 0x04 }, // 7
{ 0x00, 0x0e, 0x11, 0x11, 0x0e, 0x11, 0x11, 0x0e }, // 8
{ 0x00, 0x0e, 0x11, 0x11, 0x0f, 0x01, 0x01, 0x0e }, // 9
{ 0x00, 0x00, 0x04, 0x04, 0x00, 0x04, 0x04, 0x00 }, // :
{ 0x00, 0x00, 0x04, 0x04, 0x00, 0x04, 0x04, 0x08 }, // ;
{ 0x00, 0x01, 0x02, 0x04, 0x08, 0x04, 0x02, 0x01 }, // <
{ 0x00, 0x00, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x00 }, // =
{ 0x00, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08 }, // >
{ 0x00, 0x0e, 0x11, 0x13, 0x04, 0x04, 0x00, 0x04 }, // ?
{ 0x00, 0x0e, 0x13, 0x15, 0x15, 0x16, 0x11, 0x0e }, // @
{ 0x00, 0x0e, 0x11, 0x11, 0x11, 0x1f, 0x11, 0x11 }, // A
{ 0x00, 0x1e, 0x11, 0x11, 0x1e, 0x11, 0x11, 0x1e }, // B
{ 0x00, 0x0e, 0x11, 0x11, 0x10, 0x10, 0x11, 0x0e }, // C
{ 0x00, 0x1e, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1e }, // D
{ 0x00, 0x1f, 0x10, 0x10, 0x1e, 0x10, 0x10, 0x1f }, // E
{ 0x00, 0x1f, 0x10, 0x10, 0x1e, 0x10, 0x10, 0x10 }, // F
{ 0x00, 0x0e, 0x11, 0x11, 0x10, 0x17, 0x11, 0x0f }, // G
{ 0x00, 0x11, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11 }, // H
{ 0x00, 0x0e, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0e }, // I
{ 0x00, 0x03, 0x01, 0x01, 0x01, 0x11, 0x11, 0x0e }, // J
{ 0x00, 0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11 }, // K
{ 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x11, 0x1f }, // L
{ 0x00, 0x11, 0x1b, 0x1b, 0x15, 0x15, 0x15, 0x15 }, // M
{ 0x00, 0x11, 0x19, 0x15, 0x15, 0x13, 0x11, 0x11 }, // N
{ 0x00, 0x0e, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e }, // O
{ 0x00, 0x1e, 0x11, 0x11, 0x1e, 0x10, 0x10, 0x10 }, // P
{ 0x00, 0x0e, 0x11, 0x11, 0x11, 0x15, 0x13, 0x0d }, // Q
{ 0x00, 0x1c, 0x12, 0x12, 0x1c, 0x14, 0x12, 0x11 }, // R
{ 0x00, 0x0e, 0x11, 0x10, 0x0e, 0x01, 0x11, 0x0e }, // S
{ 0x00, 0x1f, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04 }, // T
{ 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e }, // U
{ 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0a, 0x04 }, // V
{ 0x00, 0x11, 0x15, 0x15, 0x15, 0x15, 0x15, 0x0a }, // W
{ 0x00, 0x11, 0x11, 0x0a, 0x04, 0x0a, 0x11, 0x11 }, // X
{ 0x00, 0x11, 0x11, 0x11, 0x0a, 0x04, 0x04, 0x04 }, // Y
{ 0x00, 0x0f, 0x11, 0x02, 0x04, 0x08, 0x11, 0x1f }, // Z
{ 0x00, 0x0e, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0e }, // [
{ 0x00, 0x00, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00 }, //
{ 0x00, 0x0e, 0x02, 0x02, 0x02, 0x02, 0x02, 0x0e }, // ]
{ 0x00, 0x04, 0x0a, 0x11, 0x00, 0x00, 0x00, 0x00 }, // ^
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f }, // _
{ 0x00, 0x0c, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00 }, // `
{ 0x00, 0x02, 0x04, 0x04, 0x08, 0x04, 0x04, 0x02 }, // {
{ 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04 }, // |
{ 0x00, 0x08, 0x04, 0x04, 0x02, 0x04, 0x04, 0x08 }, // }
{ 0x00, 0x00, 0x0a, 0x15, 0x11, 0x0a, 0x04, 0x00 }, // ~(Heart)
{ 0x00, 0x00, 0x04, 0x0a, 0x11, 0x0a, 0x04, 0x00 }  // Diamond
};


//GPIO関数----------------------------------------
Value *PinMode::evaluate(){
	int pin,io;
	pin = _list->getElement(0)->getInt();
	io = _list->getElement(1)->getInt();
	if(io==0){
		pinMode(pin,OUTPUT);
	}else if(io==1){
		pinMode(pin,INPUT);
	}else if(io==2){
		pinMode(pin,INPUT_PULLUP);
	}else{
		printf("Invalid Parameter(pinMode)\n");
	}
	
	return NULL;
}

Value *DigitalWrite::evaluate(){
	int pin,HL;
	pin = _list-> getElement(0)->getInt();
	HL = _list-> getElement(1)->getInt();
	if(HL == 0){
		digitalWrite(pin,LOW);
	}else if(HL == 1){
		digitalWrite(pin,HIGH);
	}else{
		printf("Invalid Parameter(digitalWrite)...HIGH or LOW\n");
	}
	return NULL;
}

Value *AnalogRead::evaluate(){
	int pin;
	pin = _list-> getElement(0) ->getInt();
	int analogValue = analogRead(pin);
	return new Integer(analogValue);
}

Value *DigitalRead::evaluate(){
	int pin;
	pin = _list-> getElement(0) ->getInt();
	int digitalValue = digitalRead(pin);
	return new Integer(digitalValue);
}
//--------------------------------------------------
/*
Value *Buzzer::evaluate(){
	int pin;
	unsigned int tones;
	pin = _list-> getElement(0)->getInt();
	tones = _list-> getElement(1)->getInt();
	ledcSetup(1,tones,16);
	ledcAttachPin(pin,1);
	ledcWrite(1,tones);
	return NULL;
}

Value *StopBuzzer::evaluate(){
	int ch;
	ch = _list-> getElement(0)->getInt();
	ledcWrite(ch,0);
	return NULL;
}
*/
/*
仕様変更により削除
Value *LedcSetup::evaluate(){
	int ch,bits;
	unsigned int freq;
	ch = _list-> getElement(0)->getInt();
	freq = _list-> getElement(1)->getInt();
	bits = _list-> getElement(2)->getInt();
	ledcSetup(ch,freq,bits);
	return NULL;
}
*/

Value *LedcDetach::evaluate(){
	int pin;
	pin = _list-> getElement(0)->getInt();
	ledcDetach(pin);
	return NULL;
}

Value *LedcAttach::evaluate(){
	int pin,freq,bits;
	pin = _list-> getElement(0)->getInt();
	freq = _list-> getElement(1)->getInt();
	bits = _list-> getElement(2)->getInt();
	ledcAttach(pin,freq,bits);
	return NULL;
}

Value *LedcWrite::evaluate(){
	int ch;
	unsigned int tones;
	ch = _list-> getElement(0)->getInt();
	tones = _list-> getElement(1)->getInt();
	ledcWrite(ch,tones);
	return NULL;
}

Value *LedcWriteTone::evaluate(){
	int ch;
	unsigned int freq;
	ch = _list-> getElement(0)->getInt();
	freq = _list-> getElement(1)->getInt();
	ledcWriteTone(ch,freq);
	return NULL;
}

Value *LedcRead::evaluate(){
	int ch;
	ch = _list-> getElement(0)->getInt();
	int readDuty = ledcRead(ch);
	return new Integer(readDuty);
}

Value *LedcReadFreq::evaluate(){
	int ch;
	ch = _list-> getElement(0)->getInt();
	int readFreq = ledcReadFreq(ch);
	return new Integer(readFreq);
}

//--------------------------------------------------

void AbstructObject::printTree(int s){
	printSpace(s);
	print();
	printf("\n");
}

//--------------------------------------------------
char _high[] = "HIGH";
char _low[] = "LOW";

char *Output::getText() {
	return _state ? _high : _low;
}

char *Output::getType() {
	return "Output";
}

Value *Output::evaluate() {
	return new Output(_pin);
}

void Output::print() {
	printf("{Output(%d) state = %d}", _pin, _state);
}

Value *Output::methodCall(char *name, List *params) {
#ifdef DEBUG
	printf("Output::methodCall(%s)\n", name);
#endif
	if (strcmp(name, "high") == 0) {
		high();
		return new Integer(1);
	} else if (strcmp(name, "low") == 0) {
		low();
		return new Integer(0);
	} else if (strcmp(name, "toggle") == 0) {
		toggle();
		return new Integer(_state);
	} else if (strcmp(name, "status") == 0){
		return new Integer(_state);
	} else {
		printf("Undefined function: %s\n", name);
	}
	return NULL;
}

Value *OutputGen::evaluate() {
	int n = _list->size();
	if (n == 0){
		printf("Insufficient number of parameters\n");
		throw ErrorException();
	}
	int port = _list->getElement(0)->getInt();
	uint8_t state = 0;
	if (n >= 2){
		state = _list->getElement(1)->getInt();
	}
	return new Output(port, state);
}
//////////////////////////////////////////////////tatch
Value *PWM::methodCall(char *name, List *params){
#ifdef DEBUG
	printf("PWM::methodCall(%s)\n", name);
#endif
	if(strcmp(name, "start") == 0){
    uint32_t freq = params->getElement(0)->getInt();
	  double duty = params->getElement(1)->getFloat();
    ledcDetach(_pin);
    ledcAttach(_pin, freq, _bits);
    uint32_t _duty = _range * duty;
//    printf("_duty = %d\n", _duty);
		ledcWrite(_pin, _duty);
	}else if(strcmp(name, "bit") == 0){
		_bits = params->getElement(0)->getInt();
    uint32_t n = 1;
		for(int i=0; i<_bits; i++){
      n = n * 2;
    }
    _range = n - 1;
	}else if(strcmp(name, "stop") == 0){
		ledcDetach(_pin);
	}
	return NULL;
}

Value *PWMGen::evaluate(){
	int pin;
  pin = _list->getElement(0)->getInt();
	return new PWM(pin);
}

Value	*AQM1602obj::methodCall(char *name, List *params){
  if(strcmp(name, "begin") == 0){
    if(params->size() == 2){
      int sda = params->getElement(0)->getInt();
      int scl = params->getElement(1)->getInt();
      Wire.begin(sda,scl);
    }else if(params->size() != 0){
      printf("This has no params\n");
    }else{
      Wire.begin();
    }
    begin();
    
  }else if(strcmp("clear", name) == 0){
    if(!(params->size() == 0)){
      printf("This has no params\n");
    }
    clear();
  }else if(strcmp("cursor", name) == 0){
    if(params->size() != 2){
      printf("not match params\n");
      return NULL;
    }
    int y = params->getElement(0)->getInt();
    int x = params->getElement(1)->getInt();
    cursor(x,y);
  }else if(strcmp("print", name) == 0){
    if(params->size() != 1){
      printf("not match params\n");
      return NULL;
    }
    Value *v = params->getElement(0);
    if(strcmp("Integer", v->getType()) == 0){
      print(v->getInt());
    }else{
      print(v->getText());
    }
  }else if(strcmp("putChar", name) == 0){
    if(params->size() != 1){
      printf("not match params\n");
      return NULL;
    }
    char *c = params->getElement(0)->getText();
    putChar(*c);
  }else if(strcmp("setContrast", name) == 0){
    if(params->size() != 1){
      printf("not match params\n");
      return NULL;
    }
    int contrast = params->getElement(0)->getInt();
    if(contrast < 0 || contrast > 63){
      printf("contrast : 0 - 63\n");
      return NULL;
    }
    setContrast(contrast);
  }else{
    printf("not found such name of method\n");
  }
  return NULL;
}
/*
 * コマンドを送る。
 */
void AQM1602obj::writeCommand(byte command){
	Wire.beginTransmission(_address);
	Wire.write(0x00);
	Wire.write(command);
	Wire.endTransmission();
	delay(10);
}
/*
 * データを送る。
 */
void AQM1602obj::writeData(byte data){
	Wire.beginTransmission(_address);
	Wire.write(0x40);
	Wire.write(data);
	Wire.endTransmission();
	delay(1);
}
/*
 * 画面を消去する。
 */
void AQM1602obj::clear(){
	writeCommand(0x01);
}
/*
 * カーソルを(x,y)へ移動する。
 */
void AQM1602obj::cursor(int y, int x){
	unsigned char address = 0x40 * x + y;
	writeCommand(address | 0x80);
}
/*
 * 文字列sを表示する。
 */
void AQM1602obj::print(char *s){
	for( ; *s ; s++){
		writeData(*s);
	}
}
/*
 * 文字cを表示する。
 */
void AQM1602obj::putChar(char c){
	writeData(c);
}
/*
 * 整数nを十進数で表示する。
 */
void AQM1602obj::print(int n){
	sprintf((char*)_buff, "%d", n);
	print(_buff);
}
/*
 * コントラストを設定する。
 * c: 0～63
 */
void AQM1602obj::setContrast(int c){
	unsigned char c1 = (unsigned char)(0x70 | (c & 0x0f));
	unsigned char c2 = (unsigned char)(0x50 | 0x04 | ((c & 0x30) >> 4));
	writeCommand(0x39);	// Function set: N=1 DH=0 IS=1
	writeCommand(c1);	// Contrast set:(C3,C2,C1,C0)=(x,x,x,x)
	writeCommand(c2);	// Contrast set:(Ion,Bon,C5,C4)=(0,1,x,x)
	writeCommand(0x38);	// Function set: N=1 DH=0 IS=0
}
/*
 * LCDを初期化する。
 */
void AQM1602obj::begin(){
//	Wire.begin();
	delay(60);				// wait time > 40mS
	writeCommand(0x38);		// Function set: N=1 DH=0 IS=0
	delayMicroseconds(40);	// wait time > 26.3 uS
	writeCommand(0x39); 	// Function set: N=1 DH=0 IS=1
	delayMicroseconds(40);	// wait time > 26.3 uS
	writeCommand(0x14);		// Internal OSC frequency: BS=0 (F2,F2,F1)=(1,0,0)
	delayMicroseconds(40);	// wait time > 26.3 uS
	writeCommand(0x7A);		// Contrast set:(C3,C2,C1,C0)=(0,1,0,1)
	delayMicroseconds(40);	// wait time > 26.3 uS
	writeCommand(0x54); 	// Contrast set: (Ion,Bon,C5,C4)=(0,1,1,0)
	delayMicroseconds(40);	// wait time > 26.3 uS
	writeCommand(0x6c);		// Follower control: 
	delay(200);				// wait time > 200mS
	writeCommand(0x38);		// Function set: N=1 DH=0 IS=0
	delayMicroseconds(40);	// wait time > 26.3 uS
	writeCommand(0x06);		// Entry Mode: I/D=1
	delayMicroseconds(40);	// wait time > 26.3 uS
	writeCommand(0x01);		// Clear Display
	delayMicroseconds(40);	// wait time > 26.3 uS
	writeCommand(0x0c);		// Display ON
	delayMicroseconds(40);	// wait time > 26.3 uS
}


Value *AQM1602Gen::evaluate(){
  uint8_t address = _list->getElement(0)->getInt();
  return new AQM1602obj(address);
}

void DHT20::begin(){
	Wire.begin();
}

int DHT20::scan(){
	uint8_t buff[8];
	long value;
	bool loop = true;
	uint8_t	cnt = 0;

	while (loop) {
		Wire.beginTransmission(0x38);
		Wire.write(0xAC);
		Wire.write(0x33);
		Wire.write(0x00);
		Wire.endTransmission();
		delay(100);

		Wire.requestFrom(0x38, 7);
		for (uint8_t i = 0; i < 7; i++){
			buff[i] = Wire.read();
		}
		if (buff[0] & 0x80){
			Serial.println("Sensor is busy...");
			cnt++;
			if (cnt >= 5){
				loop = false;
			}
		} else {
			loop = false;
		}
	}
	// Humidity
	value = buff[1];
	value <<= 8;
	value |= buff[2];
	value <<= 4;
	value |= ((buff[3] >> 4) & 0x0f);
	_humid = value / 10485.76;

	// Temperature
	value = (buff[3] & 0xf);
	value <<= 8;
	value |= buff[4];
	value <<= 8;
	value |= buff[5];
	_temp = value / 5242.88 - 50;

	// CRC
	uint16_t crc = 0xff;
	for(int i = 0; i < 6; i++){
		crc ^= buff[i];
		for(int j = 0; j < 8; j++){
			crc <<= 1;
			if (crc > 0xff){
				crc ^= 0x31;
			}
			crc &= 0xff;
		}
	}
	if (crc == buff[6]){
		_status = 1;	// Sucess
	} else {
		_status = 2;	// fail
	}
	return _status;
}

float DHT20::getHumid(){
  scan();
	return _humid;
}

float DHT20::getTemper(){
  scan();
	return _temp;
}

int DHT20::getStatus(){
  scan();
  printf("SDA %d,SCL %d\n",_sda,_scl);
	return _status;
}

Value *DHT20::evaluate() {
	return new DHT20(_sda, _scl);
}

void DHT20::print() {
  printf("DHT20");
}

Value *DHT20::methodCall(char *name, List *params) {
#ifdef DEBUG
	printf("DHT20::methodCall(%s)\n", name);
#endif
	if (strcmp(name, "humidity") == 0) {
		float humid = getHumid();
    //Serial.println("hu");
		return new Float(humid);
	} else if (strcmp(name, "temperature") == 0) {
		float temp = getTemper();
		return new Float(temp);
	} else if (strcmp(name, "status") == 0) {
      int status = getStatus();
      return new Integer(status);
  } else {
		printf("Undefined function: %s\n", name);
	}
	return NULL;
}

Value *DHT20Gen::evaluate(){
  int sda = _list->getElement(0)->getInt();
  int scl = _list->getElement(1)->getInt();
	DHT20 *dht20 = new DHT20(sda,scl);
	dht20->begin();
  //Serial.printf("%d,%d",sda,scl);
  //Wire.setPins(sda,scl);
	return dht20;
}

//--------------------------------------------------

ButtonList		_buttonList;
TimerList		_timerlist;
MatrixList  _matrixlist;          // tatch
SimpleLocker	_timerlist_locker;
SimpleLocker	_buttonlist_locker;
SimpleLocker  _matrixlist_locker;        // tatch

char _timerName[] = "Timer";

void buttonObserver(void *args){
	while(1){
		if (_buttonlist_locker.lock(400)){
			_buttonList.scan();
			_buttonlist_locker.unlock(400);
		}
		delay(10);
	}
}

void timeObserver(void *args){
	while(1){
		if (_timerlist_locker.lock(300)){
			_timerlist.dispatch();
			_timerlist_locker.unlock(300);
		}
		delay(1);
	}
}

void matrixObserver(void *args){
  while(1){
    if(_matrixlist_locker.lock(500)){
      _matrixlist.display();
      _matrixlist_locker.unlock(500);
    }
    delay(1);
  }
}
void ObjList::clearNodes(){
	ObjNode *node, *next;
	for(node = _top; node ; node = next){
		next = node->_next;
		delete node;
	}
}

ObjNode *ObjList::search(Value *obj){
	ObjNode *node;
	for(node = _top; node ; node = node->_next){
		if (node->_obj == obj)
			return node;
	}
	return NULL;
}

void ObjList::add(Value *obj){
	ObjNode *node = search(obj);
	if (node == NULL){
		_top = new ObjNode(obj, _top);
	}
}

void ObjList::remove(Value *obj){
	ObjNode *node, *next, *prev;
	prev = NULL;
	for(node = _top; node ; node = next){
		next = node->_next;
		if (node->_obj == obj)
			break;
		prev = node;
	}
	if (node){
		if (node == _top){	// or if (prev == NULL)
			_top = _top->_next;
		} else {
			prev->_next = node->_next;
		}
		delete node;
	}
}

int _listCNT = 0;
int _listSPD = 20;
bool _match = false;

void MatrixList::display(){ // マルチタスク対応用
  ObjNode *node;
	DotMatrix *matrix;
  bool flag = 0;
  if(_match){
    _listCNT++;
    if(_listCNT >= _listSPD){
      _listCNT = 0;
      flag = 1;
    }
  }  
	for(node = _top; node ; node = node->_next){
		matrix = (DotMatrix *)node->_obj;
    if(flag){
      matrix->shiftLeft();
    }
		matrix->display();
	}
}
DotMatrix::DotMatrix(uint8_t *rows, uint8_t *cols){
  uint8_t i;
  _foward = 0;

  // set pinMode
  for(i = 0; i < dotSize; i++){
    _cols[i] = cols[i];
    _rows[i] = rows[i];
    pinMode(_cols[i], OUTPUT);
    digitalWrite(cols[i], !_foward); // HIGH
    pinMode(_rows[i], OUTPUT);
    digitalWrite(rows[i], _foward);  // LOW
    _dotBuff[i] = 0x00;
  }
  _stop = false;
  _speed = 20;
  _isRunning = false;
  _state = 0;
  _stateB = 0;
  setText("HELLO");
  _matrixlist_locker.lock(510);
  _matrixlist.add(this);
  _matrixlist_locker.unlock(510);
}
void DotMatrix::display(){
  if(_state == 0){
    off();
  }else if(_state == 1){
    scroll();
  }
}

void DotMatrix::nextLetter(){
	char c = _msg[_next_char];
	if (c >= 'a' && c <= 'z')
		c -= 'a' - 'A';
	if (c < ' ' || c > '~')
		c = 0x7f;

	if (c >= ' ' && c <= '`'){
		_next_index = c - ' ';
	} else if (c >= '{' && c <= '~'){
		_next_index = c - ' ' - 26;
	} else {
		_next_index = 0x7f - ' ' - 26; // Diamond
	}
	_next_col = 2;
//	_count = 0;
	if (_msg[_next_char] == '\0'){
		_next_char = 0;
	} else {
		_next_char++;
		if (_msg[_next_char] == '\0')
			_next_char = 0;
	}
}
void DotMatrix::setText(char *msg){
	strncpy(_msg, msg, MAX_MSG);
	_msg[MAX_MSG - 1] = '\0';
	_next_char = 0;
	nextLetter();
}

void DotMatrix::lightRows(uint8_t row){
	uint8_t c = _dotBuff[row];
	uint8_t i, bit;
	for(i = 0; i < DOT_SIZE; i++){
		bit = (c >> (DOT_SIZE - 1 - i)) & 1;
    if(_foward == 0){
		  digitalWrite(_rows[i], bit);
    } else {
      digitalWrite(_rows[i], !bit);
    }
  }
}

void DotMatrix::lightLED(){
	uint8_t i;
	for(i = 0; i < DOT_SIZE; i++){
		lightRows(i);
		digitalWrite(_cols[i], _foward);
		delay(1);
		digitalWrite(_cols[i], !_foward);
	}
}

void DotMatrix::shiftLeft(){
	uint8_t i, bit, index;
	for(i = 0; i < DOT_SIZE; i++){
		_dotBuff[i] <<= 1;
		bit = (_letters[_next_index][i] >> (DOT_SIZE - 1 - _next_col)) & 1;
		_dotBuff[i] |= bit;
	}
	if (_next_col >= 7){
		nextLetter();
	} else {
		_next_col++;
	}
}

// マルチタスク対応メソッド {---------------------------------------------
void DotMatrix::scroll(){
	lightLED();

  if(_stop == false && _match == false){
   _count++;
    if (_count > _speed){
      shiftLeft();
      _count = 0;
    }
  }
}
void DotMatrix::off(){
/*
//  スクロールしていた文字列の表示位置を先頭に戻し消灯する
//　High・Lowを初期化する必要あり？
*/
}
// }--------------------------------------------------------------------

Value *DotMatrix::methodCall(char *name, List *params){
  /*
	if(strcmp("on",name) == 0){
		int col = params->getElement(0)->getInt();
		Value *v = params->getElement(1);

		digitalWrite(_cols[col],LOW);
		for(int i=0; i<8; i++){
			if(v->getElement(i)->getInt()){
				digitalWrite(_rows[i],HIGH);
			}
		}
	}else if(strcmp("off",name) == 0){
		int col = params->getElement(0)->getInt();
		digitalWrite(_cols[col],HIGH);
		for(int i=0; i<8; i++){
      digitalWrite(_rows[i],LOW);
		}
  }
  */
  if(strcmp("set",name) == 0){
    setText(params->getElement(0)->getText());
  /*
	}else if(strcmp("set2",name) == 0){
    Value *data = params->getElement(0);
    if(strcmp("string", data->getType()) == 0){
      setText(data->getText());
    }else{
      _datalist.setNode(params->getElement(0));
    }

	}else if(strcmp("print",name) == 0){
      DotListNode *data;
      long now, past;
      data = _datalist.getNode(params->getElement(0)->getInt());
      if(data){
        past = now = millis();
        while(now - past <= params->getElement(1)->getInt()){
          for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
              digitalWrite(_rows[j], data->_data[i][j]);
            }
            digitalWrite(_cols[i], LOW);
            delay(2);
            digitalWrite(_cols[i], HIGH);
          }
          now = millis();
        }
      }
  */
	}else if(strcmp("scroll",name) == 0){
    if(params->size()){
      int s = params->getElement(0)->getInt();
      if(s > 100){
        s = 100;
      }else if(s < 0){
        s = 0;
      }
      if(s == 0){
        _stop = true;
      }else{
        _stop = false;
      }
      _listSPD = 100 - s + 1;
//      _count = 0;
      _speed = _listSPD; 
    }
    _state = 1;
  }else if(strcmp("off",name) == 0){
    _next_col = 0;
    _next_char = 0;
    _listCNT = 0;
    _count = 0;
    uint8_t i;
    for(i=0; i<DOT_SIZE; i++){
      _dotBuff[i] = 0x00;
    }
    nextLetter();
    _state = 0;
  }else if(strcmp("shiftL",name) == 0){
    int shift = params->getElement(0)->getInt();
    for(int i=0; i<shift; i++)
      shiftLeft();
  }else if(strcmp("matchSpeed",name) == 0){
    _match = params->getElement(0)->getBool();
    _speed = _listSPD;
    printf("%d\n", _match);
  }else if(strcmp("direction",name) == 0){
    _foward = params->getElement(0)->getBool();
  for(int i = 0; i < dotSize; i++){
    digitalWrite(_cols[i], !_foward); // HIGH
    digitalWrite(_rows[i], _foward);  // LOW
  }

  } else {
		printf("Undefined function: %s\n", name);
	}

	return NULL;
}

Value *DotMatrixGen::evaluate(){
  uint8_t rows[dotSize];
  uint8_t cols[dotSize];
	Value *r = _list->getElement(0);
	Value *c = _list->getElement(1);
  for(int i=0; i<dotSize; i++){
    rows[i] = r->getElement(i)->getInt();
    cols[i] = c->getElement(i)->getInt();
  }
	return new DotMatrix(rows,cols);
}
/////////////////////////////////////////////////////////////////////////////

void ButtonList::scan(){
	ObjNode *node;
	Button *button;
	for(node = _top; node ; node = node->_next){
		button = (Button *)node->_obj;
		button->scan();
	}
}

Button::Button(uint8_t	pin) {
	_pin = pin;
	pinMode(_pin, INPUT_PULLUP);
	_state = digitalRead(_pin);
	_fOnPress = NULL;
	_fOnRelease = NULL;
}

Button::~Button(){
	delete _fOnPress;
	delete _fOnRelease;
}

char *Button::getText() {
	static char high[] = "HIGH";
	static char low[] = "LOW";
	return _state ? high : low;
}

char *Button::getType() {
	return "Button";
}
Value *Button::evaluate() {
	return new Button(_pin);
}

void Button::print() {
	printf("Button(%d)", _pin);
}

Value *Button::methodCall(char *name, List *params) {
#ifdef DEBUG
	printf("Button::methodCall(%s)\n", name);
#endif
	int n = params->size();
	if (strcmp(name, "onPress") == 0) {
		if (n == 0){
			onPress(NULL);
		} else {
			Value *v = params->getElement(0);
			onPress(v->getText());
		}
	} else if (strcmp(name, "onRelease") == 0){
		if (n == 0){
			onRelease(NULL);
		} else {
			Value *v = params->getElement(0);
			onRelease(v->getText());
		}
	} else {
		printf("Undefined function: %s\n", name);
	}
	return NULL;
}

void Button::onPress(char *fname) {
	if (_fOnPress){
		delete _fOnPress;
		_fOnPress = NULL;
	}
	if (fname == NULL){
		if (_fOnRelease == NULL){
			_buttonlist_locker.waitForUnlock(410);
			_buttonList.remove(this);
			_buttonlist_locker.unlock(410);
		}
	} else {
		_fOnPress = new FuncName(fname);
		_buttonlist_locker.waitForUnlock(420);
		_buttonList.add(this);
		_buttonlist_locker.unlock(420);
	}
}

void Button::onRelease(char *fname) {
	if (_fOnRelease){
		delete _fOnRelease;
		_fOnRelease = NULL;
	}
	if (fname == NULL){
		if (_fOnPress == NULL){
			_buttonlist_locker.waitForUnlock(430);
			_buttonList.remove(this);
			_buttonlist_locker.unlock(430);
		}
	} else {
		_fOnRelease = new FuncName(fname);
		_buttonlist_locker.waitForUnlock(440);
		_buttonList.add(this);
		_buttonlist_locker.unlock(440);
	}
}

void Button::scan() {
	Value *v;
	uint8_t state = digitalRead(_pin);
	if (_state == HIGH && state == LOW) {			// Press
		if (_fOnPress) {
			try{
				v = _fOnPress->evaluate();
				delete v;
			}catch(ErrorException e){}
		}
	} else if (_state == LOW && state == HIGH) {		// Release
		if (_fOnRelease) {
			try{
				v = _fOnRelease->evaluate();
				delete v;
			}catch(ErrorException e){}
		}
	}
	_state = state;
}

void Button::clear() {
	_fOnPress = NULL;
	_fOnRelease = NULL;
//	_btnlist.remove(this);
}

Value *ButtonGen::evaluate() {
	int n = _list->size();
	if (n == 0){
		printf("Insufficient number of parameters\n");
		throw ErrorException();
	}
	int port = _list->getElement(0)->getInt();
	return new Button(port);
}

//----------------------------------------------------------------

void TimerList::dispatch(){
	ObjNode *node;
	Timer *timer;
	for(node = _top; node ; node = node->_next){
		timer = (Timer *)node->_obj;
		timer->dispatch();
	}
}

Timer::Timer(){
	_func = NULL;
	_lastMils = millis();
	_interval = 0;
	_count = 0;
	_isRunning = false;
}

Timer::Timer(unsigned long interval, unsigned long count){
	_func = NULL;
	_lastMils = millis();
	_interval = interval;
	_count = count;
	_isRunning = false;
}

bool Timer::isRunning(){
	return _isRunning;
}

void Timer::dispatch(){
	if (_isRunning){
		unsigned long now = millis();
		if (now - _lastMils >= _interval){
			_lastMils = now;
			try{
				Value *v = _func->evaluate();
				delete v;
			}catch(ErrorException e){}
		}
	}
}

char *Timer::getText() {
	return _timerName;
}

char *Timer::getType() {
	return _timerName;
}

Value *Timer::evaluate() {
	return new Timer(_interval, _count);
}

void Timer::print() {
	printf("Timer: interval = %d\n", _interval);
}

Value *Timer::methodCall(char *name, List *params) {
#ifdef DEBUG
	printf("Timer::methodCall(%s)\n", name);
#endif
	int n = params->size();
	if (strcmp(name, "setTimer") == 0) {
		if (n < 2){
			printf("Insufficient number of parameters.\n");
			throw ErrorException();
		}
		char *fname = params->getElement(0)->getText();
		if (fname == NULL){
			printf("Invalid function name.\n");
			throw ErrorException();
		}
		_func = new FuncName(fname);
		_interval = params->getElement(1)->getInt();
		if (n >= 3){
			_count = params->getElement(2)->getInt();
		}
		if (_func && _interval > 0){
			_isRunning = true;
			_timerlist_locker.waitForUnlock(310);
			_timerlist.add(this);
			_timerlist_locker.unlock(310);
		} else {
			printf("Invalid parameter.\n");
		}
	} else if (strcmp(name, "stop") == 0){
		_timerlist_locker.waitForUnlock(320);
		_timerlist.remove(this);
		_timerlist_locker.unlock(320);
		_isRunning = false;
	} else if (strcmp(name, "resume") == 0){
		if (_isRunning){
			printf("Already running.\n");
			return NULL;
		}
		if (_func && _interval > 0){
			_isRunning = true;
			_timerlist_locker.waitForUnlock(330);
			_timerlist.add(this);
			_timerlist_locker.unlock(330);
		} else {
			printf("Invalid parameter.\n");
		}
	} else {
		printf("Undefined function: %s\n", name);
	}
	return NULL;
}

Value *TimerGen::evaluate() {
	return new Timer();
}

//--------------------------------------------------

void AM2320Obj::print(){
	printf("AM2320");
}



Value *AM2320Obj::methodCall(char *name, List *params) {
#ifdef DEBUG
	printf("AM2320Obj::methodCall(%s)\n", name);
#endif
	if (strcmp(name, "humidity") == 0) {
		float humid = _am2320.readHumidity();
		return new Float(humid);
	} else if (strcmp(name, "temperature") == 0) {
		float temp = _am2320.readTemperature();
		return new Float(temp);
	}  else if(strcmp(name, "status") == 0){
    printf("SDA = %d SCL = %d\n",_sda,_scl);
    int _status = 1;
    return new Integer(_status);
  } else {
		printf("Undefined function: %s\n", name);
	}
	return NULL;
}

Value *AM2320Obj::evaluate(){
	return NULL;
}

void AM2320Obj::begin(){
	_am2320.begin();
}

Value *AM2320Gen::evaluate(){
  int sda = _list->getElement(0)->getInt();
  int scl = _list->getElement(1)->getInt();
	AM2320Obj *am2320 = new AM2320Obj(sda,scl);
	am2320->begin();
	return am2320;
}

//--------------------------------------------------

Speaker::Speaker(uint8_t pin){
	_pin = pin;
//	_channel = ch;
	ledcAttach(_pin, 980, 12);
}

void Speaker::print(){
	printf("Speaker");
}

Value *Speaker::evaluate(){
	return NULL;
}

Value *Speaker::methodCall(char *name, List *params){
#ifdef DEBUG
	printf("Speaker::methodCall(%s)\n", name);
#endif
	if (strcmp(name, "tone") == 0) {
		int n = params->size();
		if (n == 0){
			printf("Invalid parameter\n");
			return NULL;
		}
		uint32_t freq = params->getElement(0)->getInt();
//		ledcWriteTone(_channel, freq);
		ledcDetach(_pin);
		ledcAttach(_pin, freq, 12);
		ledcWrite(_pin, 2046);
		return NULL;
	} else if (strcmp(name, "stop") == 0){
//		ledcWriteTone(_channel, 0);
		ledcWrite(_pin, 0);
	} else {
		printf("Undefined function: %s\n", name);
	}
	return NULL;
}

Value *SpeakerGen::evaluate(){
	int n = _list->size();
	int pin = 21, ch = 0;
	if (n >= 1){
		pin = _list->getElement(0)->getInt();
	}
//	if (n >= 2){
//		ch = _list->getElement(1)->getInt();
//	}
	return new Speaker(pin);
}
