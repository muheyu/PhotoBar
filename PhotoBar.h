#pragma once
//主窗口 头文件
#include <QtWidgets/QWidget>
#include "ui_PhotoBar.h"
#include "cameraWidget.h"
#include "processWidget.h"
class PhotoBar : public QWidget   //主窗口
{
	Q_OBJECT

public:
	PhotoBar(QWidget *parent = Q_NULLPTR);	//主窗口构造函数
 
public slots:				 // 三个槽函数
	void OpenCamera();		//  打开相机窗口，同时隐藏主窗口
	void OpenProcess();		// 打开图片处理窗口，隐藏主窗口
	void OpenMenu();		//打开菜单栏， 当主窗口接收到子窗口返回菜单的信号时调用

private:
	Ui::MainWidget ui;				//使用QT designer 设计了 主窗口界面
	cameraWidget* cWidget;		//  cWidget是相机窗口
	processWidget * pWidget;	// pwidget 相片处理窗口
};
