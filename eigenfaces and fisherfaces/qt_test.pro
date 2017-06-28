QT += core
QT += gui

TARGET = qt_test
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    FDetect.cpp \
    lbprec.cpp \
    eigenrec.cpp \
    EigenFaces/eigen_faces.cpp \
    EigenFaces/facerec_borrowed.cpp \
    EigenFaces/eigenfacemodel.cpp \
    EigenFaces/opencveigenfacerecognizer.cpp

LIBS += -I/usr/local/include/opencv -I/usr/local/include -L/usr/local/lib -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dpm -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_rgbd -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_face -lopencv_plot -lopencv_dnn -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core

HEADERS += \
    debugger.h \
    FDetect.h \
    lbprec.h \
    eigenrec.h \
    EigenFaces/eigen_faces.h \
    EigenFaces/face_basic.hpp \
    EigenFaces/face.hpp \
    EigenFaces/facerec_borrowed.h \
    EigenFaces/eigenfacemodel.h \
    EigenFaces/opencveigenfacerecognizer.h

