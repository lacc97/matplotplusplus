//
// Created by Alan Freitas on 2020-07-05.
//

#ifndef MATPLOTPLUSPLUS_TYPE_TRAITS_H
#define MATPLOTPLUSPLUS_TYPE_TRAITS_H

#include <string>
#include <type_traits>
#include <vector>

namespace matplot {
    namespace detail {
        // Implementation of is_detected, detected_t, detected_or
        // https://en.cppreference.com/w/cpp/experimental/is_detected
        namespace detail {
            template <class Default, class AlwaysVoid,
                template<class...> class Op, class... Args>
            struct detector {
                using value_t = std::false_type;
                using type = Default;
            };

            template <class Default, template<class...> class Op, class... Args>
            struct detector<Default, std::void_t<Op<Args...>>, Op, Args...> {
                using value_t = std::true_type;
                using type = Op<Args...>;
            };

        }

        template <template<class...> class Op, class... Args>
        using is_detected = typename detail::detector<void, void, Op, Args...>::value_t;

        template <template<class...> class Op, class... Args>
        inline constexpr bool is_detected_v = is_detected<Op, Args...>::value;

        template <template<class...> class Op, class... Args>
        using detected_t = typename detail::detector<std::enable_if_t<is_detected_v<Op, Args...>, void>, void, Op, Args...>::type;

        template <class Default, template<class...> class Op, class... Args>
        using detected_or = detail::detector<Default, void, Op, Args...>;

        template <class Default, template<class...> class Op, class... Args>
        using detected_or_t = typename detected_or<Default, Op, Args...>::type;


        template <typename T>
        using iterator_value_type = typename std::iterator_traits<T>::value_type;

        template <typename T>
        constexpr inline bool is_iterator = is_detected_v<iterator_value_type, T>;

        template <typename C>
        using iterable_begin = decltype(std::declval<C>().begin());

        template <typename C>
        using iterable_end = decltype(std::declval<C>().end());

        template <typename C>
        using iterable_value_type = iterator_value_type<iterable_begin<C>>;

        template <typename C>
        inline constexpr bool is_iterable_v =
            is_detected_v<iterable_begin, std::add_const_t<C>> &&
            is_detected_v<iterable_end, std::add_const_t<C>> &&
            std::is_same_v<detected_or_t<void, iterable_begin, std::add_const_t<C>>,
                           detected_or_t<void, iterable_end, std::add_const_t<C>>> &&
            is_iterator<detected_or_t<void, iterable_begin, std::add_const_t<C>>>;
    }

    template <typename C>
    using is_iterable = std::bool_constant<detail::is_iterable_v<C>>;

    template <typename C> constexpr bool is_iterable_v = is_iterable<C>::value;

    // Something like std::vector<double>
    template <class C>
    using is_iterable_1d =
        std::bool_constant<is_iterable_v<C> &&
                           !is_iterable_v<detail::detected_or_t<void, detail::iterable_value_type, C>>>;

    template <typename C>
    constexpr bool is_iterable_1d_v = is_iterable_1d<C>::value;

    // Something like std::vector<std::vector<double>>
    template <class C>
    using is_iterable_2d =
        std::bool_constant<is_iterable_v<C> &&
                           is_iterable_1d_v<detail::detected_or_t<void, detail::iterable_value_type, C>>>;

    template <typename C>
    constexpr bool is_iterable_2d_v = is_iterable_2d<C>::value;

    template <class T>
    using is_string =
        std::bool_constant<std::is_same_v<std::decay_t<T>, std::string> &&
                           std::is_convertible_v<T, std::string>>;

    template <typename C> constexpr bool is_string_v = is_string<C>::value;

    template <typename> struct is_pair : std::false_type {};

    template <typename T, typename U>
    struct is_pair<std::pair<T, U>> : std::true_type {};

    template <typename T> constexpr bool is_pair_v = is_pair<T>::value;

    template <class C>
    using is_iterable_pair =
        std::bool_constant<is_iterable_v<C> &&
                           is_pair_v<detail::detected_or_t<void, detail::iterable_value_type, C>>>;

    template <typename C>
    constexpr bool is_iterable_pair_v = is_iterable_pair<C>::value;

    template <typename> struct is_vector : std::false_type {};

    template <typename T, typename U>
    struct is_vector<std::vector<T, U>> : std::true_type {};

    template <typename> struct is_initializer_list : std::false_type {};

    template <typename T>
    struct is_initializer_list<std::initializer_list<T>> : std::true_type {};

    template <typename T> struct first_type_if_pair { using type = T; };

    template <typename T, typename U>
    struct first_type_if_pair<std::pair<T, U>> {
        using type = T;
    };

    // also tried this, but it didn't help
    template <typename T, typename U>
    struct first_type_if_pair<std::pair<const T, U>> {
        using type = T;
    };

    template <typename T> struct second_type_if_pair { using type = T; };

    template <typename T, typename U>
    struct second_type_if_pair<std::pair<T, U>> {
        using type = U;
    };

    // also tried this, but it didn't help
    template <typename T, typename U>
    struct second_type_if_pair<std::pair<const T, U>> {
        using type = U;
    };

    template <typename ITERABLE> struct iterable_traits {
        using container_type = ITERABLE;
        using iterator_type = decltype(std::declval<container_type>().begin());
        using value_type = typename std::remove_reference<decltype(
            std::declval<iterator_type>().operator*())>::type;
        static constexpr bool is_map = is_pair<value_type>::value;
        using key_type =
            std::conditional_t<is_map, typename value_type::first_type,
                               value_type>;
        using mapped_type =
            std::conditional_t<is_map, typename value_type::second_type,
                               value_type>;
    };
} // namespace matplot

#endif // MATPLOTPLUSPLUS_TYPE_TRAITS_H
