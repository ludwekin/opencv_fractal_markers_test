#include "cvdrawingutils.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <opencv2/highgui/highgui.hpp>
#include "fractaldetector.h"
#include "aruco_cvversioning.h"

using namespace std;
using namespace cv;
using namespace aruco;

// ================== Timer ==================
struct TimerAvrg {
    std::vector<double> times;
    size_t curr = 0, n;
    std::chrono::high_resolution_clock::time_point begin, end;

    TimerAvrg(int _n = 30) {
        n = _n;
        times.reserve(n);
    }

    inline void start() {
        begin = std::chrono::high_resolution_clock::now();
    }

    inline void stop() {
        end = std::chrono::high_resolution_clock::now();
        double duration =
            double(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) * 1e-6;

        if (times.size() < n)
            times.push_back(duration);
        else {
            times[curr] = duration;
            curr = (curr + 1) % times.size();
        }
    }

    double getAvrg() {
        if (times.empty()) return 0;
        double sum = 0;
        for (auto t : times) sum += t;
        return sum / double(times.size());
    }
};

static TimerAvrg Tdetect, Tpose;

// ================== Resize ==================
cv::Mat __resize(const cv::Mat& in, int width) {
    if (in.cols <= width) return in;
    float scale = float(width) / float(in.cols);
    cv::Mat out;
    cv::resize(in, out, cv::Size(width, int(in.rows * scale)));
    return out;
}

// ================== Draw enhanced info for 5-layer fractal ==================
static void drawEnhancedInfo(cv::Mat& img, const cv::Mat& tvec, const cv::Mat& rvec, bool valid = true) {
    int startX = 30;
    int startY = 40;
    int lineHeight = 35;
    int currentY = startY;

    char buf[256];

    // 标题
    cv::putText(img, "5-Layer Fractal Tracker (20cm)", cv::Point(startX, currentY),
                cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 0), 2, cv::LINE_AA);
    currentY += lineHeight + 10;

    if (valid && !tvec.empty()) {
        cv::Mat tvec_d, rvec_d;
        if (tvec.type() != CV_64F)
            tvec.convertTo(tvec_d, CV_64F);
        else
            tvec_d = tvec;
            
        if (!rvec.empty()) {
            if (rvec.type() != CV_64F)
                rvec.convertTo(rvec_d, CV_64F);
            else
                rvec_d = rvec;
        }

        double x = tvec_d.at<double>(0, 0);
        double y = tvec_d.at<double>(1, 0);
        double z = tvec_d.at<double>(2, 0);
        double dist = cv::norm(tvec_d);

        // 位置信息
        snprintf(buf, sizeof(buf), "Position [m]: X=%.3f  Y=%.3f  Z=%.3f", x, y, z);
        cv::putText(img, buf, cv::Point(startX, currentY),
                    cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
        currentY += lineHeight;

        snprintf(buf, sizeof(buf), "Distance: %.3f m", dist);
        cv::putText(img, buf, cv::Point(startX, currentY),
                    cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 255), 2, cv::LINE_AA);
        currentY += lineHeight;

        // 旋转信息（如果有）
        if (!rvec_d.empty()) {
            double rx = rvec_d.at<double>(0, 0) * 180.0 / CV_PI;
            double ry = rvec_d.at<double>(1, 0) * 180.0 / CV_PI;
            double rz = rvec_d.at<double>(2, 0) * 180.0 / CV_PI;
            
            snprintf(buf, sizeof(buf), "Rotation [deg]: RX=%.1f  RY=%.1f  RZ=%.1f", rx, ry, rz);
            cv::putText(img, buf, cv::Point(startX, currentY),
                        cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 0, 255), 2, cv::LINE_AA);
            currentY += lineHeight;
        }

        // 绘制位置指示器（红色方框）
        double scale = 300.0; // 调整比例以适应20cm标记
        int cx = img.cols / 2 + int(x * scale);
        int cy = img.rows / 2 - int(y * scale);
        int boxSize = 15;

        // 确保方框在图像范围内
        cx = max(boxSize, min(img.cols - boxSize, cx));
        cy = max(boxSize, min(img.rows - boxSize, cy));

        cv::rectangle(img, cv::Point(cx - boxSize, cy - boxSize),
                      cv::Point(cx + boxSize, cy + boxSize),
                      cv::Scalar(0, 0, 255), 3);
                      
        // 绘制十字线
        cv::line(img, cv::Point(cx - boxSize*2, cy), cv::Point(cx + boxSize*2, cy),
                 cv::Scalar(0, 0, 255), 2);
        cv::line(img, cv::Point(cx, cy - boxSize*2), cv::Point(cx, cy + boxSize*2),
                 cv::Scalar(0, 0, 255), 2);
    } else {
        snprintf(buf, sizeof(buf), "Status: No marker detected");
        cv::putText(img, buf, cv::Point(startX, currentY),
                    cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    }
    
    // 性能信息
    currentY = img.rows - 80;
    snprintf(buf, sizeof(buf), "Detection: %.1f ms  |  Pose: %.1f ms", 
             Tdetect.getAvrg() * 1000, Tpose.getAvrg() * 1000);
    cv::putText(img, buf, cv::Point(startX, currentY),
                cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2, cv::LINE_AA);
}

// ================== CmdLine ==================
class CmdLineParser {
    int argc;
    char** argv;
public:
    CmdLineParser(int _argc, char** _argv) : argc(_argc), argv(_argv) {}

    bool operator[](string param) {
        for (int i = 0; i < argc; i++)
            if (string(argv[i]) == param) return true;
        return false;
    }

    string operator()(string param, string defvalue = "-1") {
        for (int i = 0; i < argc - 1; i++)
            if (string(argv[i]) == param)
                return argv[i + 1];
        return defvalue;
    }
};

// ================== main ==================
int main(int argc, char** argv) {
    try {
        CmdLineParser cml(argc, argv);

        if (argc < 2 || cml["-h"]) {
            cerr << "Usage: (video.avi|live[:index]) "
                 << "[-config fractal_config.yml] [-s markerSize_in_meters (0.2 default for 20cm)] "
                 << "[-cam camera.yml]" << endl;
            cerr << "5-Layer Fractal Tracker optimized for 20cm markers" << endl;
            return 0;
        }

        aruco::CameraParameters CamParam;
        VideoCapture vreader;
        Mat InImage;
        bool isVideo = false;

        // -------- Open input --------
        if (string(argv[1]).find("live") == string::npos) {
            vreader.open(argv[1]);
            isVideo = true;
        } else {
            string s = argv[1];
            replace(s.begin(), s.end(), ':', ' ');
            string tmp;
            int idx = 0;
            stringstream ss(s);
            ss >> tmp >> idx;
            vreader.open(idx);
        }

        if (!vreader.isOpened()) {
            cerr << "Could not open input" << endl;
            return -1;
        }

        vreader >> InImage;
        if (InImage.empty()) return -1;

        // -------- Camera params --------
        if (cml["-cam"])
            CamParam.readFromXMLFile(cml("-cam"));

        // 默认20cm标记
        float MarkerSize = stof(cml("-s", "0.2"));
        cout << "Marker size: " << MarkerSize << " meters (" << MarkerSize*100 << " cm)" << endl;

        FractalDetector FDetector;
        
        // 加载自定义配置或使用默认配置
        if (cml["-config"]) {
            string configFile = cml("-config");
            cout << "Loading fractal config: " << configFile << endl;
            
            aruco::FractalMarkerSet fractalSet;
            cv::FileStorage fs(configFile, cv::FileStorage::READ);
            if (fs.isOpened()) {
                fractalSet.readFromFile(configFile);
                FDetector.setConfiguration(configFile);
                fs.release();
            } else {
                cerr << "Warning: Could not load config file, using default" << endl;
                FDetector.setConfiguration("FRACTAL_2L_6");
            }
        } else {
            cout << "Using default fractal configuration" << endl;
            FDetector.setConfiguration("FRACTAL_2L_6");
        }

        if (CamParam.isValid()) {
            CamParam.resize(InImage.size());
            FDetector.setParams(CamParam, MarkerSize);
            cout << "Camera parameters loaded successfully" << endl;
        } else {
            cout << "Warning: No camera parameters loaded" << endl;
        }

        // -------- Loop --------
        int frameId = 0;
        int waitTime = 10;
        char key = 0;

        cout << "\n=== 5-Layer Fractal Tracker Started ===" << endl;
        cout << "Press 's' to toggle pause, ESC to exit" << endl;

        do {
            vreader.retrieve(InImage);
            if (InImage.empty()) break;

            cout << "\rFrame: " << frameId++ << flush;

            // ---- Detect ----
            Tdetect.start();
            bool detected = FDetector.detect(InImage);
            Tdetect.stop();

            if (detected) {
                FDetector.drawMarkers(InImage);
            }

            // ---- Pose ----
            Tpose.start();
            bool poseOK = FDetector.poseEstimation();
            Tpose.stop();

            cv::Mat tvec = FDetector.getTvec();
            cv::Mat rvec = FDetector.getRvec();

            if (poseOK) {
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

                FDetector.draw3d(InImage);
            } else {
                FDetector.draw2d(InImage);
            }

            // ---- Display ----
            Mat disp = __resize(InImage, 1600);
            drawEnhancedInfo(disp, tvec, rvec, poseOK);
            imshow("5-Layer Fractal Tracker (20cm)", disp);

            key = cv::waitKey(waitTime);
            if (key == 's') waitTime = (waitTime == 0) ? 10 : 0;

        } while (key != 27 && vreader.grab());
        
        cout << "\n=== Tracker Stopped ===" << endl;
    }
    catch (std::exception& ex) {
        cerr << "Exception: " << ex.what() << endl;
    }

    return 0;
}