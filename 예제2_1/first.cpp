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
    // �̹����� �ε��մϴ�.
    cv::Mat image = cv::imread(imagePath);
    if (image.empty())
    {
        std::cout << "Could not open or find the image: " << imagePath << std::endl;
        return;
    }

    // �ʱ� �ٿ�� �ڽ��� �����մϴ�. (����ڰ� ������ �ʱ� ���� ����)
    cv::Rect rectangle(50, 50, image.cols , image.rows - 100);

    // �ʿ��� �������� �ʱ�ȭ�մϴ�.
    cv::Mat result; // ��� ����ũ
    cv::Mat bgModel, fgModel; // ��� �� ���� ��

    // grabCut�� �����մϴ�.
    cv::grabCut(image,    // �Է� �̹���
        result,   // ��� ����ũ
        rectangle,// �ʱ� �ٿ�� �ڽ�
        bgModel,  // ��� ��
        fgModel,  // ���� ��
        5,        // �ݺ� Ƚ��
        cv::GC_INIT_WITH_RECT); // �ʱ�ȭ �÷���

    // ����ũ�� ����� ������� �и��մϴ�.
    cv::Mat binMask;
    cv::compare(result, cv::GC_PR_FGD, binMask, cv::CMP_EQ);

    // ������ �Ķ������� �����մϴ�.
    cv::Mat blue(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
    blue.setTo(cv::Scalar(255, 0, 0), binMask);

    // ���� �̹����� �Ķ��� ������ �ռ��մϴ�.
    cv::Mat output;
    addWeighted(blue, 0.5, image, 0.5, 0.0, output);

    // ��� �̹����� ���Ϸ� �����մϴ�.
    cv::imwrite(outputPath, output);

    std::cout << "Processed and saved: " << outputPath << std::endl;
}


int main()
{
    // �̹����� �ִ� ���� ��θ� �����մϴ�.
    String folderPath = "C:\\computer_vision\\Case 2\\img_png\\";

    // ó���� �̹����� ����� ���� ��θ� ����
    String outputPath = "C:\\computer_vision\\Case 2\\output\\";
    vector<String> filenames;

    // ���� ���� ��� PNG ������ �˻��մϴ�.
    glob(folderPath + "*.png", filenames, false);

    if (filenames.empty())
    {
        cout << "No PNG files found in the folder." << endl;
        return -1;
    }

    // �� ������ ó���մϴ�.
    int count = 0;
    for (const auto& file : filenames)
    {
        // ��� ���� ��θ� �����մϴ�.
       
        string fileName = getFileName(file);
        string outputFilePath = outputPath + fileName;

        // �̹����� ó���ϰ� �����մϴ�.

        // �̹����� ó���ϰ� �����մϴ�.
        processImage(file, outputFilePath);
        count += 1;
    }

    return 0;
}
*/