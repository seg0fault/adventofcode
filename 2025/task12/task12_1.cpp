#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <initializer_list>

struct solution_t
{
    int64_t out{};

    int64_t get_out() const { return out; }

public:
    void solve(FILE *fp)
    {
        int x{}, y{};
        int a1{}, a2{}, a3{}, a4{}, a5{}, a6{};

        while (fscanf(fp, "%dx%d: %d %d %d %d %d %d", &x, &y, &a1, &a2, &a3, &a4, &a5, &a6) == 8)
        {
            compute(x * y, a1, a2, a3, a4, a5, a6);
        }
    }

private:
    void compute(int area, int a0, int a1, int a2, int a3, int a4, int a5)
    {
        // ###
        // ###
        // ###
        // .##
        while (a4 > 0 && a5 > 0)
        {
            area -= 12;
            a4--;
            a5--;
        }

        // ###
        // ###
        // ###
        // ###
        // #.#
        while (a0 > 0 && a3 > 0)
        {
            area -= 15;
            a0--;
            a3--;
        }

        for (int val : {a0, a1, a2, a3, a4, a5})
        {
            for (; val > 0; val--)
                area -= 9;
        }

        if (area > 0)
            out++;
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
