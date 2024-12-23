#include <cassert>

#include <numeric>
#include <vector>
using namespace std;

template <bool UnionByRank = false, bool RecordSize = false>
class DSU
{
private:
    size_t union_impl(size_t a, size_t b, bool use_path_compression) noexcept
    {
        size_t root_a = use_path_compression ? find_with_path_compression(a) : find(a);
        size_t root_b = use_path_compression ? find_with_path_compression(b) : find(b);
        m_fathers[root_a] = root_b;
        return root_b;
    }
public:
    DSU(size_t n) : m_fathers(n)
    {
        iota(m_fathers.begin(), m_fathers.end(), 0);
    }

    size_t find_with_path_compression(size_t x) noexcept
    {
        return m_fathers[x] == x ? x : m_fathers[x] = find_with_path_compression(m_fathers[x]);
    }

    size_t find(size_t x) const noexcept
    {
        return m_fathers[x] == x ? x : find(m_fathers[x]);
    }

    size_t union_sets(size_t a, size_t b) noexcept
    {
        return union_impl(a, b, false);
    }

    size_t union_with_path_compression(size_t a, size_t b) noexcept
    {
        return union_impl(a, b, true);
    }
protected:
    vector<size_t> m_fathers;
};

// Specialization for UnionByRank
template <>
class DSU<true, false> : public DSU<false, false>
{
private:
    using base_type = DSU<false, false>;

    size_t union_impl(size_t a, size_t b, bool use_path_compression) noexcept
    {
        size_t root_a =
            use_path_compression ? base_type::find_with_path_compression(a) : base_type::find(a);
        size_t root_b =
            use_path_compression ? base_type::find_with_path_compression(b) : base_type::find(b);
        if (root_a == root_b) {
            return root_a;
        }

        if (m_ranks[root_a] > m_ranks[root_b]) {
            swap(root_a, root_b);
        }

        base_type::m_fathers[root_a] = root_b;
        if (m_ranks[root_a] == m_ranks[root_b]) {
            ++m_ranks[root_b];
        }

        return root_b;
    }
public:
    DSU(size_t n) : base_type(n), m_ranks(n, 1) {}

    size_t union_by_rank(size_t a, size_t b) noexcept
    {
        return union_impl(a, b, false);
    }

    size_t union_with_path_compression_by_rank(size_t a, size_t b) noexcept
    {
        return union_impl(a, b, true);
    }

    size_t rank(size_t x) const noexcept
    {
        return m_ranks[x];
    }
protected:
    vector<size_t> m_ranks;
};

// Specialization for RecordSize
template <>
class DSU<false, true> : public DSU<false, false>
{
private:
    using base_type = DSU<false, false>;

    size_t union_impl(size_t a, size_t b, bool use_path_compression) noexcept
    {
        size_t root_a =
            use_path_compression ? base_type::find_with_path_compression(a) : base_type::find(a);
        size_t root_b =
            use_path_compression ? base_type::find_with_path_compression(b) : base_type::find(b);
        if (root_a == root_b) {
            return root_b;
        }

        if (m_sizes[root_b] < m_sizes[root_a]) {
            swap(root_a, root_b);
        }

        base_type::m_fathers[root_a] = root_b;
        m_sizes[root_b] += m_sizes[root_a];
        return root_b;
    }
public:
    DSU(size_t n) : base_type(n), m_sizes(n, 1) {}

    size_t union_sets(size_t a, size_t b) noexcept
    {
        return union_impl(a, b, false);
    }

    size_t union_with_path_compression(size_t a, size_t b) noexcept
    {
        return union_impl(a, b, true);
    }

    size_t size(size_t x) const noexcept
    {
        return m_sizes[x];
    }
protected:
    vector<size_t> m_sizes;
};

// Specialization for both UnionByRank and RecordSize
template <>
class DSU<true, true> : public DSU<false, false>
{
private:
    using base_type = DSU<false, false>;

    size_t union_impl(size_t a, size_t b, bool use_path_compression)
    {
        size_t root_a =
            use_path_compression ? base_type::find_with_path_compression(a) : base_type::find(a);
        size_t root_b =
            use_path_compression ? base_type::find_with_path_compression(b) : base_type::find(b);
        if (root_a == root_b) {
            return root_b;
        }

        if (m_ranks[root_a] > m_ranks[root_b]) {
            swap(root_a, root_b);
        }

        base_type::m_fathers[root_a] = root_b;
        if (m_ranks[root_a] == m_ranks[root_b]) {
            ++m_ranks[root_b];
        }

        m_sizes[root_b] += m_sizes[root_a];
        return root_b;
    }
public:
    DSU(size_t n) : base_type(n), m_ranks(n, 1), m_sizes(n, 1) {}

    size_t union_sets(size_t a, size_t b)
    {
        return union_impl(a, b, false);
    }

    size_t union_with_path_compression(size_t a, size_t b)
    {
        return union_impl(a, b, true);
    }

    size_t size(size_t x) const noexcept
    {
        return m_sizes[x];
    }

    size_t rank(size_t x) const noexcept
    {
        return m_ranks[x];
    }
protected:
    vector<size_t> m_ranks;
    vector<size_t> m_sizes;
};

int main()
{
    DSU<true, true> dsu(10);
    dsu.union_sets(1, 2);
    dsu.union_sets(3, 4);
    dsu.union_sets(1, 4);

    assert(dsu.find(1) == dsu.find(2));
    assert(dsu.find(3) == dsu.find(4));
    assert(dsu.find(1) == dsu.find(4));
    assert(dsu.size(4) == 4);
    assert(dsu.rank(4) == 3);

    return 0;
}
