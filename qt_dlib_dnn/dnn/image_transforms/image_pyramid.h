#include "../assert.h"
#include "../geomotry/rectangle.h"
#include "../geomotry/drectangle.h"

//using namespace dlib;

template <
        typename pyramid_type
        >
    drectangle tiled_pyramid_to_image (
        const std::vector<rectangle>& rects,
        drectangle r 
    )
    {
        DLIB_CASSERT(rects.size() > 0);

        size_t pyramid_down_iter = nearest_rect(rects, dcenter(r));

        dpoint origin = rects[pyramid_down_iter].tl_corner();
        r = drectangle(r.tl_corner()-origin, r.br_corner()-origin);
        pyramid_type pyr;
        return pyr.rect_up(r, pyramid_down_iter);
    }

 // ----------------------------------------------------------------------------------------

    template <
        typename pyramid_type
        >
    drectangle image_to_tiled_pyramid (
        const std::vector<rectangle>& rects,
        double scale,
        drectangle r
    )
    {
        DLIB_ASSERT(rects.size() > 0);
        DLIB_ASSERT(0 < scale && scale <= 1);
        return drectangle(image_to_tiled_pyramid<pyramid_type>(rects, scale, r.tl_corner()),
                          image_to_tiled_pyramid<pyramid_type>(rects, scale, r.br_corner()));
    }

// ----------------------------------------------------------------------------------------

    template <
        typename pyramid_type,
        typename image_type1,
        typename image_type2
        >
    void create_tiled_pyramid (
        const image_type1& img,
        image_type2& out_img,
        std::vector<rectangle>& rects,
        const unsigned long padding = 10
    )
    {
        DLIB_ASSERT(!is_same_object(img, out_img));

        rects.clear();
        if (num_rows(img)*num_columns(img) == 0)
        {
            set_image_size(out_img,0,0);
            return;
        }

        const long min_height = 5;
        pyramid_type pyr;
        std::vector<matrix<rgb_pixel>> pyramid;
        matrix<rgb_pixel> temp;
        assign_image(temp, img);
        pyramid.push_back(std::move(temp));
        // build the whole pyramid
        while(true)
        {
            matrix<rgb_pixel> temp;
            pyr(pyramid.back(), temp);
            if (temp.size() == 0 || temp.nr() < min_height)
                break;
            pyramid.push_back(std::move(temp));
        }

        // figure out output image size
        long total_height = 0;
        for (auto&& i : pyramid)
            total_height += i.nr()+padding;
        total_height -= padding*2; // don't add unnecessary padding to the very right side.
        long height = 0;
        long prev_width = 0;
        for (auto&& i : pyramid)
        {
            // Figure out how far we go on the first column.  We go until the next image can
            // fit next to the previous one, which means we can double back for the second
            // column of images.
            if (i.nc() <= img.nc()-prev_width-(long)padding && 
                (height-img.nr())*2 >= (total_height-img.nr()))
            {
                break;
            }
            height += i.nr() + padding;
            prev_width = i.nc();
        }
        height -= padding; // don't add unnecessary padding to the very right side.

        set_image_size(out_img,height,img.nc());
        assign_all_pixels(out_img, 0);

        long y = 0;
        size_t i = 0;
        while(y < height)
        {
            rectangle rect = translate_rect(get_rect(pyramid[i]),point(0,y));
            DLIB_ASSERT(get_rect(out_img).contains(rect));
            rects.push_back(rect);
            auto si = sub_image(out_img, rect);
            assign_image(si, pyramid[i]);
            y += pyramid[i].nr()+padding;
            ++i;
        }
        y -= padding;
        while (i < pyramid.size())
        {
            point p1(img.nc()-1,y-1);
            point p2 = p1 - get_rect(pyramid[i]).br_corner();
            rectangle rect(p1,p2);
            DLIB_ASSERT(get_rect(out_img).contains(rect));
            // don't keep going on the last row if it would intersect the original image.
            if (!get_rect(img).intersect(rect).is_empty())
                break;
            rects.push_back(rect);
            auto si = sub_image(out_img, rect);
            assign_image(si, pyramid[i]);
            y -= pyramid[i].nr()+padding;
            ++i;
        }

    }
