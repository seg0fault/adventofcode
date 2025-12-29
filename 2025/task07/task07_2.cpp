#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <vector>

#define BUFSIZE 1234

struct solution_t
{
    int64_t out{};

    std::vector<char *> lines;

    size_t line_size{};
    size_t line_count{};

    std::vector<int64_t> t_ctr;

public:
    solution_t() = default;

    ~solution_t()
    {
        for (char *line : lines)
            delete[] line;
    }

    int64_t get_out() const { return out; }

    void solve(FILE *fp)
    {
        char buf[BUFSIZE];

        while (fgets(buf, BUFSIZE, fp))
        {
            if (!line_size)
                line_size = strlen(buf) - 1;

            lines.push_back(new char[line_size + 1]);
            memcpy(lines.back(), buf, line_size);
            lines.back()[line_size] = '\0';
        }

        t_ctr = std::vector<int64_t>(line_size, 0);

        compute();
    }

    void compute()
    {
        for (size_t it = 1; it < lines.size(); it++)
        {
            compute_line(it);
        }

        char *end_line = lines.back();
        for (size_t it = 0; it < t_ctr.size(); it++)
        {
            int64_t e_ctr = get_ectr(it, end_line);
            out += e_ctr;
        }
    }

    int64_t get_ectr(size_t it, char *line)
    {
        return line[it] == '|' ? t_ctr[it] : 0;
    }

    void compute_line(size_t line_it)
    {
        char *pline = lines[line_it - 1];
        char *line = lines[line_it];

        std::vector<int64_t> t_ctr_prev = t_ctr;

        for (size_t it = 0; it <= line_size; it++)
        {
            if (pline[it] == 'S')
            {
                line[it] = '|';
                t_ctr[it] = 1;
            }
            else if (pline[it] == '|')
            {
                if (line[it] == '^')
                {
                    if (it > 0)
                    {
                        line[it - 1] = '|';
                        t_ctr[it - 1] += t_ctr_prev[it];
                    }

                    if (it < line_size)
                    {
                        line[it + 1] = '|';
                        t_ctr[it + 1] += t_ctr_prev[it];
                    }

                    t_ctr[it] = 0;
                }
                else
                {
                    line[it] = '|';
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
