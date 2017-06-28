#ifndef LBPREC_H
#define LBPREC_H

#include "debugger.h"

class LBPRec
{
public:
    LBPRec();
    LBPRec(string tainf, string testf);
    void recognize();




    vector<string> train_vec, test_vec;
    vector<int> train_label, test_label;
};

#endif // LBPREC_H

