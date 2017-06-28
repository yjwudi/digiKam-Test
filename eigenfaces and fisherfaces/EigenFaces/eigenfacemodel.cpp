#include "eigenfacemodel.h"

// Qt includes

#include <QList>

// local includes

//#include "digikam_debug.h"

namespace FacesEngine
{

EigenFaceMatMetadata::EigenFaceMatMetadata()
    : databaseId(0),
      identity(0),
      storageStatus(Created)
{
}

EigenFaceMatMetadata::~EigenFaceMatMetadata()
{
}

// ------------------------------------------------------------------------------------

EigenFaceModel::EigenFaceModel()
    : cv::Ptr<EigenFaceRecognizer>(EigenFaceRecognizer::create()),
      databaseId(0)
{
    /*
#if OPENCV_TEST_VERSION(3,0,0)
    ptr()->set("threshold", 100.0);
#else
    ptr()->setThreshold(100.0);
#endif
*/
}

EigenFaceModel::~EigenFaceModel()
{
}

EigenFaceRecognizer* EigenFaceModel::ptr()
{
    EigenFaceRecognizer* const ptr = cv::Ptr<EigenFaceRecognizer>::operator FacesEngine::EigenFaceRecognizer*();

    //if (!ptr)
      //  qCWarning(DIGIKAM_FACESENGINE_LOG) << "EigenFaceRecognizer pointer is null";

    return ptr;
}

const EigenFaceRecognizer* EigenFaceModel::ptr() const
{
    //const EigenFaceRecognizer* const ptr = cv::Ptr<EigenFaceRecognizer>::operator const FacesEngine::EigenFaceRecognizer*();
    const EigenFaceRecognizer* const ptr = cv::Ptr<EigenFaceRecognizer>::operator FacesEngine::EigenFaceRecognizer*();
    /*
#if OPENCV_TEST_VERSION(3,0,0)
    const EigenFaceRecognizer* const ptr = cv::Ptr<EigenFaceRecognizer>::operator const FacesEngine::EigenFaceRecognizer*();
#else
    const EigenFaceRecognizer* const ptr = cv::Ptr<EigenFaceRecognizer>::operator FacesEngine::EigenFaceRecognizer*();
#endif

    if (!ptr)
        qCWarning(DIGIKAM_FACESENGINE_LOG) << "EigenFaceRecognizer pointer is null";
    */

    return ptr;
}

std::vector<Mat> EigenFaceModel::getSrc() const
{
    return ptr()->getSrc();
}

void EigenFaceModel::setSrc(std::vector<cv::Mat> new_src)
{
    ptr()->setSrc(new_src);
}

cv::Mat EigenFaceModel::getLabels() const
{
    return ptr()->getLabels();
}

void EigenFaceModel::setLabels(cv::Mat new_labels)
{
    ptr()->setLabels(new_labels);
}


QList<EigenFaceMatMetadata> EigenFaceModel::matMetadata() const
{
    return m_matMetadata;
}

/*
void EigenFaceModel::setWrittenToDatabase(int index, int id)
{
    m_histogramMetadata[index].databaseId    = id;
    m_histogramMetadata[index].storageStatus = LBPHistogramMetadata::InDatabase;
}
*/

//void EigenFaceModel::setHistograms(const QList<OpenCVMatData>& histograms, const QList<LBPHistogramMetadata>& histogramMetadata)
//{
//    /*
//     * Does not work with standard OpenCV, as these two params are declared read-only in OpenCV.
//     * One reason why we copied the code.
//     */
//    std::vector<cv::Mat> newHistograms;
//    cv::Mat newLabels;
//    newHistograms.reserve(histograms.size());
//    newLabels.reserve(histogramMetadata.size());

//    foreach (const OpenCVMatData& histogram, histograms)
//    {
//        newHistograms.push_back(histogram.toMat());
//    }

//    m_histogramMetadata.clear();

//    foreach (const LBPHistogramMetadata& metadata, histogramMetadata)
//    {
//        newLabels.push_back(metadata.identity);
//        m_histogramMetadata << metadata;
//    }

//#if OPENCV_TEST_VERSION(3,0,0)
//    std::vector<cv::Mat> currentHistograms = ptr()->get<std::vector<cv::Mat> >("histograms");
//    cv::Mat currentLabels                  = ptr()->get<cv::Mat>("labels");
//#else
//    std::vector<cv::Mat> currentHistograms = ptr()->getHistograms();
//    cv::Mat currentLabels                  = ptr()->getLabels();
//#endif

//    currentHistograms.insert(currentHistograms.end(), newHistograms.begin(), newHistograms.end());
//    currentLabels.push_back(newLabels);

//#if OPENCV_TEST_VERSION(3,0,0)
//    ptr()->set("histograms", currentHistograms);
//    ptr()->set("labels",     currentLabels);
//#else
//    ptr()->setHistograms(currentHistograms);
//    ptr()->setLabels(currentLabels);
//#endif

///*
//    //Most cumbersome and inefficient way through a file storage which we were forced to use if we used standard OpenCV
//    cv::FileStorage store(".yml", cv::FileStorage::WRITE + cv::FileStorage::MEMORY);
//    // store current parameters to preserve them
//    store << "radius"     << radius();
//    store << "neighbors"  << neighbors();
//    store << "grid_x"     << gridX();
//    store << "grid_y"     << gridY();
//    // Write histogram data
//    store << "histograms" << "[";

//    foreach (const OpenCVMatData& histogram, histograms)
//    {
//        store << histogram.toMat();
//    }

//    store << "]";
//    // write matching labels
//    cv::Mat labels;

//    foreach (const LBPHistogramMetadata& metadata, histogramMetadata)
//    {
//        labels.push_back(metadata.identity);
//    }

//    store << "labels" << labels;
//    // harvest
//    cv::String yaml = store.releaseAndGetString();

//    cv::FileStorage read(yaml, cv::FileStorage::READ + cv::FileStorage::MEMORY);
//    ptr()->load(read);
//*/
//}

void EigenFaceModel::update(const std::vector<cv::Mat>& images, const std::vector<int>& labels, const QString& context)
{
    ptr()->update(images, labels);

    // Update local information
    // We assume new labels are simply appended
    cv::Mat currentLabels = ptr()->getLabels();
//#if OPENCV_TEST_VERSION(3,0,0)
//    cv::Mat currentLabels = ptr()->get<cv::Mat>("labels");
//#else
//    cv::Mat currentLabels = ptr()->getLabels();
//#endif

    for (int i = m_matMetadata.size() ; i < currentLabels.rows ; i++)
    {
        EigenFaceMatMetadata metadata;
        metadata.storageStatus = EigenFaceMatMetadata::Created;
        metadata.identity      = currentLabels.at<int>(i);
        metadata.context       = context;
        m_matMetadata << metadata;
    }
}

} // namespace FacesEngine

