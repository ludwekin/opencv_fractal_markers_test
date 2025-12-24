#include "fractallabelers/fractalmarkerset.h"
#include "dictionary.h"
#include <cstdio>
#include <opencv2/highgui/highgui.hpp>
#include <string>

using namespace std;
using namespace cv;

class CmdLineParser
{
    int argc;char** argv;public:
    CmdLineParser(int _argc, char** _argv): argc(_argc), argv(_argv){}
    bool operator[](string param)
    {int idx = -1;   for (int i = 0; i < argc && idx == -1; i++)if (string(argv[i]) == param) idx = i;return (idx != -1); }
    string operator()(string param, string defvalue = "-1"){int idx = -1;for (int i = 0; i < argc && idx == -1; i++)if (string(argv[i]) == param) idx = i;if (idx == -1) return defvalue;else return (argv[idx + 1]);}
};

int main(int argc, char** argv)
{
    try
    {
        CmdLineParser cml(argc, argv);
        if (argc < 2 || cml["-h"])
        {
            cerr << "Usage: output_config.yml [-s bitSize (For the last level, in px. Default: 50)]" << endl;
            cerr << "Creates a 5-layer fractal marker configuration optimized for 20cm printing" << endl;
            return -1;
        }

        // 5层fractal配置，针对20cm打印优化
        // 层级从外到内：更多的区域用于更好的检测
        std::vector<std::pair<int,int>> regionsConfig = {
            {16, 12},  // 第1层：16个区域，12个用于编码
            {14, 10},  // 第2层：14个区域，10个用于编码  
            {12, 8},   // 第3层：12个区域，8个用于编码
            {10, 6},   // 第4层：10个区域，6个用于编码
            {8, 4}     // 第5层：8个区域，4个用于编码
        };

        // 最小位大小，针对20cm打印优化
        int bitSize = stoi(cml("-s", "50"));

        cout << "Creating 5-layer fractal marker configuration:" << endl;
        for (size_t i = 0; i < regionsConfig.size(); i++) {
            cout << "Layer " << (i+1) << ": " << regionsConfig[i].first 
                 << " regions, " << regionsConfig[i].second << " for encoding" << endl;
        }
        cout << "Bit size: " << bitSize << " pixels" << endl;

        // 创建配置
        aruco::FractalMarkerSet fractalmarkerset;
        fractalmarkerset.create(regionsConfig, bitSize);

        // 保存配置文件
        cv::FileStorage fs(argv[1], cv::FileStorage::WRITE);
        fractalmarkerset.saveToFile(fs);
        
        cout << "Configuration saved to: " << argv[1] << endl;
        cout << "Use this config with fractal_print_marker_20cm and fractal_tracker_5layer" << endl;
    }
    catch (std::exception& ex)
    {
        cout << "Error: " << ex.what() << endl;
        return -1;
    }
    
    return 0;
}