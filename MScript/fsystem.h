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

/*
 * fsystem.h
 * ファイルシステム
 */
#ifndef _FSYSSTEM_H_

#define _FSYSTEM_H_

class MSFileSystem {
private:
	File file;
	char currentDir[256];   // ★ カレントディレクトリ

public:
	MSFileSystem() {
	}
#define MAX_PARTS 16
#define MAX_SEG_LEN 64
	bool  makeParentDirs(const char* filepath);
	void	initFSystem();
	void	listFiles(const char* path);
	void	saveFile(const char* path);
	void	showFile(const char* path);
	void	removeFile(const char* path);
	void  editFile(const char* path);


	void makeDir(const char* path);
	void removeDir(const char* path);
	void changeDir(const char* path);
	void printWorkingDir();
	const char* getCurrentDir() const;

};

#endif
