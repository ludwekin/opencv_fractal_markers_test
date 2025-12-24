#!/bin/bash

echo "=== Fixing Fractal Tracker Issues ==="

# 1. Copy the config file to the expected location
echo "1. Copying config file..."
cp fractal_5layer_config.yml config.yml
echo "   ✓ Copied fractal_5layer_config.yml to config.yml"

# 2. Set display environment for GUI applications
echo "2. Setting display environment..."
export DISPLAY=:0
export QT_QPA_PLATFORM=xcb
echo "   ✓ Set DISPLAY=:0 and QT_QPA_PLATFORM=xcb"

# 3. Create a wrapper script for the tracker
echo "3. Creating wrapper script..."
cat > run_fractal_tracker.sh << 'EOF'
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
EOF

chmod +x run_fractal_tracker.sh
echo "   ✓ Created run_fractal_tracker.sh wrapper script"

echo ""
echo "=== Fix Complete ==="
echo ""
echo "Now you can run the tracker using:"
echo "  ./run_fractal_tracker.sh"
echo ""
echo "Or with custom parameters:"
echo "  ./run_fractal_tracker.sh -c 0 -s 0.2"
echo ""
echo "The script will automatically:"
echo "  - Use the correct config file"
echo "  - Set proper display environment"
echo "  - Handle camera selection"