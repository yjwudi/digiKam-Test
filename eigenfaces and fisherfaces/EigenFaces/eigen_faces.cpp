#include "eigen_faces.h"

Eigenfaces::Eigenfaces(string trainf, string testf)
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




//------------------------------------------------------------------------------
// Eigenfaces
//------------------------------------------------------------------------------
void Eigenfaces::train()
{

    vector<Mat> train_pic;
    int i;
    for(i = 0; i < train_vec.size(); i++)
    {
        string fname = train_vec[i];
        Mat img_gray = imread(fname, 0);
        equalizeHist(img_gray,img_gray);
        resize(img_gray, img_gray, Size(256, 256), (0, 0), (0, 0), INTER_LINEAR);
        train_pic.push_back(img_gray);
    }

    //Mat labels = _local_labels.getMat();
    // observations in row
    Mat data = asRowMatrix(train_pic, CV_64FC1);

    // number of samples
    int n = data.rows;

    // clear existing model data
    _projections.clear();
    // clip number of components to be valid
    if((_num_components <= 0) || (_num_components > n))
        _num_components = n;

    // perform the PCA
    PCA pca(data, Mat(), PCA::DATA_AS_ROW, _num_components);
    // copy the PCA results
    _mean = pca.mean.reshape(1,1); // store the mean vector
    _eigenvalues = pca.eigenvalues.clone(); // eigenvalues by row
    transpose(pca.eigenvectors, _eigenvectors); // eigenvectors by column
    // save projections
    for(int sampleIdx = 0; sampleIdx < data.rows; sampleIdx++) {
        Mat p = LDA::subspaceProject(_eigenvectors, _mean, data.row(sampleIdx));
        _projections.push_back(p);
    }
}

void Eigenfaces::predict() const
{
    int i, sum = 0;
    for(i = 0; i < test_vec.size(); i++)
    {
        string fname = test_vec[i];
        Mat img_gray = imread(fname, 0);
        equalizeHist(img_gray,img_gray);
        resize(img_gray, img_gray, Size(256, 256), (0, 0), (0, 0), INTER_LINEAR);
        Mat q = LDA::subspaceProject(_eigenvectors, _mean, img_gray.reshape(1, 1));
        double dist = 0, min_dist, label = -1;
        for (size_t sampleIdx = 0; sampleIdx < _projections.size(); sampleIdx++)
        {
            double dist = norm(_projections[sampleIdx], q, NORM_L2);
            if(sampleIdx==0)
            {
                min_dist = dist;
            }
            if(dist < min_dist)
            {
                min_dist = dist;
                label = train_label[sampleIdx];
            }
        }
        cout << fname << " " << label << " " << test_label[i] << endl;
        if(label==test_label[i])
        {
            sum++;
        }
    }

    cout << "accuracy: " << sum << "/" << test_vec.size() << endl;

}
Mat Eigenfaces::asRowMatrix(vector<Mat> src, int rtype, double alpha, double beta) {

    // number of samples
    size_t n = src.size();
    // return empty matrix if no matrices given
    if(n == 0)
        return Mat();
    // dimensionality of (reshaped) samples
    size_t d = src[0].total();
    // create data matrix
    Mat data((int)n, (int)d, rtype);
    // now copy data
    for(unsigned int i = 0; i < n; i++)
    {
        Mat xi = data.row(i);
        // make reshape happy by cloning for non-continuous matrices
        if(src[i].isContinuous())
        {
            src[i].reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        }
        else
        {
            src[i].clone().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        }
    }
    return data;
}
