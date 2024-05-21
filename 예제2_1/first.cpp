#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include <iostream>
#include <string>
#include <format>
/*

using namespace cv;
using namespace std;

string getFileName(const std::string& path)
{
    size_t pos = path.find_last_of("\\/");
    if (pos == std::string::npos)
        return path;
    else
        return path.substr(pos + 1);
}

void processImage(const std::string& imagePath, const std::string& outputPath)
{
    // 이미지를 로드합니다.
    cv::Mat image = cv::imread(imagePath);
    if (image.empty())
    {
        std::cout << "Could not open or find the image: " << imagePath << std::endl;
        return;
    }

    // 초기 바운딩 박스를 정의합니다. (사용자가 설정한 초기 전경 영역)
    cv::Rect rectangle(50, 50, image.cols , image.rows - 100);

    // 필요한 변수들을 초기화합니다.
    cv::Mat result; // 결과 마스크
    cv::Mat bgModel, fgModel; // 배경 및 전경 모델

    // grabCut을 실행합니다.
    cv::grabCut(image,    // 입력 이미지
        result,   // 출력 마스크
        rectangle,// 초기 바운딩 박스
        bgModel,  // 배경 모델
        fgModel,  // 전경 모델
        5,        // 반복 횟수
        cv::GC_INIT_WITH_RECT); // 초기화 플래그

    // 마스크를 전경과 배경으로 분리합니다.
    cv::Mat binMask;
    cv::compare(result, cv::GC_PR_FGD, binMask, cv::CMP_EQ);

    // 전경을 파란색으로 설정합니다.
    cv::Mat blue(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
    blue.setTo(cv::Scalar(255, 0, 0), binMask);

    // 원본 이미지와 파란색 전경을 합성합니다.
    cv::Mat output;
    addWeighted(blue, 0.5, image, 0.5, 0.0, output);

    // 결과 이미지를 파일로 저장합니다.
    cv::imwrite(outputPath, output);

    std::cout << "Processed and saved: " << outputPath << std::endl;
}


int main()
{
    // 이미지가 있는 폴더 경로를 지정합니다.
    String folderPath = "C:\\computer_vision\\Case 2\\img_png\\";

    // 처리된 이미지가 저장될 폴더 경로를 저장
    String outputPath = "C:\\computer_vision\\Case 2\\output\\";
    vector<String> filenames;

    // 폴더 내의 모든 PNG 파일을 검색합니다.
    glob(folderPath + "*.png", filenames, false);

    if (filenames.empty())
    {
        cout << "No PNG files found in the folder." << endl;
        return -1;
    }

    // 각 파일을 처리합니다.
    int count = 0;
    for (const auto& file : filenames)
    {
        // 출력 파일 경로를 설정합니다.
       
        string fileName = getFileName(file);
        string outputFilePath = outputPath + fileName;

        // 이미지를 처리하고 저장합니다.

        // 이미지를 처리하고 저장합니다.
        processImage(file, outputFilePath);
        count += 1;
    }

    return 0;
}
*/