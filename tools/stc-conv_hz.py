import sys
import binascii

# -------------------------- 点阵偏移计算函数 --------------------------
def hz_24_offset(hzg: int, hzd: int) -> int:
    h = 0
    if 0xA1 <= hzg <= 0xAB and hzd >= 0xA1:
        h = (hzg - 0xA1) * 94 + (hzd - 0xA1)
    elif 0xA8 <= hzg <= 0xA9 and hzd < 0xA1:
        if hzd > 0x7F:
            hzd -= 1
        h = (hzg - 0xA8) * 96 + (hzd - 0x40) + 846
    if 0xB0 <= hzg <= 0xF7 and hzd >= 0xA1:
        h = (hzg - 0xB0) * 94 + (hzd - 0xA1) + 1038
    elif 0x81 <= hzg < 0xA1 and hzd >= 0x40:
        if hzd > 0x7F:
            hzd -= 1
        h = (hzg - 0x81) * 190 + (hzd - 0x40) + 1038 + 6768
    elif hzg >= 0xAA and hzd < 0xA1:
        if hzd > 0x7F:
            hzd -= 1
        h = (hzg - 0xAA) * 96 + (hzd - 0x40) + 1038 + 12848
    h = h * 72 + 0x1F43DE
    return h

def hz_12_offset(hzg: int, hzd: int) -> int:
    h = 0
    if 0xA1 <= hzg <= 0xA9 and hzd >= 0xA1:
        h = (hzg - 0xA1) * 94 + (hzd - 0xA1)
    elif 0xA8 <= hzg <= 0xA9 and hzd < 0xA1:
        if hzd > 0x7F:
            hzd -= 1
        h = (hzg - 0xA8) * 96 + (hzd - 0x40) + 846
    if 0xB0 <= hzg <= 0xF7 and hzd >= 0xA1:
        h = (hzg - 0xB0) * 94 + (hzd - 0xA1) + 1038
    elif 0x81 <= hzg < 0xA1 and hzd >= 0x40:
        if hzd > 0x7F:
            hzd -= 1
        h = (hzg - 0x81) * 190 + (hzd - 0x40) + 1038 + 6768
    elif hzg >= 0xAA and hzd < 0xA1:
        if hzd > 0x7F:
            hzd -= 1
        h = (hzg - 0xAA) * 96 + (hzd - 0x40) + 1038 + 12848
    h = h * 24 + 0x093D0E
    return h

def hz_16_offset(hzg: int, hzd: int) -> int:
    h = 0
    if 0xA0 <= hzg <= 0xA7 and hzd >= 0xA1:
        h = (hzg - 0xA1) * 94 + (hzd - 0xA1)
    elif 0xA8 <= hzg <= 0xA9 and hzd < 0xA1:
        if hzd > 0x7F:
            hzd -= 1
        h = (hzg - 0xA8) * 96 + (hzd - 0x40) + 846
    if 0xB0 <= hzg <= 0xF7 and hzd >= 0xA1:
        h = (hzg - 0xB0) * 94 + (hzd - 0xA1) + 1038
    elif 0x81 <= hzg < 0xA1 and hzd >= 0x40:
        if hzd > 0x7F:
            hzd -= 1
        h = (hzg - 0x81) * 190 + (hzd - 0x40) + 1038 + 6768
    elif hzg >= 0xAA and hzd < 0xA1:
        if hzd > 0x7F:
            hzd -= 1
        h = (hzg - 0xAA) * 96 + (hzd - 0x40) + 1038 + 12848
    h = h * 32 + 0x114FDE
    return h

def hz_32_offset(hzg: int, hzd: int) -> int:
    return ((hzg - 0xA1) * 94 + (hzd - 0xA1)) * 128

# -------------------------- 点阵C数组生成函数（增强健壮性） --------------------------
def query_hz_12(hz_gb2312: bytes, font_file: str = "../ziku3.bin") -> str:
    # 防御：必须是偶数长度（双字节字符）
    if len(hz_gb2312) % 2 != 0:
        raise ValueError(f"GB2312字节流长度必须为偶数，当前长度为 {len(hz_gb2312)}，请检查是否混入了非汉字字符")
    array_size = len(hz_gb2312) * 12
    hz_hex = binascii.hexlify(hz_gb2312).decode("utf-8")
    tts = f"const unsigned char hz12_{hz_hex}[{array_size}] = {{"
    with open(font_file, "rb") as f:
        for i in range(0, len(hz_gb2312), 2):
            hzg = hz_gb2312[i]
            hzd = hz_gb2312[i+1]
            offset = hz_12_offset(hzg, hzd)
            f.seek(offset)
            ss = f.read(24)
            for byte in ss:
                tts += f"0x{byte:02X},"
    tts += "};"
    return tts

def query_hz_16(hz_gb2312: bytes, font_file: str = "../ziku3.bin") -> str:
    if len(hz_gb2312) % 2 != 0:
        raise ValueError(f"GB2312字节流长度必须为偶数，当前长度为 {len(hz_gb2312)}")
    array_size = len(hz_gb2312) * 16
    hz_hex = binascii.hexlify(hz_gb2312).decode("utf-8")
    tts = f"const unsigned char hz16_{hz_hex}[{array_size}] = {{"
    with open(font_file, "rb") as f:
        for i in range(0, len(hz_gb2312), 2):
            hzg = hz_gb2312[i]
            hzd = hz_gb2312[i+1]
            offset = hz_16_offset(hzg, hzd)
            f.seek(offset)
            ss = f.read(32)
            for byte in ss:
                tts += f"0x{byte:02X},"
    tts += "};"
    return tts

def query_hz_24(hz_gb2312: bytes, font_file: str = "../ziku3.bin") -> str:
    if len(hz_gb2312) % 2 != 0:
        raise ValueError(f"GB2312字节流长度必须为偶数，当前长度为 {len(hz_gb2312)}")
    array_size = len(hz_gb2312) * 36
    hz_hex = binascii.hexlify(hz_gb2312).decode("utf-8")
    tts = f"const unsigned char hz24_{hz_hex}[{array_size}] = {{"
    with open(font_file, "rb") as f:
        for i in range(0, len(hz_gb2312), 2):
            hzg = hz_gb2312[i]
            hzd = hz_gb2312[i+1]
            offset = hz_24_offset(hzg, hzd)
            f.seek(offset)
            ss = f.read(72)
            for byte in ss:
                tts += f"0x{byte:02X},"
    tts += "};"
    return tts

def query_hz_32(hz_gb2312: bytes, font_file: str = "../HZK32") -> str:
    if len(hz_gb2312) % 2 != 0:
        raise ValueError(f"GB2312字节流长度必须为偶数，当前长度为 {len(hz_gb2312)}")
    array_size = len(hz_gb2312) * 64
    hz_hex = binascii.hexlify(hz_gb2312).decode("utf-8")
    tts = f"const unsigned char hz32_{hz_hex}[{array_size}] = {{"
    with open(font_file, "rb") as f:
        for i in range(0, len(hz_gb2312), 2):
            hzg = hz_gb2312[i]
            hzd = hz_gb2312[i+1]
            offset = hz_32_offset(hzg, hzd)
            f.seek(offset)
            ss = f.read(128)
            for byte in ss:
                tts += f"0x{byte:02X},"
    tts += "};"
    return tts

def clean_chinese_string(s: str) -> str:
    """只保留基本汉字字符（U+4E00～U+9FFF）"""
    return ''.join(ch for ch in s if '\u4e00' <= ch <= '\u9fff')

def process_c_code(input_code: str) -> str:
    """
    处理c/c++代码：替换show_fontxx调用，插入点阵数组
    """
    code = input_code  # 不要 copy()，直接使用字符串（不可变，replace返回新串）
    
    # 辅助函数：处理特定尺寸的函数调用
    def process_size(size: int):
        nonlocal code
        func_name = f"show_font{size}"
        array_prefix = f"hz{size}"
        
        while True:
            hztest = code.find(f'{func_name}("')
            if hztest == -1:
                break
            hzstart = code.find('"', hztest)
            hzend = code.find('"', hzstart + 1)
            original_hz_str = code[hzstart + 1: hzend]
            hz_str = original_hz_str

            # 过滤非汉字字符
            clean_str = clean_chinese_string(hz_str)
            if not clean_str:
                print(f"警告：跳过空字符串或无效汉字调用：\"{hz_str}\"")
                code = code.replace(f'{func_name}("{hz_str}")', "")
                continue
            if clean_str != hz_str:
                print(f"注意：字符串\"{hz_str}\" 已被过滤为 \"{clean_str}\"")
                hz_str = clean_str

            # 转换为GB2312
            try:
                hz_gb2312 = hz_str.encode("gb2312")
            except UnicodeEncodeError as e:
                raise ValueError(f"汉字 '{hz_str}' 无法转换为GB2312编码：{e}")

            hz_count = len(hz_gb2312) // 2
            hz_hex = binascii.hexlify(hz_gb2312).decode("utf-8")

            # 替换函数调用（使用原始字符串匹配）
            old_call = f'{func_name}("{original_hz_str}"'
            new_call = f'show_font{size}({hz_count}, {array_prefix}_{hz_hex}'
            code = code.replace(old_call, new_call)
            
            # 生成点阵数组(使用unsigned char)
            if size == 12:
                hz_array = query_hz_12(hz_gb2312)
            elif size == 16:
                hz_array = query_hz_16(hz_gb2312)
            elif size == 24:
                hz_array = query_hz_24(hz_gb2312)
            elif size == 32:
                hz_array = query_hz_32(hz_gb2312)
            else:
                raise ValueError(f"不支持的字体大小：{size}")
            code = hz_array + "\n" + code
    
    # 处理四种尺寸
    for sz in (12, 16, 24, 32):
        process_size(sz)
    
    return code

def conv_asr_code_file(input_path="_main.c", output_path=None):
    if output_path is None:
        if input_path.startswith("_"):
            output_path = input_path[1:]  # 去掉开头的下划线
        else:
            output_path = input_path  # 保持原样
    try:
        with open(input_path, "r", encoding="utf-8") as f:
            input_c_code = f.read()
        processed_c = process_c_code(input_c_code)
        with open(output_path, "w", encoding="utf-8") as f:
            f.write(processed_c)
        # print(f"处理完成！输出文件已保存至：{output_path}")
    except FileNotFoundError as e:
        print(f"错误：文件不存在 → {e.filename}")
    except ValueError as e:
        print(f"处理失败：{e}")
    except Exception as e:
        print(f"未知错误：{str(e)}")
        import traceback
        traceback.print_exc()

def main():
    if len(sys.argv) >= 3:
        conv_asr_code_file(sys.argv[1], sys.argv[2])
    elif len(sys.argv) == 2:
        conv_asr_code_file(sys.argv[1])
    else:
        conv_asr_code_file()

if __name__ == "__main__":
    main()