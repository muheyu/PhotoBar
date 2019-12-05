#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "processWidget.h"
#include <QPalette>
#include <QFIleDialog>
#include <QDebug>
#include <string>
#include <QString>
#include <QImageReader>
#include <QTime>
#include "functions.h"
#include <qbuffer.h>
#include <qtimer.h>
#include <string>
processWidget::processWidget(QWidget* parent)
	:QWidget(parent)
{

	resize(1600, 900);
	setParent(parent);
	setWindowIcon(QIcon(".//icon//icon.jpg"));
	setStyleSheet("background-color: rgb(0, 0, 0);");
	openPhoto.setParent(this);
	savePhoto.setParent(this);
	changeStyle.setParent(this);
	backMenu.setParent(this);
	faceDetect.setParent(this);
	imageLabel.setParent(this);
	imageLabel.resize(1080, 700);
	imageLabel.setStyleSheet("background-color:rgb(47,79,79)");
	imageLabel.show();
	imageLabel.setAlignment(Qt::AlignCenter);
	Info.setParent(this); Info.setGeometry(0, 700, 1080, 60);
	Info.setStyleSheet("font: 15pt ""方正舒体"";color:rgb(255,255,255);");
	Info.show();
	Info.setAlignment(Qt::AlignRight);

	openPhoto.move(50, 770);
	openPhoto.resize(150, 60);
	openPhoto.setText(QString::fromLocal8Bit(std::string("选择图片").data()));
	setBtnQss(&openPhoto, "#34495E", "#FFFFFF", "#4E6D8C", "#F0F0F0", "#2D3E50", "#B8C6D1");

	faceDetect.setText(QString::fromLocal8Bit(std::string("人脸识别").data()));
	faceDetect.resize(150, 60);  faceDetect.move(950, 770);
	setBtnQss(&faceDetect, "#34495E", "#FFFFFF", "#4E6D8C", "#F0F0F0", "#2D3E50", "#B8C6D1");

	setBtnQss(&changeStyle, "#34495E", "#FFFFFF", "#4E6D8C", "#F0F0F0", "#2D3E50", "#B8C6D1");
	changeStyle.move(1400, 770);
	changeStyle.setText(QString::fromLocal8Bit(std::string("画风转换").data()));
	changeStyle.resize(150, 60);

	setBtnQss(&savePhoto, "#34495E", "#FFFFFF", "#4E6D8C", "#F0F0F0", "#2D3E50", "#B8C6D1");
	savePhoto.move(500, 770);
	savePhoto.resize(150, 60);
	savePhoto.setText(QString::fromLocal8Bit(std::string("保存图片").data()));
	backMenu.setParent(this); backMenu.move(1500, 0); backMenu.resize(100, 50); backMenu.setText(QString::fromLocal8Bit(std::string("主菜单").data()));
	setBtnQss(&backMenu, "#34495E", "#FFFFFF", "#4E6D8C", "#F0F0F0", "#2D3E50", "#B8C6D1");

	sample.setMinimum(0); sample.setMaximum(10); sample.setValue(1);
	sample.move(1280, 100); sample.setParent(this); sample.setTickPosition(QSlider::TicksBothSides);
	sample.show();  sample.setOrientation(Qt::Horizontal);  sample.resize(300, 40);
	sampleTimes.move(1080, 100); sampleTimes.resize(140, 40); sampleTimes.setParent(this); sampleTimes.setText(QString::fromLocal8Bit(std::string("采样次数:").data()));
	sampleTimes.setStyleSheet(" font: 10pt ""方正舒体""; color: rgb(255,255,255);");
	sample_spin.setParent(this); sample_spin.move(1220, 100); sample_spin.resize(50, 40); sample_spin.setValue(sample.value()); sample_spin.setMinimum(0);
	sample_spin.setMaximum(10);  sample_spin.setStyleSheet(" font: 10pt ""方正舒体""; color: rgb(255,255,255);");

	bilateral_filter.setMinimum(1); bilateral_filter.setMaximum(50); bilateral_filter.setValue(10); bilateral_filter.setParent(this);
	bilateral_filter.move(1280, 350); bilateral_filter.resize(300, 40); bilateral_filter.setOrientation(Qt::Horizontal);
	bilateral_filter.setTickPosition(QSlider::TicksBothSides); bilateral_filter.show();
	bf_times.move(1080, 350); bf_times.resize(140, 40); bf_times.setParent(this); bf_times.setText(QString::fromLocal8Bit(std::string("双边滤波次数:").data()));
	bf_times.setStyleSheet(" font: 10pt ""方正舒体"";  color: rgb(255,255,255);");
	bf_spin.setParent(this); bf_spin.resize(50, 40);  bf_spin.setValue(bilateral_filter.value()); bf_spin.move(1220, 350);
	bf_spin.setMaximum(50); bf_spin.setMinimum(1); bf_spin.setValue(bilateral_filter.value()); 
	bf_spin.setStyleSheet(" font: 10pt ""方正舒体""; color: rgb(255,255,255);");

	edge_detect.setParent(this); edge_detect.setOrientation(Qt::Horizontal); edge_detect.resize(300, 40); edge_detect.move(1280, 600);
	edge_detect.setMinimum(1); edge_detect.setMaximum(100); edge_detect.setValue(30); edge_detect.show();
	edge_num.move(1080, 600); edge_num.resize(140, 40); edge_num.setParent(this); edge_num.setText(QString::fromLocal8Bit(std::string("边缘检测强度:").data()));
	edge_num.setStyleSheet(" font: 10pt ""方正舒体""; color: rgb(255,255,255);");
	edge_spin.setParent(this); edge_spin.setMaximum(100); edge_spin.setMinimum(1); edge_spin.setValue(edge_detect.value());
	edge_spin.move(1220, 600); edge_spin.resize(50, 40); edge_detect.setTickPosition(QSlider::TicksBothSides);
	edge_spin.setStyleSheet(" font: 10pt ""方正舒体""; color: rgb(255,255,255);");

	faceDetect.setDisabled(true);
	changeStyle.setDisabled(true);
	savePhoto.setDisabled(true);

	connect(&changeStyle, &QPushButton::clicked,
		[=]() 
		{
			Info.setText(QString::fromLocal8Bit(std::string("loading···").data()));
			cv::waitKey(500);
			change();
		});
	connect(&faceDetect, &QPushButton::clicked,
		[=]()
		{
			Info.setText(QString::fromLocal8Bit(std::string("loading···").data()));
			cv::waitKey(500);
			predict();
		});
	

	connect(&openPhoto, &QPushButton::clicked, this, &processWidget::openFile);
	connect(&sample, &QSlider::valueChanged,
		[=]
		{
			sample_spin.setValue(sample.value());
		});
	connect(&bilateral_filter, &QSlider::valueChanged,
		[=]
		{
			bf_spin.setValue(bilateral_filter.value());
		});
	connect(&edge_detect, &QSlider::valueChanged,
		[=]
		{
			edge_spin.setValue(edge_detect.value());
		});
	connect(&sample_spin, QOverload<int>::of(&QSpinBox::valueChanged),
		[=](int i)
		{
			sample.setValue(i);
		});
	connect(&bf_spin, QOverload<int>::of(&QSpinBox::valueChanged),
		[=](int i)
		{
			bilateral_filter.setValue(i);
		});
	connect(&edge_spin, QOverload<int>::of(&QSpinBox::valueChanged),
		[=](int i)
		{
			edge_detect.setValue(i);
		});
	connect(&savePhoto, &QPushButton::clicked,
		[=]() {
			QString path = QFileDialog::getSaveFileName(this, "Save Photo", QDir::currentPath() + "//DCIM", "Image(*.jpg )");
			if (path.isEmpty() == false)
			{
				QFile file(path);
				if (file.open(QIODevice::ReadWrite))
				{
					QByteArray ba;
					QBuffer buffer(&ba);
					buffer.open(QIODevice::WriteOnly);
					photo.save(&buffer, "jpg");
					file.write(ba);
				}
			}
		});
	connect(&backMenu, &QPushButton::clicked,
		[=]()
		{
			emit this->back();
		});
}

void processWidget::openFile()
{
	QString path = QFileDialog::getOpenFileName(this, "Open Photo", QDir::currentPath() + "//DCIM", "image(*.jpg *.png *.jpeg)");
	QImageReader reader(path);
	reader.setDecideFormatFromContent(true);
	photo = reader.read();
	if (photo.isNull())
	{
		reader.setFileName(photoPath);
		photo = reader.read();
	}
	else
		photoPath = path;
	if (photo.isNull())
	{
		changeStyle.setDisabled(true);
		savePhoto.setDisabled(true);
		faceDetect.setDisabled(true);
	}
	else
	{
		faceDetect.setDisabled(false);
		changeStyle.setDisabled(false);
		savePhoto.setDisabled(false);
	}
	imageLabel.setPixmap(QPixmap::fromImage(photo.scaled(imageLabel.size(), Qt::KeepAspectRatio)));
}

void processWidget::change()
{
	changeStyle.setDisabled(true);
	image = cv::imread(photoPath.toLocal8Bit().toStdString());
	cartoonChange(image, sample.value(), bilateral_filter.value(), 1200.0 / edge_detect.value()-10, 2000.0 / edge_detect.value()-18);
	cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
	photo = QImage((const uchar*)(image.data), image.cols, image.rows, image.step, QImage::Format_RGB888);
	
	imageLabel.setPixmap(QPixmap::fromImage(photo.scaled(imageLabel.size(), Qt::KeepAspectRatio)));
	changeStyle.setDisabled(false);
	Info.setText(QString::fromLocal8Bit(std::string("转换完成！").data()));
}

void processWidget::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	double  wRatio = width()/1600.0, hRatio = height()/900.0;
	openPhoto.setGeometry(50 * wRatio, 770 * hRatio, 150 * wRatio, 60 * hRatio);
	changeStyle.setGeometry(1400 * wRatio, 770 * hRatio, 150 * wRatio, 60 * hRatio);
	savePhoto.setGeometry(500 * wRatio, 770 * hRatio, 150 * wRatio, 60 * hRatio);
	backMenu.setGeometry(width()-150*wRatio,0, 150 * wRatio, 60 * hRatio);
	imageLabel.setGeometry(0, 0, 1080 * wRatio, 700 * hRatio);
	faceDetect.setGeometry(950* wRatio, 770 * hRatio, 150 * wRatio, 60 * hRatio);
	Info.setGeometry(0, 710 * hRatio, 1080*wRatio, 60*hRatio);


	sample.setGeometry(1280 * wRatio, 100 * hRatio, 300 * wRatio, 40 * hRatio);
	bilateral_filter.setGeometry(1280 * wRatio, 350 * hRatio, 300 * wRatio, 40 * hRatio);
	edge_detect.setGeometry(1280 * wRatio, 600 * hRatio, 300 * wRatio, 40 * hRatio);

	sample_spin.setGeometry(1220*wRatio, 100*hRatio, 50*wRatio, 40*hRatio);
	bf_spin.setGeometry(1220 * wRatio, 350 * hRatio, 50 * wRatio, 40 * hRatio);
	edge_spin.setGeometry(1220 * wRatio, 600 * hRatio, 50 * wRatio, 40 * hRatio);

	sampleTimes.setGeometry(1090 * wRatio, 100 * hRatio, 130 * wRatio, 40 * hRatio);
	bf_times.setGeometry(1090 * wRatio, 350 * hRatio, 130* wRatio, 40 * hRatio);
	edge_num.setGeometry(1090 * wRatio, 600 * hRatio, 130 * wRatio, 40 * hRatio);
	imageLabel.setPixmap(QPixmap::fromImage(photo.scaled(imageLabel.size(), Qt::KeepAspectRatio)));
}

void processWidget::predict()
{
	
	faceDetect.setDisabled(true);
	image = cv::imread(photoPath.toLocal8Bit().toStdString());
	if (image.empty())
		return;
	cv::Mat temp;
	double ratio = 500.0/ image.cols;
	cv::resize(image, temp, cv::Size(), ratio, ratio);
	std::vector<int> pos = FaceDetection(temp, 70);
	for (int i = 0; i < pos.size(); i++)
	{
		pos[i] /= ratio;
	}
	for (int i = 0; i < pos.size(); i += 4)
	{
		int a = pos[i], b = pos[i + 1], c = pos[i] + pos[i + 2], d = pos[i + 1] + pos[i + 3];
		cv::Point leftTop = cv::Point(a >= 0 ? a : 0, b >= 0 ? b : 0);
		cv::Point rightBottom = cv::Point(c < image.cols ? c : image.cols - 1, d < image.rows ? d : image.rows - 1);
		ratio = (rightBottom - leftTop).x / 50.0;
		cv::rectangle(image, leftTop, rightBottom, cv::Scalar(180, 105, 255),2.5*ratio);
	}

	cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
	photo = QImage((const uchar*)(image.data), image.cols, image.rows, image.step, QImage::Format_RGB888);
	imageLabel.setPixmap(QPixmap::fromImage(photo.scaled(imageLabel.size(), Qt::KeepAspectRatio)));
	faceDetect.setDisabled(false);
	std::string text("检测到"); text += std::to_string(pos.size() / 4) + "张人脸";
	Info.setText(QString::fromLocal8Bit(std::string(text).data()));
}