#include "faceDetectionTest.h"
#include <QCameraInfo>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QPixmap>
#include <QTimer>
#include <QThread>
#include <QDebug>
#include <QFileDialog>
faceDetectionTest::faceDetectionTest(QWidget *parent)
: QWidget(parent)
{
	ui.setupUi(this);
	qRegisterMetaType<QPixmap>("QPixmap");
	m_cvDeviceMut.lock();
	m_cvVideoCapture = new cv::VideoCapture(0);
	m_cvVideoCapture->release();
	m_cvDeviceMut.unlock();

	connect(ui.startCamareBtn, &QPushButton::clicked, this, &faceDetectionTest::startCamareBtn_clicked_slots);
	connect(this, &faceDetectionTest::pixmapData_signal, [this] (QPixmap& pixmap) {pixmapData_slots(pixmap);});
	connect(ui.saveCamarePicBtn, &QPushButton::clicked, [this] () {saveCamarePicBtn_slots(); });
	connect(ui.choseSrcBtn, &QPushButton::clicked, [this] () {choseSrcBtn_clicked_slots(); });

	startCamare();
}

faceDetectionTest::~faceDetectionTest()
{

}

void faceDetectionTest::startCamare()
{
	auto camareThread = [=] ()->void {
		while (1)
		{
			m_cvDeviceMut.lock();
			if (!m_cvVideoCapture->isOpened())
			{
				QThread::msleep(500);
				m_cvDeviceMut.unlock();
				continue;
			}
			cv::Mat tMat;
			*m_cvVideoCapture >> tMat;
			m_cvDeviceMut.unlock();
			if (ui.faceDetectCheck->isChecked())
			{
				std::vector<SeetaPointF> facePoints;
				faceDetectionFunc(tMat, facePoints);
			}
			else if (ui.face2faceCheck->isChecked())
			{
				emit similarValue_signal(getSimilar(tMat, QImage2cvMat(ui.srcPicLb->pixmap()->toImage())) * 100);
			}
			else
			{
				QThread::msleep(1000 / 25);
			}
			emit pixmapData_signal(QPixmap::fromImage(cvMat2QImage(tMat)));
		}
	};
	QtConcurrent::run(camareThread);
}

void faceDetectionTest::startCamareBtn_clicked_slots()
{
	if (ui.startCamareBtn->text().contains(QStringLiteral("启动")))
	{
		ui.startCamareBtn->setText(QStringLiteral("关闭摄像头"));
		m_cvDeviceMut.lock();
		m_cvVideoCapture->open(0);
		m_cvDeviceMut.unlock();
		ui.saveCamarePicBtn->setEnabled(false);
	}
	else
	{
		ui.startCamareBtn->setText(QStringLiteral("启动摄像头"));
		m_cvDeviceMut.lock();
		m_cvVideoCapture->release();
		m_cvDeviceMut.unlock();
		ui.saveCamarePicBtn->setEnabled(true);
	}
}

void faceDetectionTest::pixmapData_slots(QPixmap &newPixmap)
{
	ui.videoLb->setPixmap(newPixmap.scaled(ui.videoLb->size()));
}

void faceDetectionTest::saveCamarePicBtn_slots()
{
	const QPixmap * pCurLbPixmap = ui.videoLb->pixmap();
	if (pCurLbPixmap == NULL||pCurLbPixmap->size()==QSize(0,0))
	{
		return;
	}
	static QString fileName;
	fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
		fileName,
		tr("Images (*.jpg)"));
	if (fileName.isEmpty())
		return;
	pCurLbPixmap->save(fileName);
}

void faceDetectionTest::choseSrcBtn_clicked_slots()
{
	static QString fileName;
	fileName = QFileDialog::getOpenFileName(this, tr("open File"),
		fileName,
		tr("Images (*.jpg *.bmp *.gif)"));
	if (fileName.isEmpty())
		return;
	ui.srcPicLb->setPixmap(QPixmap(fileName).scaled(ui.srcPicLb->size()));
	ui.face2faceCheck->setEnabled(true);
}

void faceDetectionTest::similarValue_slots(int similar)
{

}
