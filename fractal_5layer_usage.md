# 5层Fractal标记系统使用说明

这是一个基于现有fractal_tracker_tvec的增强版本，专门为生成和检测5层fractal码设计，支持20cm大尺寸打印。

## 特性

- **5层嵌套结构**：更多的编码层级，提供更好的检测精度
- **20cm优化**：专门针对20cm打印尺寸优化
- **高精度检测**：增强的tvec输出和可视化
- **打印友好**：支持300 DPI高质量打印输出

## 编译

在项目根目录下运行：

```bash
cd build
cmake .. || exit 1
cmake --build . -j$(nproc) || exit 1
```

## 使用步骤

### 1. 创建5层Fractal配置

```bash
./build/utils_fractal/fractal_create_5layer fractal_5layer_config.yml
```

这将创建一个5层配置文件，层级结构为：
- 第1层：16个区域，12个用于编码
- 第2层：14个区域，10个用于编码  
- 第3层：12个区域，8个用于编码
- 第4层：10个区域，6个用于编码
- 第5层：8个区域，4个用于编码

### 2. 生成20cm打印标记

```bash
./build/utils_fractal/fractal_print_marker_20cm fractal_5layer_config.yml fractal_marker_20cm.png
```

可选参数：
- `-dpi 300`：设置DPI（默认300）
- `-size 20.0`：设置尺寸（默认20cm）
- `-noborder`：移除白色边框

示例：
```bash
./build/utils_fractal/fractal_print_marker_20cm fractal_5layer_config.yml fractal_marker_20cm.png -dpi 300 -size 20.0
```

### 3. 打印标记

将生成的PNG文件以300 DPI打印，确保实际尺寸为20cm x 20cm。

### 4. 运行检测程序

```bash
./build/utils_fractal/fractal_tracker_5layer live:0 -config fractal_5layer_config.yml -s 0.2 -cam camera.yml
```

参数说明：
- `live:0`：使用摄像头0，也可以使用视频文件
- `-config fractal_5layer_config.yml`：使用自定义5层配置
- `-s 0.2`：标记实际尺寸（0.2米 = 20cm）
- `-cam camera.yml`：相机标定文件

## 输出信息

程序会实时显示：
- **位置信息**：X, Y, Z坐标（米）
- **距离**：到标记的直线距离
- **旋转信息**：RX, RY, RZ角度（度）
- **性能指标**：检测和姿态估计时间
- **可视化**：红色方框和十字线显示标记位置

## 控制键

- `s`：暂停/继续
- `ESC`：退出程序

## 配置层级说明

5层配置相比2层配置的优势：
1. **更高精度**：更多层级提供更精确的角点检测
2. **更好鲁棒性**：即使部分层级被遮挡，仍能检测
3. **更大检测范围**：适合20cm大尺寸标记的远距离检测
4. **更多编码信息**：总共40个编码位，支持更多标记ID

## 注意事项

1. **相机标定**：使用高质量的相机标定文件以获得准确的tvec
2. **打印质量**：确保300 DPI打印质量，避免模糊
3. **光照条件**：保持均匀光照，避免强烈阴影
4. **标记平整**：确保打印的标记平整贴合表面

## 故障排除

- **检测失败**：检查光照、标记清晰度、相机焦距
- **tvec不准确**：确认相机标定文件正确，标记尺寸参数正确
- **编译错误**：确保OpenCV 4.6+和所有依赖库已安装