esptool --chip esp32s3 --port "COM16" --baud 921600  --before default_reset --after hard_reset write_flash -z --flash_mode keep --flash_freq keep --flash_size keep 0x0 "MScript.ino.bootloader.bin" 0x8000 "MScript.ino.partitions.bin" 0xe000 "boot_app0.bin" 0x10000 "MScript.ino.bin" 

