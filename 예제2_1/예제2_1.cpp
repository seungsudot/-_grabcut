#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

struct Data_t {
	Mat image;
	Mat mask;

	Point prevPt;
	Rect initRect;
	bool is_draw ;
	Mat bgdModel, fgdModel;
	Mat res;

	Data_t() : initRect(0, 0, 0, 0), is_draw(false) {}

};

string getFileName(const std::string& path)
{
	size_t pos = path.find_last_of("\\/");
	if (pos == std::string::npos)
		return path;
	else
		return path.substr(pos + 1);
}


void mouseEvent(int event, int x, int y, int flags, void* userData)
{
	


	Data_t* data = (Data_t*)userData;
	
	Mat& mask = data->mask;
	Mat& dst = data->image;
	Point& prevPt = data->prevPt;
	Rect& initRect = data->initRect;
	bool& is_draw = data->is_draw;
	Mat& bgdModel = data->bgdModel;
	Mat& fgdModel = data->fgdModel;
	Mat& res = data->res;




	if (is_draw == false ) {
		switch (event) {
		case EVENT_LBUTTONDOWN:
			initRect.x = x;
			initRect.y = y;
			break;
		case EVENT_LBUTTONUP:
		{
			initRect.width = x - initRect.x;
			initRect.height = y - initRect.y;

			is_draw = true;
			// 마스크를 확실한 배경으로 설정
			mask.setTo(cv::GC_BGD);

			// 전경을 나타내는 사각형의 좌표 설정
			// x 좌표와 y 좌표가 모두 이미지의 크기를 넘지 않게 조정
			initRect.x = max(0, initRect.x);
			initRect.y = max(0, initRect.y);
			initRect.width = min(initRect.width, mask.cols - initRect.x);
			initRect.height = min(initRect.height, mask.rows - initRect.y);

			// 여기서 구한 사각형을 마스크로 해서 잠재적 전경 설정
			(mask(initRect)).setTo(Scalar(GC_PR_FGD));

			// 해당 정보를 바탕으로 grabCut 함수 실행
			grabCut(dst, mask, initRect, bgdModel, fgdModel, 1, cv::GC_INIT_WITH_RECT);

			// 전경 픽셀이 1로 설정된 이진마스크
			Mat binMask = mask & 1;
			// 배경을 까만색으로 만들고
			Mat blue(binMask.rows, binMask.cols, CV_8UC3, cv::Scalar(0, 0, 0));
			// 전경은 파란색으로 만듬 
			blue.setTo(cv::Scalar(255,0,0), binMask);
			res = dst.clone();

			// 이걸 흐릿하게 만들어서 res에 다시 저장
			addWeighted(blue, 0.5, res, 0.5, 0.0, res);
			rectangle(res, initRect, cv::Scalar(0, 0, 255), 2);
			imshow("src", res);
		}

		break;
		// 마우스가 움직일때 현재 드래그 중인 사각형을 화면에 실시간 표시
		case EVENT_MOUSEMOVE:
			if (flags & EVENT_FLAG_LBUTTON) {
				Mat ddst = dst.clone();
				rectangle(ddst, Rect(initRect.x, initRect.y, x - initRect.x, y - initRect.y), Scalar(0, 0, 255), 2);
				imshow("src", ddst);
			}
			break;
		}
	}
	// else : 초기 설정이 끝난경우
	else {
		// event에 따라 결정
		switch (event) { 

		// 마우스 왼쪽 버튼이 눌리면 현재 커서 위치를 prevPt에 저장
		case EVENT_LBUTTONDOWN:
			prevPt = Point(x, y);
			break;
		// 마우스가 움직이면
		case EVENT_MOUSEMOVE:
			// 움직이는데 -> 컨트롤이 눌려져 있다.
			if ((flags & EVENT_FLAG_LBUTTON) && (flags & EVENT_FLAG_CTRLKEY)) {
				// Background
				cv::line(res, prevPt, Point(x, y), Scalar(0, 255, 255), 12);
				cv::line(mask, prevPt, Point(x, y), Scalar(cv::GC_BGD), 12, -1);
				prevPt = Point(x, y);
			}
			// 움직이는데 쉬프트가 눌려져 있다. -> 전경
			else if ((flags & cv::EVENT_FLAG_LBUTTON) && (flags & cv::EVENT_FLAG_SHIFTKEY)) {
				// Foreground
				// line( 그릴 영상, 시작점 좌표, 끝점 좌표, 색깔, 굵기)
				cv::line(res, prevPt, Point(x, y), Scalar(255, 255, 0), 12);
				cv::line(mask, prevPt, Point(x, y), Scalar(cv::GC_FGD), 12, -1);
				prevPt = Point(x, y);
			}
		}
		imshow("src", res);
		
	}
}

int main()
{
	// 이미지가 있는 폴더 경로를 지정
	String folderPath = "C:\\computer_vision\\Case 3\\img_png\\";
	// 처리된 이미지 폴더 경로 지정
	String outputPath = "C:\\computer_vision\\Case 3\\output\\";
	vector<String> filenames;

	// 폴더 내의 모든 PNG 파일을 검색한다.
	glob(folderPath + "*.png", filenames, false);

	if (filenames.empty())
	{
		cout << "No PNG files found in the folder." << endl;
		return -1;
	}



	
	const int startIndex = 0; // 어디서 부터 돌지 -> 중간부터 다시 가능
	try {
		// 파일네임에 들어있는 파일을 전부 돈다.
		for (size_t i = startIndex; i < filenames.size(); ++i)
		{


			const auto& file = filenames[i];
			string fileName = getFileName(file);
			// outputFilePath 는 완성된 파일 경로
			String inputFilePath = folderPath + fileName;
			string outputFilePath = outputPath + "output_" + fileName;

			Mat src = imread(inputFilePath);

			imshow("src", src);
			// dst 는 src의 복사본
			Mat dst = src.clone();
			// mask는 src 의 사이즈에 다 0으로 채운 값
			Mat mask = Mat::zeros(src.size(), CV_8UC1);
			// Data_t  구조체에 dst와 mask를 넣는다.
			Data_t data;
			data.image = dst;
			data.mask = mask;

			// callback 함수로 mouseEvent를 실시
			setMouseCallback("src", mouseEvent, (void*)&data);
			Mat resultImg;




			bool bEscKey = false;
			int nKey;
			while (!bEscKey) {
				nKey = cv::waitKey(0);
				switch (nKey) {
				case 'e' : {		// esc
					// 지금 까지 했던 걸 저장 
					Mat binMask = mask & 1;
					Mat blue(binMask.rows, binMask.cols, CV_8UC3, Scalar(0, 0, 0));
					blue.setTo(Scalar(255, 0, 0), binMask);
					resultImg = 0;
					src.copyTo(resultImg, blue);
					imwrite(outputFilePath, resultImg);
					bEscKey = true;
					break;
				}
				case 13: {		// 엔터
					// 지금 까지 했던 걸 저장 
					Mat binMask = mask & 1;
					Mat blue(binMask.rows, binMask.cols, CV_8UC3, Scalar(0, 0, 0));
					blue.setTo(Scalar(255, 0, 0), binMask);
					resultImg = 0;
					src.copyTo(resultImg, blue);
					imwrite(outputFilePath, resultImg);
					bEscKey = true;
					break;
				}

				case 'b': // 뒷사진
					i=i- 2;
					bEscKey = true;
					break;
				case 'f': // 앞사진
					bEscKey = true;
					break;



				case 'c':		// grabcut
					if (data.is_draw) {
						grabCut(dst, mask, data.initRect, data.bgdModel, data.fgdModel, 1);

						Mat binMask = mask & 1;
						Mat blue(binMask.rows, binMask.cols, CV_8UC3, Scalar(0, 0, 0));
						blue.setTo(Scalar(255, 0, 0), binMask);
						data.res = dst.clone();
						addWeighted(blue, 0.5, data.res, 0.5, 0.0, data.res);

						cv::rectangle(data.res, data.initRect, Scalar(0, 0, 255), 2);
						imshow("src", data.res);


					}
					break;
				case 'm':		// make again
					mask = 0;
					src.copyTo(dst);
					data.initRect = Rect(0, 0, 0, 0);
					data.is_draw = false;
					imshow("src", dst);
					break;
				case 'r': {// show result image			
					Mat binMask = mask & 1;
					Mat blue(binMask.rows, binMask.cols, CV_8UC3, Scalar(0, 0, 0));
					blue.setTo(Scalar(255, 0, 0), binMask);
					resultImg = 0;
					src.copyTo(resultImg, blue);
					imshow("result", resultImg);

					break; }
				case 27: {	// esc
					bEscKey = true;
					break; }
				}
				



			}





		}
	}
	catch (const std::exception& e) {
		cout << "오류가 발생했습니다";
	}


	return 0;
}
