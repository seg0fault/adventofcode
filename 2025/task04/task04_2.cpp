#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <initializer_list>

#include <vector>

#define BUFLEN 1234

struct solution_t
{
    int64_t out{};
    size_t line_size{};

    std::vector<char *> lines;

    char *up = nullptr;
    char *middle = nullptr;
    char *down = nullptr;

public:
    solution_t(size_t l_size)
        : line_size(l_size)
    {
    }

    ~solution_t()
    {
        for (char *l : lines)
            delete[] l;
    }

    int64_t get_out() { return out; }

    void clean_line(char *l)
    {
        for (size_t it = 0; it < line_size; it++)
        {
            if (l[it] == 'R')
            {
                l[it] = '.';
                out++;
            }
        }
    }

    void clean_lines()
    {
        for (char *l : lines)
            clean_line(l);
    }

    void solve(FILE *fp)
    {
        char buf[BUFLEN];
        while (fgets(buf, BUFLEN, fp))
        {
            char *line = new char[line_size];
            memcpy(line, buf, line_size);
            lines.push_back(line);
        }

        process_lines();
    }

    void process_lines()
    {
        int old_out = out;
        for (char *l : lines)
        {
            down = l;
            count_middle();
            move_lines();
        }

        count_middle();
        clean_lines();

        if (out > old_out)
            process_lines();
    }

    void move_lines()
    {
        up = middle;
        middle = down;
        down = nullptr;
    }

    void count_middle()
    {
        process_generic(up, middle, down);
    }

    void process_generic(char *up, char *middle, char *down)
    {
        for (size_t it = 0; it < line_size; it++)
        {
            int count = count_for_pos_generic(it, up, middle, down);
            if (count < 4)
            {
                middle[it] = 'R';
            }
        }
    }

    static inline int is_wrap(char c)
    {
        return (c == '@' || c == 'R') ? 1 : 0;
    }

    int count_for_pos_generic(size_t pos, char *up, char *middle, char *down)
    {
        if (!middle || !is_wrap(middle[pos]))
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
