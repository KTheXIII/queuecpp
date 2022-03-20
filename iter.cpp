#include <iostream>
#include <cstdint>
#include <iterator>
#include <vector>

namespace nerv {
// Fixed size queue buffer, FIFO
template <typename T, std::size_t SIZE>
class queue {
  public:
    template <typename pointer_type, typename reference_type>
    struct iterator_base {
        using iterator_category = std::random_access_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = pointer_type;
        using reference         = reference_type;

        constexpr iterator_base(pointer buffer, std::size_t const& ptr, std::size_t const& max, std::size_t const& end)
            : m_buffer(buffer), m_ptr(ptr), m_max(max), m_end(end) {}

        constexpr auto operator*() const -> reference { return m_buffer[m_ptr]; }
        constexpr auto operator->() -> pointer { return &m_buffer[m_ptr]; }

        constexpr auto operator++() -> iterator_base& {    // prefix increment
            m_ptr = (m_ptr + m_max - 1) % m_max;
            return *this;
        }
        constexpr auto operator++(int) -> iterator_base {  // postfix increment
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        constexpr auto operator--() -> iterator_base& {
            m_ptr = (m_ptr + 1) % m_max;
            return *this;
        }
        constexpr auto operator--(int) -> iterator_base {
            auto tmp = *this;
            --(*this);
            return tmp;
        }

        constexpr friend auto operator==(iterator_base const& a, iterator_base const& b) -> bool {
            return a.m_ptr == b.m_ptr;
        }
        constexpr friend auto operator!=(iterator_base const& a, iterator_base const& b) -> bool {
            return !(a == b);
        }

        // random access iterator requirements
        constexpr auto operator+=(difference_type const& n) -> iterator_base& {
            if ((n > 0 && n > m_max - 1) || (n < 0 && -n > m_max - 1))  // sets the iterator to end iterator
                m_ptr = (m_ptr + m_max + m_max - 1) % m_max;
            else                                                        // do normal addition with wrap
                m_ptr = (m_ptr + m_max + n) % m_max;
            return *this;
        }

        constexpr auto operator+(difference_type const& n) -> iterator_base {
            if ((n > 0 && n > m_max - 1) || (n < 0 && -n > m_max - 1))  // sets the iterator to end iterator
                m_ptr = (m_ptr + m_max + m_max - 1) % m_max;
            else                                                        // do normal addition with wrap
                m_ptr = (m_ptr + m_max + n) % m_max;
            return *this;
        }
        constexpr auto operator-(difference_type const& n) -> iterator_base {
            return (*this) + (-n);
        }

        constexpr auto operator-=(difference_type const& n) -> iterator_base& {
            return (*this) += (-n);
        }

        constexpr friend auto operator-(iterator_base const& a, iterator_base const& b) -> difference_type {
            // calcuate the distance to the end iterator
            auto a_end = a.dist_to_end();
            auto b_end = b.dist_to_end();
            return a_end - b_end;
        }

        constexpr auto operator[](difference_type const& n) -> reference {
            if ((n > 0 && n > m_max - 1) || (n < 0 && -n > m_max - 1)) // offset the pointer with wrap
                return m_buffer[(m_ptr + m_max + n) % m_max];
            else                                                       // deference the unused part of the memory,
                return m_buffer[(m_ptr + m_max + m_max - 1) % m_max];  // i.e the place where end iterator is pointed at
        }

        constexpr friend auto operator<(iterator_base const& a, iterator_base const& b) -> bool {
            return b - a > 0;
        }
        constexpr friend auto operator>(iterator_base const& a, iterator_base const& b) -> bool {
            return b < a;
        }
        constexpr friend auto operator>=(iterator_base const& a, iterator_base const& b) -> bool {
            return !(a < b);
        }
        constexpr friend auto operator<=(iterator_base const& a, iterator_base const& b) -> bool {
            return !(a > b);
        }

      private:
        // FIXME: Find a better way to get the distance from the current ptr to the end, O(n)
        auto dist_to_end() const -> difference_type {
            auto start = m_ptr;
            auto dist = 0;
            while(start != m_end) {
                dist++;
                start = (start + m_max - 1) % m_max;
            }
            return dist;
        }

      private:
        pointer     m_buffer;
        std::size_t m_ptr;
        std::size_t m_max;
        std::size_t m_end;
    };

    using iterator       = iterator_base<T*, T&>;
    using const_iterator = iterator_base<T const*, T const&>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    auto begin() -> iterator { return iterator(m_buffer, m_tail, m_max, m_head); }
    auto end()   -> iterator { return iterator(m_buffer, m_head, m_max, m_head); }
    auto cbegin() const -> const_iterator { return const_iterator(m_buffer, m_tail, m_max, m_head); }
    auto cend()   const -> const_iterator { return const_iterator(m_buffer, m_head, m_max, m_head); }

    auto rbegin() -> reverse_iterator { return reverse_iterator(end()); }
    auto rend()   -> reverse_iterator { return reverse_iterator(begin()); }
    auto crbegin() const -> const_reverse_iterator { return const_reverse_iterator(cend()); }
    auto crend()   const -> const_reverse_iterator { return const_reverse_iterator(cbegin()); }

  public:
    auto enq(T const& value) -> void {
        m_buffer[m_head] = value;
        if (queue::dec_wrap(m_head, m_max) == m_tail)
            queue::dec_wrap(m_tail, m_max);
        else
            ++m_size;
    }
    auto enq_keep(T const& value) -> void {
        if (m_head == m_tail) return;
        enq(value);
    }
    auto deq() -> T {
        auto const ret = m_buffer[m_tail];
        if (m_tail != m_head) {
            queue::dec_wrap(m_tail, m_max);
            --m_size;
        }
        return ret;
    }

    auto size() const -> std::size_t { return m_size; }
    auto back() -> T { return m_buffer[m_tail]; }
    auto front() -> T { return m_buffer[(m_head + 1) % m_max]; }

    using ref_type       = T&;
    using const_ref_type = T const&;
    auto operator[](std::size_t const& offset) -> ref_type { return m_buffer[(m_tail + m_max + offset) % m_max]; }
    auto operator[](std::size_t const& offset) const -> const_ref_type { return m_buffer[(m_tail + m_max + offset) % m_max]; }
  private:
    static auto inc_wrap(std::size_t& value, std::size_t const& max) -> std::size_t const& {
        value = (value + 1) % max;
        return value;
    }
    static auto dec_wrap(std::size_t& value, std::size_t const& max) -> std::size_t const& {
        value = (value + max - 1) % max;
        return value;
    }

  private:
    std::size_t m_max   = SIZE + 1;
    T           m_buffer[SIZE + 1];  // allocate extra space for end iterator
    std::size_t m_head  = 0;
    std::size_t m_tail  = SIZE;
    std::size_t m_size  = 0;
};
}

auto main([[maybe_unused]]std::int32_t argc, [[maybe_unused]]char const* argv[]) -> std::int32_t {
    std::cout << "C++ Iterator Implementation" << "\n";
    std::cout << "┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄" << "\n";

    constexpr auto SIZE = 4;
    nerv::queue<std::uint32_t, SIZE> test{};
    test.enq(1);
    test.enq(2);
    test.enq(3);
    test.enq(4);

    std::for_each(std::begin(test), std::end(test), [i = 0](auto const& v) mutable {
        std::cout << v;
        if (i++ < SIZE - 1) std::cout << " · ";
        else std::cout << "\n";
    });
    std::cout << "┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄" << "\n";

    std::for_each(test.crbegin(), test.crend(), [i = 0](auto v) mutable {
        std::cout << v;
        if (i++ < SIZE - 1) std::cout << " · ";
        else std::cout << "\n";
    });
    std::cout << "┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄" << "\n";

    std::cout << "max: " << *std::max_element(std::begin(test), std::end(test)) << "\n";;
    auto it = std::find(std::begin(test), std::end(test), 2);
    std::cout << *it << "\n";

    //std::for_each(std::begin(test), std::end(test), [i = 0](auto const& v) mutable {
    //    std::cout << v;
    //    if (i++ < SIZE - 1) std::cout << " · ";
    //    else std::cout << "\n";
    //});
    //std::cout << "┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄" << "\n";

    return 0;
}
