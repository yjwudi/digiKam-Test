#ifndef EIGEN_FACES_H
#define EIGEN_FACES_H

#include "debugger.h"

class CV_EXPORTS_W Fisherfaces
{

public:
    // Initializes an empty Fisherfaces model.
    Fisherfaces();
    Fisherfaces(string tainf, string testf);

    // Computes an Fisherfaces model with images in src and corresponding labels
    // in labels.
    void train();

    // Send all predict results to caller sit asRowMatrix(InputArrayOfArrays src, int rtype, double alpha=1, double beta=0) {
    // make sure the input data is a vector of matrices or vector of vectorde for custom result handling
    void predict() const;
    Mat asRowMatrix(vector<Mat> src, int rtype, double alpha=1, double beta=0);


    vector<string> train_vec, test_vec;
    vector<int> train_label, test_label;

    int _num_components;
    vector<Mat> _projections;
    Mat _labels;
    Mat _eigenvectors;
    Mat _eigenvalues;
    Mat _mean;
};



#endif // EIGEN_FACES_H

