#ifndef EIGENREC_H
#define EIGENREC_H

#include "debugger.h"

class EGRec
{
public:
    EGRec();
    EGRec(string trainf, string testf);
    void recognize();
    Mat norm_0_255(InputArray _src);
    Mat ave_mat(vector<Mat> mat_vec);
    void recognize_avg();


    vector<string> train_vec, test_vec;
    vector<int> train_label, test_label;
};




#endif // EIGENREC_H

