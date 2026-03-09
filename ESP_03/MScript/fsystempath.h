#pragma once

#define MAX_PARTS   32
#define MAX_SEG_LEN 64

// パス情報をまとめて返す構造体（必要に応じて拡張可能）
struct PathInfo {
	char full[256];     // 正規化された絶対パス
	char parent[256];   // 親ディレクトリ
	char name[128];     // ファイル名
};

// 相対パス → 絶対パスへ正規化する
// 例: "abc/def" → "/abc/def"
//     "./abc"   → "/abc"
//     "../def"  → "/def"
//     "/abc//def/" → "/abc/def"
bool normalizePath(const char* input, const char* currentDir, PathInfo* out);

// 親ディレクトリを取得（"/abc/def" → "/abc"）
void getParentDir(const char* fullpath, char* parent);

// パス結合（base + relative → output）
// 例: "/abc/def" + "../ghi" → "/abc/ghi"
void joinPath(const char* base, const char* relative, char* output);
