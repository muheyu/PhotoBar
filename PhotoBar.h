#pragma once
//������ ͷ�ļ�
#include <QtWidgets/QWidget>
#include "ui_PhotoBar.h"
#include "cameraWidget.h"
#include "processWidget.h"
class PhotoBar : public QWidget   //������
{
	Q_OBJECT

public:
	PhotoBar(QWidget *parent = Q_NULLPTR);	//�����ڹ��캯��
 
public slots:				 // �����ۺ���
	void OpenCamera();		//  ��������ڣ�ͬʱ����������
	void OpenProcess();		// ��ͼƬ�����ڣ�����������
	void OpenMenu();		//�򿪲˵����� �������ڽ��յ��Ӵ��ڷ��ز˵����ź�ʱ����

private:
	Ui::MainWidget ui;				//ʹ��QT designer ����� �����ڽ���
	cameraWidget* cWidget;		//  cWidget���������
	processWidget * pWidget;	// pwidget ��Ƭ������
};
