# 在项目根目录执行（zsh）
mkdir -p build
cd build
cmake .. || exit 1
cmake --build . -j$(nproc) || exit 1

# 在构建目录中寻找 fractal_tracker 可执行文件并运行，使用你的摄像头编号 4
EXE=$(find . -type f -executable -name fractal_tracker -print -quit)
if [ -z "$EXE" ]; then
  echo "fractal_tracker 可执行文件未找到，请确认 cmake 构建成功。"
  exit 1
fi
# 运行 fractal_tracker 并打开摄像头编号 4（程序支持参数 "live:INDEX"）
"$EXE" live:4