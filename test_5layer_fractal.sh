#!/bin/bash

echo "=== 5层Fractal标记系统测试 ==="
echo

# 确保在build目录中
cd build

echo "1. 创建5层Fractal配置..."
./utils_fractal/fractal_create_5layer fractal_5layer_config.yml
echo

echo "2. 生成20cm标记图像..."
./utils_fractal/fractal_print_marker_20cm fractal_5layer_config.yml fractal_marker_20cm.png
echo

echo "3. 生成不同尺寸的标记（15cm和25cm）..."
./utils_fractal/fractal_print_marker_20cm fractal_5layer_config.yml fractal_marker_15cm.png -size 15.0
./utils_fractal/fractal_print_marker_20cm fractal_5layer_config.yml fractal_marker_25cm.png -size 25.0
echo

echo "4. 检查生成的文件..."
ls -la fractal_*.png fractal_*.yml
echo

echo "=== 使用说明 ==="
echo "打印标记："
echo "  - 使用300 DPI打印fractal_marker_20cm.png"
echo "  - 确保实际尺寸为20cm x 20cm"
echo
echo "运行检测："
echo "  ./utils_fractal/fractal_tracker_5layer live:0 -config fractal_5layer_config.yml -s 0.2 -cam camera.yml"
echo
echo "参数说明："
echo "  -config: 使用自定义5层配置"
echo "  -s 0.2:  标记实际尺寸（20cm = 0.2米）"
echo "  -cam:    相机标定文件（可选）"
echo
echo "控制键："
echo "  s:   暂停/继续"
echo "  ESC: 退出"
echo

echo "=== 测试完成 ==="