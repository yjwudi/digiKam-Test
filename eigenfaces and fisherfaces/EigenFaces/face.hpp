
#ifndef __OPENCV_FACE_HPP__
#define __OPENCV_FACE_HPP__

/**
@defgroup face Face Recognition

- @ref face_changelog
- @ref tutorial_face_main

*/

#include "opencv2/core.hpp"
//#include "predict_collector.hpp"
#include <map>

namespace cv
{
namespace face
{



class CV_EXPORTS_W FaceRecognizer : public Algorithm
{
public:
    /** @brief Trains a FaceRecognizer with given data and associated labels.

    @param src The training images, that means the faces you want to learn. The data has to be
    given as a vector\<Mat\>.
    @param labels The labels corresponding to the images have to be given either as a vector\<int\>
    or a

    The following source code snippet shows you how to learn a Fisherfaces model on a given set of
    images. The images are read with imread and pushed into a std::vector\<Mat\>. The labels of each
    image are stored within a std::vector\<int\> (you could also use a Mat of type CV_32SC1). Think of
    the label as the subject (the person) this image belongs to, so same subjects (persons) should have
    the same label. For the available FaceRecognizer you don't have to pay any attention to the order of
    the labels, just make sure same persons have the same label:

    @code
    // holds images and labels
    vector<Mat> images;
    vector<int> labels;
    // images for first person
    images.push_back(imread("person0/0.jpg", CV_LOAD_IMAGE_GRAYSCALE)); labels.push_back(0);
    images.push_back(imread("person0/1.jpg", CV_LOAD_IMAGE_GRAYSCALE)); labels.push_back(0);
    images.push_back(imread("person0/2.jpg", CV_LOAD_IMAGE_GRAYSCALE)); labels.push_back(0);
    // images for second person
    images.push_back(imread("person1/0.jpg", CV_LOAD_IMAGE_GRAYSCALE)); labels.push_back(1);
    images.push_back(imread("person1/1.jpg", CV_LOAD_IMAGE_GRAYSCALE)); labels.push_back(1);
    images.push_back(imread("person1/2.jpg", CV_LOAD_IMAGE_GRAYSCALE)); labels.push_back(1);
    @endcode

    Now that you have read some images, we can create a new FaceRecognizer. In this example I'll create
    a Fisherfaces model and decide to keep all of the possible Fisherfaces:

    @code
    // Create a new Fisherfaces model and retain all available Fisherfaces,
    // this is the most common usage of this specific FaceRecognizer:
    //
    Ptr<FaceRecognizer> model =  createFisherFaceRecognizer();
    @endcode

    And finally train it on the given dataset (the face images and labels):

    @code
    // This is the common interface to train all of the available cv::FaceRecognizer
    // implementations:
    //
    model->train(images, labels);
    @endcode
     */
    CV_WRAP virtual void train(InputArrayOfArrays src, InputArray labels) = 0;

    /** @brief Updates a FaceRecognizer with given data and associated labels.

    @param src The training images, that means the faces you want to learn. The data has to be given
    as a vector\<Mat\>.
    @param labels The labels corresponding to the images have to be given either as a vector\<int\> or
    a

    This method updates a (probably trained) FaceRecognizer, but only if the algorithm supports it. The
    Local Binary Patterns Histograms (LBPH) recognizer (see createLBPHFaceRecognizer) can be updated.
    For the Eigenfaces and Fisherfaces method, this is algorithmically not possible and you have to
    re-estimate the model with FaceRecognizer::train. In any case, a call to train empties the existing
    model and learns a new model, while update does not delete any model data.

    @code
    // Create a new LBPH model (it can be updated) and use the default parameters,
    // this is the most common usage of this specific FaceRecognizer:
    //
    Ptr<FaceRecognizer> model =  createLBPHFaceRecognizer();
    // This is the common interface to train all of the available cv::FaceRecognizer
    // implementations:
    //
    model->train(images, labels);
    // Some containers to hold new image:
    vector<Mat> newImages;
    vector<int> newLabels;
    // You should add some images to the containers:
    //
    // ...
    //
    // Now updating the model is as easy as calling:
    model->update(newImages,newLabels);
    // This will preserve the old model data and extend the existing model
    // with the new features extracted from newImages!
    @endcode

    Calling update on an Eigenfaces model (see createEigenFaceRecognizer), which doesn't support
    updating, will throw an error similar to:

    @code
    OpenCV Error: The function/feature is not implemented (This FaceRecognizer (FaceRecognizer.Eigenfaces) does not support updating, you have to use FaceRecognizer::train to update it.) in update, file /home/philipp/git/opencv/modules/contrib/src/facerec.cpp, line 305
    terminate called after throwing an instance of 'cv::Exception'
    @endcode

    @note The FaceRecognizer does not store your training images, because this would be very
    memory intense and it's not the responsibility of te FaceRecognizer to do so. The caller is
    responsible for maintaining the dataset, he want to work with.
     */
    CV_WRAP virtual void update(InputArrayOfArrays src, InputArray labels);

    /** @overload */
    CV_WRAP_AS(predict_label) int predict(InputArray src) const;


    /** @brief Predicts a label and associated confidence (e.g. distance) for a given input image.

    @param src Sample image to get a prediction from.
    @param label The predicted label for the given image.
    @param confidence Associated confidence (e.g. distance) for the predicted label.

    The suffix const means that prediction does not affect the internal model state, so the method can
    be safely called from within different threads.

    The following example shows how to get a prediction from a trained model:

    @code
    using namespace cv;
    // Do your initialization here (create the cv::FaceRecognizer model) ...
    // ...
    // Read in a sample image:
    Mat img = imread("person1/3.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    // And get a prediction from the cv::FaceRecognizer:
    int predicted = model->predict(img);
    @endcode

    Or to get a prediction and the associated confidence (e.g. distance):

    @code
    using namespace cv;
    // Do your initialization here (create the cv::FaceRecognizer model) ...
    // ...
    Mat img = imread("person1/3.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    // Some variables for the predicted label and associated confidence (e.g. distance):
    int predicted_label = -1;
    double predicted_confidence = 0.0;
    // Get the prediction and associated confidence from the model
    model->predict(img, predicted_label, predicted_confidence);
    @endcode
     */
    CV_WRAP void predict(InputArray src, CV_OUT int &label, CV_OUT double &confidence) const;


    /** @brief - if implemented - send all result of prediction to collector that can be used for somehow custom result handling
    @param src Sample image to get a prediction from.
    @param collector User-defined collector object that accepts all results

    To implement this method u just have to do same internal cycle as in predict(InputArray src, CV_OUT int &label, CV_OUT double &confidence) but
    not try to get "best@ result, just resend it to caller side with given collector
    */
    //CV_WRAP_AS(predict_collect) virtual void predict(InputArray src, Ptr<PredictCollector> collector) const = 0;

    /** @brief Saves a FaceRecognizer and its model state.

    Saves this model to a given filename, either as XML or YAML.
    @param filename The filename to store this FaceRecognizer to (either XML/YAML).

    Every FaceRecognizer overwrites FaceRecognizer::save(FileStorage& fs) to save the internal model
    state. FaceRecognizer::save(const String& filename) saves the state of a model to the given
    filename.

    The suffix const means that prediction does not affect the internal model state, so the method can
    be safely called from within different threads.
     */
    CV_WRAP virtual void save(const String& filename) const;

    /** @brief Loads a FaceRecognizer and its model state.

    Loads a persisted model and state from a given XML or YAML file . Every FaceRecognizer has to
    overwrite FaceRecognizer::load(FileStorage& fs) to enable loading the model state.
    FaceRecognizer::load(FileStorage& fs) in turn gets called by
    FaceRecognizer::load(const String& filename), to ease saving a model.
     */
    CV_WRAP virtual void load(const String& filename);

    /** @overload
    Saves this model to a given FileStorage.
    @param fs The FileStorage to store this FaceRecognizer to.
    */
    virtual void save(FileStorage& fs) const = 0;

    /** @overload */
    virtual void load(const FileStorage& fs) = 0;

    /** @brief Sets string info for the specified model's label.

    The string info is replaced by the provided value if it was set before for the specified label.
     */
    CV_WRAP virtual void setLabelInfo(int label, const String& strInfo);

    /** @brief Gets string information by label.

    If an unknown label id is provided or there is no label information associated with the specified
    label id the method returns an empty string.
     */
    CV_WRAP virtual String getLabelInfo(int label) const;

    /** @brief Gets vector of labels by string.

    The function searches for the labels containing the specified sub-string in the associated string
    info.
     */
    CV_WRAP virtual std::vector<int> getLabelsByString(const String& str) const;
    /** @brief threshhold parameter accessor - required for default BestMinDist collector */
    //virtual double getThreshold() const = 0;
protected:
    // Stored pairs "label id - string info"
    std::map<int, String> _labelsInfo;
};

//! @}

}}

//#include "facerec.hpp"

#endif
