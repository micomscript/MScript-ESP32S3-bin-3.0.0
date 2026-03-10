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

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * fsystem.cpp
 * ファイルシステム(LittleFS)
 */
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <FS.h>
#include <LittleFS.h>
#include "common.h"
#include "fsystem.h"
#include "fsystempath.h"   //  追加：相対パス処理
#include "editor.h"

MSFileSystem _fileSystem;
extern int _line_number;

void MSFileSystem::initFSystem() {

	if (!LittleFS.begin(true)) {
		printf("Error : Failed to Mount LittleFS.\n");
		return;
	}
	printf("LittleFS Mount Succeeded.\n");
	printf("total %8d bytes\n", LittleFS.totalBytes());
	printf("used  %8d bytes\n", LittleFS.usedBytes());

	// ★ カレントディレクトリは常に "/" から開始
	strcpy(currentDir, "/");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// listFiles
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MSFileSystem::listFiles(const char* path) {

	PathInfo info;
	if (!normalizePath(path, currentDir, &info)) {
		printf("Error : Invalid path (%s)\n", path);
		return;
	}

	File dir = LittleFS.open(info.full);
	if (!dir) {
		printf("Error : Failed to open directory: %s\n", info.full);
		return;
	}

	if (!dir.isDirectory()) {
		return; // ファイルなら何も表示しない
	}

	size_t baseLen = strlen(info.full);
	if (info.full[baseLen - 1] != '/') baseLen++;

	File file = dir.openNextFile();
	while (file) {
		const char* p = file.path();
		const char* relative = p + baseLen;

		if (file.isDirectory()) {
			printf("     dir /%s\n", relative);
		}
		else {
			printf("%8d /%s\n", file.size(), relative);
		}

		file = dir.openNextFile();
	}
}

bool MSFileSystem::makeParentDirs(const char* fullpath) {

	// fullpath は normalizePath() によって絶対パスになっている前提
	// 例: "/abc/def/ghi.txt"

	char parent[256];
	getParentDir(fullpath, parent);   // "/abc/def"

	// ルートなら何もしない
	if (strcmp(parent, "/") == 0) {
		return true;
	}

	// 親ディレクトリを "/" で順に作っていく
	char temp[256] = "/";
	const char* p = parent + 1;  // 先頭の "/" を飛ばす

	while (*p) {
		const char* slash = strchr(p, '/');
		size_t len = slash ? (size_t)(slash - p) : strlen(p);

		strncat(temp, p, len);

		if (!LittleFS.exists(temp)) {
			if (!LittleFS.mkdir(temp)) {
				Serial.printf("mkdir failed: %s\n", temp);
				return false;
			}
		}

		if (!slash) break;

		strncat(temp, "/", sizeof(temp) - strlen(temp) - 1);
		p = slash + 1;
	}

	return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// saveFile
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MSFileSystem::saveFile(const char* path) {

	PathInfo info;
	if (!normalizePath(path, currentDir, &info)) {
		Serial.println("Error: invalid path");
		return;
	}

	if (!makeParentDirs(info.full)) {
		Serial.println("Error: cannot create parent directories");
		return;
	}

	File file = LittleFS.open(info.full, "w");
	if (!file) {
		Serial.println("Error: cannot open file");
		return;
	}

	uint8_t ch, c = ' ';
	int n = 0;
	bool flag = true;

	while (flag) {
		while (Serial.available()) {
			ch = Serial.read();

			if (ch == 0x0d) {
				Serial.write(ch);
				file.write(ch);
				ch = 0x0a;
			}

			Serial.write(ch);

			if (c == 0x0a && ch == ';') {
				flag = false;
				break;
			}

			file.write(ch);
			n++;
			c = ch;
		}
	}

	file.close();
	Serial.printf("Wrote %d characters\r\n", n);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// showFile
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MSFileSystem::showFile(const char* path) {

	PathInfo info;
	if (!normalizePath(path, currentDir, &info)) {
		printf("Error : Invalid path (%s)\n", path);
		return;
	}

	File file = LittleFS.open(info.full);
	if (!file) {
		printf("Error : Failed to open script file (%s)\n", info.full);
		return;
	}

	while (file.available()) {
		Serial.write(file.read());
	}
	file.close();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// removeFile
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MSFileSystem::removeFile(const char* path) {
	PathInfo info;
	if (!normalizePath(path, currentDir, &info)) {
		printf("Error : Invalid path (%s)\n", path);
		return;
	}

	if (!LittleFS.exists(info.full)) {
		printf("Error : No such file or directory (%s)\n", info.full);
		return;
	}

	if (!LittleFS.remove(info.full)) {
		printf("Error : Failed to remove (%s)\n", info.full);
		return;
	}

	printf("File removed : %s\n", info.full);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// editFile
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MSFileSystem::editFile(const char* path) {

	PathInfo info;
	if (!normalizePath(path, currentDir, &info)) {
		printf("Error : Invalid path (%s)\n", path);
		return;
	}

	// ファイルが存在しない場合は新規作成
	if (!LittleFS.exists(info.full)) {
		File f = LittleFS.open(info.full, FILE_WRITE);
		if (f) {
			f.close();   // 空ファイルを作る
		}
		// 新規ファイルとして開く
		MTextEditor ed;
		ed.openNew(info.full);
		ed.run();
		return;
	}

	// 既存ファイルを開く
	MTextEditor ed;
	ed.openExisting(info.full);
	ed.run();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// makeDir
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MSFileSystem::makeDir(const char* path) {

	PathInfo info;
	if (!normalizePath(path, currentDir, &info)) {
		printf("Error: invalid path (%s)\n", path);
		return;
	}

	if (!makeParentDirs(info.full)) {
		Serial.println("Error: cannot create parent directories");
		return;
	}

	if (LittleFS.exists(info.full)) {
		printf("Directory already exists: %s\n", info.full);
		return;
	}

	if (!LittleFS.mkdir(info.full)) {
		printf("Error: mkdir failed (%s)\n", info.full);
		return;
	}

	printf("Directory created: %s\n", info.full);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// removeDir
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MSFileSystem::removeDir(const char* path) {

	PathInfo info;
	if (!normalizePath(path, currentDir, &info)) {
		printf("Error: invalid path (%s)\n", path);
		return;
	}

	File dir = LittleFS.open(info.full);
	if (!dir || !dir.isDirectory()) {
		printf("Error: not a directory (%s)\n", info.full);
		return;
	}

	// ★ 空ディレクトリか確認
	File f = dir.openNextFile();
	if (f) {
		printf("Error: directory not empty (%s)\n", info.full);
		return;
	}

	if (!LittleFS.rmdir(info.full)) {
		printf("Error: rmdir failed (%s)\n", info.full);
		return;
	}

	printf("Directory removed: %s\n", info.full);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// changeDir
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MSFileSystem::changeDir(const char* path) {

	PathInfo info;
	if (!normalizePath(path, currentDir, &info)) {
		printf("Error: invalid path (%s)\n", path);
		return;
	}

	File dir = LittleFS.open(info.full);
	if (!dir || !dir.isDirectory()) {
		printf("Error: not a directory (%s)\n", info.full);
		return;
	}

	strcpy(currentDir, info.full);
	printf("Current directory: %s\n", currentDir);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cwd
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MSFileSystem::printWorkingDir() {
	printf("%s\n", currentDir);
}

const char* MSFileSystem::getCurrentDir() const {
	return currentDir;
}

