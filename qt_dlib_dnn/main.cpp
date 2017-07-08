#include <QCoreApplication>
#include "dnn/dnn.h"
#include "dnn/matrix.h"
#include "dnn/cv_image.h"
#include "dnn/image_transforms/assign_image.h"
#include <iostream>
#include <string>
#include <fstream>

using std::cout;
using std::endl;
#define Debug(x) cout << #x << "=" << (x) << endl;

#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/core/persistence.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <opencv2/objdetect/objdetect.hpp>

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

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    anet_type net;

    std::cout << "start read\n";
    //serialize.h 1521行读文件
    deserialize("dlib_face_recognition_resnet_model_v1.dat") >> net;
    std::cout << "read over\n";

    std::string trainf = "/home/yjwudi/face_recognizer/orl/orltrain.txt";
    std::string testf = "/home/yjwudi/face_recognizer/orl/orltest.txt";
    std::vector<std::string> train_vec, test_vec;
    std::vector<int> train_label, test_label;
    int i, j;
    std::vector<int> label_vec;

    std::ifstream in;
    in.open(trainf.c_str());
    if(in.bad())
    {
        cout << "no such file: " << trainf << endl;
        return  0;
    }
    std::string fname;
    int label;
    std::cout << "reading " << trainf << endl;
    while(in >> fname >> label)
    {
        train_vec.push_back(fname);
        train_label.push_back(label);
        //cout << fname << endl;
    }
    in.close();
    in.open(testf.c_str());
    if(in.bad())
    {
        cout << "no such file: " << testf << endl;
        return  0;
    }
    cout << "reading " << testf << endl;
    while(in >> fname >> label)
    {
        test_vec.push_back(fname);
        test_label.push_back(label);
        cout << fname << endl;
    }
    in.close();

    //training
    matrix<rgb_pixel> img;
    std::vector<matrix<rgb_pixel>> faces;
    std::vector<matrix<rgb_pixel>> test_faces;
    cout << "training...\n";
    for(i = 0; i < (int)train_vec.size(); i++)
    {
        Debug(i);
        cv::Mat tmp_mat = cv::imread(train_vec[i]);
        cv::resize(tmp_mat, tmp_mat, cv::Size(150, 150), (0, 0), (0, 0), cv::INTER_LINEAR);
        assign_image(img, cv_image<rgb_pixel>(tmp_mat));
        faces.push_back(img);
        /*
        int cc = 0;
        for (auto face : detector(img))
        {
            if(cc>1)
                break;
            auto shape = sp(img, face);
            matrix<rgb_pixel> face_chip;
            extract_image_chip(img, get_face_chip_details(shape,150,0.25), face_chip);
            faces.push_back(move(face_chip));
            cc++;
            // Also put some boxes on the faces so we can see that the detector is finding
            // them.
            //win.add_overlay(face);
            //image_window win(face);
            //cin.get();
        }
        if(cc==0)
        {
            puts("bad");
            cv::resize(tmp_mat, tmp_mat, cv::Size(150, 150));
            assign_image(img, cv_image<rgb_pixel>(tmp_mat));
            faces.push_back(img);
        }
        */
    }

    cout << "input\n";
    std::vector<matrix<float,0,1>> face_descriptors = net(faces);
    cout << "face descriptors size: " << face_descriptors.size() << endl;

/*
    for(i = 0; i < face_descriptors[0].nr(); i++)
    {
        for(j = 0; j < face_descriptors[0].nc(); j++)
        {
            cout << face_descriptors[0](i, j) << " ";
        }
        cout << endl;
    }
*/

    cout << "read testing...\n";
    for(i = 0; i < (int)test_vec.size(); i++)
    {
        Debug(i);
        cv::Mat tmp_mat = cv::imread(test_vec[i]);
        cv::resize(tmp_mat, tmp_mat, cv::Size(150, 150), (0, 0), (0, 0), cv::INTER_LINEAR);
        assign_image(img, cv_image<rgb_pixel>(tmp_mat));
        test_faces.push_back(img);
    }

    std::vector<matrix<float,0,1>> test_descriptors = net(test_faces);
    double dist, min_dist = 100000;
    int sum = 0;
    label = -1;
    for(i = 0; i < test_descriptors.size(); i++)
    {
        matrix<float,0,1> tmp_descriptor = test_descriptors[i];
        min_dist = 100000;
        for(j = 0; j < (int)face_descriptors.size(); j++)
        {
            dist = length(tmp_descriptor-face_descriptors[j]);
            if(dist < min_dist && dist < 0.6)
            {
                min_dist = dist;
                label = train_label[j];
            }
        }
        cout << label << " " << test_label[i] << endl;
        if(label == test_label[i])
        {
            sum++;
        }
    }
    cout << sum << "/" << test_vec.size() << endl;


    int aaa;
    std::cin >> aaa;

    return 0;
}



































