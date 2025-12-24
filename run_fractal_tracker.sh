#!/bin/bash

# Set environment variables
export DISPLAY=:0
export QT_QPA_PLATFORM=xcb

# Default parameters
CAMERA_INDEX=0
MARKER_SIZE=0.2
CONFIG_FILE="config.yml"
CAMERA_FILE="camera.yml"

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -c|--camera)
            CAMERA_INDEX="$2"
            shift 2
            ;;
        -s|--size)
            MARKER_SIZE="$2"
            shift 2
            ;;
        -config)
            CONFIG_FILE="$2"
            shift 2
            ;;
        -cam)
            CAMERA_FILE="$2"
            shift 2
            ;;
        -h|--help)
            echo "Usage: $0 [options]"
            echo "Options:"
            echo "  -c, --camera INDEX    Camera index (default: 0)"
            echo "  -s, --size SIZE       Marker size in meters (default: 0.2)"
            echo "  -config FILE          Config file (default: config.yml)"
            echo "  -cam FILE             Camera parameters file (default: camera.yml)"
            echo "  -h, --help            Show this help"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

echo "Starting 5-Layer Fractal Tracker..."
echo "Camera: $CAMERA_INDEX, Marker size: ${MARKER_SIZE}m, Config: $CONFIG_FILE"

# Run the tracker
./build/utils_fractal/fractal_tracker_5layer live:$CAMERA_INDEX -config $CONFIG_FILE -s $MARKER_SIZE -cam $CAMERA_FILE
