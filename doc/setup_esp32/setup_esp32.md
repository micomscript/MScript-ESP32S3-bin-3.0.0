MicomScriptセットアップ手順（ESP32S3-DevKitC）

1.必要なソフトをインストール
・Tera Term
・Arduino IDE

2.ESP32をPCに接続
・USBシリアルケーブルで接続
・Arduino IDEでMScript.inoを開く

3.必要なライブラリの確認
Arduino IDE > ライブラリマネージャーで以下を確認（未インストールなら追加）
・Adafruit AM2320 sensor library
・Adafruit BusIO
・Adafruit Unified Sensor
・AsyncTCP (dvarrel)
・ESP Async WebServer
・ESPAsyncTCP (dvarrel)
・ESPAsyncWebSrv (dvarrel)

4.Arduino IDEの設定
ツール > ボード設置
ボード		 : ESP32 Dev Module
ポート		 : 接続されているCOMポート
Flash Size	 : 4MB（32Mb）
Partition Scheme : No OTA（2MB APP / 2MB SPIFFS）
UPload Speed	 : 115200

5.コンパイル
3分以上かかる場合あり

6.Tera Termの設定
・Tera Termを開く
・ 「シリアル」を選択
・ 設定 > シリアルポート

ポート		: 各自の COM
スピード	: 115200
データ		: 8bit
パリティ	: none
ストップビット	: 1bit
フロー制御	: none
送信遅延	: 0ms / 字、0ms / 行

7.接続確認
・最初は文字化けすることがある
・Enter を2回押すと $ が表示される
・文字化けが直らない場合はESP32のbootを押し Tera Term を再起動

8.スクリプト開始
$が出たらMicomScriptの入力が可能