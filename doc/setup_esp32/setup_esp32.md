# MicomScript セットアップ手順（ESP32S3-DevKitC）

## 1. 必要なソフトをインストール
- Tera Term
- Arduino IDE

---

## 2. ESP32 を PC に接続
- USB シリアルケーブルで接続
- Arduino IDE で **MScript.ino** を開く

---

## 3. 必要なライブラリの確認
Arduino IDE → ライブラリマネージャーで以下を確認  
（未インストールの場合は追加）

- Adafruit AM2320 sensor library  
- Adafruit BusIO  
- Adafruit Unified Sensor  
- AsyncTCP (dvarrel)  
- ESP Async WebServer  
- ESPAsyncTCP (dvarrel)  
- ESPAsyncWebSrv (dvarrel)

---

## 4. Arduino IDE の設定
**ツール → ボード設定**

| 項目 | 設定値 |
|------|--------|
| ボード | ESP32 Dev Module |
| ポート | 接続されている COM ポート |
| Flash Size | 4MB（32Mb） |
| Partition Scheme | No OTA（2MB APP / 2MB SPIFFS） |
| Upload Speed | 115200 |

---

## 5. コンパイル
- 3分以上かかる場合があります

---

## 6. Tera Term の設定
1. Tera Term を開く  
2. 「シリアル」を選択  
3. 設定 → シリアルポート

**設定値：**

| 項目 | 値 |
|------|------|
| ポート | 各自の COM |
| スピード | 115200 |
| データ | 8bit |
| パリティ | none |
| ストップビット | 1bit |
| フロー制御 | none |
| 送信遅延 | 0ms / 字、0ms / 行 |

---

## 7. 接続確認
- 最初は文字化けすることがあります  
- **Enter を 2 回押すと `$` が表示される**  
- 文字化けが直らない場合  
  - ESP32 の **BOOT ボタン**を押す  
  - Tera Term を再起動

---

## 8. スクリプト開始
`$` が表示されたら MicomScript の入力が可能です。
