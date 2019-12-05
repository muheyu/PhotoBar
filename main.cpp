#pragma once
#include "PhotoBar.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	PhotoBar w;
	w.show();
	return a.exec();
}
