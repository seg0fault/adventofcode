#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include <vector>

class solution_t
{
    using point_t = std::pair<int64_t, int64_t>;
    int64_t max{};

    std::vector<point_t> points;

public:
    int64_t get_out() const { return max; }

    void solve(FILE *fp)
    {
        int64_t a, b = 0;
        while (fscanf(fp, "%ld,%ld", &a, &b) == 2)
        {
            points.emplace_back(a, b);
        }

        compute();
    }

private:
    static inline int64_t get_area(const point_t &lhs, const point_t &rhs)
    {
        return (llabs(lhs.first - rhs.first) + 1) * (llabs(lhs.second - rhs.second) + 1);
    }

    static inline bool is_inside2d(int64_t l, int64_t r, int64_t p)
    {
        if (l > r)
            std::swap(l, r);
        return p > l && p < r;
    }

    static inline bool is_inside(const point_t &lhs, const point_t &rhs, const point_t &p)
    {
        return is_inside2d(lhs.first, rhs.first, p.first) || is_inside2d(lhs.second, rhs.second, p.second);
    }

    void compute()
    {
        for (size_t i = 0; i < points.size(); i++)
        {
            for (size_t j = i + 1; j < points.size(); j++)
            {
                const point_t &p1 = points[i];
                const point_t &p2 = points[j];
                int64_t area = get_area(p1, p2);
                if (area > max)
                {
                    max = area;
                }
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
