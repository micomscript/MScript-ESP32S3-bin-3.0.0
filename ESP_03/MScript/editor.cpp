#include "editor.h"
#include <LittleFS.h>

static const int MAX_VISIBLE_LINES = 24;  // 最大表示行数
static const int MAX_VISIBLE_COLS = 73;  // 最大表示列数（横幅）
static const int LINE_NO_WIDTH = 6;       // 行数表示" 123 |"
bool MTextEditor::pasteMode = false;
unsigned long MTextEditor::lastInputTime = 0;

// エディタ表示開始行（上部にヘッダを出したければ 2 行目以降にするなど調整）
static const int editorStartLine = 1;

// 差分描画用の前回行バッファ
static std::vector<String> prevLines;




// 画面全消去＋カーソルホーム
static void clearScreen() {
	Serial.write("\x1b[2J");
	Serial.write("\x1b[H");
}

// ------------------------
// 公開 API
// ------------------------

void MTextEditor::openNew(const char* path) {
	savePath = path;
	ed.lines.clear();
	ed.lines.push_back("");
	ed.cx = 0;
	ed.cy = 0;
	ed.scrollY = 0;
	ed.scrollX = 0;
}

void MTextEditor::openExisting(const char* path) {
	savePath = path;
	loadFile(path);
	ed.cx = 0;
	ed.cy = 0;
	ed.scrollY = 0;
	ed.scrollX = 0;
	if (ed.lines.empty()) {
		ed.lines.push_back("");
	}
}

void MTextEditor::run() {
	clearScreen();
	prevLines.clear();

	refresh();

	static int fastCount = 0;

	exitRequested = false;

	while (!exitRequested) {

		// ★ 貼り付け終了検知
		if (pasteMode && millis() - lastInputTime > 30) {
			pasteMode = false;
			fastCount = 0;
			refresh();
		}

		// ★★★ ここが最重要：あるだけ全部読む ★★★
		while (Serial.available()) {

			char ch = Serial.read();
			unsigned long now = millis();

			bool isPrintable =
				(ch >= 0x20 && ch <= 0x7E) ||
				ch == '\n' || ch == '\r';

			// 高速入力検知（paste）
			if (isPrintable && now - lastInputTime < 5) {
				fastCount++;
				if (fastCount > 3) {
					pasteMode = true;
				}
			}
			else {
				fastCount = 0;
			}

			lastInputTime = now;

			processKey(ch);
		}

		// ★ 通常入力のみ描画
		if (!pasteMode) {
			refresh();
		}
	}

	clearScreen();
	Serial.println("Editor closed.");
}

// ------------------------
// 内部処理
// ------------------------

// ファイル読み込み（行単位）
void MTextEditor::loadFile(const char* path) {
	ed.lines.clear();

	File rf = LittleFS.open(path, FILE_READ);
	if (!rf) {
		// 存在しない・開けない → 空の状態で開始
		ed.lines.push_back("");
		return;
	}

	String line;
	while (rf.available()) {
		char c = rf.read();
		if (c == '\r') {
			// CR → 無視（LF で区切る）
			continue;
		}
		if (c == '\n') {
			ed.lines.push_back(line);
			line = "";
		}
		else {
			line += c;
		}
	}
	rf.close();

	if (line.length() > 0 || ed.lines.empty()) {
		ed.lines.push_back(line);
	}
}

// ファイル保存
void MTextEditor::saveFile() {
	File wf = LittleFS.open(savePath.c_str(), FILE_WRITE);
	if (!wf) {
		Serial.println("Error: Cannot create file.");
		return;
	}

	ed.savedLines = ed.lines;   // ★ 保存時のスナップショット

	for (int i = 0; i < (int)ed.lines.size(); i++) {
		wf.println(ed.lines[i]);
	}
	wf.close();
}


// 差分描画＋カーソル移動
void MTextEditor::refresh() {
	const int maxVisibleLines = MAX_VISIBLE_LINES;
	const int visibleWidth = MAX_VISIBLE_COLS;  // 行番号の右側に表示する横幅（必要なら調整）


	Serial.write("\x1b[?25l"); // カーソルを消す

	for (int i = 0; i < maxVisibleLines; i++) {
		int lineIndex = ed.scrollY + i;
		String curr = (lineIndex < (int)ed.lines.size())
			? ed.lines[lineIndex]
			: "";

		// 行頭へ移動
		Serial.printf("\x1b[%d;1H", editorStartLine + i);

		// ---- 行番号表示 ----
		if (lineIndex < (int)ed.lines.size()) {
			Serial.printf("%4d |", lineIndex + 1);
		}
		else {
			Serial.print("     |");
		}

		// ---- 本文表示 ----
		String visible = curr;
		/*
				if (ed.scrollX < curr.length()) {
					int endPos = ed.scrollX + visibleWidth;
					if (endPos > curr.length()) endPos = curr.length();
					visible = curr.substring(ed.scrollX, endPos);
				}
				else {
					visible = "";
				}
		*/
		if (lineIndex == ed.cy) {
			// カーソル行の描画（反転込み）
			for (int j = 0; j < visible.length(); j++) {
				if (j /*+ ed.scrollX*/ == ed.cx) {
					Serial.write("\x1b[7m");
					Serial.write(visible[j]);
					Serial.write("\x1b[0m");
				}
				else {
					Serial.write(visible[j]);
				}
			}

			// カーソルが行末にある場合（見えていない位置でもとりあえず末尾に表示）
			if (ed.cx == curr.length()) {
				Serial.write("\x1b[7m \x1b[0m");
			}
		}
		else {
			Serial.print(visible);
		}



		Serial.write("\x1b[K"); // 行末クリア
	}

	// ---- カーソル位置 ----
	int screenY = ed.cy /*- ed.scrollY*/;
	if (screenY < 0) screenY = 0;
	if (screenY >= maxVisibleLines) screenY = maxVisibleLines - 1;

	int cursorX = ed.cx /*- ed.scrollX*/;
	if (cursorX < 0) cursorX = 0;

	Serial.printf("\x1b[%d;%dH",
		editorStartLine + screenY,
		LINE_NO_WIDTH + cursorX + 1
	);


	Serial.write("\x1b[?25h"); // カーソル再表示
}




void MTextEditor::scrollAfterDelete() {
	const int maxVisible = MAX_VISIBLE_LINES;

	// 画面に表示できる行数より少ない → スクロール不要
	if ((int)ed.lines.size() <= maxVisible) {
		ed.scrollY = 0;
		return;
	}

	// カーソルが画面より上に行ってしまった場合 → 下にスクロール
	if (ed.cy < ed.scrollY) {
		ed.scrollY = ed.cy;
		return;
	}

	// カーソルが画面より下に行ってしまった場合 → 上にスクロール
	if (ed.cy >= ed.scrollY + maxVisible) {
		ed.scrollY = ed.cy - maxVisible + 1;
		return;
	}

	// 行削除で画面が空いた場合 → 上に詰める
	int maxscrollY = ed.lines.size() - maxVisible;
	if (ed.scrollY > maxscrollY) {
		ed.scrollY = maxscrollY;
		if (ed.scrollY < 0) ed.scrollY = 0;
	}
}



void MTextEditor::processKey(char ch) {
	bool mergedLine = false;

	// ★★★ ペースト中は制御キーを無視 ★★★
	if (pasteMode) {
		if (!((ch >= 0x20 && ch <= 0x7E) || ch == '\n' || ch == '\r' || ch == '\t')) {
			return; // Ctrl系・ESCなどは無視
		}
	}

	// Ctrl 系
	switch (ch) {
	case 0x13: saveFile(); refresh(); return;      // Ctrl+S
	case 0x11: exitRequested = true; return;       // Ctrl+Q
	case 0x18: saveFile(); exitRequested = true; return; // Ctrl+X
	case 0x15:  // Ctrl+U → 保存時状態に戻す
		if (!ed.savedLines.empty()) {
			ed.lines = ed.savedLines;
			ed.cx = 0; ed.cy = 0; ed.scrollX = 0; ed.scrollY = 0;
			refresh();
		}
		return;
	case 0x04:  // Ctrl+D → 一行削除
		if (!ed.lines.empty()) {
			ed.lines.erase(ed.lines.begin() + ed.cy);
			if (ed.cy >= (int)ed.lines.size()) ed.cy = ed.lines.size() - 1;
			if (ed.cy < 0) { ed.lines.push_back(""); ed.cy = 0; }
			ed.cx = min(ed.cx, (int)ed.lines[ed.cy].length());
			scrollAfterDelete();
			refresh();
		}
		return;
	case 0x0B:  // Ctrl+K → 全削除
		ed.lines.clear();
		ed.lines.push_back("");
		ed.cx = ed.cy = ed.scrollX = ed.scrollY = 0;
		refresh();
		return;
	}

	String& line = ed.lines[ed.cy];

	// Tab → スペース4つ
	if (ch == '\t') {
		line = line.substring(0, ed.cx) + "    " + line.substring(ed.cx);
		ed.cx += 4;
	}
	// Enter
	else if (ch == '\r' || ch == '\n') {
		String rest = line.substring(ed.cx);
		line.remove(ed.cx);
		ed.lines.insert(ed.lines.begin() + ed.cy + 1, rest);
		ed.cy++;
		ed.cx = 0;
	}
	// Backspace
	else if (ch == 0x08 || ch == 0x7F) {
		if (ed.cx > 0) {
			// 行内削除
			line.remove(ed.cx - 1, 1);
			ed.cx--;
		}
		else if (ed.cy > 0) {
			// 行先頭 → 前行と結合
			int prevLen = ed.lines[ed.cy - 1].length();
			ed.lines[ed.cy - 1] += line;
			ed.lines.erase(ed.lines.begin() + ed.cy);
			ed.cy--;
			ed.cx = prevLen;
			mergedLine = true;
		}
		scrollAfterDelete();
	}
	// 矢印キー
	else if (ch == 0x1B && !pasteMode) {
		if (!Serial.available() || Serial.read() != '[') return;
		if (!Serial.available()) return;
		char arrow = Serial.read();
		switch (arrow) {
		case 'A': if (ed.cy > 0) { ed.cy--; ed.cx = min(ed.cx, (int)ed.lines[ed.cy].length()); } break; // 上
		case 'B': if (ed.cy < (int)ed.lines.size() - 1) { ed.cy++; ed.cx = min(ed.cx, (int)ed.lines[ed.cy].length()); } break; // 下
		case 'C': if (ed.cx < (int)ed.lines[ed.cy].length()) ed.cx++; break; // 右
		case 'D': if (ed.cx > 0) ed.cx--; break; // 左
		}
	}
	// 通常文字
	else if (ch >= 0x20 && ch <= 0x7E) {
		// 手入力時のみ閉じカッコ補正
		if (!pasteMode && ch == '}' && ed.cx == 0) {
			int remove = min(4, (int)line.length());
			line.remove(0, remove);
		}
		line = line.substring(0, ed.cx) + ch + line.substring(ed.cx);
		ed.cx++;
	}

	// --------------------------------
	// カーソル・スクロール補正（共通化）
	// --------------------------------
	const int visibleWidth = MAX_VISIBLE_COLS;
	const int visibleHeight = MAX_VISIBLE_LINES;

	// 水平スクロール
	// 水平スクロール補正
/*
if (mergedLine) {
	// 行結合後は一気に見える位置へ
	if (ed.cx >= visibleWidth) ed.scrollX = ed.cx - visibleWidth + 1;
	else ed.scrollX = 0;
} else if ((ch == 0x08 || ch == 0x7F) && ed.cx == 0 && ed.cy > 0) {
	// Backspaceで1列戻すような動き（必要なら）
	if (ed.scrollX > 0) ed.scrollX--;
	if (ed.scrollY > 0) ed.scrollY--;
} else {
	// 通常スクロール
	if (ed.cx < ed.scrollX) ed.scrollX = ed.cx;
	else if (ed.cx >= ed.scrollX + visibleWidth) ed.scrollX = ed.cx - visibleWidth + 1;
}

	if (ed.scrollX < 0) ed.scrollX = 0;
*/
// 垂直スクロール
	if (ed.cy < ed.scrollY) ed.scrollY = ed.cy;
	else if (ed.cy >= ed.scrollY + visibleHeight) ed.scrollY = ed.cy - visibleHeight + 1;
	if (ed.scrollY < 0) ed.scrollY = 0;

}
