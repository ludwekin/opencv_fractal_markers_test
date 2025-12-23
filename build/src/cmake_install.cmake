# Install script for directory: /home/uni/桌面/opencv_fractal_markers_test/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "main" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libaruco.so.3.1.12"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libaruco.so.3.1"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES
    "/home/uni/桌面/opencv_fractal_markers_test/build/src/libaruco.so.3.1.12"
    "/home/uni/桌面/opencv_fractal_markers_test/build/src/libaruco.so.3.1"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libaruco.so.3.1.12"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libaruco.so.3.1"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "main" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/home/uni/桌面/opencv_fractal_markers_test/build/src/libaruco.so")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aruco" TYPE FILE FILES
    "/home/uni/桌面/opencv_fractal_markers_test/src/aruco_cvversioning.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/cameraparameters.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/dictionary_based.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/ippe.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/markerdetector_impl.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/markermap.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/timers.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/aruco_export.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/cvdrawingutils.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/dictionary.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/levmarq.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/marker.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/picoflann.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/aruco.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/debug.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/markerdetector.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/markerlabeler.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/posetracker.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/fractaldetector.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aruco/fractallabelers" TYPE FILE FILES
    "/home/uni/桌面/opencv_fractal_markers_test/src/fractallabelers/fractalposetracker.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/fractallabelers/fractalmarkerset.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/fractallabelers/fractalmarker.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/fractallabelers/fractallabeler.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aruco/dcf" TYPE FILE FILES
    "/home/uni/桌面/opencv_fractal_markers_test/src/dcf/dcfmarkermaptracker.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/dcf/dcfmarkertracker.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/dcf/dcf_utils.h"
    "/home/uni/桌面/opencv_fractal_markers_test/src/dcf/trackerimpl.h"
    )
endif()

