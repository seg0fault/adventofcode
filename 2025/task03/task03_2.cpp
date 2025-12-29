#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <algorithm>
#include <cstring>
#include <array>

#define BUFLEN 1234

struct arr_val_t
{
    static constexpr size_t max_digit_count = 12;
    std::array<int, max_digit_count> arr;

    arr_val_t()
    {
        reset();
    }

    void reset()
    {
        for (int &val : arr)
            val = -1;
    }

    void try_push(int val, size_t max_w)
    {
        size_t size = max_digit_count;
        size_t e_max_w = std::min(size, max_w);

        bool found{};
        for (size_t it = size - e_max_w; it < size; it++)
        {
            if (found)
            {
                arr[it] = -1;
            }
            if (!found && val > arr[it])
            {
                arr[it] = val;
                found = true;
            }
        }
    }

    int64_t get_int() const
    {
        int64_t out{};
        int64_t mult = 1;

        for (auto it = arr.rbegin(); it != arr.rend(); ++it)
        {
            out += *it * mult;
            mult *= 10;
        }

        return out;
    }

    void print() const
    {
        for (int val : arr)
        {
            printf("%d ", val);
        }
        printf("\n");
    }

    bool check() const
    {
        for (int val : arr)
        {
            if (val == -1)
            {
                print();
                return false;
            }
        }
        return true;
    }
};

class solution_t
{
    int64_t out{};
    size_t line_size{};

public:
    int64_t get_out() const { return out; }

    void solve(FILE *fp)
    {
        char buf[BUFLEN];

        if (!fgets(buf, BUFLEN, fp))
        {
            printf("Failed to define line size\n");
            return;
        }

        line_size = strlen(buf) - 1;
        rewind(fp);

        arr_val_t helper;
        while (fgets(buf, BUFLEN, fp))
        {
            out += max_str(buf, helper);
            helper.reset();
        }
    }

private:
    static inline int from_digit(char c)
    {
        return c - '0';
    }

    int64_t max_str(const char *str, arr_val_t &helper)
    {
        for (size_t it = 0; it < line_size; it++)
        {
            int val = from_digit(str[it]);
            size_t max_width = line_size - it;
            helper.try_push(val, max_width);
        }

        // helper.check();
        // helper.print();

        int64_t res = helper.get_int();
        return res;
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
