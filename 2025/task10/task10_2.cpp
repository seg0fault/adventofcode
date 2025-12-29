#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include <vector>
#include <unordered_map>

#define BUFLEN 1234

struct vec_t;

struct vec_t
{
    std::vector<int> data;

    vec_t(size_t sz) : data(std::vector<int>(sz, 0)) {}
    vec_t(std::vector<int> data_) : data(std::move(data_)) {}

    inline size_t get_size() const { return data.size(); }

    void reset()
    {
        data.assign(get_size(), 0);
    }

    int sum() const
    {
        int s = 0;
        for (size_t it = 0; it < get_size(); it++)
        {
            s += data[it];
        }
        return s;
    }

    void print() const
    {
        for (int v : data)
        {
            printf("%d ", v);
        }
        printf("\n");
    }

    bool operator>(const vec_t &rhs) const
    {
        for (size_t it = 0; it < get_size(); it++)
        {
            if (data[it] > rhs.data[it])
                return true;
        }
        return false;
    }

    bool operator==(const vec_t &rhs) const
    {
        for (size_t it = 0; it < get_size(); it++)
        {
            if (data[it] != rhs.data[it])
                return false;
        }
        return true;
    }

    vec_t &operator+=(const vec_t &rhs)
    {
        for (size_t it = 0; it < get_size(); it++)
        {
            data[it] += rhs.data[it];
        }
        return *this;
    }

    vec_t &operator-=(const vec_t &rhs)
    {
        for (size_t it = 0; it < get_size(); it++)
        {
            data[it] -= rhs.data[it];
        }
        return *this;
    }

    bool is_eq_even(const vec_t &rhs)
    {
        for (size_t it = 0; it < get_size(); it++)
        {
            int difference = rhs.data[it] - data[it];
            if (difference < 0 || (difference & 1))
                return false;
        }
        return true;
    }

    bool is_all_even()
    {
        for (size_t it = 0; it < get_size(); it++)
        {
            if (data[it] & 1)
                return false;
        }
        return true;
    }

    bool is_null()
    {
        for (size_t it = 0; it < get_size(); it++)
        {
            if (data[it])
                return false;
        }
        return true;
    }

    bool is_valid()
    {
        for (size_t it = 0; it < get_size(); it++)
        {
            if (data[it] < 0)
                return false;
        }
        return true;
    }

    void divide2()
    {
        for (size_t it = 0; it < get_size(); it++)
        {
            data[it] >>= 1;
        }
    }

    void mult2()
    {
        for (size_t it = 0; it < get_size(); it++)
        {
            data[it] <<= 1;
        }
    }
};

struct vec_sum_hash
{
    int operator()(const vec_t &val) const
    {
        return val.sum();
    }
};

class solution_t
{
    int64_t out{};
    std::unordered_map<vec_t /*tsk*/, int /*sol*/, vec_sum_hash> cache;

public:
    int64_t get_out() const { return out; }

    void solve(FILE *fp)
    {
        char buf[BUFLEN];

        while (fgets(buf, BUFLEN, fp))
        {
            int val = process_line(buf);
            out += val;
        }
    }

private:
    static inline int to_digit(char c)
    {
        return c - '0';
    }

    static inline bool is_digit(char c)
    {
        return c >= '0' && c <= '9';
    }

    int process_line(char *buf)
    {
        size_t it;
        for (it = 0; buf[it]; it++)
        {
            if (buf[it] == ']')
            {
                break;
            }
        }

        size_t task_size = it - 1;
        std::vector<int> comp;

        std::vector<vec_t> components;
        for (; buf[it]; it++)
        {
            if (buf[it] == '(')
            {
                comp = std::vector<int>(task_size, 0);
                continue;
            }

            if (is_digit(buf[it]))
            {
                comp[to_digit(buf[it])] = true;
                continue;
            }

            if (buf[it] == ')')
            {
                components.emplace_back(std::move(comp));
                continue;
            }

            if (buf[it] == '{')
            {
                break;
            }
        }

        std::vector<int> task_vector;

        for (; buf[it]; it++)
        {
            if (is_digit(buf[it]))
            {
                int val = 0;
                sscanf(buf + it, "%d", &val);
                if (val >= 10)
                    it++;
                if (val >= 100)
                    it++;
                if (val >= 1000)
                    it++;
                task_vector.push_back(val);
                continue;
            }

            if (buf[it] == '}')
            {
                break;
            }
        }

        vec_t task(task_vector);
        vec_t task_ini(task_vector);

        cache.clear();
        int solution = compute_task(task, components);

        if (solution > task.sum())
        {
            printf("TSK NOT CONVERGE:\n");
            task.print();
            printf("COMPONENTS:\n");
            for (const vec_t &v : components)
                v.print();
            printf("\n");

            return 0;
        }

        return solution;
    }

    int compute_task(vec_t &task, const std::vector<vec_t> &components)
    {
        auto it = cache.find(task);
        if (it != cache.end())
            return it->second;

        int solution{};
        if (!task.is_null())
        {
            int mult2 = 0;

            if (task.is_all_even())
            {
                task.divide2();
                mult2++;
            }

            if (task.is_null())
            {
                printf("Error: tsk is null before reduce\n");
            }

            solution = reduce_task_r(task, components, 0);

            solution = solution << mult2;

            for (; mult2 > 0; mult2--)
                task.mult2();
        }
        else
        {
            solution = 0;
        }

        cache[task] = solution;
        return solution;
    }

    int reduce_task_r(vec_t &task, const std::vector<vec_t> &components, size_t start)
    {
        size_t comp_size = components.size();

        int min_sol = 999999;

        if (task.is_all_even())
        {
            int curr_sol = compute_task(task, components);

            if (curr_sol < min_sol)
                min_sol = curr_sol;
        }

        for (size_t it = start; it < comp_size; it++)
        {
            task -= components[it];

            if (!task.is_valid())
            {
                task += components[it];
                continue;
            }

            if (task.is_all_even())
            {
                int curr_sol = 1 + compute_task(task, components);

                if (curr_sol < min_sol)
                    min_sol = curr_sol;
            }

            if (true)
            {
                int curr_sol = 1 + reduce_task_r(task, components, it + 1);

                if (curr_sol < min_sol)
                    min_sol = curr_sol;
            }

            task += components[it];
        }

        return min_sol;
    }
};

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: a.out file\n");
        return 1;
    }

    const char *test_file = argv[1];
    FILE *fp = fopen(test_file, "rb");
    if (!fp)
    {
        printf("Cannot open %s\n", test_file);
        return 2;
    }

    solution_t solution;

    double t = clock();
    solution.solve(fp);
    t = clock() - t;

    printf("RESULT:  %ld\n", solution.get_out());
    printf("ELAPSED: %lf\n", t / CLOCKS_PER_SEC);

    fclose(fp);
    return 0;
}
