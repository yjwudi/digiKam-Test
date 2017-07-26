#ifndef _DNN_FACE_H_
#define _DNN_FACE_H_

// Qt includes

#include <QString>
#include <QFile>
#include <QDataStream>
#include <QStandardPaths>

//c++ includes
#include <vector>

// local includes
//#include "shapepredictor.h"
//#include "fullobjectdetection.h"

//dnn module
#include "nn/tensor.h"
#include "nn/input.h"
#include "nn/layers.h"
#include "nn/loss.h"
#include "nn/core.h"
#include "nn/solvers.h"
#include "nn/cpu_dlib.h"
#include "nn/tensor_tools.h"
#include "nn/utilities.h"
#include "nn/validation.h"
#include "dnn_base/serialize.h"
#include "matrix.h"
#include "dnn_base/cv_image.h"
#include "image_transforms/assign_image.h"
#include "image_transforms/interpolation.h"
#include "image_processing/frontal_face_detector.h"

/*
template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = add_prev1<block<N,BN,1,tag1<SUBNET>>>;

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = add_prev2<avg_pool<2,2,2,2,skip1<tag2<block<N,BN,2,tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET>
using block  = BN<con<N,3,3,1,1,relu<BN<con<N,3,3,stride,stride,SUBNET>>>>>;

template <int N, typename SUBNET> using ares      = relu<residual<block,N,affine,SUBNET>>;
template <int N, typename SUBNET> using ares_down = relu<residual_down<block,N,affine,SUBNET>>;
template <typename SUBNET> using alevel0 = ares_down<256,SUBNET>;
template <typename SUBNET> using alevel1 = ares<256,ares<256,ares_down<256,SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128,ares<128,ares_down<128,SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64,ares<64,ares<64,ares_down<64,SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32,ares<32,ares<32,SUBNET>>>;

using anet_type = loss_metric<fc_no_bias<128,avg_pool_everything<
                                alevel0<
                                alevel1<
                                alevel2<
                                alevel3<
                                alevel4<
                                max_pool<3,3,2,2,relu<affine<con<32,7,7,2,2,
                                input_rgb_image_sized<150>
                                >>>>>>>>>>>>;

using namespace Digikam::redeye;
class DNNFaceKernel
{
public:
	DNNFaceKernel(){};
	void getFaceVector(cv::Mat tmp_mat, std::vector<float>& vecdata)
	{          
		anet_type net;
	    frontal_face_detector detector = get_frontal_face_detector();
	    //qCDebug(DIGIKAM_FACEDB_LOG) << "Start reading model file";
	    QString path1 = QStandardPaths::locate(QStandardPaths::GenericDataLocation,
	                                              QLatin1String("digikam/facesengine/dlib_face_recognition_resnet_model_v1.dat")); 
	    deserialize(path1.toStdString()) >> net;
	    //qCDebug(DIGIKAM_FACEDB_LOG) << "End reading model file";
	    //qCDebug(DIGIKAM_FACEDB_LOG) << "Start reading shape file";
	    redeye::ShapePredictor sp;
	    QString path2 = QStandardPaths::locate(QStandardPaths::GenericDataLocation,
	                                              QLatin1String("digikam/facesengine/shapepredictor.dat"));
	    QFile model(path2);
	    redeye::ShapePredictor* const temp = new redeye::ShapePredictor();
	    std::cout << "read file\n";
	    if (model.open(QIODevice::ReadOnly))
	    {
	        QDataStream dataStream(&model);
	        dataStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
	        dataStream >> *temp;
	        sp = *temp;
	    }
	    else
	    {
	        std::cout << "Error open file shapepredictor.dat\n";
	        return ;
	    }
	    delete temp;

	    //cv::Mat tmp_mat = data.toMat();
	    matrix<rgb_pixel> img;
	    std::vector<matrix<rgb_pixel>> faces;
	    std::cout << "tmp_mat channels: " << tmp_mat.channels() << std::endl;
	    assign_image(img, cv_image<rgb_pixel>(tmp_mat));
	    bool face_flag = false;
	    for (auto face : detector(img))
	    {
	    	std::cout << "detected face\n";
	        face_flag = true;
	        cv::Mat gray;
	        
	        int type = tmp_mat.type();
	        std::cout << "type: " << type << std::endl;
	        if(type == CV_8UC3 || type == CV_16UC3)
	        {
	            cv::cvtColor(tmp_mat, gray, CV_RGB2GRAY);  // 3 channels
	        }
	        else
	        {
	            cv::cvtColor(tmp_mat, gray, CV_RGBA2GRAY);  // 4 channels
	        }

	        if (type == CV_16UC3 || type == CV_16UC4)
	        {
	            gray.convertTo(gray, CV_8UC1, 1 / 255.0);
	        }

	        cv::Rect new_rect(face.left(), face.top(), face.right()-face.left(), face.bottom()-face.top());
	        FullObjectDetection object = sp(gray,new_rect);
	        std::cout << "FullObjectDetection finished\n";
	        matrix<rgb_pixel> face_chip;
	        extract_image_chip(img, get_face_chip_details(object,150,0.25), face_chip);
	        std::cout << "extract_image_chip finished\n";
	        faces.push_back(move(face_chip));
	        break;
	    }
	    if(!face_flag)
	    {
	        cv::resize(tmp_mat, tmp_mat, cv::Size(150, 150));
	        assign_image(img, cv_image<rgb_pixel>(tmp_mat));
	        faces.push_back(img);
	    }
	    std::cout << "start net\n";
	    std::vector<matrix<float,0,1>> face_descriptors = net(faces);
	    std::cout << "face_descriptors size: " << face_descriptors.size() << std::endl;
	    if(face_descriptors.size()!=0)
	    {
	        vecdata.clear();
	        for(int i = 0; i < face_descriptors[0].nr(); i++)
	        {
	            for(int j = 0; j < face_descriptors[0].nc(); j++)
	            {
	                vecdata.push_back(face_descriptors[0](i, j));
	            }
	        }
	    }
        else
	    {
	        qCDebug(DIGIKAM_FACEDB_LOG) << "Error calculate face vector";
        }
	};
};
*/
#endif // _DNN_FACE_H_
