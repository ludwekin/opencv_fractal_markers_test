#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <fractallabelers/fractalmarkerset.h>

using namespace cv;
using namespace std;

class CmdLineParser
{
    int argc;
    char** argv;
public:
    CmdLineParser(int _argc, char** _argv)
          : argc(_argc)
          , argv(_argv)
    {
    }
    bool operator[](string param)
    {
        int idx = -1;
        for (int i = 0; i < argc && idx == -1; i++)
            if (string(argv[i]) == param)
                idx = i;
        return (idx != -1);
    }
    string operator()(string param, string defvalue = "-1")
    {
        int idx = -1;
        for (int i = 0; i < argc && idx == -1; i++)
            if (string(argv[i]) == param)
                idx = i;
        if (idx == -1)
            return defvalue;
        else
            return (argv[idx + 1]);
    }
};

int main(int argc, char** argv)
{
    try
    {
        CmdLineParser cml(argc, argv);

        if (argc < 3)
        {
            cerr << "Usage: config.yml outfile.(jpg|png|ppm|bmp) [-dpi DPI (300 default)] [-size SIZE_CM (20 default)]"
                 << " [-noborder: removes the white border around the marker]" << endl;
            cerr << "Creates a fractal marker optimized for printing at specified size" << endl;
            return -1;
        }

        string configFile = argv[1];
        string outputFile = argv[2];
        
        // 打印参数
        int dpi = stoi(cml("-dpi", "300"));
        float sizeCm = stof(cml("-size", "20.0"));
        bool addBorder = !cml["-noborder"];
        
        cout << "Creating fractal marker for printing:" << endl;
        cout << "Size: " << sizeCm << " cm" << endl;
        cout << "DPI: " << dpi << endl;
        cout << "Border: " << (addBorder ? "Yes" : "No") << endl;

        // 加载fractal配置
        aruco::FractalMarkerSet fractalmarkerSet;
        if (configFile.find(".yml") != string::npos || configFile.find(".yaml") != string::npos) {
            fractalmarkerSet = aruco::FractalMarkerSet::readFromFile(configFile);
        } else {
            fractalmarkerSet = aruco::FractalMarkerSet::load(configFile);
        }

        // 计算像素大小
        // sizeCm * dpi / 2.54 = 总像素数
        int totalPixels = int(sizeCm * dpi / 2.54);
        
        // 根据fractal层数估算最小位大小
        // 对于5层fractal，外层需要更多像素
        int bitSize = totalPixels / 32; // 经验值，可调整
        
        cout << "Total image size: " << totalPixels << "x" << totalPixels << " pixels" << endl;
        cout << "Bit size: " << bitSize << " pixels" << endl;

        // 生成fractal标记
        cv::Mat result = fractalmarkerSet.getFractalMarkerImage(bitSize, addBorder);
        
        // 如果需要，调整到精确的打印尺寸
        if (result.rows != totalPixels || result.cols != totalPixels) {
            cv::Mat resized;
            cv::resize(result, resized, cv::Size(totalPixels, totalPixels), 0, 0, cv::INTER_NEAREST);
            result = resized;
        }
        
        // 保存图像
        vector<int> compression_params;
        compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
        compression_params.push_back(0); // 无压缩，保证打印质量
        
        bool success = cv::imwrite(outputFile, result, compression_params);
        
        if (success) {
            cout << "Fractal marker saved to: " << outputFile << endl;
            cout << "Image size: " << result.cols << "x" << result.rows << " pixels" << endl;
            cout << "Print this at " << dpi << " DPI for " << sizeCm << " cm size" << endl;
        } else {
            cerr << "Error: Failed to save image" << endl;
            return -1;
        }
    }
    catch (std::exception& ex)
    {
        cout << "Error: " << ex.what() << endl;
        return -1;
    }
    
    return 0;
}