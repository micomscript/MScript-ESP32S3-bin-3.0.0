# Bluetooth RC Car – Requirements

このドキュメントは、MicomScript を使って Bluetooth 操作のラジコンカーを動かすために必要なハードウェア、ピン配置をまとめたものです。

## 1. 必要なハードウェア
- ESP32S3-DevKitC
- モータードライバ（L298N / L293D / TB6612FNG など）
- DC モーター × 2（左右駆動用）
- モーター用電源
- ジャンパーワイヤ
- Bluetooth 接続先（スマホ or 別の ESP32）
- ボタン × 4（b1, b2, b3, b4 用）

## 2. ピン配置（car）

右モーター（A 側）
- ENA → GPIO27 (PWM)
- IN1 → GPIO25
- IN2 → GPIO26

左モーター（B 側）
- ENB → GPIO14 (PWM)
- IN3 → GPIO32
- IN4 → GPIO33

## 3. ピン配置（controller）
- b1 → GPIO2
- b2 → GPIO4
- b3 → GPIO5
- b4 → GPIO18
