#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <vector>

#define BUFSIZE 5000

class solution_t
{
    int64_t out{};

    std::vector<char *> lines;
    std::vector<char> ops;

    size_t line_size{};

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

        char *ops_str = lines.back();

        for (size_t it = 0; it < line_size; it++)
        {
            char c = ops_str[it];
            if (is_operation(c))
                ops.push_back(c);
        }

        lines.pop_back();
        delete[] ops_str;

        compute();
    }

private:
    static inline char is_operation(char c)
    {
        return c == '*' || c == '+';
    }

    static inline void compute_op(int64_t &lhs, int64_t rhs, char op)
    {
        switch (op)
        {
        case '+':
            lhs += rhs;
            break;
        case '*':
            lhs *= rhs;
            break;
        default:
            printf("Op mismatch\n");
            break;
        }
        return;
    }

    static inline int to_digit(char c)
    {
        return c - '0';
    }

    static inline bool is_digit(char c)
    {
        return c >= '0' && c <= '9';
    }

    void compute()
    {
        int64_t res = -1;
        size_t it_op = 0;

        for (size_t it = 0; it <= line_size; it++)
        {
            int64_t val = get_val(it);
            if (val >= 0)
            {
                if (res < 0)
                    res = val;
                else
                    compute_op(res, val, ops[it_op]);
            }
            else if (res >= 0)
            {
                out += res;
                it_op++;
                res = -1;
            }
            else
            {
                printf("Parsing mismatch at %lu\n", it);
            }
        }
    }

    int64_t get_val(size_t it)
    {
        bool found_digit = false;
        int64_t out = 0;
        int64_t pow_10 = 1;
        for (auto rit = lines.rbegin(); rit != lines.rend(); ++rit)
        {
            char *line = *rit;
            char c = line[it];
            if (is_digit(c))
            {
                found_digit = true;
                out += to_digit(c) * pow_10;
                pow_10 *= 10;
            }
        }

        if (!found_digit)
            return -1;

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
