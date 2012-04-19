//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICY_HPP
#define BOOST_DI_POLICY_HPP

#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/void.hpp>
#include "boost/di/aux/utility.hpp"
#include "boost/di/policies/check_for_binding_correctness.hpp"
#include "boost/di/policies/check_for_circular_dependencies.hpp"
#include "boost/di/policies/check_for_creation_ownership.hpp"
#include "boost/di/policies/check_for_references_without_ownership.hpp"
#include "boost/di/config.hpp"

namespace boost {
namespace di {

namespace detail { class policy { }; } // namespace detail

template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
class policy
    : detail::policy
{
    typedef mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> sequence;

    template<typename TDeps, typename T, typename TPolicy>
    struct verify_impl
        : TPolicy::template verify<TDeps, typename aux::make_plain<T>::type>::type
    { };

public:
    template<
        typename TDeps,
        typename T
    >
    struct verify
        : mpl::fold<
            sequence,
            mpl::void_,
            verify_impl<TDeps, T, mpl::_2>
          >::type
    {
        typedef void type;
    };
};

template<typename TDefault>
struct defaults<detail::policy, TDefault>
{
    typedef policy<
        policies::check_for_binding_correctness,
        policies::check_for_circular_dependencies,
        policies::check_for_creation_ownership,
        policies::check_for_references_without_ownership
    > type;
};

} // namespace di
} // namespace boost

#endif

