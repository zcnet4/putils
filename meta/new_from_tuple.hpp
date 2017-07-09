#pragma once

namespace pmeta
{
    namespace detail {
        template <class T, class Tuple, std::size_t... I>
        constexpr T *new_from_tuple_impl( Tuple&& t, std::index_sequence<I...> )
        {
            return new T(std::get<I>(std::forward<Tuple>(t))...);
        }
    } // namespace detail

    template <class T, class Tuple>
    constexpr T *new_from_tuple( Tuple&& t )
    {
        return detail::new_from_tuple_impl<T>(std::forward<Tuple>(t),
                                               std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>{});
    }
}