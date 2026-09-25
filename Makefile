# SDCC Makefile
# 编译到 8051 等嵌入式平台

# 编译器配置
CC = sdcc.exe
PACKIHX = packihx.exe
MAKEBIN = makebin.exe
INCLUDES = -I lib -I myLib
CFLAGS = --model-small --stack-auto --stack-loc 0x80 --out-fmt-ihx

# 源文件（默认 main.c）
SRC ?= main.c

# 输出目录
OUTDIR = out

# 输出文件
OUTPUT = $(OUTDIR)/main.ihx
HEX = $(OUTDIR)/main.hex
BIN = $(OUTDIR)/main.bin

# 默认目标
all: $(OUTDIR) $(OUTPUT) $(HEX) $(BIN)
	@echo Compilation complete!
	@echo Output: $(OUTDIR)/main.*
	@echo ========== Memory Usage ==========
	@FLASH=$$(grep -E '^CSEG\s+[0-9A-F]+\s+[0-9A-F]+\s+=\s+([0-9]+)' $(OUTDIR)/main.map | head -1 | sed -n 's/.*=\s*\([0-9]*\)\..*/\1/p'); \
	DRAM=$$(grep -E '^DSEG\s+[0-9A-F]+\s+[0-9A-F]+\s+=\s+([0-9]+)' $(OUTDIR)/main.map | head -1 | sed -n 's/.*=\s*\([0-9]*\)\..*/\1/p'); \
	XRAM=$$(grep -E '^XISEG\s+[0-9A-F]+\s+[0-9A-F]+\s+=\s+([0-9]+)' $(OUTDIR)/main.map | head -1 | sed -n 's/.*=\s*\([0-9]*\)\..*/\1/p'); \
	echo "Program Size: data=$${DRAM:-0} xdata=$${XRAM:-0} code=$${FLASH:-0}"
	@echo ==================================

# 创建输出目录
$(OUTDIR):
	@mkdir -p "$(OUTDIR)"

# 编译
$(OUTPUT): $(SRC)
	@echo Compiling $(SRC)...
	$(CC) $(INCLUDES) $(CFLAGS) -o $(OUTDIR)/main.ihx $(SRC)

# ihx -> hex
$(HEX): $(OUTPUT)
	@echo Generating main.hex...
	$(PACKIHX) $(OUTPUT) > $(HEX)

# ihx -> bin
$(BIN): $(OUTPUT)
	@echo Generating main.bin...
	$(MAKEBIN) -s 65536 -p $(OUTPUT) $(BIN)

# 清理
clean:
	@echo Cleaning...
	@rm -rf "$(OUTDIR)"
	@echo Done.

.PHONY: all clean