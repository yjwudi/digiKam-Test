

#include "opencveigenfacerecognizer.h"

// local includes

//#include "libopencv.h"
//#include "facedbaccess.h"
//#include "facedb.h"
//#include "lbphfacemodel.h"
//#include "digikam_debug.h"
#include "../debugger.h"


namespace FacesEngine
{

class OpenCVEIGENFaceRecognizer::Private
{
public:

    Private()
        : threshold(10000),
          loaded(false)
    {
    }

public:

    EigenFaceModel& eigen()
    {
        if (!loaded)
        {
            //m_lbph = FaceDbAccess().db()->lbphFaceModel();
            m_eigen = EigenFaceModel();
            loaded = true;
        }

        return m_eigen;
    }

public:

    float             threshold;

private:

    EigenFaceModel     m_eigen;
    bool              loaded;
};

OpenCVEIGENFaceRecognizer::OpenCVEIGENFaceRecognizer()
    : d(new Private)
{
    //setThreshold(0.5);
}

OpenCVEIGENFaceRecognizer::~OpenCVEIGENFaceRecognizer()
{
    delete d;
}
/*
void OpenCVEIGENFaceRecognizer::setThreshold(float threshold) const
{
    // threshold for our purposes within 20..150
    const float min = 30.0;
    const float max = 150.0;
    // Applying a mirrored sigmoid curve
    // map threshold [0,1] to [-4, 4]
    float t         = (8.0 * qBound(0.f, threshold, 1.f)) - 4.0;
    // 1/(1+e^(t))
    float factor    = 1.0 / (1.0 + exp(t));
    d->threshold    = min + factor*(max-min);
}
*/
namespace
{
    enum
    {
        TargetInputSize = 256
    };
}

cv::Mat OpenCVEIGENFaceRecognizer::prepareForRecognition(const QImage& inputImage)
{
    QImage image(inputImage);

    if (inputImage.width() != TargetInputSize || inputImage.height() != TargetInputSize)
    {
        image = inputImage.scaled(TargetInputSize, TargetInputSize, Qt::IgnoreAspectRatio);
    }

    cv::Mat cvImage = cv::Mat(image.height(), image.width(), CV_8UC1);
    cv::Mat cvImageWrapper;

    switch (image.format())
    {
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied:
            // I think we can ignore premultiplication when converting to grayscale
            cvImageWrapper = cv::Mat(image.height(), image.width(), CV_8UC4, image.scanLine(0), image.bytesPerLine());
            cvtColor(cvImageWrapper, cvImage, CV_RGBA2GRAY);
            break;
        default:
            image          = image.convertToFormat(QImage::Format_RGB888);
            cvImageWrapper = cv::Mat(image.height(), image.width(), CV_8UC3, image.scanLine(0), image.bytesPerLine());
            cvtColor(cvImageWrapper, cvImage, CV_RGB2GRAY);
            break;
    }

    resize(cvImage, cvImage, Size(256, 256), (0, 0), (0, 0), INTER_LINEAR);
    equalizeHist(cvImage, cvImage);
    return cvImage;
}

int OpenCVEIGENFaceRecognizer::recognize(const cv::Mat& inputImage)
{
    int predictedLabel = -1;
    double confidence  = 0;
    d->eigen()->predict(inputImage, predictedLabel, confidence);
    cout << predictedLabel << " " << confidence << " ";
    //qCDebug(DIGIKAM_FACESENGINE_LOG) << predictedLabel << confidence;
/*
    if (confidence > d->threshold)
    {
        return -1;
    }
*/
    return predictedLabel;
}

void OpenCVEIGENFaceRecognizer::train(const std::vector<cv::Mat>& images, const std::vector<int>& labels, const QString& context)
{
    //qCDebug(DIGIKAM_FACESENGINE_LOG) << "train train train\n\n\n";
    if (images.empty() || labels.size() != images.size())
    {
        return;
    }

    d->eigen().update(images, labels, context);
    // add to database
    //FaceDbAccess().db()->updateLBPHFaceModel(d->lbph());
}

} // namespace FacesEngine
