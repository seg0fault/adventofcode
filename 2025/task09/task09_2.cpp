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

    static inline bool is_inside1d(int64_t l, int64_t r, double p)
    {
        if (l > r)
            std::swap(l, r);

        return (p > l && p < r);
    }

    static bool is_mult_inside(const point_t sq1, const point_t &sq2, const point_t &p1, const point_t &p2, double mult)
    {
        double m1 = (double)llabs(p1.first - p2.first) * mult;
        double m2 = (double)llabs(p1.second - p2.second) * mult;

        double pm1 = std::min(p1.first, p2.first) + m1;
        double pm2 = std::min(p1.second, p2.second) + m2;

        return (is_inside1d(sq1.first, sq2.first, pm1) && is_inside1d(sq1.second, sq2.second, pm2));
    }

    static inline bool is_inside(const point_t sq1, const point_t &sq2, const point_t &p1, const point_t &p2)
    {
        for (double mult : {0.1, 0.25, 0.5, 0.75, 0.9})
        {
            if (is_mult_inside(sq1, sq2, p1, p2, mult))
                return true;
        }
        return false;
    }

    void compute()
    {
        points.push_back(points.front());
        for (size_t i = 0; i < points.size(); i++)
        {
            for (size_t j = i + 1; j < points.size(); j++)
            {
                const point_t &p1 = points[i];
                const point_t &p2 = points[j];
                int64_t area = get_area(p1, p2);
                if (area > max)
                {
                    bool found_inside = false;
                    for (size_t k = 0; k < points.size(); k++)
                    {
                        if (k == i || k == j)
                            continue;

                        if (is_inside(p1, p2, points[k], points[k + 1]))
                        {
                            found_inside = true;
                            break;
                        }
                    }

                    if (!found_inside)
                    {
                        max = area;
                    }
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
