
#include "lbprec.h"


LBPRec::LBPRec()
{
}

LBPRec::LBPRec(string trainf, string testf)
{
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
        cout << fname << endl;
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
        cout << fname << endl;
    }
    in.close();
}

void LBPRec::recognize()
{
    vector<Mat> train_pic;
    int i;
    cout << "reading train files\n";
    for(i = 0; i < train_vec.size(); i++)
    {
        string fname = train_vec[i];
        cout << fname << endl;
        //Mat img = imread(fname, 1);
        Mat img_gray = imread(fname, 0);
        //cvtColor(img,img_gray,COLOR_BGR2GRAY );
        equalizeHist(img_gray,img_gray);
        resize(img_gray, img_gray, Size(256, 256), (0, 0), (0, 0), INTER_LINEAR);
        train_pic.push_back(img_gray);
    }
    cout << "training...\n";
    Ptr<FaceRecognizer> LBPHRecog = createLBPHFaceRecognizer(1, 8 ,3, 3, 50);
    LBPHRecog->train(train_pic, train_label);

    cout << "reading test files\n";
    int sum = 0;
    for(i =0; i < test_vec.size(); i++)
    {
        string fname = test_vec[i];
        cout << fname << endl;
        Mat img = imread(fname);
        Mat img_gray;
        cvtColor(img,img_gray,COLOR_BGR2GRAY );
        equalizeHist(img_gray,img_gray);
        resize(img_gray, img_gray, Size(256, 256), (0, 0), (0, 0), INTER_LINEAR);
        int result = LBPHRecog->predict(img_gray);
        cout << fname << " " << result << " " << test_label[i] << endl;
        if(test_label[i]==result)
        {
            sum++;
        }
    }
    cout << "accuracy: " << sum << "/" << test_vec.size() << endl;
}



























