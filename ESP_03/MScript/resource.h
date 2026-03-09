// リソース -------------------------------------------------------

#ifndef __RESOURCE_H__
#define __RESOURCE_H__

//#include <WiFi.h>
//#include <WiFiUDP.h>



//#include <WiFiClient.h>

#define AMBIENT_WRITEKEY_SIZE 18
#define AMBIENT_READKEY_SIZE 18		//(new)
#define AMBIENT_MAX_RETRY 5
#define AMBIENT_DATA_SIZE 24
#define AMBIENT_NUM_PARAMS 11
#define AMBIENT_CMNT_SIZE 64		//(new)
#define AMBIENT_TIMEOUT 30000UL		//(new)

#define AM_BLACK 1			//(new)
#define AM_WHITE 8			//(new)
#define AM_RED 9			//(new)
#define AM_ORANGE 10			//(new)
#define AM_YELLOW 11			//(new)
#define AM_GREEN 12			//(new)
#define AM_CYAN 13			//(new)
#define AM_BLUE 14			//(new)
#define AM_PURPLE 15			//(new)
#define AM_PINK 16			//(new)

class Resource {
private:

public:
	virtual void	 print() = 0;
	virtual void	 printTree() = 0;
	virtual Value* methodCall(char* name, List* params) = 0;
};

class WiFiRes : public Resource {
private:

public:
	WiFiRes() {
	}

	void	 print();
	void	 printTree();
	void	 printValue(char* s, Value* value);
	Value* methodCall(char* name, List* params);

	Value* begin(List* params);
	Value* status();
	Value* localIP();
	Value* macAddress();
	Value* subnetMask();
	Value* gatewayIP();
	Value* dnsIP();
	Value* disconnect();
	Value* toText(String s);
};

class Bluetooth;
class BluetoothRes : public Resource {
private:
	enum BTState {
		BT_NOT_STARTED,
		BT_STARTED,
		BT_ENDED
	};
	BTState _state = BT_NOT_STARTED;
	Bluetooth* _bt;

public:
	BluetoothRes(Bluetooth* bt) : _bt(bt) {}
	virtual void print() {
		printf("Bluetooth\n");
	}
	virtual void printTree() {
		printf("Bluetooth\n");
	}
	void run();
	virtual Value* methodCall(char* name, List* params);
	String readUntil();
};

class WiFi_UDP : public AbstructObject {
private:
	uint16_t	 _localPort;
	WiFiUDP		 _udp;
	FuncName* _func;

public:
	WiFi_UDP() {
		_localPort = 0;
		_func = NULL;
	}

	~WiFi_UDP() {
		delete _func;
	}

	long getInt() {
		return _localPort;
	}

	double getFloat() {
		return (double)_localPort;
	}

	bool getBool() {
		return _localPort != 0;
	}

	char* getText() {
		return getType();
	}

	char* getType() {
		return "WiFiUDP";
	}

	Value* evaluate() {
		return new WiFi_UDP();
	}

	void print();

	void printTree(int s) {
		printSpace(s);
		print();
		printf("\n");
	}

	void	 confirmArrival();

	Value* methodCall(char* name, List* params);

	Value* begin(List* params);
	Value* send(List* params);
	Value* available(List* params);
	Value* read(List* params);
	Value* onReceive(List* params);
	Value* remote_ip();
	Value* remote_port();
	Value* toText(String s);
};

//-------------------------------------------------------------


//------------------------------------------------------------------
class Ambient {
private:

	WiFiClient* client;
	unsigned int channelId;
	char writeKey[AMBIENT_WRITEKEY_SIZE];
	char readKey[AMBIENT_READKEY_SIZE];		//(new)
	int dev = 0;
	char host[18];
	int port;
	unsigned long lastsend;
	//uint8_t mac[6];
	//uint32_t tmout = AMBIENT_TIMEOUT;//なんとなく			//(new)


	struct {//structはclassみたいなの。structはデフォルトのアクセシビリティが public
		int set;//フラグみたいなもの
		char item[AMBIENT_DATA_SIZE];//AMBIENT_DATA_SIZEは24
	} data[AMBIENT_NUM_PARAMS];
	struct {						//(new)
		int set;					//(new)
		char item[AMBIENT_CMNT_SIZE];			//(new)
	} cmnt;						//(new)
	//(new)
//bool connect2host(uint32_t tmout = AMBIENT_TIMEOUT);//(new)
//int getStatusCode(void);				//(new)


public:

	Ambient() {}


	int begin(int channelId, char* writeKey, WiFiClient* c, const char* readKey = NULL);//(new)
	int set(int field, const char* data);
	int clear(int field);
	int setcmnt(const char* cmnt);	//(new)
	int send();		//(new)				//(new)
	int delete_data(char* userKey);	//(new)
	int status;	//(new)
	int getStatus();
	bool connect2host(uint32_t tmout = AMBIENT_TIMEOUT);//(new)
	int getStatusCode(void);				//(new)

};

//------------------------------------------------------------------------------------

class AmbientRes : public Resource {
private:

	Ambient ambient;//念のためのクラス定義

public:
	AmbientRes() {

	}

	void	 print();
	void	 printTree();
	void	 printValue(char* s, Value* value);
	Value* methodCall(char* name, List* params);

	Value* begin(List* params);
	Value* set(List* params);
	Value* clear(List* params);
	Value* setcmnt(List* params);	//(new)
	Value* send();
	Value* delete_data(List* params);
	Value* getStatus();		//(new)
};

//------------------------------------------------------------------------------------
class pageNode {
public:
	String _uri;
	char* _fname;
	pageNode* _next;

	pageNode(String uri, char* fname, pageNode* next) {
		_uri = uri;
		_fname = fname;
		_next = next;
	}
};

//------------------------------------------------------------------------------------

class PageList {
public:
	pageNode* _top;

	PageList() {
		_top = NULL;
	}

	virtual ~PageList() {
		clearNodes();
	}
	void	 clearNodes();
	pageNode* search(String uri);
	char* getName(String uri);
	void	 add(String uri, char* fname);
	void	 remove(String uri);
};


//------------------------------------------------------------------------------------


class WebServerObj : public AbstructObject {
private:
	char* URI;
	int HTTPcode;
	char* content_type;
	char* content;
	int a = 0;


public:
	WebServerObj() {
		if (WiFi.mode(WIFI_STA)) {
			Serial.println("Success");
		}

	}

	~WebServerObj();

	char* getType() {
		return "Webserver";
	}

	long getInt() {
		return 0;
	}

	double getFloat() {
		return 0.0;
	}

	bool getBool() {
		return false;
	}

	char* getText() {
		return NULL;
	}

	Value* evaluate();

	void	 print();
	Value* methodCall(char* name, List* params);
};


class WebServerGen : public AbstructFunction {
public:
	WebServerGen(char* name) : AbstructFunction(name) {}

	int getParamCount() {
		return 1;
	}

	Value* evaluate();
};




#endif
