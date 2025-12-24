# 自定义Fractal标记生成与使用指南

本文档详细介绍如何生成自定义规格的5层Fractal标记，将其转换为PDF格式用于打印，以及如何编译运行检测程序。

## 目录
1. [系统概述](#系统概述)
2. [环境准备](#环境准备)
3. [生成自定义Fractal标记](#生成自定义fractal标记)
4. [转换为PDF格式](#转换为pdf格式)
5. [编译项目](#编译项目)
6. [运行检测程序](#运行检测程序)
7. [完整示例](#完整示例)
8. [故障排除](#故障排除)

## 系统概述

本项目提供了一个完整的5层Fractal标记系统，具有以下特点：

### 5层嵌套结构
- **第1层**：16个区域，12个用于编码
- **第2层**：14个区域，10个用于编码  
- **第3层**：12个区域，8个用于编码
- **第4层**：10个区域，6个用于编码
- **第5层**：8个区域，4个用于编码

### 技术优势
- **总共40个编码位**：支持更多标记ID
- **高精度检测**：多层结构提供更精确的角点检测
- **强鲁棒性**：部分层级被遮挡时仍能检测
- **大尺寸优化**：专门针对15-25cm打印尺寸优化

## 环境准备

### 系统要求
- Linux系统（推荐Ubuntu 18.04+）
- OpenCV 4.6+
- CMake 3.10+
- Python 3.6+（用于PDF转换）

### Python依赖安装
```bash
# 安装PDF转换所需的Python库
pip3 install reportlab pillow
```

### 编译依赖
确保已安装以下开发库：
```bash
sudo apt update
sudo apt install build-essential cmake libopencv-dev libeigen3-dev
```

## 生成自定义Fractal标记

### 步骤1：创建5层配置文件

使用`fractal_create_5layer`程序创建配置文件：

```bash
./build/utils_fractal/fractal_create_5layer 配置文件名.yml [选项]
```

**参数说明：**
- `配置文件名.yml`：输出的配置文件名
- `-s 位大小`：最小位大小（像素），默认50，针对20cm打印优化

**示例：**
```bash
# 创建默认配置（适合20cm打印）
./build/utils_fractal/fractal_create_5layer fractal_5layer_config.yml

# 创建自定义位大小配置
./build/utils_fractal/fractal_create_5layer my_config.yml -s 60
```

**输出信息：**
```
Creating 5-layer fractal marker configuration:
Layer 1: 16 regions, 12 for encoding
Layer 2: 14 regions, 10 for encoding
Layer 3: 12 regions, 8 for encoding
Layer 4: 10 regions, 6 for encoding
Layer 5: 8 regions, 4 for encoding
Bit size: 50 pixels
Configuration saved to: fractal_5layer_config.yml
```

### 步骤2：生成高质量PNG标记

使用`fractal_print_marker_20cm`程序生成打印用的PNG图像：

```bash
./build/utils_fractal/fractal_print_marker_20cm 配置文件.yml 输出文件.png [选项]
```

**参数说明：**
- `配置文件.yml`：步骤1生成的配置文件
- `输出文件.png`：输出的PNG图像文件
- `-dpi DPI值`：设置DPI，默认300
- `-size 尺寸`：设置尺寸（厘米），默认20.0
- `-noborder`：移除白色边框

**示例：**
```bash
# 生成20cm标记（默认设置）
./build/utils_fractal/fractal_print_marker_20cm fractal_5layer_config.yml marker_20cm.png

# 生成15cm标记，300 DPI
./build/utils_fractal/fractal_print_marker_20cm fractal_5layer_config.yml marker_15cm.png -dpi 300 -size 15.0

# 生成25cm标记，无边框
./build/utils_fractal/fractal_print_marker_20cm fractal_5layer_config.yml marker_25cm.png -size 25.0 -noborder
```

**输出信息：**
```
Creating fractal marker for printing:
Size: 20.0 cm
DPI: 300
Border: Yes
Total image size: 2362x2362 pixels
Bit size: 73 pixels
Fractal marker saved to: marker_20cm.png
Print this at 300 DPI for 20.0 cm size
```

## 转换为PDF格式

### 使用Python脚本转换

项目提供了专用的Python脚本`tools/png_to_pdf_print.py`，可以将PNG图像转换为精确尺寸的PDF文件：

```bash
python3 tools/png_to_pdf_print.py 输入.png 输出.pdf --size-mm 尺寸毫米 [--border-mm 边框毫米]
```

**参数说明：**
- `输入.png`：步骤2生成的PNG文件
- `输出.pdf`：输出的PDF文件
- `--size-mm`：物理尺寸（毫米）
- `--border-mm`：边框大小（毫米），默认5mm

**示例：**
```bash
# 转换20cm标记为PDF（200mm）
python3 tools/png_to_pdf_print.py marker_20cm.png marker_20cm.pdf --size-mm 200

# 转换15cm标记为PDF，带10mm边框
python3 tools/png_to_pdf_print.py marker_15cm.png marker_15cm.pdf --size-mm 150 --border-mm 10

# 转换25cm标记为PDF，无边框
python3 tools/png_to_pdf_print.py marker_25cm.png marker_25cm.pdf --size-mm 250 --border-mm 0
```

**输出信息：**
```
Input: marker_20cm.png
Output: marker_20cm.pdf
Size (mm): 200.0, Border (mm): 5.0
PDF created successfully.
```

### PDF打印注意事项

1. **打印设置**：
   - 使用"实际尺寸"或"100%缩放"打印
   - 不要使用"适合页面"选项
   - 确保打印机设置为高质量模式

2. **纸张选择**：
   - 推荐使用白色厚纸或卡纸
   - 避免使用光面纸（可能产生反光）

3. **验证尺寸**：
   - 打印后用尺子测量实际尺寸
   - 确保与设定尺寸一致

## 编译项目

### 创建构建目录并编译

```bash
# 进入项目根目录
cd /path/to/your/project

# 创建并进入构建目录
mkdir -p build
cd build

# 配置CMake
cmake .. || exit 1

# 编译项目（使用所有CPU核心）
cmake --build . -j$(nproc) || exit 1
```

### 验证编译结果

编译成功后，检查关键程序是否存在：

```bash
# 检查fractal工具
ls -la utils_fractal/fractal_create_5layer
ls -la utils_fractal/fractal_print_marker_20cm  
ls -la utils_fractal/fractal_tracker_5layer
```

## 运行检测程序

### 基本用法

使用`fractal_tracker_5layer`程序进行实时检测：

```bash
./build/utils_fractal/fractal_tracker_5layer live:摄像头ID -config 配置文件.yml -s 标记尺寸 -cam 相机参数文件.yml
```

**参数说明：**
- `live:摄像头ID`：摄像头设备ID，通常为0、1、2等
- `-config`：配置文件路径
- `-s`：标记实际尺寸（米），如0.2表示20cm
- `-cam`：相机标定参数文件

### 使用便捷脚本

项目提供了便捷的shell脚本`run_fractal_tracker.sh`：

```bash
./run_fractal_tracker.sh [选项]
```

**选项说明：**
- `-c, --camera ID`：摄像头ID，默认0
- `-s, --size 尺寸`：标记尺寸（米），默认0.2
- `-config 文件`：配置文件，默认config.yml
- `-cam 文件`：相机参数文件，默认camera.yml
- `-h, --help`：显示帮助信息

### 实时检测界面

程序运行时会显示：

1. **视频窗口**：实时摄像头画面
2. **检测可视化**：
   - 红色方框：标记边界
   - 十字线：标记中心
   - 坐标轴：标记方向

3. **控制台输出**：
   - **位置信息**：X, Y, Z坐标（米）
   - **距离**：到标记的直线距离
   - **旋转信息**：RX, RY, RZ角度（度）
   - **性能指标**：检测和姿态估计时间

### 控制键

- `s`：暂停/继续检测
- `ESC`：退出程序

## 完整示例

以下是一个完整的工作流程示例，生成并使用20cm的Fractal标记：

```bash
# 1. 编译项目
cd build
cmake .. && cmake --build . -j$(nproc)

# 2. 创建5层配置
./utils_fractal/fractal_create_5layer my_fractal_config.yml

# 3. 生成20cm PNG标记
./utils_fractal/fractal_print_marker_20cm my_fractal_config.yml my_marker_20cm.png -dpi 300 -size 20.0

# 4. 转换为PDF
python3 ../tools/png_to_pdf_print.py my_marker_20cm.png my_marker_20cm.pdf --size-mm 200 --border-mm 5

# 5. 打印PDF文件（使用系统打印机）
# 注意：确保以实际尺寸打印

# 6. 运行检测程序
./utils_fractal/fractal_tracker_5layer live:0 -config my_fractal_config.yml -s 0.2 -cam ../camera.yml

# 或者使用便捷脚本
cd ..
./run_fractal_tracker.sh -c 0 -s 0.2 -config my_fractal_config.yml
```

### 不同尺寸的示例

**15cm标记：**
```bash
# 生成15cm标记
./utils_fractal/fractal_print_marker_20cm config.yml marker_15cm.png -size 15.0
python3 ../tools/png_to_pdf_print.py marker_15cm.png marker_15cm.pdf --size-mm 150

# 检测15cm标记
./run_fractal_tracker.sh -s 0.15
```

**25cm标记：**
```bash
# 生成25cm标记
./utils_fractal/fractal_print_marker_20cm config.yml marker_25cm.png -size 25.0
python3 ../tools/png_to_pdf_print.py marker_25cm.png marker_25cm.pdf --size-mm 250

# 检测25cm标记  
./run_fractal_tracker.sh -s 0.25
```

## 故障排除

### 编译问题

**问题：CMake配置失败**
```bash
# 解决方案：检查依赖
sudo apt install build-essential cmake libopencv-dev libeigen3-dev
```

**问题：OpenCV版本过低**
```bash
# 解决方案：升级OpenCV
sudo apt update
sudo apt install libopencv-dev
```

### 生成标记问题

**问题：配置文件创建失败**
- 检查输出目录权限
- 确保磁盘空间充足

**问题：PNG生成失败**
- 检查配置文件是否存在
- 验证配置文件格式正确

### PDF转换问题

**问题：Python库缺失**
```bash
# 解决方案：安装依赖
pip3 install reportlab pillow
```

**问题：PDF尺寸不正确**
- 检查`--size-mm`参数是否正确
- 验证打印设置为"实际尺寸"

### 检测问题

**问题：无法检测到标记**
- 检查光照条件（避免强烈阴影或反光）
- 确认标记打印清晰
- 验证相机焦距设置
- 检查标记尺寸参数（-s）是否正确

**问题：位置信息不准确**
- 确认相机标定文件正确
- 检查标记实际尺寸与参数一致
- 验证标记平整贴合表面

**问题：检测性能差**
- 降低摄像头分辨率
- 改善光照条件
- 确保标记与背景对比度足够

### 相机标定

如果没有相机标定文件，可以使用OpenCV的标定工具：

```bash
# 使用棋盘格进行相机标定
# 详细步骤请参考OpenCV文档
```

---

## 总结

本指南提供了完整的Fractal标记生成、转换和使用流程。通过5层嵌套结构，系统能够提供高精度的位置检测，特别适合需要精确位置信息的应用场景。

关键要点：
1. 使用专用工具生成高质量标记
2. 通过Python脚本精确转换PDF
3. 确保打印尺寸准确
4. 正确配置检测参数
5. 保持良好的光照条件

遵循本指南，您可以成功创建和使用自定义规格的Fractal标记系统。