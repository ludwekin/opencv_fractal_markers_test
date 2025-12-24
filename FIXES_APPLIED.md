# Fractal Tracker Fixes Applied

## Issues Fixed

### 1. Config File Not Found
**Problem**: The program was looking for `config.yml` but only `fractal_5layer_config.yml` existed.
**Solution**: Copied `fractal_5layer_config.yml` to `config.yml`.

### 2. OpenCV Assertion Error
**Problem**: Code was trying to access `tvec.at<float>()` but the matrix was of type `CV_64F` (double).
**Solution**: Added proper type conversion in `utils_fractal/fractal_tracker_5layer.cpp`:
```cpp
// Convert to double to avoid type issues
cv::Mat tvec_d;
if (tvec.type() != CV_64F)
    tvec.convertTo(tvec_d, CV_64F);
else
    tvec_d = tvec;
    
cout << " | Pose OK | tvec: [" 
     << tvec_d.at<double>(0) << ", " 
     << tvec_d.at<double>(1) << ", " 
     << tvec_d.at<double>(2) << "]" << flush;
```

### 3. Display Environment Issues
**Problem**: Qt platform plugin warnings and display issues.
**Solution**: Set proper environment variables:
- `DISPLAY=:0`
- `QT_QPA_PLATFORM=xcb`

## Files Created

1. **`config.yml`** - Copy of the fractal configuration file
2. **`run_fractal_tracker.sh`** - Easy-to-use wrapper script with proper environment setup
3. **`fix_fractal_tracker.sh`** - Script that applies all fixes
4. **`test_fixes.sh`** - Verification script to test all fixes

## Usage

### Simple Usage
```bash
./run_fractal_tracker.sh
```

### With Custom Parameters
```bash
./run_fractal_tracker.sh -c 0 -s 0.2    # Camera 0, 20cm marker
./run_fractal_tracker.sh -c 1 -s 0.15   # Camera 1, 15cm marker
```

### Help
```bash
./run_fractal_tracker.sh --help
```

## What the Wrapper Script Does

1. Sets proper display environment variables
2. Handles camera selection (default: camera 0)
3. Sets marker size (default: 0.2m = 20cm)
4. Uses the correct config file
5. Passes all parameters to the fractal tracker

## Testing

Run the test script to verify all fixes:
```bash
./test_fixes.sh
```

All fixes have been tested and verified to work correctly.