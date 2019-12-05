#pragma once
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "facedetectcnn.h"
#include <vector>
#include <opencv2/dnn/dnn.hpp>
#include <qpushbutton.h>
#include <qcheckbox.h>
//define the buffer size. Do not change the size!
#define DETECT_BUFFER_SIZE 0x20000
using namespace cv;
using namespace std;
using namespace cv::dnn;
vector<int> FaceDetection(Mat& image, int threshold)
{

	int* pResults = NULL;
	//pBuffer is used in the detection functions.
	//If you call functions in multiple threads, please create one buffer for each thread!
	unsigned char* pBuffer = (unsigned char*)malloc(DETECT_BUFFER_SIZE);
	///////////////////////////////////////////
	// CNN face detection 
	// Best detection rate
	//////////////////////////////////////////
	//!!! The input image must be a BGR one (three-channel) instead of RGB
	//!!! DO NOT RELEASE pResults !!!
	pResults = facedetect_cnn(pBuffer, (unsigned char*)(image.ptr(0)), image.cols, image.rows, (int)image.step);
	//print the detection results
	vector<int> result;
	for (int i = 0; i < (pResults ? *pResults : 0); i++)
	{
		short* p = ((short*)(pResults + 1)) + 142 * i;
		if (p[4] < threshold)
			continue;
		for (int j = 0; j < 4; j++)
			result.push_back(p[j]);
		/*printf("face_rect=[%d, %d, %d, %d], confidence=%d, angle=%d\n", x, y, w, h, confidence, angle);*/
	}
	free(pBuffer);
	return result;
}


int predict_gender(Net& net, Mat image, Scalar mean_face)
{
	//cvtColor(image, gray, COLOR_BGR2GRAY);
	Mat blob = blobFromImage(image, 1.0, Size(227, 227), mean_face);
	net.setInput(blob, "data");
	Mat prob = net.forward("prob");
	if (prob.at<float>(0, 0) > prob.at<float>(0, 1))
		return 0;
	else
		return 1;
}
int predict_age(Net& net, Mat image, Scalar mean_face)
{
	//cvtColor(image, gray, COLOR_BGR2GRAY);
	Mat blob = blobFromImage(image, 1.0, Size(227, 227), mean_face);
	net.setInput(blob, "data");
	Mat prob = net.forward("prob");
	int max = 0;
	for (int i = 1; i < prob.cols; i++)
	{
		if (prob.at<float>(0, i) > prob.at<float>(0, max))
			max = i;
	}
	return max;
}

using namespace std;
using namespace cv;
int cartoonChange(Mat& input, int sample_times = 1, int bF_times = 10, int min_canny_threshold = 30, int max_canny_threshold = 60)
{
	for (int i = 0; i < sample_times; i++)
	{
		pyrDown(input, input);
	}
	for (int i = 0; i < bF_times; i++)
	{
		Mat out;
		bilateralFilter(input, out, 9, 20, 15);
		input = out;
	}
	for (int i = 0; i < sample_times; i++)
	{
		pyrUp(input, input);
	}

	medianBlur(input, input, 3);
	Mat gray;
	cvtColor(input, gray, COLOR_BGR2GRAY);
	blur(gray, gray, Size(3, 3));
	Mat edge;
	Canny(gray, edge, min_canny_threshold, max_canny_threshold);
	Mat_<uchar>::iterator edge_it = edge.begin<uchar>(), edge_it_end = edge.end<uchar>();
	Mat_<Vec3b>::iterator X_it = input.begin<Vec3b>(), X_it_end = input.end<Vec3b>();
	Mat result;
	for (; edge_it != edge_it_end; edge_it++)
	{
		if (*edge_it > 150)
		{
			(*X_it)[0] = 0;
			(*X_it)[1] = 0;
			(*X_it)[2] = 0;
		}
		X_it++;
	}
	return 0;
}

void setBtnQss(QPushButton* btn, QString normalColor, QString normalTextColor, QString hoverColor, QString hoverTextColor, QString pressedColor, QString pressedTextColor)
{
	QStringList qss;
	qss.append(QString("QPushButton{border-style:none;padding:10px;border-radius:3px;color:%1;background:%2;%3}").arg(normalTextColor).arg(normalColor).arg(QString::fromLocal8Bit(" font-family:'微软雅黑';"" font: 15pt ""方正舒体"";")));  //基本样式

	qss.append(QString("QPushButton:hover{color:%1;background:%2;}").arg(hoverTextColor).arg(hoverColor));  // 鼠标悬停的时候放的图片
	qss.append(QString("QPushButton:pressed{color:%1;background:%2;}").arg(pressedTextColor).arg(pressedColor));// 鼠标按下的图片
	btn->setStyleSheet(qss.join(""));
}

void setSwitchQss(QCheckBox* box)
{
	box->resize(200, 100);
	box->setStyleSheet("QCheckBox{color: rgb(255, 255, 255); font: 15pt ""方正舒体"";} "
		"QCheckBox::indicator:unchecked{image: url(:/PhotoBar/icon/close.png);}"  // 打钩之后显示的图片
		"QCheckBox::indicator:checked{ image: url(:/PhotoBar/icon/open.png);}");  //未打钩的时候显示的图片
}

void sketchChange(cv::Mat& input)
{

	/*GaussianBlur(input, input, Size(5, 5), 0);*/
	cvtColor(input, input, COLOR_BGR2GRAY);
	Mat out;
	bilateralFilter(input, out, -1, 15, 8);
	input = out;

	adaptiveThreshold(input, input, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 3, 2);

	GaussianBlur(input, input, Size(3, 3), 0);

	threshold(input, input, 240, 255, THRESH_BINARY);

	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	bitwise_not(input, input);
	morphologyEx(input, input, MORPH_OPEN, kernel);
	bitwise_not(input, input);

	medianBlur(input, input, 3);
	//medianBlur(input, input, 3);
}