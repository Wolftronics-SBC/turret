#include "mainwindow.h"
#include <QApplication>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"

#include <QDebug>

char waitkey;

int main(int argc, char *argv[])
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    /*QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();*/

    qDebug() << "CV";

    cvNamedWindow("Camera_Output", 1);
    CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);

    while(1) {
        IplImage* frame = cvQueryFrame(capture);
        cv::Mat mat(frame);
        int x = 0;
        int y = 0;
        for(int i = 0; i < mat.rows; i++) {
            for(int j = 0; j < mat.cols; j++) {
                if ((mat.at<cv::Vec3b>(i,j)[0] > 150) && (mat.at<cv::Vec3b>(i,j)[0] < 250) &&
                    (mat.at<cv::Vec3b>(i,j)[1] > 170) && (mat.at<cv::Vec3b>(i,j)[1] < 250) &&
                    (mat.at<cv::Vec3b>(i,j)[2] > 218) && (mat.at<cv::Vec3b>(i,j)[2] <= 255)) {
                        x = j - mat.cols / 2;
                        y = i - mat.rows / 2;
                }
            }
        }
        cvCircle(frame, cvPoint(x + 320, y + 240), 4, CV_RGB(0,255,0), -1);
        double k = 745;
        int anglex = 2048 - x * k / 640;
        qDebug() << "x:\t" << x << "\ta:\t" << anglex;
        cvShowImage("Camera_Output", frame);

        waitkey = cvWaitKey(10);
        if (char(waitkey) == 27){
            break;
        }
    }

    cvReleaseCapture(&capture);
    cvDestroyWindow("Camera_Output");

    return 0;
}
