//�����ڹ���ʵ��

#pragma once
#include "PhotoBar.h"
#include <string>
#include <qdebug.h>

using namespace std;

PhotoBar::PhotoBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);  // qt designer��ƺý�����Զ����������ڵĽ�����ʽ
	resize(1200, 675);		//���������ڴ�С

	cWidget = Q_NULLPTR;	//�������Ӵ�����Ϊ��
	pWidget = Q_NULLPTR;
	
	QPalette pallette;			//���������ڵı���ͼƬ�� ���ֺ��ݱ�
	QPixmap pixmap("background1");
	pixmap = pixmap.scaled(size(), Qt::KeepAspectRatio);
	setMinimumSize(size()); setMaximumSize(size());   //ʹ�����ڵĴ�С���ɱ�
	pallette.setBrush(backgroundRole(), QBrush(pixmap));
	resize(pixmap.size());
	this->setPalette(pallette);

	connect(ui.cameraButton, &QPushButton::clicked, this, &PhotoBar::OpenCamera);  //�������������ť�����ġ����¡� �ź� �� �����ڵ�OpenCamera �ۺ���
																					//�����ť���º�ᷢ��clicked�źţ������ڽ���֮����� OpenCamera�ۺ���


	connect(ui.photoButton, &QPushButton::clicked, this, &PhotoBar::OpenProcess);   //ͬ��
	
}
void PhotoBar::OpenCamera()
{
	this->hide();     // ������������ Ȼ�󴴽��������
	cWidget = new cameraWidget(Q_NULLPTR);   
	cWidget->show();       
	cWidget->setWindowTitle(QString::fromLocal8Bit(string("��Ӱ").data()));    // ���ô��ڵı���
	connect(cWidget, &cameraWidget::back, this, &PhotoBar::OpenMenu);		//�����ɵ��������еķ��ز˵���ť�� �����ڵ�openMenu�������ӣ��Ա㷵�ز˵�
}
void PhotoBar::OpenProcess()
{
	this->hide();							//������һ�����߼�
	pWidget= new processWidget(Q_NULLPTR);
	pWidget->show();
	pWidget->setWindowTitle(QString::fromLocal8Bit(string("ͼ����").data()));
	connect(pWidget, &processWidget::back, this, &PhotoBar::OpenMenu);
}
void PhotoBar::OpenMenu()
{
	show();				//    �򿪲˵���� �����Ӵ����ͷţ���ֹ�ڴ�й©�� Q_NULLPTR���� ��ָ��
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


