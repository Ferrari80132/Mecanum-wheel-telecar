#!/bin/bash
# 一键编译 + 用 J-Link 烧录 STM32F407VET6 固件
set -e

# 项目根目录（本脚本所在目录）
PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$PROJECT_DIR"

echo "========================================="
echo "  1/2  编译项目 (Debug)..."
echo "========================================="
cmake --build build/Debug

echo ""
echo "========================================="
echo "  2/2  烧录固件到 STM32F407VET6..."
echo "========================================="
JLinkExe -autoconnect 1 -CommanderScript flash.jlink

echo ""
echo "========================================="
echo "  完成！固件已烧录并开始运行。"
echo "========================================="
