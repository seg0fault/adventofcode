#include <cstdio>
#include <cstdint>
#include <ctime>
#include <string>
#include <string_view>

class solution_t
{
    uint64_t sum{};

public:
    uint64_t get_sum() const { return sum; }

    void solve(FILE *fp)
    {
        uint64_t left, right = 0;
        while (fscanf(fp, "%lu-%lu,", &left, &right) == 2)
        {
            // printf ("RANGE: %lu-%lu\n", left, right);
            process_range(left, right);
        }
    }

private:
    static bool is_invalid(uint64_t value)
    {
        std::string valstr = std::to_string(value);
        size_t size = valstr.size();
        if (size & 1)
            return false;

        size_t size_half = size / 2;
        std::string_view left(valstr.begin(), valstr.begin() + size_half);
        std::string_view right(valstr.begin() + size_half, valstr.end());

        // printf ("VALUE: %s-%s\n", left.c_str (), right.c_str ());

        return left == right;
    }

    void process_range(uint64_t left, uint64_t right)
    {
        for (uint64_t value = left; value <= right; value++)
        {
            if (is_invalid(value))
            {
                sum += value;
                // printf("VALUE: %lu\n", value);
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

    printf("RESULT:  %lu\n", solution.get_sum());
    printf("ELAPSED: %lf\n", t / CLOCKS_PER_SEC);

    fclose(fp);
    return 0;
}
