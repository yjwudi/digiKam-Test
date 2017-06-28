#include "fisherrec.h"


FisherRec::FisherRec()
{
}

FisherRec::FisherRec(string trainf, string testf)
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
void FisherRec::recognize()
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
    Ptr<BasicFaceRecognizer> model = createFisherFaceRecognizer();
    model->train(train_pic, train_label);

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
        int result = model->predict(img_gray);
        cout << fname << " " << result << " " << test_label[i] << endl;
        if(test_label[i]==result)
        {
            sum++;
        }
    }
    cout << "accuracy: " << sum << "/" << test_vec.size() << endl;

    /*
    // Here is how to get the eigenvalues of this Eigenfaces model:
    Mat eigenvalues = model->getEigenValues();
    // And we can do the same to display the Eigenvectors (read Eigenfaces):
    Mat W = model->getEigenVectors();
    // Get the sample mean from the training data
    Mat mean = model->getMean();
    imshow("mean", norm_0_255(mean.reshape(1, 256)));
    waitKey();
    */

}
Mat FisherRec::ave_mat(vector<Mat> mat_vec)
{
    if(mat_vec.size()==0)
        return Mat();
    Mat average=Mat::zeros(mat_vec[0].size(),CV_64FC1);
    int i, j, k;
    for(i = 0; i < mat_vec.size(); i++)
    {
        accumulate(mat_vec[i], average);
    }
    average /=  mat_vec.size();

    return average;
}

void FisherRec::recognize_avg()
{
    vector<Mat> train_pic;
    vector<int> label_vec;
    int i;
    cout << "reading train files\n";
    vector<Mat> kind_vec[45];
    for(i = 0; i < train_vec.size(); i++)
    {
        string fname = train_vec[i];
        cout << fname << endl;
        //Mat img = imread(fname, 1);
        Mat img_gray = imread(fname, 0);
        //cvtColor(img,img_gray,COLOR_BGR2GRAY );
        equalizeHist(img_gray,img_gray);
        resize(img_gray, img_gray, Size(256, 256), (0, 0), (0, 0), INTER_LINEAR);

        kind_vec[train_label[i]].push_back(img_gray);
    }

    for(i = 1; i <= 40; i++)
    {
        Mat tmp_mat = ave_mat(kind_vec[i]);
        train_pic.push_back(tmp_mat);
        label_vec.push_back(i);
    }


    cout << "training...\n";
    Ptr<BasicFaceRecognizer> model = createEigenFaceRecognizer();
    model->train(train_pic, label_vec);

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
        int result = model->predict(img_gray);
        cout << fname << " " << result << " " << test_label[i] << endl;
        if(test_label[i]==result)
        {
            sum++;
        }
    }
    cout << "accuracy: " << sum << "/" << test_vec.size() << endl;

}
Mat FisherRec::norm_0_255(InputArray _src)
{
    Mat src = _src.getMat();
    // Create and return normalized image:
    Mat dst;
    switch(src.channels()) {
    case 1:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}
















