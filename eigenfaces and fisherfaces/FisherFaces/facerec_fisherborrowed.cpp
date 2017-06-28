/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date    2012-01-03
 * @brief   <a href="http://docs.opencv.org/modules/contrib/doc/facerec/facerec_tutorial.html#local-binary-patterns-histograms">Face Recognition based on Local Binary Patterns Histograms</a>
 *          Ahonen T, Hadid A. and Pietikäinen M. "Face description with local binary
 *          patterns: Application to face recognition." IEEE Transactions on Pattern
 *          Analysis and Machine Intelligence, 28(12):2037-2041.
 *
 * @section DESCRIPTION
 *
 * @author Copyright (C) 2012-2013 by Marcel Wiesweg
 *         <a href="mailto:marcel dot wiesweg at gmx dot de">marcel dot wiesweg at gmx dot de</a>
 * @author Copyright (c) 2011-2012 Philipp Wagner
 *         <a href="mailto:bytefish at gmx dot de">bytefish at gmx dot de</a>
 *
 * @section LICENSE
 *
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 *
 * ============================================================ */

#include "facerec_fisherborrowed.h"
#include "face_basic.hpp"

// C++ includes

#include <set>
#include <limits>

// Local includes

//#include "digikam_debug.h"

using namespace cv;

namespace FacesEngine
{

inline Mat asRowMatrix(vector<Mat> src, int rtype, double alpha=1, double beta=0)
{

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

// Removes duplicate elements in a given vector.
template<typename _Tp>
inline std::vector<_Tp> remove_dups(const std::vector<_Tp>& src) {
    typedef typename std::set<_Tp>::const_iterator constSetIterator;
    typedef typename std::vector<_Tp>::const_iterator constVecIterator;
    std::set<_Tp> set_elems;
    for (constVecIterator it = src.begin(); it != src.end(); ++it)
        set_elems.insert(*it);
    std::vector<_Tp> elems;
    for (constSetIterator it = set_elems.begin(); it != set_elems.end(); ++it)
        elems.push_back(*it);
    return elems;
}

void FisherFaceRecognizer::train(InputArrayOfArrays _in_src, InputArray _inm_labels)
{
    this->train(_in_src, _inm_labels, false);
}

void FisherFaceRecognizer::update(InputArrayOfArrays _in_src, InputArray _inm_labels)
{
    // got no data, just return
    if (_in_src.total() == 0)
        return;

    this->train(_in_src, _inm_labels, true);
}

void FisherFaceRecognizer::train(InputArrayOfArrays _in_src, InputArray _inm_labels, bool preserveData)
{

    if (_in_src.kind() != _InputArray::STD_VECTOR_MAT && _in_src.kind() != _InputArray::STD_VECTOR_VECTOR)
    {
        String error_message = "The images are expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< std::vector<...> >).";
        CV_Error(CV_StsBadArg, error_message);
    }

    if (_in_src.total() == 0)
    {
        String error_message = format("Empty training data was given. You'll need more than one sample to learn a model.");
        CV_Error(CV_StsUnsupportedFormat, error_message);
    }
    else if (_inm_labels.getMat().type() != CV_32SC1)
    {
        String error_message = format("Labels must be given as integer (CV_32SC1). Expected %d, but was %d.", CV_32SC1, _inm_labels.type());
        CV_Error(CV_StsUnsupportedFormat, error_message);
    }


    // get the vector of matrices
    std::vector<Mat> src;
    _in_src.getMatVector(src);

    // get the label matrix
    Mat labels = _inm_labels.getMat();

    // check if data is well- aligned
    /*
    if (labels.total() != src.size())
    {
        String error_message = format("The number of samples (src) must equal the number of labels (labels). Was len(samples)=%d, len(labels)=%d.", src.size(), m_labels.total());
        CV_Error(CV_StsBadArg, error_message);
    }
    */

    // if this model should be trained without preserving old data, delete old model data
    if (!preserveData)
    {
        m_labels.release();
        m_src.clear();
    }

    // append labels to m_labels matrix
    for (size_t labelIdx = 0; labelIdx < labels.total(); labelIdx++)
    {
        m_labels.push_back(labels.at<int>((int)labelIdx));
        m_src.push_back(src[(int)labelIdx]);
    }


    // observations in row
    Mat data = asRowMatrix(m_src, CV_64FC1);

    // number of samples
    int n = data.rows;

    // clear existing model data
    m_projections.clear();
    std::vector<int> ll;
    for(unsigned int i = 0; i < labels.total(); i++)
    {
        ll.push_back(labels.at<int>(i));
    }
    // get the number of unique classes
    int C = (int) remove_dups(ll).size();
    // clip number of components to be valid
    //if((m_num_components <= 0) || (m_num_components > n))
        m_num_components = (C-1);

    // perform the PCA
    PCA pca(data, Mat(), PCA::DATA_AS_ROW, (n-C));
    LDA lda(pca.project(data),labels, m_num_components);
    // Now calculate the projection matrix as pca.eigenvectors * lda.eigenvectors.
    // Note: OpenCV stores the eigenvectors by row, so we need to transpose it!
    gemm(pca.eigenvectors, lda.eigenvectors(), 1.0, Mat(), 0.0, m_eigenvectors, GEMM_1_T);
    // store the projections of the original data
    for(int sampleIdx = 0; sampleIdx < data.rows; sampleIdx++)
    {
        Mat p = LDA::subspaceProject(m_eigenvectors, m_mean, data.row(sampleIdx));
        m_projections.push_back(p);
    }

}

/*
#if OPENCV_TEST_VERSION(3,1,0)
void FisherFaceRecognizer::predict(InputArray _src, int &minClass, double &minDist) const
#else
void FisherFaceRecognizer::predict(cv::InputArray _src, cv::Ptr<cv::face::PredictCollector> collector) const
#endif
*/
void FisherFaceRecognizer::predict(InputArray _src, int &minClass, double &minDist) const
{
        minDist      = DBL_MAX;
        minClass     = -1;
        Mat img_gray = _src.getMat();
        Mat q = LDA::subspaceProject(m_eigenvectors, m_mean, img_gray.reshape(1, 1));
        for (size_t sampleIdx = 0; sampleIdx < m_projections.size(); sampleIdx++)
        {
            double dist = norm(m_projections[sampleIdx], q, NORM_L2);
            if(dist>=m_threshold)
                continue;
            //cout << dist << " ";
            if((dist < minDist)/* && (dist < m_threshold)*/)
            {
                minDist = dist;
                minClass = m_labels.at<int>((int) sampleIdx);
            }
        }
        Debug(minDist);
}

//#if OPENCV_TEST_VERSION(3,1,0)
int FisherFaceRecognizer::predict(InputArray _src) const
{
    int    label;
    double dummy;
    predict(_src, label, dummy);

    return label;
}
//#endif

// Static method ----------------------------------------------------

Ptr<FisherFaceRecognizer> FisherFaceRecognizer::create()
{
    Ptr<FisherFaceRecognizer> ptr;

    FisherFaceRecognizer* const fr = new FisherFaceRecognizer();

//    if (!fr)
//    {
//        qCWarning(DIGIKAM_FACESENGINE_LOG) << "Cannot create FisherFaceRecognizer instance";
//        return ptr;
//    }

    ptr = Ptr<FisherFaceRecognizer>(fr);

//    if (ptr.empty())
//    {
//        qCWarning(DIGIKAM_FACESENGINE_LOG) << "FisherFaceRecognizer instance is empty";
//    }

    return ptr;
}

} // namespace FacesEngine
