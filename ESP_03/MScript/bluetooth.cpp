#include "bluetooth.h"
#include <BluetoothSerial.h>
#include <esp_mac.h>
#include <Arduino.h>
#include <LittleFS.h>
#include <FS.h>

Bluetooth::Bluetooth() {}

bool Bluetooth::saveToFile(const char* path, const String& code) {
	File f = LittleFS.open(path, "w");
	if (!f) return false;

	f.print(code);     // ← MicomScript のコードを書き込む
	f.print("\n");     // ← MicomScript は改行で文を区切る

	f.close();

	return true;
}

bool Bluetooth::begin(const char* name, bool mode) {
	isClient = mode;
	return SerialBT.begin(name, mode);
}

void Bluetooth::end() {
	SerialBT.end();
}

String Bluetooth::getAddress() {
	uint8_t mac[6];
	SerialBT.getBtAddress(mac);
	char macStr[18];
	sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X",
		mac[0], mac[1], mac[2],
		mac[3], mac[4], mac[5]);
	return String(macStr);
}

bool Bluetooth::connect(const char* address) {
	if (!isClient)
		return false;
	uint8_t mac[6];
	sscanf(address, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
		&mac[0], &mac[1], &mac[2],
		&mac[3], &mac[4], &mac[5]);
	if (SerialBT.connect(mac)) {
		Serial.println("Connected to Server!");
		return true;
	}
	else {
		Serial.println("Failed to connect.");
		return false;
	}
}

void Bluetooth::disconnect() {
	SerialBT.disconnect();
}

void Bluetooth::write(const char* text) {
	String s = text;
	if (!s.endsWith("\n")) {
		s += "\n";       // 末尾に \n がついていなければ自動でつける
	}
	SerialBT.print(s);
}

bool Bluetooth::isConnected() {
	return SerialBT.connected();
}

String Bluetooth::read() {
	if (!SerialBT.available()) {
		return "";   // 何も来ていないときは空文字
	}
	String result;
	result.reserve(64);  // 断片化を防ぐ（重要）
	while (SerialBT.available()) {
		char c = SerialBT.read();
		result += c;
		if (c == '\n') {   // 改行まで読む
			break;
		}
	}
	result.trim();
	return result;
}

int Bluetooth::available() {
	return SerialBT.available();
}