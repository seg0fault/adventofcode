#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <map>

#define BUFLEN 1234

enum class node_type_t
{
    start,
    end,
    fft,
    dac,
    out,

    common
};

struct node_t
{
    node_type_t type = node_type_t::common;

    bool found_fft{};
    bool found_dac{};

    int64_t cache{};

    std::vector<node_t *> parents;
    std::vector<node_t *> children;
};

class solution_t
{
    std::map<std::string, node_t *> known_nodes;

    node_t *start{};

    int64_t out{};

public:
    solution_t() = default;

    ~solution_t()
    {
        for (auto it = known_nodes.begin(); it != known_nodes.end(); ++it)
        {
            delete it->second;
        }
    }

    int64_t get_out() const { return out; }

    void solve(FILE *fp)
    {
        char buf[BUFLEN];

        std::string node_name;
        std::vector<std::string> children_names;
        while (fgets(buf, BUFLEN, fp))
        {
            parse_string(buf, node_name, children_names);
            add_node(node_name, children_names);
        }

        if (!start || !known_nodes.count("out"))
        {
            printf("Invalid input: no start or end node\n");
            return;            
        }

        if (!known_nodes.count("fft") || !known_nodes.count("dac"))
        {
            printf("Invalid input: no fft or dac nodes\n");
            return;
        }

        compute();
    }

private:
    void reset_cache ()
    {
        for (auto it = known_nodes.begin(); it != known_nodes.end(); ++it)
        {
            it->second->cache = 0;
        }
    }

    void compute()
    {
        size_t x1 = 0, x2 = 0, x3 = 0;

        preprocess_fft(known_nodes["fft"]);
        preprocess_dac(known_nodes["dac"]);

        do
        {
            reset_cache ();
            x2 = compute_fft_r(known_nodes["dac"]);
            if (!x2)
                break;

            reset_cache ();
            x1 = compute_dac_r(known_nodes["svr"]);
            if (!x1)
                break;

            reset_cache ();
            x3 = compute_end_r(known_nodes["fft"]);
            if (!x3)
                break;

        } while (false);

        size_t y1 = 0, y2 = 0, y3 = 0;
        do
        {
            reset_cache ();
            y3 = compute_end_r(known_nodes["dac"]);
            if (!y3)
                break;

            reset_cache ();
            y2 = compute_dac_r(known_nodes["fft"]);
            if (!y2)
                break;

            reset_cache ();
            y1 = compute_fft_r(known_nodes["svr"]);
            if (!y1)
                break;

        } while (false);

        out = (x1 * x2 * x3) + (y1 * y2 * y3);
    }

    void parse_string(char *buf, std::string &node_name, std::vector<std::string> &children_names)
    {
        size_t child_start = 0;
        children_names.clear();

        for (size_t it = 0; buf[it]; it++)
        {
            if (buf[it] == ':')
            {
                buf[it] = '\0';
                node_name = std::string(buf);

                it++;
                child_start = it + 1;
                continue;
            }

            if (buf[it] == ' ' || buf[it] == '\n')
            {
                buf[it] = '\0';
                children_names.emplace_back(buf + child_start);
                child_start = it + 1;
            }
        }
    }

    node_t *get_or_create_node(const std::string &node_name)
    {
        auto it = known_nodes.find(node_name);
        if (it != known_nodes.end())
            return it->second;

        node_t *ptr = new node_t();

        if (node_name == "svr")
        {
            ptr->type = node_type_t::start;
        }
        else if (node_name == "out")
        {
            ptr->type = node_type_t::end;
        }
        else if (node_name == "fft")
        {
            ptr->type = node_type_t::fft;
        }
        else if (node_name == "dac")
        {
            ptr->type = node_type_t::dac;
        }

        known_nodes[node_name] = ptr;
        return ptr;
    }

    void add_node(std::string &node_name, std::vector<std::string> &children_names)
    {
        node_t *parent = get_or_create_node(node_name);

        if (parent->type == node_type_t::start)
            start = parent;

        for (const auto &child : children_names)
        {
            node_t *child_node = get_or_create_node(child);

            child_node->parents.emplace_back(parent);
            parent->children.emplace_back(child_node);
        }
    }

    static void preprocess_fft(node_t *root)
    {
        root->found_fft = true;

        for (node_t *parent : root->parents)
        {
            if (!parent->found_fft)
                preprocess_fft(parent);
        }
    }

    static void preprocess_dac(node_t *root)
    {
        root->found_dac = true;

        for (node_t *parent : root->parents)
        {
            if (!parent->found_dac)
                preprocess_dac(parent);
        }
    }

    static size_t compute_fft_r(node_t *root)
    {
        if (root->cache)
            return root->cache;

        if (root->type == node_type_t::fft)
            return 1;

        size_t out{};
        for (node_t *child : root->children)
            out += child->found_fft ? compute_fft_r(child) : 0;

        root->cache = out;

        return out;
    }

    static size_t compute_dac_r(node_t *root)
    {
        if (root->cache)
            return root->cache;

        if (root->type == node_type_t::dac)
            return 1;

        size_t out{};
        for (node_t *child : root->children)
            out += child->found_dac ? compute_dac_r(child) : 0;

        root->cache = out;

        return out;
    }

    static size_t compute_end_r(node_t *root)
    {
        if (root->cache)
            return root->cache;

        if (root->type == node_type_t::end)
            return 1;

        size_t out{};
        for (node_t *child : root->children)
            out += compute_end_r(child);

        root->cache = out;

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
