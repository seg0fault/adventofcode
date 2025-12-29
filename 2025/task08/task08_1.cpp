#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>

#include <algorithm>
#include <vector>
#include <array>

struct cache_t
{
    struct cache_record_t
    {
        cache_record_t(size_t i, size_t j, int64_t dist)
            : box_i(i), box_j(j), distance(dist)
        {
        }

        size_t box_i{};
        size_t box_j{};
        int64_t distance{};

        bool operator<(const cache_record_t &rhs) { return distance < rhs.distance; }
    };

    static constexpr size_t cache_size = 1000;
    size_t current_pos{};
    size_t current_size{};

    std::array<cache_record_t *, cache_size> cache;

    cache_t()
    {
        cache.fill(nullptr);
    }

    ~cache_t()
    {
        reset();
    }

    bool need_recompute_cache() { return cache_size <= current_pos; }

    int64_t get_next_recompute_min_distance()
    {
        if (current_size == 0)
            return -1;
        return cache[current_size - 1]->distance;
    }

    void reset()
    {
        for (size_t it = 0; it < current_size; it++)
        {
            delete cache[it];
        }

        cache.fill(nullptr);
        current_pos = 0;
        current_size = 0;
    }

    void get(size_t &i, size_t &j, int64_t &distance)
    {
        cache_record_t *record = cache[current_pos];
        i = record->box_i;
        j = record->box_j;
        distance = record->distance;

        current_pos++;
    }

    void insert(size_t i, size_t j, int64_t distance)
    {
        cache_record_t *last_record{};
        if (current_size > 0)
            last_record = cache[current_size - 1];

        size_t insert_pos{};
        if (last_record && last_record->distance < distance)
        {
            insert_pos = current_size;
        }
        else
        {
            for (insert_pos = 0; insert_pos < cache_size; insert_pos++)
            {
                if (!cache[insert_pos] || distance < cache[insert_pos]->distance)
                    break;
            }
        }

        if (insert_pos == cache_size)
            return;

        cache_record_t *new_record = new cache_record_t(i, j, distance);
        
        if (!cache[insert_pos])
        {
            current_size++;
            cache[insert_pos] = new_record;
            return;
        }

        if (last_record)
        {
            delete last_record;
        }

        memmove (cache.data () + insert_pos + 1, cache.data () + insert_pos, (current_size - insert_pos - 1) * sizeof (cache_record_t*));
        cache[insert_pos] = new_record;
    }
};

struct solution_t
{
    struct box_t
    {
        box_t(int64_t x_, int64_t y_, int64_t z_) : x(x_), y(y_), z(z_) {}
        int64_t x{};
        int64_t y{};
        int64_t z{};
    };

    int64_t out{};

    int connect_count = 0;

    cache_t cache;

    std::vector<box_t> points;
    std::vector<int> circut_num;   // point_num -> curcut_id
    std::vector<int> circut_count; // curcut_id -> curcut_size

    int last_circut_id = 0;
    inline int get_new_circut_id() { return last_circut_id++; }

    int64_t last_min_distance = -1;

public:
    int64_t get_out() const { return out; }

    void solve(FILE *fp)
    {
        int64_t a, b, c = 0;
        while (fscanf(fp, "%ld,%ld,%ld", &a, &b, &c) == 3)
        {
            points.emplace_back(a, b, c);
        }

        circut_num = std::vector<int>(points.size(), -1);
        circut_count = std::vector<int>(points.size(), 1);

        recompute_cache();
        while (connect_count < 1000)
        {
            connect_one();

            if (cache.need_recompute_cache())
            {
                recompute_cache();
            }
        }

        compute_out_task1();
    }

private:
    void connect_one()
    {
        size_t min_i{}, min_j{};
        int64_t min_distance{};

        cache.get(min_i, min_j, min_distance);

        connect_count++;

        connect_boxes(min_i, min_j);
    }

    static inline int64_t get_distance(const box_t &lhs, const box_t &rhs)
    {
        int64_t xx = lhs.x - rhs.x;
        int64_t yy = lhs.y - rhs.y;
        int64_t zz = lhs.z - rhs.z;

        return xx * xx + yy * yy + zz * zz;
    }

    void recompute_cache()
    {
        cache.reset();
        for (size_t i = 0; i < points.size(); i++)
        {
            const box_t &p1 = points[i];

            for (size_t j = i + 1; j < points.size(); j++)
            {
                const box_t &p2 = points[j];
                int64_t distance = get_distance(p1, p2);

                if (distance > last_min_distance)
                    cache.insert(i, j, distance);
            }
        }

        last_min_distance = cache.get_next_recompute_min_distance();
    }

    void connect_boxes(size_t min_i, size_t min_j)
    {
        int circut_id = -1;
        if (circut_num[min_i] == -1 && circut_num[min_j] == -1)
        {
            circut_id = get_new_circut_id();
            circut_num[min_j] = circut_num[min_i] = circut_id;
        }
        else if (circut_num[min_i] == circut_num[min_j])
        {
            circut_id = circut_num[min_i];
            return;
        }
        else if (circut_num[min_i] != -1 && circut_num[min_j] != -1)
        {
            int consumed_circut = circut_num[min_j];
            circut_num[min_j] = circut_id = circut_num[min_i];
            circut_count[consumed_circut] = 0;

            for (size_t it = 0; it < circut_num.size(); it++)
            {
                int &num = circut_num[it];
                if (num == consumed_circut)
                {
                    circut_count[circut_id] += 1;
                    num = circut_id;
                }
            }

            // printf ("CONSUME %d by %d\n", consumed_circut, c_num);
        }
        else if (circut_num[min_i] != -1)
        {
            circut_num[min_j] = circut_id = circut_num[min_i];
        }
        else if (circut_num[min_j] != -1)
        {
            circut_num[min_i] = circut_id = circut_num[min_j];
        }
        else
        {
            printf("Circut connect error\n");
            return;
        }

        circut_count[circut_id] += 1;
    }

    void compute_out_task1()
    {
        std::sort(circut_count.begin(), circut_count.end());

        int max1 = *circut_count.rbegin();
        int max2 = *(circut_count.rbegin() + 1);
        int max3 = *(circut_count.rbegin() + 2);

        out = max1 * max2 * max3;
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
