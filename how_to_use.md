# 在项目根目录下运行
./build/utils_fractal/fractal_tracker live:4

我写了个优化版本，打印tvec。

## 新增：5层Fractal标记系统

基于fractal_tracker_tvec的增强版本，支持5层嵌套和20cm大尺寸打印：

### 快速开始
```bash
# 1. 创建5层配置
./build/utils_fractal/fractal_create_5layer fractal_5layer_config.yml

# 2. 生成20cm标记
./build/utils_fractal/fractal_print_marker_20cm fractal_5layer_config.yml marker_20cm.png

# 3. 运行检测
./build/utils_fractal/fractal_tracker_5layer live:0 -config fractal_5layer_config.yml -s 0.2 -cam camera.yml
```

详细说明请参考：`fractal_5layer_usage.md`

## 编译
cmake .. || exit 1
cmake --build . -j$(nproc) || exit 1







现在用的是 CMake + Unix Makefiles 生成器。