#include "WiFiType.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
Begin license text.

Copyright 2023- MicomScript Project

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the $FFFDgSoftware$FFFDh), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED $FFFDgAS IS$FFFDh, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

End license text.
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiUDP.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>
#include "common.h"
#include "lock.h"
#include "expression.h"
#include "logic.h"
#include "varmap.h"
#include "sentence.h"
#include "function.h"
#include "function_m.h"
#include "resource.h"
// 追加
#include "bluetooth.h"
//

WiFiRes		          _wifi;
AmbientRes          _ambient;
// 追加
Bluetooth           _btDev;               // Bluetoothデバイス
BluetoothRes        _bluetooth(&_btDev);  // Bluetoothリソースで使用するデバイスの登録
//
WiFiClient          c;
extern FunctionMap  _funcMap;
int g_port;
WebServer  _server(80);
PageList pageList;
char* _fncname;
char* N_fncname;
int _state;

#define AMBIENT_DEBUG 0

#if AMBIENT_DEBUG
#define DBG(...) { Serial.print(__VA_ARGS__); }
#define ERR(...) { Serial.print(__VA_ARGS__); }
#else
#define DBG(...)
#define ERR(...)
#endif /* AMBIENT_DBG */

const char* AMBIENT_HOST = "54.65.206.59";//アンビエントのサーバー側のIPアドレス
//const char* AMBIENT_HOST = "ambidata.io";	//(new)
int AMBIENT_PORT = 80;
const char* AMBIENT_HOST_DEV = "192.168.11.2";//アンビエントサーバー側開発者のIPアドレス
int AMBIENT_PORT_DEV = 4567;

const char* ambient_keys[] = { "\"d1\":\"", "\"d2\":\"", "\"d3\":\"", "\"d4\":\"", "\"d5\":\"", "\"d6\":\"", "\"d7\":\"", "\"d8\":\"", "\"lat\":\"", "\"lng\":\"", "\"created\":\"" };



void getfunction() {
	String uri = _server.uri();
	char* fname = pageList.getName(uri);
	Function* f = _funcMap.getFunction(fname);
	if (f == NULL) {
		Serial.println("Eroor : function not found");
	}
	else {
		f->evaluate();
		printf("excuted %s\n", fname);
	}
}


void N_getfunction() {
	if (N_fncname) {
		Function* n_f = _funcMap.getFunction(N_fncname);
		if (n_f == NULL) {
			Serial.println("Eroor : function not found");
		}
		else {
			n_f->evaluate();
			printf("excuted %s\n", N_fncname);
		}
	}
}

// ---------------------------------------------------------
// Resource用メソッドコール

ResMethodCall::ResMethodCall(int res_id, char* mname) {
	_resID = res_id;
	_methodName = new char[strlen(mname) + 1];
	strcpy(_methodName, mname);
}

ResMethodCall::~ResMethodCall() {
#ifdef DEBUG
	printf("delete ResMethodCall...%d.%s()\n", _resID, _methodName);
#endif
	delete[] _methodName;
	_eList.clear();
}

void ResMethodCall::addParam(Expression* e) {
	_eList.append(e);
}

void ResMethodCall::print() {
	if (_resID == R_WiFi) {
		printf("WiFi.");
	}
	else if (_resID == R_Ambient) {
		printf("Ambient.");
	}
	else if (_resID == R_Bluetooth) {
		printf("Bluetooth.");
	}
	else {
		printf("???.");
	}
	printf("%s(", _methodName);
	_eList.print();
	printf(")");
}

void ResMethodCall::printTree(int depth) {
	printSpace(depth);
	print();
	printf(")\n");
}

Value* ResMethodCall::evaluate() {
	Resource* res;

	if (_resID == R_WiFi) {
		res = &_wifi;
	}
	else if (_resID == R_Ambient) {
		res = &_ambient;
	}
	else if (_resID == R_Bluetooth) {
		res = &_bluetooth;
	}
	else {
		printf("Unknown Resource\n");
		return new Integer(0);
	}

	// 引数式→Value
	List* params = new List();
	Expression* e;
	for (e = _eList.top(); e; e = _eList.next()) {
		params->append(e->evaluate());
	}

	Value* v = res->methodCall(_methodName, params);
	delete params;

	if (v)
		return v;
	return new Integer(0);
}

// WiFiRess:: ----------------------------------------------

void WiFiRes::print() {
	printf("WiFi\n");
	printValue("status", status());
	printValue("local IP address", localIP());
	printValue("subnet mask", subnetMask());
	printValue("gateway IP address", gatewayIP());
	printValue("DNS server", dnsIP());
}

void WiFiRes::printTree() {
	printf("WiFi\n");
}

void WiFiRes::printValue(char* s, Value* value) {
	if (value) {
		printf("  %s: ", s);
		value->print();
		printf("\n");
		delete value;
	}
}

Value* WiFiRes::methodCall(char* name, List* params) {
	//	printf("Wifi.%s()\n", name);
	if (strcmp(name, "begin") == 0) {
		return begin(params);
	}
	else if (strcmp(name, "status") == 0) {
		return status();
	}
	else if (strcmp(name, "localIP") == 0) {
		return localIP();
	}
	else if (strcmp(name, "dnsIP") == 0) {
		return dnsIP();
	}
	else if (strcmp(name, "gatewayIP") == 0) {
		return gatewayIP();
	}
	else if (strcmp(name, "subnetMask") == 0) {
		return subnetMask();
	}
	else if (strcmp(name, "macAddress") == 0) {
		return macAddress();
	}
	else if (strcmp(name, "disconnect") == 0) {
		return disconnect();
	}
	else if (strcmp(name, "UDP") == 0) {
		return new WiFi_UDP();
	}
	else if (strcmp(name, "print") == 0) {
		print();
		printf("\n");
		return NULL;
	}
	printf("Unknown method name : %s\n", name);
	return NULL;
}

Value* WiFiRes::begin(List* params) {
	int nParams = params->size();
	if (nParams < 2) {
		printf("Parameter Error\n");
		return new Integer(0);
	}
	char* ssid = params->getElement(0)->getText();
	char* pass = params->getElement(1)->getText();
	int state = WiFi.begin(ssid, pass);
	return new Integer(state);
}

Value* WiFiRes::status() {
	int state = WiFi.status();
	if (state == WL_NO_SHIELD) {
		return new Text("NO_SHIELD");
	}
	else if (state == WL_IDLE_STATUS) {
		return new Text("IDLE_STATUS");
	}
	else if (state == WL_NO_SSID_AVAIL) {
		return new Text("NO_SSID_AVAIL");
	}
	else if (state == WL_SCAN_COMPLETED) {
		return new Text("SCAN_COMPLETED");
	}
	else if (state == WL_CONNECTED) {
		return new Text("CONNECTED");
	}
	else if (state == WL_CONNECT_FAILED) {
		return new Text("CONNECT_FAILED");
	}
	else if (state == WL_CONNECTION_LOST) {
		return new Text("CONNECTION_LOST");
	}
	else if (state == WL_DISCONNECTED) {
		return new Text("DISCONNECTED");
	}
	return new Text("UNKNOWN");
}

Value* WiFiRes::localIP() {
	return toText(WiFi.localIP().toString());
}

Value* WiFiRes::subnetMask() {
	return toText(WiFi.subnetMask().toString());
}

Value* WiFiRes::gatewayIP() {
	return toText(WiFi.gatewayIP().toString());
}

Value* WiFiRes::dnsIP() {
	return toText(WiFi.dnsIP().toString());
}

Value* WiFiRes::macAddress() {
	byte mac[6];
	WiFi.macAddress(mac);
	char buff[20];
	sprintf(buff, "%2X:%2X:%2X:%2X:%2X:%2X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	return new Text(buff);
}

Value* WiFiRes::disconnect() {
	bool b = WiFi.disconnect();
	return new Integer(b ? 1 : 0);
}

Value* WiFiRes::toText(String s) {
	int len = s.length() + 1;
	char* t = new char[len];
	s.toCharArray(t, len);
	Value* value = new Text(t);
	delete t;
	return value;
}

Value* WiFi_UDP::methodCall(char* name, List* params) {
	//	printf("Wifi_UDP.%s()\n", name);
	if (strcmp(name, "begin") == 0) {
		return begin(params);
	}
	else if (strcmp(name, "send") == 0) {
		return send(params);
	}
	else if (strcmp(name, "available") == 0) {
		return available(params);
	}
	else if (strcmp(name, "read") == 0) {
		return read(params);
	}
	else if (strcmp(name, "onReceive") == 0) {
		return onReceive(params);
	}
	else if (strcmp(name, "remoteIP") == 0) {
		return remote_ip();
	}
	else if (strcmp(name, "remotePort") == 0) {
		return remote_port();
	}
	else if (strcmp(name, "print") == 0) {
		print();
		printf("\n");
		return NULL;
	}
	printf("WiFiUDP:: Unknown method name : %s\n", name);
	return NULL;
}

void WiFi_UDP::print() {
	printf("WiFi_UDP\n");
	printf("  local port = %d\n", _localPort);
	if (_func) {
		printf("  onReceive --> ");
		_func->print();
		printf("\n");
	}
}

Value* WiFi_UDP::begin(List* params) {
	int nParams = params->size();
	if (nParams == 0) {
		printf("Parameter Error\n");
		return NULL;
	}
	_localPort = params->getElement(0)->getInt();
	uint8_t v = _udp.begin(_localPort);
	return new Integer(v);
}

Value* WiFi_UDP::send(List* params) {
	int nParams = params->size();
	if (nParams < 3) {
		printf("Parameter Error\n");
		return NULL;
	}
	char* ipaddress = params->getElement(0)->getText();
	int port = params->getElement(1)->getInt();
	char* msg = params->getElement(2)->getText();
	_udp.beginPacket(ipaddress, port);
	_udp.print(msg);
	_udp.endPacket();
	return NULL;
}

Value* WiFi_UDP::available(List* params) {
	int n = _udp.parsePacket();
	return new Integer(n);
}

Value* WiFi_UDP::read(List* params) {
	char buff[256];
	memset(buff, 0, 256);
	_udp.read(buff, 256);
	return new Text(buff);
}

Value* WiFi_UDP::toText(String s) {
	int len = s.length() + 1;
	char* t = new char[len];
	s.toCharArray(t, len);
	Value* value = new Text(t);
	delete t;
	return value;
}

Value* WiFi_UDP::remote_ip() {
	return(toText(_udp.remoteIP().toString()));
}

Value* WiFi_UDP::remote_port() {
	return(new Integer(_udp.remotePort()));
}

void WiFi_UDP::confirmArrival() {
	if (_udp.parsePacket() > 0) {
		if (_func) {
			try {
				Value* v = _func->evaluate();
				delete v;
			}
			catch (ErrorException e) {}
		}
	}
}


// WiFi UDP Observer -------------------------------------------------

class WiFi_UDPList : public ObjList {
public:
	void confirmArrival();
};

WiFi_UDPList		_udpList;
SimpleLocker		_udpList_locker;

void WiFi_UDPList::confirmArrival() {
	ObjNode* node;
	WiFi_UDP* udp;
	for (node = _top; node; node = node->_next) {
		udp = (WiFi_UDP*)node->_obj;
		udp->confirmArrival();
	}
}

void udpObserver(void* args) {
	while (1) {
		// 排他制御
		if (_udpList_locker.lock(100)) {
			_udpList.confirmArrival();
			_udpList_locker.unlock(100);
		}
		delay(1);
	}
}

Value* WiFi_UDP::onReceive(List* params) {
	int nParams = params->size();
	if (nParams == 0) {
		printf("WiFiUDP::onReceive() : Parameter Error\n");
		return NULL;
	}
	char* fname = params->getElement(0)->getText();
	if (fname == NULL) {
		printf("WiFi_UDP::onReceive() : handler name is required.\n");
		return NULL;
	}
	_func = new FuncName(fname);

	// 排他制御
	_udpList_locker.waitForUnlock(200);
	_udpList.add(this);
	_udpList_locker.unlock(200);

	return NULL;
}

//---------------------------------------------------------------------------------


void AmbientRes::print() {
	printf("Ambient\n");
	//printValue("set", set());
	//printValue("", localIP());
	//printValue("subnet mask", subnetMask());
	//printValue("gateway IP address", gatewayIP());
	//printValue("DNS server", dnsIP());

//begin
//set
//clear
//send
//bulk_send
//delete_data
}

void AmbientRes::printTree() {
	printf("Ambient\n");
}

void AmbientRes::printValue(char* s, Value* value) {
	if (value) {
		printf("  %s: ", s);
		value->print();
		printf("\n");
		delete value;
	}
}

Value* AmbientRes::methodCall(char* name, List* params) {
	//	printf("Ambient.%s()\n", name);
	if (strcmp(name, "begin") == 0) {
		return begin(params);
	}
	else if (strcmp(name, "set") == 0) {
		return set(params);
	}
	else if (strcmp(name, "clear") == 0) {
		return clear(params);
	}
	else if (strcmp(name, "setcmnt") == 0) {//(new)
		return setcmnt(params);		 //(new)
	}
	else if (strcmp(name, "send") == 0) {
		return send();
	}
	else if (strcmp(name, "delete_data") == 0) {
		return delete_data(params);
	}
	else if (strcmp(name, "getStatus") == 0) { //(new)
		return getStatus();		    //(new)
	}
	else if (strcmp(name, "print") == 0) {
		print();
		printf("\n");
		return NULL;
	}
	printf("Unknown method name : %s\n", name);
	return NULL;
}

Value* AmbientRes::begin(List* params) {
	int nParams = params->size();
	if (nParams < 3) {
		printf("Parameter Error\n");
		return new Integer(0);
	}
	int channelId = params->getElement(0)->getInt();
	char* writeKey = params->getElement(1)->getText();
	char* readKey = params->getElement(2)->getText();	//(new)
	int state = ambient.begin(channelId, writeKey, &c, readKey);//(new)
	return new Bool(state);//一応Ｂｏｏｌ型で返せるか確認
}

Value* AmbientRes::set(List* params) {
	int nParams = params->size();
	if (nParams < 2) {
		printf("Parameter Error\n");
		return new Integer(0);
	}
	int field = params->getElement(0)->getInt();
	char* data = params->getElement(1)->getText();
	int state = ambient.set(field, data);
	return new Bool(state);//一応Ｂｏｏｌ型で返せるか確認
}

Value* AmbientRes::clear(List* params) {
	int nParams = params->size();
	if (nParams < 1) {
		printf("Parameter Error\n");
		return new Integer(0);
	}
	int field = params->getElement(0)->getInt();
	int state = ambient.clear(field);
	return new Bool(state);//一応Ｂｏｏｌ型で返せるか確認
}

Value* AmbientRes::setcmnt(List* params) {	//(new)
	int nParams = params->size();
	if (nParams < 1) {
		printf("Parameter Error\n");
		return new Integer(0);
	}
	char* data = params->getElement(0)->getText();
	int state = ambient.setcmnt(data);
	return new Bool(state);//一応Ｂｏｏｌ型で返せるか確認
}

Value* AmbientRes::send() {
	int state = ambient.send();
	return new Bool(state);//一応Ｂｏｏｌ型で返せるか確認
}

Value* AmbientRes::delete_data(List* params) {
	int nParams = params->size();
	if (nParams < 1) {
		printf("Parameter Error\n");
		return new Integer(0);
	}
	char* userKey = params->getElement(0)->getText();
	int state = ambient.delete_data(userKey);
	return new Bool(state);//一応Ｂｏｏｌ型で返せるか確認
}

Value* AmbientRes::getStatus() {
	int state = ambient.getStatus();
	return new Bool(state);//一応Ｂｏｏｌ型で返せるか確認
}

// 追加
// Bluetooth Classic
Value* BluetoothRes::methodCall(char* name, List* params) {

	if (strcmp(name, "begin") == 0) {
		if (params->size() < 1)
			return new Text("ERROR: missing device name");
		char* devName = params->getElement(0)->getText();
		bool mode = false;
		if (params->size() >= 2)
			mode = params->getElement(1)->getText();
		if (!_bt)
			return new Text("ERROR: Bleutooth not initialized");
		_bt->begin(devName, mode);
		_state = BT_STARTED;
		return new Text("Hello!");
	}

	if (strcmp(name, "end") == 0) {
		if (_state != BT_STARTED)
			return new Text("ERROR: begin not executed");
		_bt->end();
		_state = BT_ENDED;
		return new Text("See You!");
	}

	if (_state != BT_STARTED) {
		return new Text("ERROR: begin required");
	}

	if (strcmp(name, "getAddress") == 0) {
		String s = _bt->getAddress();
		return new Text((char*)s.c_str());
	}

	if (strcmp(name, "connect") == 0) {
		char* mac = params->getElement(0)->getText();
		bool ok = _bt->connect(mac);
		return new Bool(ok);
	}

	if (strcmp(name, "isConnected") == 0) {
		bool ok = _bt->isConnected();
		return new Bool(ok);
	}

	if (strcmp(name, "write") == 0) {
		if (params->size() < 1)
			return new Text("ERROR: missing message");
		char* msg = params->getElement(0)->getText();
		_bt->write(msg);
		return new Text("OK");
	}

	if (strcmp(name, "read") == 0) {
		String s = _bt->read();
		return new Text((char*)s.c_str());
	}

	if (strcmp(name, "disconnect") == 0) {
		_bt->disconnect();
		return new Text("OK");
	}

	if (strcmp(name, "available") == 0) {
		int n = _bt->available();
		char buf[16];
		sprintf(buf, "%d", n);
		return new Text(buf);
	}


	printf("Unknown Bluetooth method: %s\n", name);
	return new Text("ERROR: unknown command");
}
//
//--------------------------------------------------------------------

int Ambient::begin(int channelId, char* writeKey, WiFiClient* c, const char* readKey) {
	this->channelId = channelId;//thisはクラスオブジェクトのメンバにアクセスする際に使われるポインタ
	if (sizeof(writeKey) > AMBIENT_WRITEKEY_SIZE) {//AMBIENT_WRITEKEY_SIZEは18＿＿変数のサイズを確認
		ERR("writeKey length > AMBIENT_WRITEKEY_SIZE");
		return false;
	}
	strcpy(this->writeKey, writeKey);//クラスオブジェクトの"writeKey"にwriteKeyの値を代入
	if (NULL == c) {//WiFiClientがNULLなら失敗
		Serial.println("no c");
		ERR("Socket Pointer is NULL, open a socket.");
		return false;
	}
	this->client = c;//クラスオブジェクトの"client"にcを代入
	if (readKey != NULL) {			//(new)
		strcpy(this->readKey, readKey);		//(new)
	}
	else {					//(new)
		strcpy(this->readKey, "");		//(new)
	}						//(new)
	//this->dev = dev;//クラスオブジェクトの"dev"にdevを代入
	if (dev) {
		strcpy(this->host, AMBIENT_HOST_DEV);//AMBIENT_HOST_DEVを比較
		this->port = AMBIENT_PORT_DEV;	//メンバ関数のポートにAMBIENT_PORT_DEVを代入
	}
	else {
		strcpy(this->host, AMBIENT_HOST);//AMBIENT_HOSTを比較
		this->port = AMBIENT_PORT;
	}
	for (int i = 0; i < AMBIENT_NUM_PARAMS; i++) {//data[]のフラグにfalse
		this->data[i].set = false;
	}
	this->cmnt.set = false;	//(new)
	this->lastsend = 0;		//(new)
	return true;
}

//--------------------------------------------------------------------------------------------------

int Ambient::set(int field, const char* data) {
	--field;
	if (field < 0 || field >= AMBIENT_NUM_PARAMS) {//なんも入っていなかったらfalse
		return false;
	}
	if (strlen(data) > AMBIENT_DATA_SIZE) {//detaが18より長かったらfalse
		return false;
	}
	this->data[field].set = true;//data[].setの中にtrueが入る
	strcpy(this->data[field].item, data);//deta[].itemの中にデータを入れる
	return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------


int Ambient::clear(int field) {
	--field;
	if (field < 0 || field >= AMBIENT_NUM_PARAMS) {
		return false;
	}
	this->data[field].set = false;//deta[].setをfalse
	this->cmnt.set = false;			//(new)

	return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------

			//(new)
int Ambient::setcmnt(const char* data) {
	if (strlen(data) > AMBIENT_CMNT_SIZE) {
		return false;
	}
	this->cmnt.set = true;
	strcpy(this->cmnt.item, data);

	return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



bool Ambient::connect2host(uint32_t tmout) {	//(new)
	int retry;
	for (retry = 0; retry < AMBIENT_MAX_RETRY; retry++) {
		int ret;
#if defined(ESP8266)
		this->client->setTimeout(tmout);
		ret = this->client->connect(this->host, this->port);
#else
		ret = this->client->connect(this->host, this->port, tmout);
#endif
		if (ret) {
			break;
		}
	}
	if (retry == AMBIENT_MAX_RETRY) {
		ERR("Could not connect socket to host\r\n");
		return false;
	}
	return true;
}

#define GetStatMaxRetry 100

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//(new)
int Ambient::getStatusCode() {
	String _buf;
	int retry = 0;
	while (retry < GetStatMaxRetry) {
		_buf = this->client->readStringUntil('\n');
		if (_buf.length() != 0) {
			break;
		}
		delay(10);
		retry++;
	}
	if (retry != 0) {
		DBG("get stat retry:"); DBG(retry); DBG("\r\n");
		;
	}
	if (retry == GetStatMaxRetry) {
		return 0;
	}
	int from = _buf.indexOf("HTTP/1.1 ") + sizeof("HTTP/1.1 ") - 1;
	int to = _buf.indexOf(' ', from);
	this->status = _buf.substring(from, to).toInt();
	return this->status;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


int Ambient::send() {

	uint32_t tmout = AMBIENT_TIMEOUT;
	char str[180];
	char body[192];
	char inChar;

	if (this->lastsend != 0 && (millis() - this->lastsend) < 4999) {	//(new)
		this->status = 403;												                    //(new)
		return false;													                        //(new)
	}																	                                //(new)
	this->status = 0;													                        //(new)
	if (connect2host(tmout) == false) {									              //(new)
		return false;													                        //(new)
	}																	                                //(new)

  /*  int retry;
	for (retry = 0; retry < AMBIENT_MAX_RETRY; retry++) {//AMBIENT_MAX_RETRYは5＿＿＿＿＿5回までリトライできる
	  int ret;
	  ret = this->client->connect(this->host, this->port);//retにクライアントの情報を入れる
	  if (ret) {//retがあったらbreak
		break ;
	  }
	}
	if(retry == AMBIENT_MAX_RETRY) {//AMBIENT_MAX_RETRYは5_____失敗を表示
	  ERR("Could not connect socket to host\r\n");
	  return false;
	}*/


	memset(body, 0, sizeof(body));//配列の先頭からbodyのバイト数までを0で埋める
	strcat(body, "{\"writeKey\":\"");//配列の連結＿＿＿bodyの後ろに{\"writeKey\":\"をつける
	strcat(body, this->writeKey);//上のやつにクラスオブジェクトのwriteKeyを連結する
	strcat(body, "\",");//上にやつに’",’をつける

	for (int i = 0; i < AMBIENT_NUM_PARAMS; i++) {//フィールドが11個あってそれぞれに値(データ)を入れている？
		if (this->data[i].set) {
			strcat(body, ambient_keys[i]);
			strcat(body, this->data[i].item);
			strcat(body, "\",");
		}
	}
	if (this->cmnt.set) {		//(new)
		strcat(body, "\"cmnt\":\"");	//(new)
		strcat(body, this->cmnt.item);	//(new)
		strcat(body, "\",");		//(new)
	}					//(new)
	body[strlen(body) - 1] = '\0';
	strcat(body, "}\r\n");
	memset(str, 0, sizeof(str));//配列の先頭からstrのバイト数までを0で埋める
	sprintf(str, "POST /api/v2/channels/%u/data HTTP/1.1\r\n", this->channelId);
	if (this->port == 80) {
		sprintf(&str[strlen(str)], "Host: %s\r\n", this->host);
	}
	else {
		sprintf(&str[strlen(str)], "Host: %s:%d\r\n", this->host, this->port);
	}
	sprintf(&str[strlen(str)], "Content-Length: %d\r\n", strlen(body));
	sprintf(&str[strlen(str)], "Content-Type: application/json\r\n\r\n");

	DBG("sending: "); DBG(strlen(str)); DBG("bytes\r\n"); DBG(str);

	int ret;
	ret = this->client->print(str);
	delay(30);
	DBG(ret); DBG(" bytes sent\n\n");
	if (ret == 0) {
		ERR("send failed\n");
		return false;
	}
	ret = this->client->print(body);
	delay(30);
	DBG(ret); DBG(" bytes sent\n\n");
	if (ret == 0) {
		ERR("send failed\n");
		return false;
	}

	getStatusCode();		//(new)
	while (this->client->available()) {
		inChar = this->client->read();
#if AMBIENT_DEBUG
		Serial.write(inChar);
#else				//(new)
		(void)inChar;		//(new)
#endif
	}

	this->client->stop();

	for (int i = 0; i < AMBIENT_NUM_PARAMS; i++) {
		this->data[i].set = false;
	}
	this->cmnt.set = false;	//(new)
	this->lastsend = millis();	//(new)


	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------

int Ambient::delete_data(char* userKey) {
	uint32_t tmout = AMBIENT_TIMEOUT;
	char str[180];
	char inChar;


	this->status = 0;			//(new)
	if (connect2host(tmout) == false) {	//(new)
		return false;			//(new)
	}					//(new)

  /*  int retry;
	for (retry = 0; retry < AMBIENT_MAX_RETRY; retry++) {
	  int ret;
	  ret = this->client->connect(this->host, this->port);
	  if (ret) {
		break ;
	  }
	}
	if(retry == AMBIENT_MAX_RETRY) {
	  ERR("Could not connect socket to host\r\n");
	  return false;
	}*/


	memset(str, 0, sizeof(str));
	sprintf(str, "DELETE /api/v2/channels/%u/data?userKey=%s HTTP/1.1\r\n", this->channelId, userKey);//(new)
	if (this->port == 80) {
		sprintf(&str[strlen(str)], "Host: %s\r\n", this->host);
	}
	else {
		sprintf(&str[strlen(str)], "Host: %s:%d\r\n", this->host, this->port);
	}
	sprintf(&str[strlen(str)], "Content-Length: 0\r\n");
	sprintf(&str[strlen(str)], "Content-Type: application/json\r\n\r\n");
	DBG(str);

	int ret;
	ret = this->client->print(str);
	delay(30);
	DBG(ret); DBG(" bytes sent\r\n");
	if (ret == 0) {
		ERR("send failed\r\n");
		return false;
	}

	getStatusCode();		//(new)
	while (this->client->available()) {
		inChar = this->client->read();
#if AMBIENT_DEBUG
		Serial.write(inChar);
#else				//(new)
		(void)inChar;		//(new)
#endif
	}

	this->client->stop();

	for (int i = 0; i < AMBIENT_NUM_PARAMS; i++) {
		this->data[i].set = false;
	}
	this->cmnt.set = false;	//(new)


	return true;
}


//--------------------------------------------------------------------------------------------------

int Ambient::getStatus() {
	Serial.printf("port %d\n", this->port);
	Serial.printf("host %s\n", this->host);
	Serial.printf("channelId %d\n", channelId);
	Serial.printf("writeKey %d\n", writeKey);
	Serial.printf("readKey %d\n", readKey);
	//esp_read_mac(mac, ESP_MAC_WIFI_STA);  // Wi-FiのMACアドレスを取得する
	//sprintf(_devKey, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	//Serial.printf("devKey %d\n",_devKey);

	return true;
}

//--------------------------------------------------
void PageList::clearNodes() {
	pageNode* node, * next;
	for (node = _top; node; node = next) {
		next = node->_next;
		delete node;
	}
}

pageNode* PageList::search(String uri) {
	pageNode* node;
	for (node = _top; node; node = node->_next) {
		if (node->_uri == uri)
			return node;
	}
	return NULL;
}

char* PageList::getName(String uri) {
	pageNode* node;
	for (node = _top; node; node = node->_next) {
		if (node->_uri == uri)
			return node->_fname;
	}
	return NULL;
}

void PageList::add(String uri, char* fname) {
	pageNode* node = search(uri);
	if (node == NULL) {
		_top = new pageNode(uri, fname, _top);
	}
	else {
		node->_fname = fname;
	}
}

void PageList::remove(String uri) {
	pageNode* node, * next, * prev;
	prev = NULL;
	for (node = _top; node; node = next) {
		next = node->_next;
		if (node->_uri == uri)
			break;
		prev = node;
	}
	if (node) {
		if (node == _top) {	// or if (prev == NULL)
			_top = _top->_next;
		}
		else {
			prev->_next = node->_next;
		}
		delete node;
	}
}



//--------------------------------------------------

TaskHandle_t thp[1];


void WebServerObj::print() {
	printf("Webserver");
}

WebServerObj::~WebServerObj() {
	free(_fncname);
	free(N_fncname);
}

void Core0(void* args) {
	while (1) {
		delay(1);
		if (_state)
			_server.handleClient();
	}
}


Value* WebServerObj::methodCall(char* name, List* params) {
#ifdef DEBUG
	printf("WebServerObj::methodCall(%s)\n", name);
#endif
	if (strcmp(name, "start") == 0) {
		_state = 1;
		_server.begin();

		if (a == 0) {
			xTaskCreatePinnedToCore(Core0, "Core0", 4096, NULL, 3, &thp[0], 0);
			a = 1;
		}
		int status = 1;
		return new Integer(status);
	}
	else if (strcmp(name, "on") == 0) {
		int nParams = params->size();
		if (nParams < 2) {
			printf("Parameter Error\n");
			return NULL;
		}
		URI = params->getElement(0)->getText();
		char* fncname = params->getElement(1)->getText();
		_fncname = (char*)malloc(strlen(fncname) + 1);
		strcpy(_fncname, fncname);
		pageList.add(URI, _fncname);
		int status = 1;
		_server.on(URI, getfunction);
		return new Integer(status);
	}
	else if (strcmp(name, "send") == 0) {
		int nParams = params->size();
		if (nParams < 3) {
			printf("Parameter Error\n");
			return NULL;
		}
		HTTPcode = params->getElement(0)->getInt();
		content_type = params->getElement(1)->getText();
		content = params->getElement(2)->getText();
		int status = 1;
		_server.send(HTTPcode, content_type, content);
		return new Integer(status);
	}
	else if (strcmp(name, "stop") == 0) {
		_state = 0;
		return new Float(1);
	}
	else if (strcmp(name, "onNotFound") == 0) {
		int nParams = params->size();
		if (nParams < 1) {
			printf("Parameter Error\n");
			return NULL;
		}
		char* n_fncname = params->getElement(0)->getText();
		N_fncname = (char*)malloc(strlen(n_fncname) + 1);
		strcpy(N_fncname, n_fncname);
		int status = 1;
		_server.onNotFound(N_getfunction);
		return new Integer(status);
	}
	else if (strcmp(name, "arg") == 0) {
		int nParams = params->size();
		if (nParams < 1) {
			printf("Parameter Error\n");
			return NULL;
		}
		char* c = params->getElement(0)->getText();
		char change = c[0];
		if (isDigit(change) == true) {
			int paraNum = params->getElement(0)->getInt();
			String status = _server.arg(paraNum);
			printf("paraNum = %s\n", status);
			char charArray[status.length() + 1];// 長さに1を加えて、ヌル終端文字分のスペースを確保
			status.toCharArray(charArray, status.length() + 1);
			return new Text(charArray);
		}
		else {
			char* p = params->getElement(0)->getText();
			String paraName = String(p);
			String status = _server.arg(paraName);
			char charArray[status.length() + 1];// 長さに1を加えて、ヌル終端文字分のスペースを確保
			status.toCharArray(charArray, status.length() + 1);
			return new Text(charArray);
		}
	}
	else if (strcmp(name, "hasArg") == 0) {
		int nParams = params->size();
		if (nParams < 1) {
			printf("Parameter Error\n");
			return NULL;
		}
		char* c = params->getElement(0)->getText();
		char change = c[0];
		bool status;
		if (isDigit(change) == true) {
			status = false;
			printf("Please enter a string");
		}
		else {
			char* p = params->getElement(0)->getText();
			String paraName = String(p);
			status = _server.hasArg(paraName);
		}
		return new Bool(status);
	}
	else if (strcmp(name, "argName") == 0) {
		int nParams = params->size();
		if (nParams < 1) {
			printf("Parameter Error\n");
			return NULL;
		}
		char* c = params->getElement(0)->getText();
		char change = c[0];
		String status;
		if (isDigit(change) == true) {
			int paraNum = params->getElement(0)->getInt();
			status = _server.argName(paraNum);
		}
		else {
			status = "0";
			printf("Please enter the parameter number\n");
		}
		char charArray[status.length() + 1];// 長さに1を加えて、ヌル終端文字分のスペースを確保
		status.toCharArray(charArray, status.length() + 1);
		return new Text(charArray);
	}
	else if (strcmp(name, "args") == 0) {
		int nParams = params->size();
		if (nParams < 0) {
			printf("Parameter Error\n");
			return NULL;
		}
		int status = 0;
		status = _server.args();
		return new Integer(status);
	}
	else {
		printf("Undefined function: %s\n", name);
	}
	return NULL;
}

Value* WebServerObj::evaluate() {
	return NULL;
}

Value* WebServerGen::evaluate() {
	int port = _list->getElement(0)->getInt();

	WebServerObj* _Web = new WebServerObj();
	return _Web;
}

