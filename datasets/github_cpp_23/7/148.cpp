
#ifndef CPPSORT_SORTERS_QUICK_MERGE_SORTER_H_
#define CPPSORT_SORTERS_QUICK_MERGE_SORTER_H_




#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/size.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/iterator_traits.h"
#include "../detail/quick_merge_sort.h"

namespace cppsort
{
    
    

    namespace detail
    {
        struct quick_merge_sorter_impl
        {
            template<
                typename ForwardIterable,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = std::enable_if_t<
                    is_projection_v<Projection, ForwardIterable, Compare>
                >
            >
            auto operator()(ForwardIterable&& iterable,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                static_assert(
                    std::is_base_of<
                        std::forward_iterator_tag,
                        iterator_category_t<decltype(std::begin(iterable))>
                    >::value,
                    "quick_merge_sorter requires at least forward iterators"
                );

                quick_merge_sort(std::begin(iterable), std::end(iterable),
                                 utility::size(iterable),
                                 std::move(compare), std::move(projection));
            }

            template<
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = std::enable_if_t<
                    is_projection_iterator_v<Projection, ForwardIterator, Compare>
                >
            >
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                static_assert(
                    std::is_base_of<
                        std::forward_iterator_tag,
                        iterator_category_t<ForwardIterator>
                    >::value,
                    "quick_merge_sorter requires at least forward iterators"
                );

                auto dist = std::distance(first, last);
                quick_merge_sort(std::move(first), std::move(last), dist,
                                 std::move(compare), std::move(projection));
            }

            
            

            using iterator_category = std::random_access_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    struct quick_merge_sorter:
        sorter_facade<detail::quick_merge_sorter_impl>
    {};

    
    

    namespace
    {
        constexpr auto&& quick_merge_sort
            = utility::static_const<quick_merge_sorter>::value;
    }
}

#endif 
