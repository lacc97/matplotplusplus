#ifndef MATPLOTPLUSPLUS_VECTOR_PROXY_H
#define MATPLOTPLUSPLUS_VECTOR_PROXY_H

#include <cassert>
#include <cstddef>

#include <initializer_list>
#include <utility>

namespace matplot {
    template <typename U> struct proxyable_traits {
        inline static constexpr bool is_specialized = false;

        /* Can I construct a vector_proxy<T> from an instance of U */
        template <typename T> inline static constexpr bool is_proxyable = false;

        /* Obtain the size and pointer to the data
         * (in your specialization you should use the appropriate type
         *  instead of void) */
        inline static std::pair<size_t, void *> proxy(const U &) noexcept;
    };

    template <typename T> class vector_proxy {
        static_assert(!std::is_const_v<T> && !std::is_volatile_v<T>,
                      "type must be non-const, non-volatile");

        template <typename U> using traits = matplot::proxyable_traits<U>;

        template <typename U>
        inline static constexpr bool is_proxyable =
            traits<U>::template is_proxyable<T>;

      public:
        using value_type = const T;
        using size_type = size_t;
        using reference = value_type &;
        using pointer = value_type *;
        using iterator = pointer;
        using const_iterator = pointer;

        inline constexpr vector_proxy() noexcept = default;
        inline constexpr vector_proxy(const vector_proxy &) noexcept = default;
        inline constexpr vector_proxy(std::nullptr_t) noexcept
            : m_Count(0), m_Ptr(nullptr) {}
        inline constexpr vector_proxy(const T &val) noexcept
            : m_Count(1), m_Ptr(&val) {}
        inline constexpr vector_proxy(size_type count, pointer ptr) noexcept
            : m_Count(count), m_Ptr(ptr) {}
        inline constexpr vector_proxy(std::initializer_list<T> il) noexcept
            : vector_proxy(il.size(), il.begin()) {}

        template <typename U, std::enable_if_t<is_proxyable<U>, bool> = true>
        inline constexpr vector_proxy(const U &u) noexcept {
            auto [s, p] = traits<U>::proxy(u);
            m_Count = s;
            m_Ptr = p;
        }

        inline constexpr iterator begin() const noexcept { return m_Ptr; }

        inline constexpr iterator end() const noexcept {
            return m_Ptr + m_Count;
        }

        inline reference front() const noexcept {
            assert(m_Count && m_Ptr);
            return *m_Ptr;
        }

        inline reference back() const noexcept {
            assert(m_Count && m_Ptr);
            return *(m_Ptr + m_Count - 1);
        }

        [[nodiscard]] inline constexpr bool empty() const noexcept {
            return (m_Count == 0);
        }

        [[nodiscard]] inline constexpr size_type size() const noexcept {
            return m_Count;
        }

        inline constexpr pointer data() const noexcept { return m_Ptr; }

        inline constexpr reference operator[](size_t index) const noexcept {
            assert(index < size());
            return m_Ptr[index];
        }

      private:
        size_type m_Count = 0;
        pointer m_Ptr = nullptr;
    };

    template <typename U, size_t N> struct proxyable_traits<U[N]> {
        inline static constexpr bool is_specialized = true;

        template <typename T>
        inline static constexpr bool is_proxyable =
            std::is_same_v<std::remove_const_t<T>, U>;

        inline static std::pair<size_t, const U *>
        proxy(const U (&u)[N]) noexcept {
            return std::make_pair<size_t, const U *>(N, u);
        }
    };
} // namespace matplot

#endif // MATPLOTPLUSPLUS_VECTOR_PROXY_H
