#pragma once
#include <QtWidgets/QWidget>
#include <QLabel>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/dnn.hpp>
#include <vector>
#include <qpushbutton.h>
#include <QLCDNumber>
#include <qcheckbox.h>
using namespace cv;
typedef struct
{
	int age, gender;
	Point leftTop, rightBottom;
}face;

class cameraWidget : public QWidget
{
	Q_OBJECT
public:
	cameraWidget(QWidget* parent = Q_NULLPTR);
	void age_gender_detect();

signals:
	void back();

public slots:
	void showImage();
	void shotPhoto();
private:

	QLabel imageLabel;
	QImage image;
	VideoCapture camera;
	Mat frame;
	QTimer* imageTime;
	std::vector<face> faceAttribute;      // <ÐÔ±ð£¬ÄêÁä > 
	dnn::Net age_net;
	dnn::Net gender_net;
	std::string age_model; 
	std::string age_text; 
	std::string gender_model; 
	std::string gender_text;
	std::vector<std::string> ages;
	std::vector<std::string> genders;
	Scalar mean_face;
	Scalar captionColor;
	QPushButton shot, backMenu;
	int looptimes;
	QCheckBox face_detect, gender_detect, age_detect,sketch,cartoon;
};