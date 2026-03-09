#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>
#include <BluetoothSerial.h>

class Bluetooth {
private:
	BluetoothSerial SerialBT;
	bool isClient = false;

public:
	Bluetooth();

	bool saveToFile(const char* path, const String& code);

	bool begin(const char* name, bool mode = false);
	void end();
	void disconnect();
	void write(const char* text);
	bool connect(const char* address);
	bool isConnected();
	String getAddress();
	String read();
	int available();
};

#endif
