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

#include "facerec_borrowed.h"

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

void EigenFaceRecognizer::train(InputArrayOfArrays _in_src, InputArray _inm_labels)
{
    this->train(_in_src, _inm_labels, false);
}

void EigenFaceRecognizer::update(InputArrayOfArrays _in_src, InputArray _inm_labels)
{
    // got no data, just return
    if (_in_src.total() == 0)
        return;

    this->train(_in_src, _inm_labels, true);
}

void EigenFaceRecognizer::train(InputArrayOfArrays _in_src, InputArray _inm_labels, bool preserveData)
{
    /*
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
    */

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
    if(m_labels.rows<320)
        return ;


    // observations in row
    Mat data = asRowMatrix(m_src, CV_64FC1);

    // number of samples
    int n = data.rows;

    // clear existing model data
    m_projections.clear();
    // clip number of components to be valid
    //if((m_num_components <= 0) || (m_num_components > n))
        m_num_components = n;

    // perform the PCA
    PCA pca(data, Mat(), PCA::DATA_AS_ROW, m_num_components);
    // copy the PCA results
    m_mean = pca.mean.reshape(1,1); // store the mean vector
    //m_eigenvalues = pca.eigenvalues.clone(); // eigenvalues by row
    transpose(pca.eigenvectors, m_eigenvectors); // eigenvectors by column

    // save projections
    for(int sampleIdx = 0; sampleIdx < data.rows; sampleIdx++)
    {
        Mat p = LDA::subspaceProject(m_eigenvectors, m_mean, data.row(sampleIdx));
        m_projections.push_back(p);
    }
}

/*
#if OPENCV_TEST_VERSION(3,1,0)
void EigenFaceRecognizer::predict(InputArray _src, int &minClass, double &minDist) const
#else
void EigenFaceRecognizer::predict(cv::InputArray _src, cv::Ptr<cv::face::PredictCollector> collector) const
#endif
*/
void EigenFaceRecognizer::predict(InputArray _src, int &minClass, double &minDist) const
{
    minDist      = DBL_MAX;
    minClass     = -1;
    Mat img_gray = _src.getMat();
    Mat q = LDA::subspaceProject(m_eigenvectors, m_mean, img_gray.reshape(1, 1));
    for (size_t sampleIdx = 0; sampleIdx < m_projections.size(); sampleIdx++)
    {
        double dist = norm(m_projections[sampleIdx], q, NORM_L2);
        //cout << dist << " ";
        if((dist < minDist)/* && (dist < m_threshold)*/)
        {
            minDist = dist;
            minClass = m_labels.at<int>((int) sampleIdx);
        }
    }
    //cout << endl;

//    if (m_histograms.empty())
//    {
//        // throw error if no data (or simply return -1?)
//        String error_message = "This LBPH model is not computed yet. Did you call the train method?";
//        CV_Error(CV_StsBadArg, error_message);
//    }

//    Mat src = _src.getMat();

//    // get the spatial histogram from input image
//    Mat lbp_image = elbp(src, m_radius, m_neighbors);
//    Mat query     = spatial_histogram(lbp_image,                                                         /* lbp_image                   */
//                                      static_cast<int>(std::pow(2.0, static_cast<double>(m_neighbors))), /* number of possible patterns */
//                                      m_grid_x,                                                          /* grid size x                 */
//                                      m_grid_y,                                                          /* grid size y                 */
//                                      true                                                               /* normed histograms           */
//                                     );
//#if OPENCV_TEST_VERSION(3,1,0)
//    minDist      = DBL_MAX;
//    minClass     = -1;
//#else
//    collector->init((int)m_histograms.size());
//#endif

//    // This is the standard method

//    if (m_statisticsMode == NearestNeighbor)
//    {
//        // find 1-nearest neighbor
//        for (size_t sampleIdx = 0; sampleIdx < m_histograms.size(); sampleIdx++)
//        {
//            double dist = compareHist(m_histograms[sampleIdx], query, CV_COMP_CHISQR);

//#if OPENCV_TEST_VERSION(3,1,0)
//            if ((dist < minDist) && (dist < m_threshold))
//            {
//                minDist  = dist;
//                minClass = m_labels.at<int>((int) sampleIdx);
//            }
//#else
//            int label = m_labels.at<int>((int) sampleIdx);

//            if (!collector->collect(label, dist))
//            {
//                return;
//            }
//#endif
//        }
//    }

//    // All other methods are just unvalidated examples.
//    // Development can take place only if there is proper alignment available

//    else if (m_statisticsMode == NearestMean)
//    {
//        // Create map "label -> vector of distances to all histograms for this label"
//        std::map<int, std::vector<int> > distancesMap;

//        for (size_t sampleIdx = 0; sampleIdx < m_histograms.size(); sampleIdx++)
//        {
//            double dist                 = compareHist(m_histograms[sampleIdx], query, CV_COMP_CHISQR);
//            std::vector<int>& distances = distancesMap[m_labels.at<int>((int) sampleIdx)];
//            distances.push_back(dist);
//        }

//        // Compute mean
//        QString s = QString::fromLatin1("Mean distances: ");
//        std::map<int, std::vector<int> >::const_iterator it;

//        for (it = distancesMap.begin(); it != distancesMap.end(); ++it)
//        {
//            double sum = 0;

//            for (std::vector<int>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
//            {
//                sum += *it2;
//            }

//            double mean = sum / it->second.size();
//            s          += QString::fromLatin1("%1: %2 - ").arg(it->first).arg(mean);

//#if OPENCV_TEST_VERSION(3,1,0)
//            if ((mean < minDist) && (mean < m_threshold))
//            {
//                minDist = mean;
//                minClass = it->first;
//            }
//#else
//            if (!collector->collect(it->first, mean))
//            {
//                return;
//            }
//#endif
//        }

//        qCDebug(DIGIKAM_FACESENGINE_LOG) << s;
//    }
//    else if (m_statisticsMode == MostNearestNeighbors)
//    {
//        // Create map "distance -> label"
//        std::multimap<double, int> distancesMap;

//        // map "label -> number of histograms"
//        std::map<int, int> countMap;

//        for (size_t sampleIdx = 0; sampleIdx < m_histograms.size(); sampleIdx++)
//        {
//            int label   = m_labels.at<int>((int) sampleIdx);
//            double dist = compareHist(m_histograms[sampleIdx], query, CV_COMP_CHISQR);
//            distancesMap.insert(std::pair<double, int>(dist, label));
//            countMap[label]++;
//        }

//        int nearestElementCount = cv::min(100, int(distancesMap.size()/3+1));

//        // map "label -> number of nearest neighbors"
//        std::map<int, int> scoreMap;

//        for (std::multimap<double, int>::iterator it = distancesMap.begin(); it != distancesMap.end() && nearestElementCount != 0; ++it, nearestElementCount--)
//        {
//            scoreMap[it->second]++;
//        }

//#if OPENCV_TEST_VERSION(3,1,0)
//        minDist   = 0;
//#endif
//        QString s = QString::fromLatin1("Nearest Neighbor score: ");

//        for (std::map<int,int>::iterator it = scoreMap.begin(); it != scoreMap.end(); ++it)
//        {
//            double score = double(it->second) / countMap.at(it->first);
//            s           += QString::fromLatin1("%1/%2 %3  ").arg(it->second).arg(countMap.at(it->first)).arg(score);

//#if OPENCV_TEST_VERSION(3,1,0)
//            if (score > minDist)
//            {
//                minDist  = score;
//                minClass = it->first;
//            }
//#else
//            // large is better thus it is -score.
//            if (!collector->collect(it->first, -score))
//            {
//                return;
//            }
//#endif
//        }

//        qCDebug(DIGIKAM_FACESENGINE_LOG) << s;
//    }
}

//#if OPENCV_TEST_VERSION(3,1,0)
int EigenFaceRecognizer::predict(InputArray _src) const
{
    int    label;
    double dummy;
    predict(_src, label, dummy);

    return label;
}
//#endif

// Static method ----------------------------------------------------

Ptr<EigenFaceRecognizer> EigenFaceRecognizer::create()
{
    Ptr<EigenFaceRecognizer> ptr;

    EigenFaceRecognizer* const fr = new EigenFaceRecognizer();

//    if (!fr)
//    {
//        qCWarning(DIGIKAM_FACESENGINE_LOG) << "Cannot create EigenFaceRecognizer instance";
//        return ptr;
//    }

    ptr = Ptr<EigenFaceRecognizer>(fr);

//    if (ptr.empty())
//    {
//        qCWarning(DIGIKAM_FACESENGINE_LOG) << "EigenFaceRecognizer instance is empty";
//    }

    return ptr;
}
/*
#if OPENCV_VERSION <= OPENCV_MAKE_VERSION(2,4,99)
    CV_INIT_ALGORITHM(EigenFaceRecognizer, "FaceRecognizer.LBPH-FacesEngine",
                      obj.info()->addParam(obj, "radius",     obj.m_radius);
                      obj.info()->addParam(obj, "neighbors",  obj.m_neighbors);
                      obj.info()->addParam(obj, "grid_x",     obj.m_grid_x);
                      obj.info()->addParam(obj, "grid_y",     obj.m_grid_y);
                      obj.info()->addParam(obj, "threshold",  obj.m_threshold);
                      obj.info()->addParam(obj, "histograms", obj.m_histograms);         // modification: Make Read/Write
                      obj.info()->addParam(obj, "labels",     obj.m_labels);             // modification: Make Read/Write
                      obj.info()->addParam(obj, "statistic",  obj.m_statisticsMode))     // modification: Add parameter
#endif
*/

} // namespace FacesEngine
