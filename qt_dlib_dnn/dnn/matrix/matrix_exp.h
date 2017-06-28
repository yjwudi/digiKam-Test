// Copyright (C) 2006  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_MATRIx_EXP_h_
#define DLIB_MATRIx_EXP_h_

#include "../algs.h"
#include "../is_kind.h"
#include "matrix_fwd.h"
#include "matrix_exp_abstract.h"
#include <iterator>

namespace dlib
{

// ----------------------------------------------------------------------------------------

    // We want to return the compile time constant if our NR and NC dimensions
    // aren't zero but if they are then we want to call ref_.nx() and return
    // the correct values. 
    template < typename exp_type, long NR >
    struct get_nr_helper
    {
        static inline long get(const exp_type&) { return NR; }
    };

    template < typename exp_type >
    struct get_nr_helper<exp_type,0>
    {
        static inline long get(const exp_type& m) { return m.nr(); }
    };

    template < typename exp_type, long NC >
    struct get_nc_helper
    {
        static inline long get(const exp_type&) { return NC; }
    };

    template < typename exp_type >
    struct get_nc_helper<exp_type,0>
    {
        static inline long get(const exp_type& m) { return m.nc(); }
    };

    template <typename EXP>
    struct matrix_traits
    {
        typedef typename EXP::type type;
        typedef typename EXP::const_ret_type const_ret_type;
        typedef typename EXP::mem_manager_type mem_manager_type;
        typedef typename EXP::layout_type layout_type;
        const static long NR = EXP::NR;
        const static long NC = EXP::NC;
        const static long cost = EXP::cost;
    };

// ----------------------------------------------------------------------------------------

    template <typename EXP> class matrix_exp;
    template <typename EXP>
    class matrix_exp_iterator : public std::iterator<std::forward_iterator_tag, typename matrix_traits<EXP>::type>
    {
        friend class matrix_exp<EXP>;
        matrix_exp_iterator(const EXP& m_, long r_, long c_)
        {
            r = r_;
            c = c_;
            nc = m_.nc();
            m = &m_;
        }

    public:

        matrix_exp_iterator() : r(0), c(0), nc(0), m(0) {}

        typedef typename matrix_traits<EXP>::type type;
        typedef type value_type;
        typedef typename matrix_traits<EXP>::const_ret_type const_ret_type;


        bool operator == ( const matrix_exp_iterator& itr) const
        { return r == itr.r && c == itr.c; }

        bool operator != ( const matrix_exp_iterator& itr) const
        { return !(*this == itr); }

        matrix_exp_iterator& operator++()
        {
            ++c;
            if (c==nc)
            {
                c = 0;
                ++r;
            }
            return *this;
        }

        matrix_exp_iterator operator++(int)
        {
            matrix_exp_iterator temp(*this);
            ++(*this);
            return temp;
        }

        const_ret_type operator* () const { return (*m)(r,c); }

    private:
        long r, c;
        long nc;
        const EXP* m;
    };

// ----------------------------------------------------------------------------------------

    template <
        typename EXP
        >
    class matrix_exp 
    {
        /*!
            REQUIREMENTS ON EXP
                EXP should be something convertible to a matrix_exp.  That is,
                it should inherit from matrix_exp
        !*/

    public:
        typedef typename matrix_traits<EXP>::type type;
        typedef type value_type;
        typedef typename matrix_traits<EXP>::const_ret_type const_ret_type;
        typedef typename matrix_traits<EXP>::mem_manager_type mem_manager_type;
        typedef typename matrix_traits<EXP>::layout_type layout_type;
        const static long NR = matrix_traits<EXP>::NR;
        const static long NC = matrix_traits<EXP>::NC;
        const static long cost = matrix_traits<EXP>::cost;

        typedef matrix<type,NR,NC,mem_manager_type,layout_type> matrix_type;
        typedef EXP exp_type;
        typedef matrix_exp_iterator<EXP> iterator;
        typedef matrix_exp_iterator<EXP> const_iterator;

        inline const_ret_type operator() (
            long r,
            long c
        ) const 
        { 

            return ref()(r,c); 
        }

        const_ret_type operator() (
            long i
        ) const 
        {

            if (nc() == 1)
                return ref()(i,0);
            else
                return ref()(0,i);
        }

        long size (
        ) const { return nr()*nc(); }

        long nr (
        ) const { return get_nr_helper<exp_type,NR>::get(ref()); }

        long nc (
        ) const { return get_nc_helper<exp_type,NC>::get(ref()); }

        template <typename U>
        bool aliases (
            const matrix_exp<U>& item
        ) const { return ref().aliases(item); }

        template <typename U>
        bool destructively_aliases (
            const matrix_exp<U>& item
        ) const { return ref().destructively_aliases(item); }

        inline const exp_type& ref (
        ) const { return *static_cast<const exp_type*>(this); }

        inline operator const type (
        ) const 
        {

            // Put the expression contained in this matrix_exp into
            // a temporary 1x1 matrix so that the expression will encounter
            // all the overloads of matrix_assign() and have the chance to
            // go through any applicable optimizations.
            matrix<type,1,1,mem_manager_type,layout_type> temp(ref());
            return temp(0);
        }

        const_iterator begin() const { return matrix_exp_iterator<EXP>(ref(),0,0); }
        const_iterator end()   const { return matrix_exp_iterator<EXP>(ref(),nr(),0); }

    protected:
        matrix_exp() {}
        matrix_exp(const matrix_exp& ) {}

    private:

        matrix_exp& operator= (const matrix_exp&);
    };

// ----------------------------------------------------------------------------------------

    // something is a matrix if it is convertible to a matrix_exp object

    /*
    template <typename T>
    struct is_matrix<T, typename enable_if<is_convertible<T, const matrix_exp<typename T::exp_type>& > >::type > 
    { static const bool value = true; }; 

    */
    /*
        is_matrix<T>::value == 1 if T is a matrix type else 0
    */

// ----------------------------------------------------------------------------------------

    template <
        typename EXP
        >
    class matrix_diag_exp : public matrix_exp<EXP> 
    {
        /*!
            This is a matrix expression type used to represent diagonal matrices.
            That is, square matrices with all off diagonal elements equal to 0.
        !*/

    protected:
        matrix_diag_exp() {}
        matrix_diag_exp(const matrix_diag_exp& item ):matrix_exp<EXP>(item) {}
    };

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_MATRIx_EXP_h_

