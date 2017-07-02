#ifndef ASSIGN_IMAGE_H
#define ASSIGN_IMAGE_H

#include "../pixel.h"
#include "assign_image_abstract.h"
//#include "../statistics.h"
#include "../generic_image.h"
#include "../cv_image.h"




template <
        typename dest_image_type,
        typename src_image_type
        >
void impl_assign_image (
        image_view<dest_image_type>& dest,
        const src_image_type& src
        )
{
    dest.set_size(src.nr(),src.nc());
    for (long r = 0; r < src.nr(); ++r)
    {
        for (long c = 0; c < src.nc(); ++c)
        {
            assign_pixel(dest[r][c], src(r,c));
        }
    }
}

template <
        typename dest_image_type,
        typename src_image_type
        >
void impl_assign_image (
        dest_image_type& dest_,
        const src_image_type& src
        )
{
    image_view<dest_image_type> dest(dest_);
    impl_assign_image(dest, src);
}

template <
        typename dest_image_type,
        typename src_image_type
        >
void assign_image (
        dest_image_type& dest,
        const src_image_type& src
        )
{
    // check for the case where dest is the same object as src
    if (is_same_object(dest,src))
        return;

    impl_assign_image(dest, mat(src));
}


#endif // ASSIGN_IMAGE_H
