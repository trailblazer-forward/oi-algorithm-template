#include <cmath>

#include <algorithm>
#include <iostream>
#include <type_traits>
#include <vector>

using namespace std;

template <typename...>
constexpr bool always_false = false;

template <typename T, typename F, typename = void>
class SparseTable
{
    static_assert(always_false<T>, "The function type is invalid!");
};

template <typename T, typename F>
class SparseTable<T, F, enable_if_t<is_invocable_r_v<T, F, T, T>>>
{
private:
    void init_log_table(size_t n)
    {
        m_log.assign(n + 1, 0);
        for (size_t i = 2; i <= n; ++i) {
            m_log[i] = m_log[i / 2] + 1;
        }
    }
public:
    template <typename Func>
    SparseTable(const vector<T>& data, Func&& func) : m_func(std::forward<Func>(func))
    {
        size_t n = data.size();
        init_log_table(n);

        int max_log = static_cast<int>(ceil(log2(n)) + 1);
        m_table.assign(n, vector<T>(max_log));
        for (int i = 0; i < n; ++i) {
            m_table[i][0] = data[i];
        }

        for (int j = 1; j < max_log; ++j) {
            for (int i = 0; i + (1 << j) <= n; ++i) {
                m_table[i][j] = m_func(m_table[i][j - 1], m_table[i + (1 << (j - 1))][j - 1]);
            }
        }
    }

    T query(size_t left, size_t right)
    {
        int j = m_log[right - left + 1];
        return m_func(m_table[left][j], m_table[right - (1LL << j) + 1][j]);
    }
private:
    const F m_func;
    vector<vector<T>> m_table;
    vector<int> m_log;
};

template <typename T, typename Func>
SparseTable(const vector<T>&, Func&&) -> SparseTable<T, decay_t<Func>>;

int main()
{
    std::vector<int> data = {1, 3, 2, 7, 9, 11, 3, 5, 6, 8};
    SparseTable st(data, [](int a, int b) { return min(a, b); });

    std::cout << "Minimum value in range [0, 5]: " << st.query(0, 5) << std::endl;
    std::cout << "Minimum value in range [3, 8]: " << st.query(3, 8) << std::endl;
    std::cout << "Minimum value in range [1, 4]: " << st.query(1, 4) << std::endl; // 添加更多测试用例
    std::cout << "Minimum value in range [2, 6]: " << st.query(2, 6) << std::endl;

    return 0;
}
