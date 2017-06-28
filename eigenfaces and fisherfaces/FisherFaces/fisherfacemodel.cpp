#include "fisherfacemodel.h"

// Qt includes

#include <QList>

// local includes

//#include "digikam_debug.h"

namespace FacesEngine
{

FisherFaceMatMetadata::FisherFaceMatMetadata()
    : databaseId(0),
      identity(0),
      storageStatus(Created)
{
}

FisherFaceMatMetadata::~FisherFaceMatMetadata()
{
}

// ------------------------------------------------------------------------------------

FisherFaceModel::FisherFaceModel()
    : cv::Ptr<FisherFaceRecognizer>(FisherFaceRecognizer::create()),
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

FisherFaceModel::~FisherFaceModel()
{
}

FisherFaceRecognizer* FisherFaceModel::ptr()
{
    FisherFaceRecognizer* const ptr = cv::Ptr<FisherFaceRecognizer>::operator FacesEngine::FisherFaceRecognizer*();

    //if (!ptr)
      //  qCWarning(DIGIKAM_FACESENGINE_LOG) << "FisherFaceRecognizer pointer is null";

    return ptr;
}

const FisherFaceRecognizer* FisherFaceModel::ptr() const
{
    //const FisherFaceRecognizer* const ptr = cv::Ptr<FisherFaceRecognizer>::operator const FacesEngine::FisherFaceRecognizer*();
    const FisherFaceRecognizer* const ptr = cv::Ptr<FisherFaceRecognizer>::operator FacesEngine::FisherFaceRecognizer*();
    /*
#if OPENCV_TEST_VERSION(3,0,0)
    const FisherFaceRecognizer* const ptr = cv::Ptr<FisherFaceRecognizer>::operator const FacesEngine::FisherFaceRecognizer*();
#else
    const FisherFaceRecognizer* const ptr = cv::Ptr<FisherFaceRecognizer>::operator FacesEngine::FisherFaceRecognizer*();
#endif

    if (!ptr)
        qCWarning(DIGIKAM_FACESENGINE_LOG) << "FisherFaceRecognizer pointer is null";
    */

    return ptr;
}

std::vector<Mat> FisherFaceModel::getSrc() const
{
    return ptr()->getSrc();
}

void FisherFaceModel::setSrc(std::vector<cv::Mat> new_src)
{
    ptr()->setSrc(new_src);
}

cv::Mat FisherFaceModel::getLabels() const
{
    return ptr()->getLabels();
}

void FisherFaceModel::setLabels(cv::Mat new_labels)
{
    ptr()->setLabels(new_labels);
}


QList<FisherFaceMatMetadata> FisherFaceModel::matMetadata() const
{
    return m_matMetadata;
}

/*
void FisherFaceModel::setWrittenToDatabase(int index, int id)
{
    m_histogramMetadata[index].databaseId    = id;
    m_histogramMetadata[index].storageStatus = LBPHistogramMetadata::InDatabase;
}
*/

//void FisherFaceModel::setHistograms(const QList<OpenCVMatData>& histograms, const QList<LBPHistogramMetadata>& histogramMetadata)
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

void FisherFaceModel::update(const std::vector<cv::Mat>& images, const std::vector<int>& labels, const QString& context)
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
        FisherFaceMatMetadata metadata;
        metadata.storageStatus = FisherFaceMatMetadata::Created;
        metadata.identity      = currentLabels.at<int>(i);
        metadata.context       = context;
        m_matMetadata << metadata;
    }
}

} // namespace FacesEngine

