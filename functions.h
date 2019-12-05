#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/dnn.hpp>
#include<vector>
#include <qcheckbox.h>
#include <qpushbutton.h>
std::vector<int> FaceDetection(cv::Mat& input, int threshold = 70);

int  predict_gender(cv::dnn::Net& net, cv::Mat image, cv::Scalar mean_face);

int  predict_age(cv::dnn::Net& net, cv::Mat image, cv::Scalar mean_face);

int cartoonChange(cv::Mat& input, int sample_times = 1, int bF_times = 15, int min_canny_threshold = 60, int max_canny_threshold = 100);

void setBtnQss(QPushButton* btn, QString normalColor, QString normalTextColor, QString hoverColor, QString hoverTextColor, QString pressedColor, QString pressedTextColor);

void setSwitchQss(QCheckBox*  box);

void sketchChange(cv::Mat& input);

