#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <vector>

class solution_t
{
    using limit_t = std::pair<int64_t, int64_t>;
    std::vector<limit_t> limits;

    int64_t out{};

public:
    int64_t get_out() const { return out; }

    void solve(FILE *fp)
    {
        int64_t a, b = 0;
        int scan = 0;
        while ((scan = fscanf(fp, "%ld-%ld", &a, &b)))
        {
            if (scan == 2)
                update_limits(a, b);
            else if (scan == 1)
                update_numbers(a);
            else
                break;
        }
    }

private:
    static inline bool is_valid(int64_t num, const limit_t &lim)
    {
        return lim.first <= num && num <= lim.second;
    }

    void update_limits(int64_t l, int64_t r)
    {
        limits.emplace_back(l, r);
    }

    void update_numbers(int64_t num)
    {
        for (const limit_t &lim : limits)
        {
            if (is_valid(num, lim))
            {
                out++;
                break;
            }
        }
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
