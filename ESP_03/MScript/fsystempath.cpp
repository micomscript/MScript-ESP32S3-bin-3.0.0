#include <string.h>
#include "fsystempath.h"

// ------------------------------------------------------------
// 内部ユーティリティ
// ------------------------------------------------------------

static void pushSegment(char stack[][MAX_SEG_LEN], int& top, const char* seg) {
	if (top < MAX_PARTS - 1) {
		strncpy(stack[top], seg, MAX_SEG_LEN - 1);
		stack[top][MAX_SEG_LEN - 1] = '\0';
		top++;
	}
}

static void popSegment(int& top) {
	if (top > 0) top--;
}

// ------------------------------------------------------------
// 親ディレクトリ取得
// ------------------------------------------------------------
void getParentDir(const char* fullpath, char* parent)
{
	strcpy(parent, "/");

	if (strcmp(fullpath, "/") == 0) return;

	const char* lastSlash = strrchr(fullpath, '/');
	if (!lastSlash || lastSlash == fullpath) {
		strcpy(parent, "/");
		return;
	}

	size_t len = lastSlash - fullpath;
	if (len >= 255) len = 255;

	strncpy(parent, fullpath, len);
	parent[len] = '\0';
}

// ------------------------------------------------------------
// base + relative を結合
// ------------------------------------------------------------
void joinPath(const char* base, const char* relative, char* output)
{
	strcpy(output, base);

	if (output[strlen(output) - 1] != '/')
		strncat(output, "/", 255 - strlen(output));

	strncat(output, relative, 255 - strlen(output));
}

// ------------------------------------------------------------
// 相対パス → 絶対パスへ正規化（currentDir 対応版）
// ------------------------------------------------------------
bool normalizePath(const char* input, const char* currentDir, PathInfo* out)
{
	if (!input || !out || !currentDir) return false;

	char work[256];

	// 先頭空白除去
	while (*input == ' ' || *input == '\t') input++;

	bool isAbsolute = (input[0] == '/');

	// 相対パスなら currentDir + "/" + input を作る
	if (!isAbsolute) {
		joinPath(currentDir, input, work);
	}
	else {
		strncpy(work, input, sizeof(work));
		work[255] = '\0';
	}

	// "/" で分割してスタックに積む
	char segs[MAX_PARTS][MAX_SEG_LEN];
	int top = 0;

	char* token = strtok(work, "/");
	while (token) {

		if (strcmp(token, ".") == 0) {
			// 無視
		}
		else if (strcmp(token, "..") == 0) {
			popSegment(top);
		}
		else if (strlen(token) > 0) {
			pushSegment(segs, top, token);
		}

		token = strtok(NULL, "/");
	}

	// 絶対パス構築
	strcpy(out->full, "/");

	for (int i = 0; i < top; i++) {
		strncat(out->full, segs[i], sizeof(out->full) - strlen(out->full) - 1);
		if (i < top - 1)
			strncat(out->full, "/", sizeof(out->full) - strlen(out->full) - 1);
	}

	// 親ディレクトリ
	getParentDir(out->full, out->parent);

	// ファイル名
	if (top == 0) {
		strcpy(out->name, "");
	}
	else {
		strncpy(out->name, segs[top - 1], sizeof(out->name));
		out->name[sizeof(out->name) - 1] = '\0';
	}

	return true;
}
