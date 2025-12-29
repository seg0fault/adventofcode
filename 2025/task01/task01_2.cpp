#include <cstdio>
#include <cstdlib>
#include <ctime>

class solution_t
{
  static constexpr int start_position = 50;
  static constexpr int max_position = 100;

  int pos = start_position;
  int64_t out = 0;

public:
  int64_t get_out() const { return out; }

  void solve(FILE *fp)
  {
    char act{};
    int rot{};
    while (fscanf(fp, "%c%d\n", &act, &rot) == 2)
    {
      int e_rot = rot % max_position;
      int e_out = rot / max_position;

      if (e_rot > 0)
        select(act, e_rot);

      out += e_out;
      // printf ("act: %c: rot: %d; erot: %d; pos: %d; out %d\n", act, rot, e_rot, pos, out);
    }
  }

private:
  void left(int rot)
  {
    bool no_add = pos == 0;
    pos -= rot;
    if (pos < 0)
    {
      if (!no_add)
        out++;
      pos += max_position;
    }
    else if (pos == 0)
    {
      out++;
    }
  }

  void right(int rot)
  {
    pos += rot;
    if (pos >= max_position)
    {
      out++;
      pos -= max_position;
    }
  }

  void select(char act, int rot)
  {
    switch (act)
    {
    case 'L':
      left(rot);
      return;
    case 'R':
      right(rot);
      return;
    default:
      break;
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

  fclose (fp);
  return 0;
}
