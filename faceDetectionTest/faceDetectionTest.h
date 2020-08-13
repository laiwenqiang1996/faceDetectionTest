#ifndef FACEDETECTIONTEST_H
#define FACEDETECTIONTEST_H

#include <QtWidgets/QWidget>
#include "ui_faceDetectionTest.h"

#include <opencv2/opencv.hpp>  
#include <opencv2/highgui/highgui.hpp> 
#include "seeTaFaceWindow.h"
#include <QtConcurrent>
#include <QMutex>
Q_DECLARE_METATYPE(QPixmap);
class faceDetectionTest : public QWidget
{
	Q_OBJECT

public:
	faceDetectionTest(QWidget *parent = 0);
	~faceDetectionTest();
private:
	void startCamare();
signals:
	void pixmapData_signal(QPixmap &newPixmap);
	void similarValue_signal(int similarValue);
private slots:
	void startCamareBtn_clicked_slots();
	void pixmapData_slots(QPixmap &newPixmap);
	void saveCamarePicBtn_slots();
	void choseSrcBtn_clicked_slots();
	void similarValue_slots(int similar);
private:
	cv::VideoCapture * m_cvVideoCapture = NULL;
	bool m_bNeedFaceDetect = false;
	QMutex m_cvDeviceMut;
private:
	Ui::faceDetectionTestClass ui;
};

#endif // FACEDETECTIONTEST_H
