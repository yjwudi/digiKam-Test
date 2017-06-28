#ifndef OpenCVFISHERFaceRecognizer_H
#define OpenCVFISHERFaceRecognizer_H




//#include "libopencv.h"

// Qt include

#include <QImage>

// Local includes

//#include "opencvmatdata.h"
#include "fisherfacemodel.h"

namespace FacesEngine
{

class OpenCVFISHERFaceRecognizer
{

public:

    /**
     *  @brief FaceRecognizer:Master class to control entire recognition using LBPH algorithm
     */
    OpenCVFISHERFaceRecognizer();
    ~OpenCVFISHERFaceRecognizer();

    //void setThreshold(float threshold) const;

    /**
     *  Returns a cvMat created from the inputImage, optimized for recognition
     */
    cv::Mat prepareForRecognition(const QImage& inputImage);

    /**
     *  Try to recognize the given image.
     *  Returns the identity id.
     *  If the identity cannot be recognized, returns -1.
     */
    int recognize(const cv::Mat& inputImage);

    /**
     *  Trains the given images, representing faces of the given matched identities.
     */
    void train(const std::vector<cv::Mat>& images, const std::vector<int>& labels, const QString& context);

private:

    class Private;
    Private* const d;
};

} // namespace FacesEngine




#endif // OpenCVFISHERFaceRecognizer_H

