#pragma once

#include <QtWidgets/QWidget>
#include <QLabel>
#include <Qpushbutton>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <QSlider>
#include <QLineEdit>
#include <QSpinBox>
#include <opencv2/dnn/dnn.hpp>
#include <vector>
#include <qtimer.h>

class processWidget 
	: public QWidget
{
	Q_OBJECT
public:
	processWidget(QWidget* parent = Q_NULLPTR);
	void predict();
	void change();

public slots:
	void openFile();

signals:
	void back();
	void stop();
private:
	QPushButton openPhoto;
	QPushButton savePhoto;
	QPushButton changeStyle;
	QPushButton backMenu;
	QPushButton faceDetect;
	QLabel imageLabel, Info;
	QString photoPath;
	QImage photo;
	QSlider sample, bilateral_filter, edge_detect;
	QLabel sampleTimes, bf_times, edge_num;
	QSpinBox sample_spin, bf_spin, edge_spin;
	cv::Mat image;
	int preW, preH;
	void processWidget::resizeEvent(QResizeEvent* event);
};