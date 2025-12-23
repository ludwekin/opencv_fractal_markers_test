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

// ================== Draw tvec ==================
static void drawTvecInfo(cv::Mat& img, const cv::Mat& tvec, bool valid = true) {
    int startX = 30;
    int startY = 40;
    int lineHeight = 40;

    char buf[128];

    if (valid && !tvec.empty()) {
        cv::Mat tvec_d;
        if (tvec.type() != CV_64F)
            tvec.convertTo(tvec_d, CV_64F);
        else
            tvec_d = tvec;

        double x = tvec_d.at<double>(0, 0);
        double y = tvec_d.at<double>(1, 0);
        double z = tvec_d.at<double>(2, 0);
        double dist = cv::norm(tvec_d);

        snprintf(buf, sizeof(buf), "tvec [m]: X=%.3f  Y=%.3f  Z=%.3f", x, y, z);
        cv::putText(img, buf, cv::Point(startX, startY),
                    cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(0, 255, 0), 2, cv::LINE_AA);

        snprintf(buf, sizeof(buf), "Distance: %.3f m", dist);
        cv::putText(img, buf, cv::Point(startX, startY + lineHeight),
                    cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(0, 255, 255), 2, cv::LINE_AA);

        // ---- Draw red box representing tvec position ----
        // 假设图像中心为相机光心，XY 映射为像素比例（可调 scale）
        double scale = 500.0; // 可根据实际距离调整
        int cx = img.cols / 2 + int(x * scale);
        int cy = img.rows / 2 - int(y * scale); // Y 方向取负，图像上方是减
        int boxSize = 10;

        cv::rectangle(img, cv::Point(cx - boxSize, cy - boxSize),
                      cv::Point(cx + boxSize, cy + boxSize),
                      cv::Scalar(0, 0, 255), 2);
    } else {
        snprintf(buf, sizeof(buf), "tvec: N/A");
        cv::putText(img, buf, cv::Point(startX, startY),
                    cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    }
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
                 << "[-s fractalSize] [-cam camera.yml] "
                 << "[-c CONFIG:FRACTAL_2L_6]" << endl;
            cerr << "Available configurations: ";
            for (auto& c : aruco::FractalMarkerSet::getConfigurations())
                cerr << c << " ";
            cerr << endl;
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

        float MarkerSize = stof(cml("-s", "-1"));

        FractalDetector FDetector;
        FDetector.setConfiguration(cml("-c", "FRACTAL_2L_6"));

        if (CamParam.isValid()) {
            CamParam.resize(InImage.size());
            FDetector.setParams(CamParam, MarkerSize);
        }

        // -------- Loop --------
        int frameId = 0;
        int waitTime = 10;
        char key = 0;

        do {
            vreader.retrieve(InImage);
            if (InImage.empty()) break;

            cout << "\nFrameId: " << frameId++ << endl;

            // ---- Detect ----
            Tdetect.start();
            bool detected = FDetector.detect(InImage);
            Tdetect.stop();

            if (detected) {
                cout << "Detection time: " << Tdetect.getAvrg() * 1000 << " ms" << endl;
                FDetector.drawMarkers(InImage);
            }

            // ---- Pose ----
            Tpose.start();
            bool poseOK = FDetector.poseEstimation();
            Tpose.stop();

            cv::Mat tvec = FDetector.getTvec(); // always get tvec

            if (poseOK) {
                cout << "Pose time: " << Tpose.getAvrg() * 1000 << " ms" << endl;
                cout << "tvec = " << tvec.t() << endl;

                FDetector.draw3d(InImage);
            } else {
                FDetector.draw2d(InImage);
            }

            // ---- Ensure tvec always displayed + mark red box ----
            Mat disp = __resize(InImage, 1800);
            drawTvecInfo(disp, tvec, poseOK);
            imshow("Fractal Marker", disp);

            key = cv::waitKey(waitTime);
            if (key == 's') waitTime = (waitTime == 0) ? 10 : 0;

        } while (key != 27 && vreader.grab());
    }
    catch (std::exception& ex) {
        cerr << "Exception: " << ex.what() << endl;
    }

    return 0;
}
