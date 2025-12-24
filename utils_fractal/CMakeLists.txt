cmake_minimum_required(VERSION 3.10)
project(OpencvFractalMarkers LANGUAGES CXX)

# ------------------ 编译选项 ------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ------------------ 头文件路径 ------------------
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/utils_fractal
)

# ------------------ 查找 OpenCV ------------------
find_package(OpenCV 4.6 REQUIRED)
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV 4.6 not found!")
endif()

# ------------------ 线程库 ------------------
if(CMAKE_COMPILER_IS_GNUCXX OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(THREADLIB pthread)
endif()

# ------------------ 可执行文件 ------------------
add_executable(fractal_create        fractal_create.cpp)
add_executable(fractal_tracker       fractal_tracker.cpp)
add_executable(fractal_print_marker  fractal_print_marker.cpp)
add_executable(fractal_pix2meters    fractal_pix2meters.cpp)
add_executable(fractal_tracker_tvec  fractal_tracker_tvec.cpp)

# ------------------ 新的5层fractal程序 ------------------
add_executable(fractal_create_5layer      fractal_create_5layer.cpp)
add_executable(fractal_print_marker_20cm  fractal_print_marker_20cm.cpp)
add_executable(fractal_tracker_5layer     fractal_tracker_5layer.cpp)

# ------------------ 链接库 ------------------
target_link_libraries(fractal_create       aruco ${OpenCV_LIBS} ${THREADLIB})
target_link_libraries(fractal_tracker      aruco ${OpenCV_LIBS} ${THREADLIB})
target_link_libraries(fractal_print_marker aruco ${OpenCV_LIBS} ${THREADLIB})
target_link_libraries(fractal_pix2meters   aruco ${OpenCV_LIBS} ${THREADLIB})
target_link_libraries(fractal_tracker_tvec aruco ${OpenCV_LIBS} ${THREADLIB})

# ------------------ 新程序链接库 ------------------
target_link_libraries(fractal_create_5layer      aruco ${OpenCV_LIBS} ${THREADLIB})
target_link_libraries(fractal_print_marker_20cm  aruco ${OpenCV_LIBS} ${THREADLIB})
target_link_libraries(fractal_tracker_5layer     aruco ${OpenCV_LIBS} ${THREADLIB})

# ------------------ 安装 ------------------
install(TARGETS fractal_create
        RUNTIME DESTINATION bin)

install(TARGETS fractal_tracker
        RUNTIME DESTINATION bin)

install(TARGETS fractal_print_marker
        RUNTIME DESTINATION bin)

install(TARGETS fractal_pix2meters
        RUNTIME DESTINATION bin)

install(TARGETS fractal_tracker_tvec
        RUNTIME DESTINATION bin)

# ------------------ 新程序安装 ------------------
install(TARGETS fractal_create_5layer
        RUNTIME DESTINATION bin)

install(TARGETS fractal_print_marker_20cm
        RUNTIME DESTINATION bin)

install(TARGETS fractal_tracker_5layer
        RUNTIME DESTINATION bin)
