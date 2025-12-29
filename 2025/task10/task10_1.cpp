#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include <vector>

#define BUFLEN 1234

struct vec_t
{
    std::vector<bool> data;

    vec_t(size_t sz) : data(std::vector<bool>(sz, false)) {}
    vec_t(std::vector<bool> data_) : data(std::move(data_)) {}

    size_t get_size() const { return data.size(); }

    void reset()
    {
        for (size_t it = 0; it < get_size(); it++)
        {
            data[it] = false;
        }
    }

    void print() const
    {
        for (bool v : data)
        {
            printf("%c", v ? '#' : '.');
        }
        printf("\n");
    }

    bool operator==(const vec_t &rhs)
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
            data[it] = data[it] ^ rhs.data[it];
        }
        return *this;
    }
};

class solution_t
{
    int64_t out{};

public:
    int64_t get_out() const { return out; }

    void solve(FILE *fp)
    {
        char buf[BUFLEN];

        while (fgets(buf, BUFLEN, fp))
        {
            out += process_line(buf);
        }
    }

private:
    static inline bool is_on(char c)
    {
        return c == '#';
    }

    static inline bool is_off(char c)
    {
        return c == '.';
    }

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
        std::vector<bool> task;

        size_t it;
        for (it = 0; buf[it]; it++)
        {
            if (is_on(buf[it]))
            {
                task.push_back(true);
                continue;
            }

            if (is_off(buf[it]))
            {
                task.push_back(false);
                continue;
            }

            if (buf[it] == ']')
            {
                break;
            }
        }

        size_t task_size = task.size();
        vec_t task_obj(std::move(task));

        std::vector<bool> comp;

        std::vector<vec_t> components;
        for (; buf[it]; it++)
        {
            if (buf[it] == '(')
            {
                comp = std::vector<bool>(task_size, false);
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
        return compute_task(task_obj, components);
    }

    int compute_task(const vec_t &task, const std::vector<vec_t> &components)
    {
        vec_t sol(task.get_size());
        return compute_task_r(task, components, sol, 1, -1);
    }

    int compute_task_r(const vec_t &task, const std::vector<vec_t> &components, vec_t &sol, int d, size_t j)
    {
        if (d > 50)
        {
            return 999;
        }
        size_t task_size = task.get_size();
        size_t comp_size = components.size();

        vec_t tmp_sol(task_size);
        for (size_t it = j + 1; it < comp_size; it++)
        {
            tmp_sol = sol;
            tmp_sol += components[it];
            if (tmp_sol == task)
            {
                return d;
            }
        }

        int min = 999;
        for (size_t it = j + 1; it < comp_size; it++)
        {
            tmp_sol = sol;
            tmp_sol += components[it];

            int val = compute_task_r(task, components, tmp_sol, d + 1, it);
            if (val < min)
            {
                min = val;
            }
        }

        return min;
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
