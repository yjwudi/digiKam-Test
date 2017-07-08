#include <QCoreApplication>
#include <QFile>
#include <QString>


#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/core/persistence.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "fullobjectdetection.h"
#include "shapepredictor.h"

#include <string>
#include <iostream>

using std::endl;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::string path1 = "/home/yjwudi/face_recognizer/orl/s1/9.pgm";
    QString path2 = "/home/yjwudi/qt_dlib_dnn/shape_test/build-shape_test-Desktop_Qt_5_6_0_GCC_64bit-Debug/shapepredictor.dat";

    ShapePredictor sp;
    QFile model(path2);
    std::cout << "read file\n";
    if (model.open(QIODevice::ReadOnly))
    {
        ShapePredictor* const temp = new ShapePredictor();
        QDataStream dataStream(&model);
        dataStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        dataStream >> *temp;
        sp = *temp;
    }
    else
    {
    	std::cout << "Error open file shapepredictor.dat";
    	return 0;
    }
    std::cout << "read over\n";

    cv::Mat img = cv::imread(path1);
    cv::Mat gray;
    int type = img.type();
    std::cout << "type: " << type << endl;
    if(type == CV_8UC3 || type == CV_16UC3)
    {
    	cout << "3 channels\n";
    	cv::cvtColor(img, gray, CV_RGB2GRAY);  // 3 channels
    }
    else
    {
    	cout << "4 channels\n";
    	cv::cvtColor(img, gray, CV_RGBA2GRAY);  // 4 channels
    }

    if (type == CV_16UC3 || type == CV_16UC4)
    {
    	cout << "16 type\n";
        gray.convertTo(gray, CV_8UC1, 1 / 255.0);
    }

    cv::Rect rect(0,0,gray.cols, gray.rows);
    std::cout << gray.cols << " " << rect.width << std::endl;
    
    FullObjectDetection object = sp(gray,rect);

    int part_num = object.num_parts();
    std::cout << "part_num: " << part_num << endl;
    for(int i = 0; i < part_num; i++)
    {
    	std::vector<float> vec = object.part(i);
    	int x = vec[0], y = vec[1];
    	cv::circle(img, cvPoint(x, y), 3, cv::Scalar(0, 0, 255), -1);
    }
    cv::imshow("aaa", img);
    cv::waitKey();


        




    return a.exec();
}
