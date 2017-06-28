#include "debugger.h"
#include "FDetect.h"
#include "lbprec.h"
#include "eigenrec.h"
#include "fisherrec.h"
#include "EigenFaces/eigen_faces.h"
#include "EigenFaces/opencveigenfacerecognizer.h"
#include "FisherFaces/opencvfisherfacerecognizer.h"

using namespace FacesEngine;


void run_model(string trainf, string testf);
void run_fisher(string trainf, string testf);
void run_model_ave(string trainf, string testf);
Mat ave_mat(vector<Mat> mat_vec);
int main()
{
    string path1 = "/home/yjwudi/face_recognizer/pic_name.txt";
    string path2 = "/home/yjwudi/face_recognizer/pic_face_name.txt";
    string path3 = "/home/yjwudi/face_recognizer/train_sample.txt";
    string path4 = "/home/yjwudi/face_recognizer/test_sample.txt";

    string orltrain = "/home/yjwudi/face_recognizer/orl/orltrain.txt";
    string orltest = "/home/yjwudi/face_recognizer/orl/orltest.txt";

    //face recognization using opencv fisherfaces
    //FisherRec fisher_model(orltrain, orltest);
    //fisher_model.recognize();
    run_fisher(orltrain, orltest);


    //face recognization using eigenfaces(naive)
    //run_model(orltrain, orltest);

    //face recognization using eigenfaces(average)
    //run_model_ave(orltrain, orltest);

    //face recognization using opencv eigenfaces(naive)
    //EGRec eg_model(orltrain, orltest);
    //eg_model.recognize();

/*
    Eigenfaces ef_model(orltrain, orltest);
    ef_model.train();
    ef_model.predict();
*/
    //EGRec eg_model(orltrain, orltest);
    //eg_model.recognize_avg();
    //LBPRec lbp_model(orltrain, orltest);
    //lbp_model.recognize();

/*
    string pgm = "/home/yjwudi/face_recognizer/orl/s1/1.pgm";
    Mat mat = imread(pgm);
    cout << mat.channels() << endl;
    cvtColor(mat, mat, CV_RGB2GRAY);
    cout << mat.channels() << endl;
    namedWindow("win");
    imshow("win",mat);
    waitKey();
*/
    /*
     * detect faces
     *
    FDetect det(path1, path2);
    det.detect_save();
    */

    //face recognization using opencv lbp
    //LBPRec lbp_model(path3, path4);
    //lbp_model.recognize();

    //face recognization using opencv eigenfaces(naive)
    //EGRec eg_model(path3, path4);
    //eg_model.recognize();

    //face recognization using opencv eigenfaces(average)
    //EGRec eg_model(path3, path4);
    //eg_model.recognize_avg();

    /*
    Eigenfaces ef_model(path3, path4);
    ef_model.train();
    ef_model.predict();
    */

    //face recognization using eigenfaces(naive)
    //run_model(path3, path4);

    //face recognization using eigenfaces(average)
    //run_model_ave(path3, path4);


    return 0;
}

void run_model_ave(string trainf, string testf)
{
    OpenCVEIGENFaceRecognizer model;
    vector<string> train_vec, test_vec;
    vector<int> train_label, test_label;
    int i, j;
    vector<Mat> train_mat;
    vector<int> label_vec;

    //reading input file
    ifstream in;
    in.open(trainf.c_str());
    if(in.bad())
    {
        cout << "no such file: " << trainf << endl;
        return ;
    }
    string fname;
    int label;
    cout << "reading " << trainf << endl;
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
        return ;
    }
    cout << "reading " << testf << endl;
    while(in >> fname >> label)
    {
        test_vec.push_back(fname);
        test_label.push_back(label);
        //cout << fname << endl;
    }
    in.close();

    //training
    vector<Mat> kind_vec[45];
    for(i = 0; i < train_vec.size(); i++)
    {
        Debug(i);
        QImage qimg(QString::fromStdString(train_vec[i]));
        Mat tmp_mat = model.prepareForRecognition(qimg);
        kind_vec[train_label[i]].push_back(tmp_mat);
        //kind_label[train_label[i]].push_back(train_label[i]);
    }
    for(i = 1; i <= 40; i++)
    {
        Mat tmp_mat = ave_mat(kind_vec[i]);
        train_mat.clear();
        label_vec.clear();
        train_mat.push_back(tmp_mat);
        label_vec.push_back(i);
        QString context;
        if(i==1)
        {
            context = "Jay";
        }
        else
        {
            context = "Robert";
        }
        model.train(train_mat, label_vec, context);
    }

    //testing
    int sum = 0;
    for(i = 0; i < test_vec.size(); i++)
    {
        QImage qimg(QString::fromStdString(test_vec[i]));
        Mat tmp_mat = model.prepareForRecognition(qimg);
        int label = model.recognize(tmp_mat);
        cout << test_label[i] << endl;
        if(label == test_label[i])
        {
            sum++;
        }
    }
    cout << sum << "/" << test_vec.size() << endl;
}

Mat ave_mat(vector<Mat> mat_vec)
{
    if(mat_vec.size()==0)
        return Mat();
    Mat average=Mat::zeros(mat_vec[0].size(),CV_64FC1);
    int i, j, k;
    for(i = 0; i < mat_vec.size(); i++)
    {
        accumulate(mat_vec[i], average);
    }
    /*
    for(j = 0; j < average.rows; j++)
    {
        for(k = 0; k < average.cols; k++)
        {
            double sum = 0.0;
            for(i = 0; i < mat_vec.size(); i++)
            {
                sum += (double)mat_vec[i].at<uchar>(j,k);
            }
            cout << sum << " " << average.at<float>(j,k) << endl;
        }
    }
    */
    //cout << average.at<double>(100,100) << endl;
    average /=  mat_vec.size();
    //cout << average.at<double>(100,100) << endl;

    return average;
}

void run_fisher(string trainf, string testf)
{
    OpenCVFISHERFaceRecognizer model;
    vector<string> train_vec, test_vec;
    vector<int> train_label, test_label;
    int i, j;
    vector<Mat> train_mat;
    vector<int> label_vec;

    //reading input file
    ifstream in;
    in.open(trainf.c_str());
    if(in.bad())
    {
        cout << "no such file: " << trainf << endl;
        return ;
    }
    string fname;
    int label;
    cout << "reading " << trainf << endl;
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
        return ;
    }
    cout << "reading " << testf << endl;
    while(in >> fname >> label)
    {
        test_vec.push_back(fname);
        test_label.push_back(label);
        //cout << fname << endl;
    }
    in.close();

    //training
    cout << "training...\n";
    for(i = 0; i < train_vec.size(); i++)
    {
        Debug(i);
        QImage qimg(QString::fromStdString(train_vec[i]));
        Mat tmp_mat = model.prepareForRecognition(qimg);
        //Mat tmp_mat = imread(train_vec[i], 0);
        equalizeHist(tmp_mat,tmp_mat);
        resize(tmp_mat, tmp_mat, Size(256, 256), (0, 0), (0, 0), INTER_LINEAR);
        //train_mat.clear();
        //label_vec.clear();
        train_mat.push_back(tmp_mat);
        label_vec.push_back(train_label[i]);
        QString context;
        if(train_label[i]==1)
        {
            context = "Jay";
        }
        else
        {
            context = "Robert";
        }
        //model.train(train_mat, label_vec, context);
    }
    QString context = "Robert";
    model.train(train_mat, label_vec, context);

    //testing
    int sum = 0;
    cout << "testing\n";
    for(i = 0; i < test_vec.size(); i++)
    {
        Debug(i);
        //QImage qimg(QString::fromStdString(test_vec[i]));
        //Mat tmp_mat = model.prepareForRecognition(qimg);
        Mat tmp_mat = imread(test_vec[i], 0);
        equalizeHist(tmp_mat,tmp_mat);
        resize(tmp_mat, tmp_mat, Size(256, 256), (0, 0), (0, 0), INTER_LINEAR);
        int label = model.recognize(tmp_mat);
        cout << test_label[i] << endl;
        if(label == test_label[i])
        {
            sum++;
        }
    }
    cout << sum << "/" << test_vec.size() << endl;
}

void run_model(string trainf, string testf)
{
    OpenCVEIGENFaceRecognizer model;
    vector<string> train_vec, test_vec;
    vector<int> train_label, test_label;
    int i, j;
    vector<Mat> train_mat;
    vector<int> label_vec;

    //reading input file
    ifstream in;
    in.open(trainf.c_str());
    if(in.bad())
    {
        cout << "no such file: " << trainf << endl;
        return ;
    }
    string fname;
    int label;
    cout << "reading " << trainf << endl;
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
        return ;
    }
    cout << "reading " << testf << endl;
    while(in >> fname >> label)
    {
        test_vec.push_back(fname);
        test_label.push_back(label);
        //cout << fname << endl;
    }
    in.close();

    //training
    cout << "training...\n";
    for(i = 0; i < train_vec.size(); i++)
    {
        Debug(i);
        QImage qimg(QString::fromStdString(train_vec[i]));
        Mat tmp_mat = model.prepareForRecognition(qimg);
        //Mat tmp_mat = imread(train_vec[i], 0);
        equalizeHist(tmp_mat,tmp_mat);
        resize(tmp_mat, tmp_mat, Size(256, 256), (0, 0), (0, 0), INTER_LINEAR);
        train_mat.clear();
        label_vec.clear();
        train_mat.push_back(tmp_mat); 
        label_vec.push_back(train_label[i]);
        QString context;
        if(train_label[i]==1)
        {
            context = "Jay";
        }
        else
        {
            context = "Robert";
        }
        model.train(train_mat, label_vec, context);
    }
    QString context = "Robert";
    //model.train(train_mat, label_vec, context);

    //testing
    int sum = 0;
    cout << "testing\n";
    for(i = 0; i < test_vec.size(); i++)
    {
        Debug(i);
        //QImage qimg(QString::fromStdString(test_vec[i]));
        //Mat tmp_mat = model.prepareForRecognition(qimg);
        Mat tmp_mat = imread(test_vec[i], 0);
        equalizeHist(tmp_mat,tmp_mat);
        resize(tmp_mat, tmp_mat, Size(256, 256), (0, 0), (0, 0), INTER_LINEAR);
        int label = model.recognize(tmp_mat);
        cout << test_label[i] << endl;
        if(label == test_label[i])
        {
            sum++;
        }
    }
    cout << sum << "/" << test_vec.size() << endl;
}








