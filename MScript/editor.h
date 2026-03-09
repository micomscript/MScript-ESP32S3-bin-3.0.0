#pragma once
#include <vector>
#include <Arduino.h>

// ----------------------------
// エディタ内部状態
// ----------------------------
struct EditorState {
	std::vector<String> lines;  // 行データ
	std::vector<String> savedLines;
	int cx = 0;                 // カーソルX
	int cy = 0;                 // カーソルY
	int scrollY = 0;             // スクロール位置 
	int scrollX = 0; // 横スクロール位置（列スクロール）
};

// ----------------------------
// テキストエディタ管理クラス
// ----------------------------
class MTextEditor {
public:
	void openNew(const char* path);       // 新規作成
	void openExisting(const char* path);  // 既存ファイルを開く
	void run();                           // メインループ（描画＋入力）

private:
	EditorState ed;   // エディタ状態
	String savePath;  // 保存先パス
	static bool pasteMode;
	static unsigned long lastInputTime;

	bool exitRequested = false;

	void refresh();               // 画面描画
	void scrollAfterDelete();
	void processKey(char ch);     // キー入力処理
	void loadFile(const char* path); // ファイル読み込み
	void saveFile();              // 保存
};

// グローバルインスタンス
extern MTextEditor _editor;
