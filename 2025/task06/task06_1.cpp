#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

class solution_t
{
    int64_t out{};

    std::vector<int64_t> nums;
    std::vector<char> ops;

    size_t line_size{};
    size_t line_count{};

public:
    int64_t get_out() const { return out; }

    void solve(FILE *fp)
    {
        int64_t num{};

        while (fscanf(fp, "%ld", &num) == 1)
        {
            nums.push_back(num);
        }

        char op{};
        ops.push_back('+');
        while (fscanf(fp, "%c", &op) == 1)
        {
            if (is_operation(op))
                ops.push_back(op);
        }

        if (nums.size() % ops.size())
        {
            printf("Input mismatch\n");
            return;
        }

        line_count = nums.size() / ops.size();
        line_size = ops.size();

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
            printf("Invalid operation: %c\n", op);
            break;
        }
        return;
    }

    void compute()
    {
        for (size_t it = 0; it < line_size; it++)
        {
            out += compute_index(it);
        }
    }

    int64_t compute_index(size_t it)
    {
        int64_t res = get_val(0, it);
        char op = ops[it];
        for (size_t line = 1; line < line_count; line++)
        {
            compute_op(res, get_val(line, it), op);
        }
        return res;
    }

    int64_t get_val(size_t line, size_t it)
    {
        return nums[line * line_size + it];
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
