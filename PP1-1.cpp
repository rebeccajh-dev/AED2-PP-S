//
//  main.cpp
//  AED-II-PP1
//
//  Created by Jessica Rodrigues on 23/08/25.
//

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <list>

using Vertex = unsigned int;
using uint = unsigned int;
using namespace std;

class GraphAL {
private:
    uint num_vertices;
    uint num_edges;
    list<Vertex>* adj; // ponteiro para vetor de listas

public:
    GraphAL(uint num_vertices) {
        this->num_vertices = num_vertices;
        this->num_edges = 0;
        adj = new list<Vertex>[num_vertices];
    }

    ~GraphAL() {
        delete[] adj;
        adj = nullptr;
    }
    
    uint get_num_vertices() const{
        return num_vertices;
    }
    
    uint get_num_edges() const{
        return num_edges;
    }

    void add_edge(const Vertex& u, const Vertex& v) {
        if (u >= num_vertices || v >= num_vertices || u == v) {
            throw invalid_argument("Vértice inválido");
        }
        
        adj[u].push_back(v);
        adj[v].push_back(u);
        num_edges++;
    }

    void remove_edge(Vertex u, Vertex v) {
        adj[u].remove(v);
        adj[v].remove(u);
        num_edges--;
    }

    list<Vertex> get_adj(const Vertex& u) const {
        if (u >= num_vertices) {
            throw invalid_argument("Vértice inválido");
        }
        return adj[u];
    }

    void print_adjacency_list(const GraphAL& g) const {
        cout << "num_vertices: " << g.get_num_vertices() << endl;
        cout << "num_edges: " << g.get_num_edges() << endl;

        for (uint u = 0; u < g.get_num_vertices(); u++) {
            const list<Vertex>& l = g.get_adj(u);
            cout << u << ": ";
            for (auto v : l) {
                cout << v << ", ";
            }
            cout << endl;
        }
    }
};

int main() {
    uint n = 0;
    uint m = 0;
    
    cin >> n;
    cin >> m;
    
    GraphAL graph(n);
    
    uint u = 0;
    uint v = 0;
    
    for (uint i = 0; i < m; i++) {
        cin >> u >> v;
        graph.add_edge(u, v);
    }

    
    
    graph.print_adjacency_list(graph);
    
}
