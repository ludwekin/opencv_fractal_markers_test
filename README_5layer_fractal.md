# 5层Fractal标记系统

基于现有的`fractal_tracker_tvec`程序，我创建了一个增强版的5层Fractal标记系统，专门针对更大的fractal码（5个叠层，最大边长20cm）进行优化。

## 新增程序

### 1. fractal_create_5layer
创建5层Fractal配置文件
```bash
./build/utils_fractal/fractal_create_5layer fractal_5layer_config.yml
```

**特点：**
- 5层嵌套结构：16→14→12→10→8个区域
- 编码位数：12→10→8→6→4个编码区域
- 总共40个编码位，支持更多标记ID
- 针对20cm打印优化的位大小

### 2. fractal_print_marker_20cm
生成高质量打印标记
```bash
./build/utils_fractal/fractal_print_marker_20cm config.yml output.png [选项]
```

**选项：**
- `-dpi 300`: 设置DPI（默认300）
- `-size 20.0`: 设置尺寸厘米（默认20cm）
- `-noborder`: 移除白色边框

**特点：**
- 300 DPI高质量输出
- 精确尺寸控制
- 自动计算最优位大小
- 支持15cm、20cm、25cm等多种尺寸

### 3. fractal_tracker_5layer
增强版检测程序
```bash
./build/utils_fractal/fractal_tracker_5layer live:0 -config config.yml -s 0.2 -cam camera.yml
```

**增强功能：**
- 实时显示位置信息（X, Y, Z坐标）
- 距离测量
- 旋转角度显示（RX, RY, RZ）
- 可视化位置指示器（红色方框+十字线）
- 性能监控（检测和姿态估计时间）
- 增强的用户界面

## 快速开始

1. **编译项目**
```bash
cd build
cmake .. && cmake --build . -j$(nproc)
```

2. **创建配置并生成标记**
```bash
# 创建5层配置
./utils_fractal/fractal_create_5layer fractal_5layer_config.yml

# 生成20cm标记
./utils_fractal/fractal_print_marker_20cm fractal_5layer_config.yml marker_20cm.png
```

3. **打印标记**
- 使用300 DPI打印`marker_20cm.png`
- 确保实际尺寸为20cm × 20cm

4. **运行检测**
```bash
./utils_fractal/fractal_tracker_5layer live:0 -config fractal_5layer_config.yml -s 0.2
```

## 技术优势

### 相比2层Fractal的改进：
1. **更高精度**：5层结构提供更精确的角点检测
2. **更好鲁棒性**：部分层级被遮挡时仍能检测
3. **更大检测范围**：适合20cm大尺寸的远距离检测
4. **更多编码信息**：40个编码位 vs 12个编码位

### 打印优化：
1. **高质量输出**：300 DPI无压缩PNG格式
2. **精确尺寸**：自动计算像素数确保准确尺寸
3. **多尺寸支持**：15cm、20cm、25cm等
4. **边框控制**：可选白色边框提高检测率

### 检测增强：
1. **实时tvec显示**：精确的位置向量
2. **距离测量**：到标记的直线距离
3. **角度信息**：完整的旋转向量
4. **可视化指示**：直观的位置标记
5. **性能监控**：实时性能指标

## 文件结构

```
utils_fractal/
├── fractal_create_5layer.cpp      # 5层配置创建
├── fractal_print_marker_20cm.cpp  # 20cm标记生成
├── fractal_tracker_5layer.cpp     # 5层检测程序
└── CMakeLists.txt                  # 构建配置

生成文件/
├── fractal_5layer_config.yml      # 5层配置文件
├── fractal_marker_20cm.png        # 20cm标记图像
├── fractal_marker_15cm.png        # 15cm标记图像
└── fractal_marker_25cm.png        # 25cm标记图像
```

## 使用建议

1. **相机标定**：使用高质量相机标定文件获得准确tvec
2. **打印质量**：确保300 DPI打印，避免模糊
3. **光照条件**：保持均匀光照，避免强烈阴影
4. **标记平整**：确保打印标记平整贴合表面
5. **距离范围**：20cm标记适合0.5-3米检测距离

## 控制说明

- `s`: 暂停/继续检测
- `ESC`: 退出程序

## 故障排除

- **检测失败**：检查光照、标记清晰度、相机焦距
- **tvec不准确**：确认相机标定文件和标记尺寸参数
- **编译错误**：确保OpenCV 4.6+和依赖库已安装

---

这个5层Fractal系统为你提供了更强大的标记检测能力，特别适合需要高精度位置检测的应用场景。