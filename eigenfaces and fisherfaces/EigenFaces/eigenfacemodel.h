#ifndef EIGENFACEMODEL_H
#define EIGENFACEMODEL_H

// OpenCV library

//#include "libopencv.h"

// Qt include

#include <QList>

// Local includes

//#include "opencvmatdata.h"
#include "facerec_borrowed.h"

namespace FacesEngine
{

class EigenFaceMatMetadata
{
public:

    enum StorageStatus
    {
        Created,
        InDatabase
    };

public:

    EigenFaceMatMetadata();
    ~EigenFaceMatMetadata();

public:

    int           databaseId;
    int           identity;//label
    QString       context;

    StorageStatus storageStatus;
};

// -------------------------------------------------------------------------------------------------------------------------------------

class EigenFaceModel : public cv::Ptr<EigenFaceRecognizer>
{
public:

    EigenFaceModel();
    ~EigenFaceModel();

    EigenFaceRecognizer*       ptr();
    const EigenFaceRecognizer* ptr() const;

    std::vector<Mat> getSrc() const;
    void setSrc(std::vector<cv::Mat> new_src);

    cv::Mat getLabels() const;
    void setLabels(cv::Mat new_labels);



    QList<EigenFaceMatMetadata> matMetadata() const;
    //OpenCVMatData               histogramData(int index) const;

    //void setWrittenToDatabase(int index, int databaseId);

    //void setHistograms(const QList<OpenCVMatData>& histograms, const QList<LBPHistogramMetadata>& histogramMetadata);

    /// Make sure to call this instead of FaceRecognizer::update directly!
    void update(const std::vector<cv::Mat>& images, const std::vector<int>& labels, const QString& context);

public:

    int databaseId;

protected:

    QList<EigenFaceMatMetadata> m_matMetadata;
};

} // namespace FacesEngine

#endif // EIGENFACEMODEL_H

