#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <initializer_list>

#define BUFLEN 1234

class solution_t
{
    int64_t out{};
    size_t line_size{};

    char *up{};
    char *middle{};
    char *down{};

public:
    solution_t(size_t l_size)
        : line_size(l_size)
    {
        up = new char[l_size];
        middle = new char[l_size];
        down = new char[l_size];

        for (char *l : {up, middle, down})
            reset_line(l);
    }

    ~solution_t()
    {
        delete[] up;
        delete[] middle;
        delete[] down;
    }

    int64_t get_out() { return out; }

    void solve(FILE *fp)
    {
        char buf[BUFLEN];
        while (fgets(buf, BUFLEN, fp))
        {
            memcpy(down, buf, line_size);
            count_middle();
            move_lines();
        }
        count_middle(); // count last line
    }

private:
    void reset_line(char *l)
    {
        memset(l, 0, line_size);
    }

    void move_lines()
    {
        memcpy(up, middle, line_size);
        memcpy(middle, down, line_size);
        reset_line(down);
    }

    void count_middle()
    {
        out += process_generic(up, middle, down);
    }

    int process_generic(char *up, char *middle, char *down)
    {
        int res{};
        for (size_t it = 0; it < line_size; it++)
        {
            int count = count_for_pos_generic(it, up, middle, down);
            if (count < 4)
            {
                res++;
            }
        }
        return res;
    }

    static inline int is_wrap(char c)
    {
        return c == '@' ? 1 : 0;
    }

    int count_for_pos_generic(size_t pos, char *up, char *middle, char *down)
    {
        if (!is_wrap(middle[pos]))
            return 1000;

        int res = 0;
        res += up && is_wrap(up[pos]);
        res += down && is_wrap(down[pos]);

        if (pos > 0)
        {
            res += up && is_wrap(up[pos - 1]);
            res += down && is_wrap(down[pos - 1]);
            res += is_wrap(middle[pos - 1]);
        }

        if (pos < line_size - 1)
        {
            res += up && is_wrap(up[pos + 1]);
            res += down && is_wrap(down[pos + 1]);
            res += is_wrap(middle[pos + 1]);
        }

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

    char buf[BUFLEN];

    if (!fgets(buf, BUFLEN, fp))
    {
        printf("Failed to define line size\n");
        return 3;
    }

    size_t line_size = strlen(buf) - 1;
    rewind(fp);

    solution_t solution(line_size);

    double t = clock();
    solution.solve(fp);
    t = clock() - t;

    printf("RESULT:  %ld\n", solution.get_out());
    printf("ELAPSED: %lf\n", t / CLOCKS_PER_SEC);

    fclose(fp);
    return 0;
}
