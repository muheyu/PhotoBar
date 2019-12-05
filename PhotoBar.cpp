//主窗口功能实现

#pragma once
#include "PhotoBar.h"
#include <string>
#include <qdebug.h>

using namespace std;

PhotoBar::PhotoBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);  // qt designer设计好界面后自动生成主窗口的界面样式
	resize(1200, 675);		//调整主窗口大小

	cWidget = Q_NULLPTR;	//将两个子窗口设为空
	pWidget = Q_NULLPTR;
	
	QPalette pallette;			//设置主窗口的背景图片， 保持横纵比
	QPixmap pixmap("background1");
	pixmap = pixmap.scaled(size(), Qt::KeepAspectRatio);
	setMinimumSize(size()); setMaximumSize(size());   //使主窗口的大小不可变
	pallette.setBrush(backgroundRole(), QBrush(pixmap));
	resize(pixmap.size());
	this->setPalette(pallette);

	connect(ui.cameraButton, &QPushButton::clicked, this, &PhotoBar::OpenCamera);  //链接来自相机按钮发出的“按下“ 信号 和 主窗口的OpenCamera 槽函数
																					//相机按钮按下后会发送clicked信号，主窗口接收之后调用 OpenCamera槽函数


	connect(ui.photoButton, &QPushButton::clicked, this, &PhotoBar::OpenProcess);   //同理
	
}
void PhotoBar::OpenCamera()
{
	this->hide();     // 先隐藏主窗口 然后创建相机窗口
	cWidget = new cameraWidget(Q_NULLPTR);   
	cWidget->show();       
	cWidget->setWindowTitle(QString::fromLocal8Bit(string("摄影").data()));    // 设置窗口的标题
	connect(cWidget, &cameraWidget::back, this, &PhotoBar::OpenMenu);		//把生成的主窗口中的返回菜单按钮和 主窗口的openMenu函数链接，以便返回菜单
}
void PhotoBar::OpenProcess()
{
	this->hide();							//和上面一样的逻辑
	pWidget= new processWidget(Q_NULLPTR);
	pWidget->show();
	pWidget->setWindowTitle(QString::fromLocal8Bit(string("图像处理").data()));
	connect(pWidget, &processWidget::back, this, &PhotoBar::OpenMenu);
}
void PhotoBar::OpenMenu()
{
	show();				//    打开菜单后把 两个子窗口释放，防止内存泄漏， Q_NULLPTR就是 空指针
	if (pWidget!= Q_NULLPTR)
	{
		pWidget->close();
		delete pWidget;
		pWidget = Q_NULLPTR;
	}
	if (cWidget != Q_NULLPTR)
	{
		cWidget->close();
		delete cWidget;
		cWidget = Q_NULLPTR;
	}

}


