#include "FDetect.h"


FDetect::FDetect()
{

}

FDetect::FDetect(string path, string pic_path)
{
    ifstream in;
    in.open(path.c_str());
    if(in.bad())
    {
        cout << "no such file: " << path << endl;
        return ;
    }
    string fname;
    cout << "reading " << path << endl;
    while(in >> fname)
    {
        fname_vec.push_back(fname);
        cout << fname << endl;
    }
    in.close();
    in.open(pic_path.c_str());
    if(in.bad())
    {
        cout << "no such file: " << pic_path << endl;
        return ;
    }
    cout << "reading " << pic_path << endl;
    while(in >> fname)
    {
        face_name_vec.push_back(fname);
    }
    in.close();
}

void FDetect::detect_save()
{
    int i;
    for(i = 0; i < fname_vec.size(); i++)
    {
        string fname = fname_vec[i];
        cout << "detecting " << fname << endl;
        Mat img = imread(fname);
        Mat img_gray;
        cvtColor(img,img_gray,COLOR_BGR2GRAY );
        equalizeHist(img_gray,img_gray);
        vector<Rect> faces = detectFaces(img_gray);
        saveFaces(img,faces,i);
    }
}

vector<Rect> FDetect::detectFaces(Mat img_gray){
    CascadeClassifier faces_cascade;
    faces_cascade.load("/home/yjwudi/opencv/opencv/data/haarcascades/haarcascade_frontalface_alt.xml");
    vector<Rect> faces;
    faces_cascade.detectMultiScale(img_gray,faces,1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
    return faces;
}

void FDetect::saveFaces(Mat img,vector<Rect> rec, int idx)
{
    for(size_t i=0; i<rec.size();i++)
    {
        stringstream buffer;
        buffer<<idx<<"_"<<i;

        string saveName = face_name_vec[idx];
        if(i > 0)
        {
            saveName = "/home/yjwudi/face_recognizer/faces/"+buffer.str()+".jpg";
        }
        Rect roi = rec[i];
        cout << "saving " << saveName << endl;
        imwrite(saveName,img(roi));
    }
}































