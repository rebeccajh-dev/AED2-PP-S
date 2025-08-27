#include <iostream>
#include <list>
#include <stdexcept>
#include <utility>

using uint = unsigned int;
using Vertex = uint;
using Weight = float;
using VertexWeightPair = std::pair<Vertex, Weight>;

class WeightedGraphAL {
private:
    uint num_vertices;
    uint num_edges;
    std::list<VertexWeightPair>* adj;

public:
    WeightedGraphAL(uint num_vertices) {
        this->num_vertices = num_vertices;
        this->num_edges = 0;
        this->adj = new std::list<VertexWeightPair>[num_vertices];
    }

    ~WeightedGraphAL() {
        delete[] adj;
        adj = nullptr;
    }

    // Getters
    uint get_num_vertices() const { return num_vertices; }
    uint get_num_edges() const { return num_edges; }

    void add_edge(const Vertex& u, const Vertex& v, const Weight& w) {
        if (u >= num_vertices || v >= num_vertices || u == v) {
            throw std::invalid_argument("Invalid vertices");
        }
        adj[u].push_back(std::make_pair(v, w));
        adj[v].push_back(std::make_pair(u, w));
        num_edges++;
    }

    const std::list<VertexWeightPair>& get_adj(const Vertex& u) const {
        if (u >= num_vertices) {
            throw std::invalid_argument("Invalid vertex");
        }
        return adj[u];
    }

    void print_adjacency_list() const {
        std::cout << "num_vertices: " << num_vertices << "\n";
        std::cout << "num_edges: " << num_edges << "\n";
        for (uint u = 0; u < num_vertices; u++) {
            std::cout << u << ": ";
            for (auto& [v, w] : adj[u]) {
                std::cout << "(" << v << ", " << w << "), ";
            }
            std::cout << "\n";
        }
    }
};
int main() {
    uint n = 0;
    uint m = 0;

    std::cin >> n >> m;

    WeightedGraphAL graph(n);

    uint u = 0;
    uint v = 0;
    Weight w = 0.0;

    for (uint i = 0; i < m; i++) {
        std::cin >> u >> v >> w;
        graph.add_edge(u, v, w);
    }

    graph.print_adjacency_list();

    return 0;
}
