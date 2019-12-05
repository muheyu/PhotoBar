#pragma once

#include "cameraWidget.h"
#include "facedetectcnn.h"
#include "PhotoBar.h"
#include <string>
#include <opencv2/opencv.hpp>
#include "functions.h"
#include <QTime>
#include <qfile.h>
#include <qbuffer.h>
using namespace std;

cameraWidget::cameraWidget(QWidget* parent)
	:QWidget(parent)
{
	setWindowIcon(QIcon(".\\icon\\icon.jpg"));;
	setFixedSize(1200, 750);
	setStyleSheet("background-color:black");
	setParent(parent);
	ages.push_back("0-2");
	ages.push_back("4 - 6");
	ages.push_back("8 - 13");
	ages.push_back("15 - 20");
	ages.push_back("25 - 32");
	ages.push_back("38 - 43");
	ages.push_back("48 - 53");
	ages.push_back("60-");
	age_model = ".\\model\\age_net.caffemodel";
	age_text = ".\\model\\deploy_age.prototxt";
	gender_model = ".\\model\\gender_net.caffemodel";
	gender_text = ".\\model\\deploy_gender.prototxt";
	mean_face = Scalar(114.895847746, 87.7689143744, 78.4263377603);
	captionColor = Scalar(255, 191, 0);
	genders.push_back("boy"); genders.push_back("girl");
	age_net = cv::dnn::readNetFromCaffe(age_text, age_model);
	gender_net = cv::dnn::readNetFromCaffe(gender_text, gender_model);
	imageLabel.setParent(this);
	imageLabel.setFixedSize(900, 600);
	imageLabel.move(0, 50);
	imageLabel.setAlignment(Qt::AlignCenter);

	shot.setParent(this); shot.move(400, 650); shot.resize(100, 100);
	shot.setStyleSheet("QPushButton{border-image: url(:/PhotoBar/icon/start.png);}"
		"QPushButton:hover{border-image: url(:/PhotoBar/icon/hover.png);}"
		"QPushButton:pressed{border-image: url(:/PhotoBar/icon/pressed.png);}");

	face_detect.setText(QString::fromLocal8Bit(string("ÈËÏñ¼ì²â").data()));
	face_detect.move(930, 100); face_detect.setParent(this);
	setSwitchQss(&face_detect);

	gender_detect.setText(QString::fromLocal8Bit(string("ÐÔ±ð¼ì²â").data()));
	gender_detect.setParent(this); gender_detect.move(930, 220);
	setSwitchQss(&gender_detect);

	age_detect.setText(QString::fromLocal8Bit(string("ÄêÁä¼ì²â").data()));
	age_detect.setParent(this); age_detect.move(930, 340);
	setSwitchQss(&age_detect);

	cartoon.setText(QString::fromLocal8Bit(string("¿¨Í¨ÂË¾µ").data()));
	cartoon.setParent(this); cartoon.move(930, 460);
	setSwitchQss(&cartoon);

	sketch.setText(QString::fromLocal8Bit(string("ËØÃèÂË¾µ").data()));
	sketch.setParent(this); sketch.move(930, 580);
	setSwitchQss(&sketch);


	backMenu.setParent(this); backMenu.move(1100, 0); backMenu.resize(100, 50); backMenu.setText(QString::fromLocal8Bit(std::string("Ö÷²Ëµ¥").data()));
	setBtnQss(&backMenu, "#34495E", "#FFFFFF", "#4E6D8C", "#F0F0F0", "#2D3E50", "#B8C6D1");


	camera.open(0);
	if (camera.isOpened())
	{
		imageTime = new QTimer(this);
		imageTime->start(50);
	}
	else
		imageTime = Q_NULLPTR;

	looptimes = 0;
	imageLabel.show();
	imageLabel.setStyleSheet("background-color:black");
	connect(imageTime, &QTimer::timeout, this, &cameraWidget::showImage);
	connect(&backMenu, &QPushButton::clicked,
		[=]()
		{
			emit this->back();
		});
	connect(&shot, &QPushButton::clicked, this, &cameraWidget::shotPhoto);
		/*[=]()
		{
			QString path = QString("//DCIM") + QTime::currentTime().toString()+".jpg";
			QFile file(path);
			if (file.open(QIODevice::ReadWrite))
			{
				QByteArray ba;
				QBuffer buffer(&ba);
				buffer.open(QIODevice::WriteOnly);
				image.save(&buffer, "jpg");
				file.write(ba);
			}
		});*/
}
void cameraWidget::showImage()
{
	camera >> frame;

	if (cartoon.isChecked())
	{
		cartoonChange(frame);
	}

	if (sketch.isChecked())
	{
		sketchChange(frame);
	}

	if ((looptimes % 2) == 0)
	{
		faceAttribute.clear();
		if(face_detect.isChecked())
			age_gender_detect();
	}

	looptimes = (looptimes + 1) % 2;
	for (int i = 0; i < faceAttribute.size(); i++)
	{
		rectangle(frame, faceAttribute[i].leftTop, faceAttribute[i].rightBottom, Scalar(180, 105, 255), 2);
		if (gender_detect.isChecked() && faceAttribute[i].gender != -1)
		{
			int x = faceAttribute[i].rightBottom.x + faceAttribute[i].leftTop.x; x = x / 2 - 50;
			int y = faceAttribute[i].leftTop.y - 5;
			putText(frame, genders[faceAttribute[i].gender], Point(x, y), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.5, captionColor, 2.1);
		}
		if (age_detect.isChecked() && faceAttribute[i].age != -1)
		{
			int x = faceAttribute[i].leftTop.x + 30; 
			int y = faceAttribute[i].rightBottom.y + 30;
			putText(frame, ages[faceAttribute[i].age] + " years old", Point(x, y), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.5, captionColor, 2.1);
		}
	}

	cvtColor(frame, frame, CV_BGR2RGB);

	image = QImage((const unsigned char*)(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);
	imageLabel.setPixmap(QPixmap::fromImage(image).scaled(imageLabel.size(),Qt::KeepAspectRatio));
	imageTime->start(50);
}

void cameraWidget::age_gender_detect()
{
	Mat temp;
	double ratio = 0.2;
	cv::resize(frame, temp, Size(0, 0), ratio, ratio);
	vector<int> pos = FaceDetection(temp);
	for (int i = 0; i < pos.size(); i++)
	{
		pos[i] /= ratio;
	}
	for (int i = 0; i < pos.size(); i += 4)
	{
		face now;
		int a = pos[i], b = pos[i + 1], c = pos[i] + pos[i + 2], d = pos[i + 1] + pos[i + 3];
		now.leftTop = Point(a >= 0 ? a : 0, b >= 0 ? b : 0);
		now.rightBottom = Point(c < frame.cols ? c : frame.cols - 1, d < frame.rows ? d : frame.rows - 1);
		
		if (age_detect.isChecked())
			now.age = predict_age(age_net, frame(Rect(now.leftTop, now.rightBottom)), mean_face);
		else now.age = -1;
		if (gender_detect.isChecked())
			now.gender = predict_gender(gender_net, frame(Rect(now.leftTop, now.rightBottom)), mean_face);
		else now.gender = -1;
		faceAttribute.push_back(now);
	}
}
void cameraWidget::shotPhoto()
{
	if (image.isNull())
		return;
	QString path = QString(".//DCIM//") + QTime::currentTime().toString("hh_mm_ss_zzz") + ".jpg";
	QFile file(path);
	file.open(QIODevice::WriteOnly);
	QByteArray ba;
	QBuffer buffer(&ba);
	buffer.open(QIODevice::WriteOnly);
	image.save(&buffer, "jpg");
	file.write(ba);
}
