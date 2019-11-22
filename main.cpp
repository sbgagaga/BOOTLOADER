#include "stdafx.h"
#include "E300.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	E300 w;
	w.show();
	return a.exec(); 
}
