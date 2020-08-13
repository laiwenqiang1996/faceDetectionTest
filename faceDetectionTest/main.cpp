#include "faceDetectionTest.h"
#include "seeTaFaceWindow.h"
#include <QtWidgets/QApplication>

int main_(int argc, char *argv[])
{
	QApplication a(argc, argv);
	seeTaFaceWindow w;
	w.show();
	return a.exec();
}
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	faceDetectionTest w;
	w.show();
	return a.exec();
}
