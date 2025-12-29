#include <cstdio>
#include <cstdint>
#include <ctime>
#include <cstring>

#define BUFLEN 1234

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
        
        while (fgets(buf, BUFLEN, fp))
        {
            out += max_str(buf);
        }
    }

private:
    int max_str(const char *str)
    {
        int pre{};
        int max{};
        int post = -1;

        size_t max_it{};
        for (size_t it = 0; it < line_size; it++)
        {
            int val = str[it] - '0';

            if (val > max)
            {
                pre = max;
                max = val;
                max_it = it;
                post = -1;
            }
            else if (val > post)
            {
                post = val;
            }

            // printf ("pre: %d; max %d; post %d\n", pre, max, post);
        }

        int out{};
        if (max_it == line_size - 1) // last
        {
            out = pre * 10 + max;
            // printf("OUT L: %d\n", out);
        }
        else
        {
            out = max * 10 + post;
            // printf("OUT N: %d\n", out);
        }

        return out;
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
