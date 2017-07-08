#include "../matrix.h"
#include "../array2d.h"
#include "../array.h"
#include "../geometry.h"



inline point image_to_fhog (
    point p,
    int cell_size = 8,
    int filter_rows_padding = 1,
    int filter_cols_padding = 1
)
{
    // make sure requires clause is not broken
    DLIB_ASSERT( cell_size > 0 &&
        filter_rows_padding > 0 &&
        filter_cols_padding > 0 ,
        "\t point image_to_fhog()"
        << "\n\t Invalid inputs were given to this function. "
        << "\n\t cell_size: " << cell_size
        << "\n\t filter_rows_padding: " << filter_rows_padding
        << "\n\t filter_cols_padding: " << filter_cols_padding
    );

    // There is a one pixel border around the image.
    p -= point(1,1);
    // There is also a 1 "cell" border around the HOG image formation.
    return p/cell_size - point(1,1) + point((filter_cols_padding-1)/2,(filter_rows_padding-1)/2);
}
inline rectangle image_to_fhog (
    const rectangle& rect,
    int cell_size = 8,
    int filter_rows_padding = 1,
    int filter_cols_padding = 1
)
{
    // make sure requires clause is not broken
    DLIB_ASSERT( cell_size > 0 &&
        filter_rows_padding > 0 &&
        filter_cols_padding > 0 ,
        "\t rectangle image_to_fhog()"
        << "\n\t Invalid inputs were given to this function. "
        << "\n\t cell_size: " << cell_size
        << "\n\t filter_rows_padding: " << filter_rows_padding
        << "\n\t filter_cols_padding: " << filter_cols_padding
    );

    return rectangle(image_to_fhog(rect.tl_corner(),cell_size,filter_rows_padding,filter_cols_padding),
                     image_to_fhog(rect.br_corner(),cell_size,filter_rows_padding,filter_cols_padding));
}
inline point fhog_to_image (
    point p,
    int cell_size = 8,
    int filter_rows_padding = 1,
    int filter_cols_padding = 1
)
{
    // make sure requires clause is not broken
    DLIB_ASSERT( cell_size > 0 &&
        filter_rows_padding > 0 &&
        filter_cols_padding > 0 ,
        "\t point fhog_to_image()"
        << "\n\t Invalid inputs were given to this function. "
        << "\n\t cell_size: " << cell_size
        << "\n\t filter_rows_padding: " << filter_rows_padding
        << "\n\t filter_cols_padding: " << filter_cols_padding
    );

    // Convert to image space and then set to the center of the cell.
    point offset;

    p = (p+point(1,1)-point((filter_cols_padding-1)/2,(filter_rows_padding-1)/2))*cell_size + point(1,1);
    if (p.x() >= 0 && p.y() >= 0) offset = point(cell_size/2,cell_size/2);
    if (p.x() <  0 && p.y() >= 0) offset = point(-cell_size/2,cell_size/2);
    if (p.x() >= 0 && p.y() <  0) offset = point(cell_size/2,-cell_size/2);
    if (p.x() <  0 && p.y() <  0) offset = point(-cell_size/2,-cell_size/2);
    return p + offset;
}

// ----------------------------------------------------------------------------------------

inline rectangle fhog_to_image (
    const rectangle& rect,
    int cell_size = 8,
    int filter_rows_padding = 1,
    int filter_cols_padding = 1
)
{
    // make sure requires clause is not broken
    DLIB_ASSERT( cell_size > 0 &&
        filter_rows_padding > 0 &&
        filter_cols_padding > 0 ,
        "\t rectangle fhog_to_image()"
        << "\n\t Invalid inputs were given to this function. "
        << "\n\t cell_size: " << cell_size
        << "\n\t filter_rows_padding: " << filter_rows_padding
        << "\n\t filter_cols_padding: " << filter_cols_padding
    );

    return rectangle(fhog_to_image(rect.tl_corner(),cell_size,filter_rows_padding,filter_cols_padding),
                     fhog_to_image(rect.br_corner(),cell_size,filter_rows_padding,filter_cols_padding));
}

// ----------------------------------------------------------------------------------------

    template <
        typename image_type, 
        typename T, 
        typename mm1, 
        typename mm2
        >
    void extract_fhog_features(
        const image_type& img, 
        dlib::array<array2d<T,mm1>,mm2>& hog, 
        int cell_size = 8,
        int filter_rows_padding = 1,
        int filter_cols_padding = 1
    ) 
    {
        impl_fhog::impl_extract_fhog_features(img, hog, cell_size, filter_rows_padding, filter_cols_padding);
        // If the image is too small then the above function outputs an empty feature map.
        // But to make things very uniform in usage we require the output to still have the
        // 31 planes (but they are just empty).
        if (hog.size() == 0)
            hog.resize(31);
    }

    template <
        typename image_type, 
        typename T, 
        typename mm
        >
    void extract_fhog_features(
        const image_type& img, 
        array2d<matrix<T,31,1>,mm>& hog, 
        int cell_size = 8,
        int filter_rows_padding = 1,
        int filter_cols_padding = 1
    ) 
    {
        impl_fhog::impl_extract_fhog_features(img, hog, cell_size, filter_rows_padding, filter_cols_padding);
    }

// ----------------------------------------------------------------------------------------

    template <
        typename image_type,
        typename T
        >
    void extract_fhog_features(
        const image_type& img, 
        matrix<T,0,1>& feats,
        int cell_size = 8,
        int filter_rows_padding = 1,
        int filter_cols_padding = 1
    )
    {
        dlib::array<array2d<T> > hog;
        extract_fhog_features(img, hog, cell_size, filter_rows_padding, filter_cols_padding);
        feats.set_size(hog.size()*hog[0].size());
        for (unsigned long i = 0; i < hog.size(); ++i)
        {
            const long size = hog[i].size();
            set_rowm(feats, range(i*size, (i+1)*size-1)) = reshape_to_column_vector(mat(hog[i]));
        }
    }

// ----------------------------------------------------------------------------------------

    template <
        typename image_type
        >
    matrix<double,0,1> extract_fhog_features(
        const image_type& img, 
        int cell_size = 8,
        int filter_rows_padding = 1,
        int filter_cols_padding = 1
    )
    {
        matrix<double, 0, 1> feats;
        extract_fhog_features(img, feats, cell_size, filter_rows_padding, filter_cols_padding);
        return feats;
    }