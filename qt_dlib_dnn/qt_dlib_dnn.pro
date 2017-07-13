QT += core
QT -= gui

CONFIG += c++11

TARGET = qt_dlib_dnn
CONFIG += console
CONFIG -= app_bundle

#TEMPLATE = app

SOURCES += main.cpp \
    dnn/nn/cpu_dlib.cpp \
    dnn/nn/gpu_data.cpp \
    dnn/nn/tensor_tools.cpp \
    shape-predictor/fullobjectdetection.cpp \
    shape-predictor/qdatastreamoverloads.cpp \
    dnn/entropy_encoder/entropy_encoder_kernel_2.cpp \
    dnn/entropy_decoder/entropy_decoder_kernel_2.cpp \
    dnn/base64/base64_kernel_1.cpp

HEADERS += \
    dnn/tensor.h \
    dnn/tensor_abstract.h \
    dnn/any.h \
    dnn/algs.h \
    dnn/numeric_constants.h \
    dnn/enable_if.h \
    dnn/noncopyable.h \
    dnn/uintn.h \
    dnn/windows_magic.h \
    dnn/memory_manager_stateless_kernel_1.h \
    dnn/scoped_ptr.h \
    dnn/matrix.h \
    dnn/matrix/cblas_constants.h \
    dnn/matrix/matrix.h \
    dnn/matrix/matrix_assign_fwd.h \
    dnn/matrix/matrix_data_layout.h \
    dnn/matrix/matrix_exp.h \
    dnn/matrix/matrix_expressions.h \
    dnn/matrix/matrix_fwd.h \
    dnn/matrix/matrix_generic_image.h \
    dnn/matrix/matrix_mat.h \
    dnn/matrix/matrix_math_functions.h \
    dnn/matrix/matrix_op.h \
    dnn/matrix/matrix_subexp.h \
    dnn/matrix/matrix_utilities.h \
    dnn/is_kind.h \
    dnn/serialize.h \
    dnn/interfaces/enumerable.h \
    dnn/interfaces/map_pair.h \
    dnn/byte_orderer/byte_orderer_kernel_1.h \
    dnn/float_details.h \
    dnn/smart_pointers/shared_ptr.h \
    dnn/general_hash/hash.h \
    dnn/general_hash/murmur_hash3.h \
    dnn/general_hash/random_hashing.h \
    dnn/pixel.h \
    dnn/stl_checked.h \
    dnn/stl_checked/std_vector_c.h \
    dnn/std_allocator.h \
    dnn/array/array_kernel.h \
    dnn/array2d/array2d_generic_image.h \
    dnn/array2d/array2d_kernel.h \
    dnn/array2d/serialize_pixel_overloads.h \
    dnn/array2d.h \
    dnn/generic_image.h \
    dnn/sort.h \
    dnn/error.h \
    dnn/assert.h \
    dnn/nn/core.h \
    dnn/nn/cpu_dlib.h \
    dnn/nn/gpu_data.h \
    dnn/nn/input.h \
    dnn/nn/layers.h \
    dnn/nn/loss.h \
    dnn/nn/solvers.h \
    dnn/nn/tensor_tools.h \
    dnn/nn/utilities.h \
    dnn/nn/validation.h \
    dnn/nn/tensor.h \
    dnn/dnn.h \
    dnn/rand_kernel_1.h \
    dnn/mersenne_twister.h \
    dnn/string.h \
    dnn/vectorstream.h \
    dnn/vectorstream/unserialize.h \
    dnn/vectorstream/vectorstream.h \
    dnn/vectorstream/vectorstream_abstract.h \
    dnn/image_transforms/image_pyramid.h \
    dnn/geomotry/drectangle.h \
    dnn/geomotry/rectangle.h \
    dnn/geomotry/vector.h \
    dnn/statistics/statistics.h \
    dnn/unicode/unicode.h \
    dnn/image_processing/box_overlap_testing.h \
    dnn/image_processing/full_object_detection.h \
    dnn/geometry.h \
    dnn/cv_image.h \
    dnn/image_transforms/assign_image.h \
    shape-predictor/fullobjectdetection.h \
    shape-predictor/libopencv.h \
    shape-predictor/matrixoperations.h \
    shape-predictor/pointtransformaffine.h \
    shape-predictor/qdatastreamoverloads.h \
    shape-predictor/shapepredictor.h \
    shape-predictor/vectoroperations.h \
    dnn/image_transforms/interpolation.h \
    dnn/matrix/matrix_la.h \
    dnn/simd/simd4f.h \
    dnn/simd/simd4i.h \
    dnn/simd/simd8f.h \
    dnn/simd/simd8i.h \
    dnn/simd/simd_check.h \
    dnn/image_processing/frontal_face_detector.h \
    dnn/image_processing/object_detector.h \
    dnn/image_processing/scan_fhog_pyramid.h \
    dnn/image_transforms/fhog.h \
    dnn/compress_stream/compress_stream_kernel_1.h \
    dnn/entropy_encoder_model.h \
    dnn/entropy_encoder.h \
    dnn/entropy_encoder/entropy_encoder_kernel_2.h \
    dnn/entropy_encoder_model/entropy_encoder_model_kernel_5.h \
    dnn/entropy_decoder.h \
    dnn/entropy_decoder_model.h \
    dnn/entropy_decoder/entropy_decoder_kernel_2.h \
    dnn/entropy_decoder_model/entropy_decoder_model_kernel_5.h \
    dnn/crc32/crc32_kernel_1.h \
    dnn/image_transforms/spatial_filtering.h \
    dnn/base64/base64_kernel_1.h \
    dnn/matrix/matrix_lu.h


LIBS += -I/usr/local/include/opencv -I/usr/local/include -L/usr/local/lib -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dpm -lopencv_freetype -lopencv_fuzzy -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_rgbd -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_face -lopencv_plot -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core
