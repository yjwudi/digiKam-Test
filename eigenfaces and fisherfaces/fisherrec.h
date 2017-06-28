#ifndef FISHERREC_H
#define FISHERREC_H

#include "debugger.h"

class FisherRec
{
public:
    FisherRec();
    FisherRec(string trainf, string testf);
    void recognize();
    Mat norm_0_255(InputArray _src);
    Mat ave_mat(vector<Mat> mat_vec);
    void recognize_avg();


    vector<string> train_vec, test_vec;
    vector<int> train_label, test_label;
};


#endif // FISHERREC_H
