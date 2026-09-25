#!/usr/bin/env python3
import sys
import os
import subprocess

# ---------- 配置 ----------
# 芯片为 STC8H1K17，stcgal 协议应选 stc8h（不是 stc8g，stc8g 是 STC8G 系列）
# 若使用的 stcgal 版本较老、不认识 stc8h，可改为 "auto" 让它自动探测协议
TARGET_MCU = "stc8h"
FREQ = 24000
HANDSHAKE_TIMEOUT = 5
# --------------------------

def flash_with_stcgal(port, hex_file):
    """调用 stcgal 进行烧录"""
    cmd = [
        "python", "-m", "stcgal",
        "-p", port,
        "-P", TARGET_MCU,
        "-t", str(FREQ),
        hex_file
    ]
    print(f"\n开始烧录: {' '.join(cmd)}\n")
    result = subprocess.run(cmd)
    return result.returncode == 0

def main():
    if len(sys.argv) < 3:
        print("用法: python flash.py COM端口 HEX文件路径")
        print("示例: python flash.py com5 out/main.ihx")
        sys.exit(1)

    com_port = sys.argv[1]
    hex_file = sys.argv[2]
    success = flash_with_stcgal(com_port, hex_file)
    if not success:
        print("\n❌ 烧录失败，请检查连接或重试")
        sys.exit(1)

if __name__ == "__main__":
    main()