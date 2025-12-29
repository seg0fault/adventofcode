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

    common
};

struct node_t
{
    node_type_t type = node_type_t::common;
    int64_t cache{};
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

        compute();
    }

private:
    void compute()
    {
        out = compute_end_r(start);
    }

    static size_t compute_end_r(node_t *root)
    {
        if (root->type == node_type_t::end)
            return 1;

        if (root->cache)
            return root->cache;

        size_t out{};
        for (node_t *child : root->children)
            out += compute_end_r(child);

        root->cache = out;

        return out;
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

        if (node_name == "you")
        {
            ptr->type = node_type_t::start;
        }
        else if (node_name == "out")
        {
            ptr->type = node_type_t::end;
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
            parent->children.emplace_back(get_or_create_node(child));
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
