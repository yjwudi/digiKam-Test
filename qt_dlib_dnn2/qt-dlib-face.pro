QT += core
QT -= gui

CONFIG += c++11

TARGET = qt-dlib-face
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    shape-predictor/fullobjectdetection.cpp \
    shape-predictor/qdatastreamoverloads.cpp \
    dnnface/dnn_base/base64_kernel_1.cpp \
    dnnface/dnn_base/entropy_decoder_kernel_2.cpp \
    dnnface/dnn_base/entropy_encoder_kernel_2.cpp \
    dnnface/nn/cpu_dlib.cpp \
    dnnface/nn/gpu_data.cpp \
    dnnface/nn/tensor_tools.cpp

HEADERS += \
    shape-predictor/fullobjectdetection.h \
    shape-predictor/libopencv.h \
    shape-predictor/matrixoperations.h \
    shape-predictor/pointtransformaffine.h \
    shape-predictor/qdatastreamoverloads.h \
    shape-predictor/shapepredictor.h \
    shape-predictor/vectoroperations.h \
    dnnface/dnn_base/algs.h \
    dnnface/dnn_base/any.h \
    dnnface/dnn_base/array2d_generic_image.h \
    dnnface/dnn_base/array2d_kernel.h \
    dnnface/dnn_base/array_kernel.h \
    dnnface/dnn_base/base64_kernel_1.h \
    dnnface/dnn_base/byte_orderer_kernel_1.h \
    dnnface/dnn_base/compress_stream_kernel_1.h \
    dnnface/dnn_base/crc32_kernel_1.h \
    dnnface/dnn_base/cv_image.h \
    dnnface/dnn_base/dlib_vector.h \
    dnnface/dnn_base/dnn_assert.h \
    dnnface/dnn_base/dnn_error.h \
    dnnface/dnn_base/dnn_sort.h \
    dnnface/dnn_base/dnn_string.h \
    dnnface/dnn_base/dnn_unicode.h \
    dnnface/dnn_base/dnn_vector.h \
    dnnface/dnn_base/drectangle.h \
    dnnface/dnn_base/enable_if.h \
    dnnface/dnn_base/entropy.h \
    dnnface/dnn_base/entropy_decoder_kernel_2.h \
    dnnface/dnn_base/entropy_decoder_model_kernel_5.h \
    dnnface/dnn_base/entropy_encoder_kernel_2.h \
    dnnface/dnn_base/entropy_encoder_model_kernel_5.h \
    dnnface/dnn_base/enumerable.h \
    dnnface/dnn_base/float_details.h \
    dnnface/dnn_base/generic_image.h \
    dnnface/dnn_base/is_kind.h \
    dnnface/dnn_base/map_pair.h \
    dnnface/dnn_base/memory_manager_stateless_kernel_1.h \
    dnnface/dnn_base/mersenne_twister.h \
    dnnface/dnn_base/murmur_hash3.h \
    dnnface/dnn_base/noncopyable.h \
    dnnface/dnn_base/numeric_constants.h \
    dnnface/dnn_base/pixel.h \
    dnnface/dnn_base/platform.h \
    dnnface/dnn_base/rand_kernel_1.h \
    dnnface/dnn_base/random_hashing.h \
    dnnface/dnn_base/rectangle.h \
    dnnface/dnn_base/scoped_ptr.h \
    dnnface/dnn_base/serialize.h \
    dnnface/dnn_base/serialize_pixel_overloads.h \
    dnnface/dnn_base/shared_ptr.h \
    dnnface/dnn_base/simd4f.h \
    dnnface/dnn_base/simd4i.h \
    dnnface/dnn_base/simd8f.h \
    dnnface/dnn_base/simd8i.h \
    dnnface/dnn_base/simd_check.h \
    dnnface/dnn_base/statistics.h \
    dnnface/dnn_base/std_allocator.h \
    dnnface/dnn_base/std_vector_c.h \
    dnnface/dnn_base/uintn.h \
    dnnface/dnn_base/unserialize.h \
    dnnface/dnn_base/vectorstream.h \
    dnnface/dnn_base/windows_magic.h \
    dnnface/image_processing/box_overlap_testing.h \
    dnnface/image_processing/frontal_face_detector.h \
    dnnface/image_processing/full_object_detection.h \
    dnnface/image_processing/object_detector.h \
    dnnface/image_processing/scan_fhog_pyramid.h \
    dnnface/image_transforms/assign_image.h \
    dnnface/image_transforms/fhog.h \
    dnnface/image_transforms/image_pyramid.h \
    dnnface/image_transforms/interpolation.h \
    dnnface/image_transforms/spatial_filtering.h \
    dnnface/nn/core.h \
    dnnface/nn/cpu_dlib.h \
    dnnface/nn/gpu_data.h \
    dnnface/nn/input.h \
    dnnface/nn/layers.h \
    dnnface/nn/loss.h \
    dnnface/nn/solvers.h \
    dnnface/nn/tensor.h \
    dnnface/nn/tensor_tools.h \
    dnnface/nn/utilities.h \
    dnnface/nn/validation.h \
    dnnface/dnn_face.h \
    dnnface/matrix/lapack/fortran_id.h \
    dnnface/matrix/lapack/gees.h \
    dnnface/matrix/lapack/geev.h \
    dnnface/matrix/lapack/geqrf.h \
    dnnface/matrix/lapack/gesdd.h \
    dnnface/matrix/lapack/gesvd.h \
    dnnface/matrix/lapack/getrf.h \
    dnnface/matrix/lapack/ormqr.h \
    dnnface/matrix/lapack/potrf.h \
    dnnface/matrix/lapack/syev.h \
    dnnface/matrix/lapack/syevr.h \
    dnnface/matrix/cblas_constants.h \
    dnnface/matrix/matrix.h \
    dnnface/matrix/matrix_abstract.h \
    dnnface/matrix/matrix_assign.h \
    dnnface/matrix/matrix_assign_fwd.h \
    dnnface/matrix/matrix_blas_bindings.h \
    dnnface/matrix/matrix_cholesky.h \
    dnnface/matrix/matrix_conj_trans.h \
    dnnface/matrix/matrix_conv.h \
    dnnface/matrix/matrix_conv_abstract.h \
    dnnface/matrix/matrix_data_layout.h \
    dnnface/matrix/matrix_data_layout_abstract.h \
    dnnface/matrix/matrix_default_mul.h \
    dnnface/matrix/matrix_eigenvalue.h \
    dnnface/matrix/matrix_exp.h \
    dnnface/matrix/matrix_exp_abstract.h \
    dnnface/matrix/matrix_expressions.h \
    dnnface/matrix/matrix_fwd.h \
    dnnface/matrix/matrix_generic_image.h \
    dnnface/matrix/matrix_la.h \
    dnnface/matrix/matrix_la_abstract.h \
    dnnface/matrix/matrix_lu.h \
    dnnface/matrix/matrix_mat.h \
    dnnface/matrix/matrix_math_functions.h \
    dnnface/matrix/matrix_math_functions_abstract.h \
    dnnface/matrix/matrix_op.h \
    dnnface/matrix/matrix_qr.h \
    dnnface/matrix/matrix_read_from_istream.h \
    dnnface/matrix/matrix_subexp.h \
    dnnface/matrix/matrix_subexp_abstract.h \
    dnnface/matrix/matrix_trsm.h \
    dnnface/matrix/matrix_utilities.h \
    dnnface/matrix/matrix_utilities_abstract.h \
    dnnface/matrix/symmetric_matrix_cache.h \
    dnnface/matrix/symmetric_matrix_cache_abstract.h \
    dnnface/matrix.h
LIBS += -lpthread -fopenmp -I/usr/local/include/opencv -I/usr/local/include -L/usr/local/lib -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dpm -lopencv_freetype -lopencv_fuzzy -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_rgbd -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_face -lopencv_plot -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core
