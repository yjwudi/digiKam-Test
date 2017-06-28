#ifndef FisherFaceModel_H
#define FisherFaceModel_H

// OpenCV library

//#include "libopencv.h"

// Qt include

#include <QList>

// Local includes

//#include "opencvmatdata.h"
#include "facerec_fisherborrowed.h"

namespace FacesEngine
{

class FisherFaceMatMetadata
{
public:

    enum StorageStatus
    {
        Created,
        InDatabase
    };

public:

    FisherFaceMatMetadata();
    ~FisherFaceMatMetadata();

public:

    int           databaseId;
    int           identity;//label
    QString       context;

    StorageStatus storageStatus;
};

// -------------------------------------------------------------------------------------------------------------------------------------

class FisherFaceModel : public cv::Ptr<FisherFaceRecognizer>
{
public:

    FisherFaceModel();
    ~FisherFaceModel();

    FisherFaceRecognizer*       ptr();
    const FisherFaceRecognizer* ptr() const;

    std::vector<Mat> getSrc() const;
    void setSrc(std::vector<cv::Mat> new_src);

    cv::Mat getLabels() const;
    void setLabels(cv::Mat new_labels);



    QList<FisherFaceMatMetadata> matMetadata() const;
    //OpenCVMatData               histogramData(int index) const;

    //void setWrittenToDatabase(int index, int databaseId);

    //void setHistograms(const QList<OpenCVMatData>& histograms, const QList<LBPHistogramMetadata>& histogramMetadata);

    /// Make sure to call this instead of FaceRecognizer::update directly!
    void update(const std::vector<cv::Mat>& images, const std::vector<int>& labels, const QString& context);

public:

    int databaseId;

protected:

    QList<FisherFaceMatMetadata> m_matMetadata;
};

} // namespace FacesEngine

#endif // FisherFaceModel_H

