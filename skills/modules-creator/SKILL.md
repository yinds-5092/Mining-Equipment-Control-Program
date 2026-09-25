---
name: modules-creator
description: Creates user library modules following header-only library standards. Invoke when user needs to create new hardware/software modules, or asks to add new libraries to myLib directory.
---

# Modules Creator

This skill helps create user library modules that follow the header-only library standard for C51/C++ embedded development.

## 库文件创建规范

### 核心约束

1. **目录位置约束**
   - 所有用户库文件必须放置于当前工作区的 `myLib` 目录下
   - 库文件结构：`myLib/<module-name>.h`

2. **文件格式约束**
   - 仅允许使用**仅头文件库（Header-only Library）**模式
   - 所有代码必须封装在 `.h` 头文件中（可拆分为多个头文件）
   - **禁止**出现 `.c`、`.cpp` 等其他源文件类型

3. **硬件依赖处理**
   - 当库需要用到 GPIO、I2C、延时等底层操作时
   - 请使用 `list_libraries` 工具扫描可用的硬件模块
   - 在头文件中通过 `#include` 引用依赖的硬件模块

4. **文档创建约束**
   - 在当前工作区根目录的 `modules` 文件夹下创建模块的说明文档
   - 文档目录结构：`modules/<module-name>/readme.md`
   - 文档必须包含完整的 API 说明、使用示例和注意事项

---

## 创建流程

### 第一步：确认模块需求

- 模块名称（建议使用小写字母和连字符，如 `<主控名>-<模块名>`），比如"tw32f003-dma"
- 模块功能描述
- 是否依赖硬件资源（GPIO、I2C、定时器等）
- 主要 API 需求

### 第二步：扫描可用硬件模块

```bash
# 使用 list_libraries 工具扫描当前可用的硬件模块
list_libraries
```

确定可用的底层硬件模块，如：
- 延时函数
- GPIO 操作
- I2C 通信
- 等等

### 第三步：创建头文件

这一步不是必须的，如果用户已经提供了头文件，或者工作区已经有的则不需要创建头文件。
如果无法确定请通过`ask_user`询问。

在 `myLib` 目录下创建头文件：

```c
// myLib/<module-name>.h

#ifndef <MODULE_NAME>_H
#define <MODULE_NAME>_H

// ========== 头文件导入 ==========
#include "<dependency>.h" // 依赖的硬件模块

// ========== 宏定义 ==========
#define <CONSTANT_NAME> <value>

// ========== 函数声明 ==========
/**
 * @brief 函数功能简述
 * @param param_name 参数说明
 * @return 返回值说明
 */
void <function_name>(<parameters>);

// ========== 内联函数实现 ==========
// 所有函数实现必须在头文件中

#endif // <MODULE_NAME>_H
```

### 第四步：创建说明文档

在 `modules/<module-name>/` 目录下创建 `readme.md` 说明文档，参考以下结构：

```markdown
---
name: <module-name>
description: <模块功能简述>
---

# <模块标题>

## 技能信息
- **Name**: <module-name>
- **Version**: 0.0.1
- **Module**: `c`

## 概述
<模块功能详细说明>

## 硬件资源
### 引脚分配
| 功能 | 引脚 | 说明 |
|------|------|------|

### 寄存器说明
<寄存器详细信息>

---

## 重要规则
<使用时必须遵守的约束和规则>

---

## 快速入门
<分步使用指南>

---

## API参考
<详细的 API 文档>

---

## 完整使用示例
<实际代码示例>
```

---

## 检查清单

创建模块时必须确认：

1. 头文件已创建在 `myLib/` 目录下
2. 文件名为 `.h` 格式（无 `.c` 或 `.cpp` 文件）
3. 所有函数实现都在头文件中
4. 依赖的硬件模块通过 `list_libraries` 确认可用
5. 说明文档已创建在 `modules/<module-name>/readme.md` 路径下
6. 说明文档包含：
   - 技能信息
   - 概述
   - 硬件资源说明
   - 重要规则
   - 快速入门
   - API 参考
   - 使用示例

---

## 最佳实践

1. **命名规范**
   - 模块名使用小写字母和连字符：`xxx-uart`
   - 宏定义使用大写字母和下划线：`UART_BUFFER_SIZE`
   - 函数名使用模块名前缀：`uart_init()`, `uart_send()`

2. **避免冲突**
   - 使用 `#ifndef` 保护符防止重复包含
   - 宏定义使用模块名前缀避免命名冲突

3. **性能优化**
   - 频繁调用的代码考虑宏实现
   - 避免在头文件中定义大数组

4. **文档完整性**
   - 每个公开 API 都必须有文档注释
   - 提供至少 2-3 个实际使用示例
   - 明确标注硬件资源占用和冲突情况

---

## 验证步骤

创建完成后，建议验证：

1. 头文件可以被正确包含
2. 所有依赖模块可用
3. 示例代码可编译通过
4. 说明文档路径为 `modules/<module-name>/readme.md` 且格式正确