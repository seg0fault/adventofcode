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
  static bool repeat_cmp(const std::string &string, std::string_view pattern)
  {
    size_t pos = 0;
    for (char c : string)
    {
      if (pattern[pos] != c)
        return false;
      pos++;
      if (pos == pattern.size())
        pos = 0;
    }
    return true;
  }

  static bool is_invalid(uint64_t value)
  {
    std::string valstr = std::to_string(value);
    size_t size = valstr.size();

    for (size_t w = 1; w <= size / 2; w++)
    {
      if (size % w != 0)
        continue;
      size_t count = size / w;
      if (count < 2)
        continue;

      std::string_view pattern(valstr.begin(), valstr.begin() + w);
      if (repeat_cmp(valstr, pattern))
        return true;
    }

    return false;
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
