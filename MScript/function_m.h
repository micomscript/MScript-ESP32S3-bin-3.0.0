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

/*
 *	ESPFunction.h
 */

#ifndef __ESPFUNCTION_H__
#define __ESPFUNCTION_H__

#define MAX_MSG     (32)
#define DOT_SIZE    (8)
//-------------------------------------------------------------------
class SetColor : public AbstructFunction {
public:
	SetColor(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};


class TextSize : public AbstructFunction {
public:
	TextSize(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};

//描画系-------------------------------------------------------------
class ClearScreen : public AbstructFunction {
public:
	ClearScreen(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};

class FillScreen : public AbstructFunction {
public:
	FillScreen(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};
class DrawLine : public AbstructFunction {
public:
	DrawLine(char *name) : AbstructFunction(name){}

	Value *evaluate(); 
};

class DrawPixel : public AbstructFunction {
public:
	DrawPixel(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};

class DrawString :public AbstructFunction {
public:
	DrawString(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};

class DrawRect : public AbstructFunction {
public:
	DrawRect(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};

class FillRect : public AbstructFunction {
public:
	FillRect(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};

class DrawCircle : public AbstructFunction {
public:
	DrawCircle(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};

class FillCircle : public AbstructFunction {
public:
	FillCircle(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};
class DrawTriangle : public AbstructFunction {
public:
	DrawTriangle(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};
class FillTriangle : public AbstructFunction {
public:
	FillTriangle(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};
class DrawFastVLine : public AbstructFunction {
public:
	DrawFastVLine(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};
class DrawFastHLine : public AbstructFunction {
public:
	DrawFastHLine(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};
class DrawEllipse : public AbstructFunction {
public:
	DrawEllipse(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};
class FillEllipse : public AbstructFunction {
public:
	FillEllipse(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};
class DrawRoundRect : public AbstructFunction {
public:
	DrawRoundRect(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};
class FillRoundRect : public AbstructFunction {
public:
	FillRoundRect(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};
class DrawCircleHelper : public AbstructFunction {
public:
	DrawCircleHelper(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};
class FillCircleHelper : public AbstructFunction {
public:
	FillCircleHelper(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};
//GPIO関係-----------------------------------------------------------------------------------------
class PinMode : public AbstructFunction {
public:
	PinMode(char *name) : AbstructFunction(name) {}

	int getParamCount(){return 2;} // tatch

	Value *evaluate();
};

class DigitalWrite : public AbstructFunction {
public:
	DigitalWrite(char *name) : AbstructFunction(name) {}

	Value *evaluate();
};

class AnalogRead :  public AbstructFunction {
public:
	AnalogRead(char *name) : AbstructFunction(name){}

	Value *evaluate(); 
};

class DigitalRead :  public AbstructFunction {
public:
	DigitalRead(char *name) : AbstructFunction(name){}

	Value *evaluate(); 
};
//-------------------------------------------------------------------------------------------------
/*class Buzzer :  public AbstructFunction {
public:
	Buzzer(char *name) : AbstructFunction(name){}

	Value *evaluate(); 
};

class StopBuzzer :  public AbstructFunction {
public:
	StopBuzzer(char *name) : AbstructFunction(name){}

	Value *evaluate(); 
};*/

/*
仕様変更により削除 2024/10/14 aikawa
class LedcSetup :  public AbstructFunction {
public:
	LedcSetup(char *name) : AbstructFunction(name){}

	Value *evaluate(); 
};
*/

class LedcAttach :  public AbstructFunction {
public:
	LedcAttach(char *name) : AbstructFunction(name){}

	int getParamCount(){return 3;} // tatch
	Value *evaluate(); 
};

class LedcDetach :  public AbstructFunction {
public:
	LedcDetach(char *name) : AbstructFunction(name){}

	Value *evaluate(); 
};

class LedcWrite :  public AbstructFunction {
public:
	LedcWrite(char *name) : AbstructFunction(name){}

	int getParamCount(){return 2;} // tatch
	Value *evaluate(); 
};

class LedcWriteTone :  public AbstructFunction {
public:
	LedcWriteTone(char *name) : AbstructFunction(name){}

	Value *evaluate(); 
};

class LedcRead :  public AbstructFunction {
public:
	LedcRead(char *name) : AbstructFunction(name){}

	Value *evaluate(); 
};

class LedcReadFreq :  public AbstructFunction {
public:
	LedcReadFreq(char *name) : AbstructFunction(name){}

	Value *evaluate(); 
};
//-------------------------------------------------------------------------------------------------
// オブジェクト生成クラス

class AbstructObject : public Value{
public:
	void	add(Value *val) {}
	void	sub(Value *val) {}
	void	mul(Value *val) {}
	void	div(Value *val) {}
	void	mod(Value *val) {}
	void	power(Value *val) {}
	void	increment(bool ch) {}
	void	decrement(bool ch) {}
	void	minus() {}
	void	printTree(int s);
	// Implement next method:
	// getInt(), getFloat(), getText(), getBool(), getType()
  Value *getElement(int index = 0){
    return new Integer(1);//tatch 俺がここで食い止める！！
  }
	// evaluate(), print()
};

class ObjNode {
public:
	Value	*_obj;
	ObjNode	*_next;

	ObjNode(Value *obj, ObjNode *next){
		_obj = obj;
		_next = next;
	}
};

class ObjList {
public:
	ObjNode	*_top;

	ObjList(){
		_top = NULL;
	}

	virtual ~ObjList(){
		clearNodes();
	}

	void	 clearNodes();
	ObjNode	*search(Value *obj);
	void	 add(Value *obj);
	void	 remove(Value *obj);
};

class ButtonList : public ObjList {
public:
	void scan();
};

class TimerList : public ObjList {
public:
	void dispatch();
};

class MatrixList : public ObjList { // tatch
  public:
  void display();
};
///////////////////////////////////////////tatch
class PWM : public AbstructObject {
private:
	uint8_t		_pin;
	uint8_t		_bits;
  uint32_t   _range;
public:
	PWM(int pin, int bits = 8){
		_pin = pin;
    _bits = bits;
    uint32_t n = 1;
		for(int i=0; i<bits; i++){
      n = n * 2;
    }
    _range = n - 1;
	}
	long getInt(){
		return _pin;
	}

	double getFloat(){
		return (double)_pin;
	}

	bool getBool(){
		return _pin != 0;
	}
	
	char	*getText() {
		return "PWM";
	}
	char	*getType() {
		return "typePWM";
	}
	Value	*evaluate() {
		return new PWM(_pin, _bits);
	}
	void	 print() {
		printf("print->{PWM(%d) bits = %d}\n", _pin, _bits);
	}

	Value	*methodCall(char *name, List *params);
};

class PWMGen : public AbstructFunction {
public:
	PWMGen(char *name) : AbstructFunction(name){}

	int getParamCount(){return 1;}

	Value *evaluate();
};

class DotListNode { //未使用
public:
  int   _data[8][8];
  DotListNode *_next;
public:
  DotListNode(Value *data, DotListNode *next){
    for(int i=0; i<8; i++){
      for(int j=0; j<8; j++){
        _data[i][j] = data->getElement(i)->getElement(j)->getInt();
      }
    }    
    _next = next;
  }

  void print(){
    for(int i=0; i<8; i++){
      for(int j=0; j<8; j++){
        int d = _data[i][j];
        printf("%d ",d);
      }
      printf("\n");
    }
  }
};
class DotList { //未使用
private:
  DotListNode *_top;
  DotListNode *_last;
public:
  DotList(){
    _top = _last = NULL;
  }
  void setNode(Value *data){
    if(_top == NULL){
      _top =_last = new DotListNode(data, NULL);
    }else{
      _last->_next = new DotListNode(data, NULL);
      _last = _last->_next;
    }
  }
  DotListNode *getNode(int n){
    DotListNode *node;
    int i=0;
    for(node = _top; node; node = node->_next){
      if(i == n){
        return node;
      }
      i++;
    }
    return NULL;
  }
};
class DotMatrix : public AbstructObject {
private:
	const uint8_t	dotSize = 8;
	uint8_t		_rows[8];
	uint8_t		_cols[8];
  uint8_t   _foward;
	DotList   _datalist;
  uint8_t   _speed;
  bool      _stop;

	char	_msg[MAX_MSG];
	uint8_t	_next_char;
	uint8_t	_next_index;
	uint8_t	_dotBuff[DOT_SIZE];
	uint8_t	_next_col;
  uint8_t _count;
  bool    _isRunning;
  uint8_t _state;
  uint8_t _stateB;

	void	nextLetter();
	void	lightRows(uint8_t row);
	void	lightLED();

public:
	DotMatrix(uint8_t *rows, uint8_t *cols);
  void  display();
	void	setText(char *msg);
	void	scroll();
  void  off();

	void	shiftLeft();

	long getInt(){
		return 8;
	}

	double getFloat(){
		return (double)8;
	}

	bool getBool(){
		return 8 != 0;
	}
	
	char	*getText() {
		return "textDotMatrix";
	}
	char	*getType() {
		return "typeDotMatrix";
	}
	Value	*evaluate() {
		return new DotMatrix(_rows,_cols);
	}
	void	 print() {
		printf("printDotMatrix");
	}

	Value	*methodCall(char *name, List *params);
};
class DotMatrixGen : public AbstructFunction {
private:
  const uint8_t dotSize = 8;
public:
	DotMatrixGen(char *name) : AbstructFunction(name){}

	int getParamCount(){return 2;}

	Value *evaluate();
};

class AQM1602obj : public AbstructObject {
private:
  uint8_t   _address;
  char      _buff[16];

	void writeCommand(byte command);
	void writeData(byte data);
public:
  AQM1602obj(uint8_t address){
    _address = address;
  }
	void begin();
	void clear();
	void cursor(int x, int y);
	void print(char *s);
	void print(int n);
	void putChar(char c);
	void setContrast(int c);

	long getInt(){
		return 0;
	}
	double getFloat(){
		return (double)0;
	}
	bool getBool(){
		return 0;
	}
	char	*getText() {
		return "textAQM1602";
	}
	char	*getType() {
		return "typeAQM1602";
	}
	Value	*evaluate() {
		return new AQM1602obj(_address);
	}
	void	 print() {
		printf("printAQM1602");
	}

	Value	*methodCall(char *name, List *params);

};
class AQM1602Gen : public AbstructFunction {
private:
public:
  AQM1602Gen(char *name) : AbstructFunction(name){}

  int getParamCount(){ return 1; }

  Value *evaluate();
};
class DHT20 : public AbstructObject {
private:
  uint8_t _sda, _scl;
  float	_humid;		// Humidity
	float	_temp;		// Temperature
	int		_status;	// ステータス  0:Not Yet  1:Success  2:Fail
public:  
	DHT20(int sda, int scl){
    _sda = sda;
    _scl = scl;
	  _status = 0;
    Wire.setPins(sda,scl);
	}
	char	*getType(){
		return "DHT20";
	}

	long getInt(){
		return 0;
	}

	double getFloat(){
		return 0.0;
	}

	bool getBool(){
		return false;
	}
	
	char	*getText(){
		return NULL;
	}

	void	begin();
	int		scan();
	float	getHumid();
	float	getTemper();
	int		getStatus();
  Value	*evaluate();
	void	 print();
	Value	*methodCall(char *name, List *params);

};

class DHT20Gen : public AbstructFunction {
public:
	DHT20Gen(char *name) : AbstructFunction(name){}

  int getParamCount(){
    return 2;
  }

	Value *evaluate();
};


////////////////////////////////////////////////////

class Output : public AbstructObject{
private:
	uint8_t			_pin;
	uint8_t			_state;

public:
	Output(uint8_t pin, uint8_t state = 0){
		_pin = pin;
		_state = state;
		pinMode(_pin, OUTPUT);
		digitalWrite(_pin, _state);
	}

	long getInt(){
		return _state;
	}

	double getFloat(){
		return (double)_state;
	}

	bool getBool(){
		return _state != 0;
	}
	
	char	*getText();
	char	*getType();
	Value	*evaluate();
	void	 print();
	Value	*methodCall(char *name, List *params);

	void high(){
		_state = 1;
		digitalWrite(_pin, _state);
	}

	void low(){
		_state = 0;
		digitalWrite(_pin, _state);
	}

	void toggle(){
		_state = 1 - _state;
		digitalWrite(_pin, _state);
	}

	uint8_t status(){
		return _state;
	}
};

class OutputGen : public AbstructFunction {
public:
	OutputGen(char *name) : AbstructFunction(name){}

	Value *evaluate();
};

class Button : public AbstructObject{
private:
	uint8_t			_pin;
	FuncName		*_fOnPress;
	FuncName		*_fOnRelease;
	uint8_t			_state;

public:
	Button(uint8_t	pin);
	~Button();
	void	onPress(char *fname);
	void	onRelease(char *fname);
	void	scan();
	void	clear();

	long getInt(){
		return _state;
	}

	double getFloat(){
		return (double)_state;
	}

	bool getBool(){
		return _state != 0;
	}
	
	char	*getText();
	char	*getType();
	Value	*evaluate();
	void	 print();
	Value	*methodCall(char *name, List *params);
};

class ButtonGen : public AbstructFunction {
public:
	ButtonGen(char *name) : AbstructFunction(name){}

	Value *evaluate();
};

class Timer : public AbstructObject{
private:
	FuncName		*_func;
	unsigned long	_lastMils;
	unsigned long	_interval;
	unsigned long	_count;
	bool			_isRunning;

public:
	Timer();
	Timer(unsigned long interval, unsigned long count);
	~Timer(){
		delete _func;
	}
	void	dispatch();
	bool	isRunning();
	
	long getInt(){
		return 0;
	}
	double getFloat(){
		return 0.0;
	}
	bool getBool(){
		return false;
	}
	
	char	*getText();
	char	*getType();
	Value	*evaluate();
	void	 print();
	Value	*methodCall(char *name, List *params);
};

class TimerGen : public AbstructFunction {
public:
	TimerGen(char *name) : AbstructFunction(name){}

  int getParamCount(){return 0;}

	Value *evaluate();
};

class AM2320Obj : public AbstructObject{
private:
	Adafruit_AM2320	_am2320;
  int _sda;
  int _scl;
public:
	AM2320Obj(int sda, int scl){
		_am2320 = Adafruit_AM2320();
    Wire.setPins(sda,scl);
    _sda = sda;
    _scl = scl;

	}

	void begin();

	char	*getType(){
		return "AM2320";
	}

	long getInt(){
		return 0;
	}

	double getFloat(){
		return 0.0;
	}

	bool getBool(){
		return false;
	}
	
	char	*getText(){
		return NULL;
	}
	
	Value	*evaluate();

	void	 print();
	Value	*methodCall(char *name, List *params);
};

class AM2320Gen : public AbstructFunction {
public:
	AM2320Gen(char *name) : AbstructFunction(name){}

  int getParamCount(){
    return 2;
  }

	Value *evaluate();
};

class Speaker : public AbstructObject{
private:
	uint8_t		_pin;
//	uint8_t		_channel;

public:
	Speaker(uint8_t pin);

	char	*getType(){
		return "Speaker";
	}

	long getInt(){
		return _pin;
	}

	double getFloat(){
		return (float)_pin;
	}

	bool getBool(){
		return false;
	}
	
	char	*getText(){
		return NULL;
	}
	
	Value	*evaluate();

	void	 print();
	Value	*methodCall(char *name, List *params);
};

class SpeakerGen : public AbstructFunction {
public:
	SpeakerGen(char *name) : AbstructFunction(name){}

	Value *evaluate();
};

#endif
