QT += core
QT -= gui

CONFIG += c++11

TARGET = qt_dlib_dnn
CONFIG += console
CONFIG -= app_bundle

#TEMPLATE = app

SOURCES += main.cpp \
    dnn/nn/cpu_dlib.cpp \
    dnn/nn/cublas_dlibapi.cpp \
    dnn/nn/cudnn_dlibapi.cpp \
    dnn/nn/curand_dlibapi.cpp \
    dnn/nn/gpu_data.cpp \
    dnn/nn/tensor_tools.cpp \
    dnn/unicode/unicode.cpp

HEADERS += \
    dnn/tensor.h \
    dnn/tensor_abstract.h \
    dnn/any_abstract.h \
    dnn/any.h \
    dnn/algs.h \
    dnn/numeric_constants.h \
    dnn/enable_if.h \
    dnn/noncopyable.h \
    dnn/uintn.h \
    dnn/windows_magic.h \
    dnn/memory_manager_stateless_kernel_1.h \
    dnn/memory_manager_stateless_kernel_abstract.h \
    dnn/scoped_ptr.h \
    dnn/scoped_ptr_abstract.h \
    dnn/matrix.h \
    dnn/matrix/lapack/fortran_id.h \
    dnn/matrix/lapack/gees.h \
    dnn/matrix/lapack/geev.h \
    dnn/matrix/lapack/geqrf.h \
    dnn/matrix/lapack/gesdd.h \
    dnn/matrix/lapack/gesvd.h \
    dnn/matrix/lapack/getrf.h \
    dnn/matrix/lapack/ormqr.h \
    dnn/matrix/lapack/potrf.h \
    dnn/matrix/lapack/syev.h \
    dnn/matrix/lapack/syevr.h \
    dnn/matrix/cblas_constants.h \
    dnn/matrix/matrix.h \
    dnn/matrix/matrix_abstract.h \
    dnn/matrix/matrix_assign.h \
    dnn/matrix/matrix_assign_fwd.h \
    dnn/matrix/matrix_blas_bindings.h \
    dnn/matrix/matrix_cholesky.h \
    dnn/matrix/matrix_conj_trans.h \
    dnn/matrix/matrix_conv.h \
    dnn/matrix/matrix_conv_abstract.h \
    dnn/matrix/matrix_data_layout.h \
    dnn/matrix/matrix_data_layout_abstract.h \
    dnn/matrix/matrix_default_mul.h \
    dnn/matrix/matrix_eigenvalue.h \
    dnn/matrix/matrix_exp.h \
    dnn/matrix/matrix_exp_abstract.h \
    dnn/matrix/matrix_expressions.h \
    dnn/matrix/matrix_fft.h \
    dnn/matrix/matrix_fft_abstract.h \
    dnn/matrix/matrix_fwd.h \
    dnn/matrix/matrix_generic_image.h \
    dnn/matrix/matrix_lu.h \
    dnn/matrix/matrix_mat.h \
    dnn/matrix/matrix_mat_abstract.h \
    dnn/matrix/matrix_math_functions.h \
    dnn/matrix/matrix_math_functions_abstract.h \
    dnn/matrix/matrix_op.h \
    dnn/matrix/matrix_qr.h \
    dnn/matrix/matrix_read_from_istream.h \
    dnn/matrix/matrix_subexp.h \
    dnn/matrix/matrix_subexp_abstract.h \
    dnn/matrix/matrix_trsm.h \
    dnn/matrix/matrix_utilities.h \
    dnn/matrix/matrix_utilities_abstract.h \
    dnn/matrix/symmetric_matrix_cache.h \
    dnn/matrix/symmetric_matrix_cache_abstract.h \
    dnn/memory_manager.h \
    dnn/memory_manager/memory_manager_kernel_1.h \
    dnn/memory_manager/memory_manager_kernel_2.h \
    dnn/memory_manager/memory_manager_kernel_abstract.h \
    dnn/is_kind.h \
    dnn/serialize.h \
    dnn/interfaces/enumerable.h \
    dnn/interfaces/map_pair.h \
    dnn/byte_orderer/byte_orderer_kernel_1.h \
    dnn/byte_orderer/byte_orderer_kernel_abstract.h \
    dnn/float_details.h \
    dnn/smart_pointers/shared_ptr.h \
    dnn/smart_pointers/shared_ptr_abstract.h \
    dnn/smart_pointers/weak_ptr_abstract.h \
    dnn/general_hash/count_bits.h \
    dnn/general_hash/count_bits_abstract.h \
    dnn/general_hash/general_hash.h \
    dnn/general_hash/hash.h \
    dnn/general_hash/hash_abstract.h \
    dnn/general_hash/murmur_hash3.h \
    dnn/general_hash/murmur_hash3_abstract.h \
    dnn/general_hash/random_hashing.h \
    dnn/general_hash/random_hashing_abstract.h \
    dnn/hash.h \
    dnn/pixel.h \
    dnn/stl_checked.h \
    dnn/stl_checked/std_vector_c.h \
    dnn/stl_checked/std_vector_c_abstract.h \
    dnn/std_allocator.h \
    dnn/array/array_kernel.h \
    dnn/array/array_kernel_abstract.h \
    dnn/array/array_tools.h \
    dnn/array/array_tools_abstract.h \
    dnn/array2d/array2d_generic_image.h \
    dnn/array2d/array2d_kernel.h \
    dnn/array2d/array2d_kernel_abstract.h \
    dnn/array2d/serialize_pixel_overloads.h \
    dnn/array.h \
    dnn/array2d.h \
    dnn/generic_image.h \
    dnn/sort.h \
    dnn/error.h \
    dnn/assert.h \
    dnn/nn/core.h \
    dnn/nn/core_abstract.h \
    dnn/nn/cpu_dlib.h \
    dnn/nn/cublas_dlibapi.h \
    dnn/nn/cuda_dlib.h \
    dnn/nn/cuda_errors.h \
    dnn/nn/cuda_utils.h \
    dnn/nn/cudnn_dlibapi.h \
    dnn/nn/curand_dlibapi.h \
    dnn/nn/gpu_data.h \
    dnn/nn/gpu_data_abstract.h \
    dnn/nn/input.h \
    dnn/nn/input_abstract.h \
    dnn/nn/layers.h \
    dnn/nn/layers_abstract.h \
    dnn/nn/loss.h \
    dnn/nn/loss_abstract.h \
    dnn/nn/solvers.h \
    dnn/nn/solvers_abstract.h \
    dnn/nn/tensor_tools.h \
    dnn/nn/utilities.h \
    dnn/nn/utilities_abstract.h \
    dnn/nn/validation.h \
    dnn/nn/tensor.h \
    dnn/nn/tensor_abstract.h \
    dnn/dnn.h \
    dnn/rand_kernel_1.h \
    dnn/mersenne_twister.h \
    dnn/string.h \
    dnn/vectorstream.h \
    dnn/vectorstream/unserialize.h \
    dnn/vectorstream/unserialize_abstract.h \
    dnn/vectorstream/vectorstream.h \
    dnn/vectorstream/vectorstream_abstract.h \
    dnn/image_transforms/image_pyramid.h \
    dnn/geomotry/drectangle.h \
    dnn/geomotry/rectangle.h \
    dnn/geomotry/vector.h \
    dnn/geomotry/vector_abstract.h \
    dnn/statistics/statistics.h \
    dnn/unicode/unicode.h \
    dnn/unicode/unicode_abstract.h \
    dnn/unicode.h \
    dnn/image_processing/box_overlap_testing.h \
    dnn/image_processing/box_overlap_testing_abstract.h \
    dnn/image_processing/full_object_detection.h \
    dnn/image_processing/full_object_detection_abstract.h \
    dnn/geometry.h \
    dnn/cv_image.h \
    dnn/cv_image_abstract.h \
    dnn/image_transforms/assign_image.h \
    dnn/image_transforms/assign_image_abstract.h


LIBS += -I/usr/local/include/opencv -I/usr/local/include -L/usr/local/lib -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dpm -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_rgbd -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_face -lopencv_plot -lopencv_dnn -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core
