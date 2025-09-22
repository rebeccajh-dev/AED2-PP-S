//
//  main.cpp
//  AED-PP2
//
//  Created by Jessica Rodrigues on 07/09/25.
//

#include <vector>
#include <strings.h>
#include <limits>
#include <list>
#include <iostream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

using Vertex = unsigned int;
using uint = unsigned int;
using namespace std;



//supostamente o tad prioridade com heap binario ANNND dijkstra

template<typename T, typename K = int>
class BinaryHeap {
private:
    vector<pair<T, K>> heap; // key, value

    static inline int parent(int i) { return (i - 1) / 2; }
    static inline int left(int i) { return 2 * i + 1; }
    static inline int right(int i) { return 2 * i + 2; }

    void sift_up(int i){
        while (i > 0 && heap[i].first < heap[parent(i)].first){
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    void sift_down(int i){
        int minIndex = (int)heap.size();
        while (true) {
            int l = left(i), r = right(i), smallest = i;
            if (l < minIndex && heap[l].first < heap[smallest].first) smallest = l;
            if (r < minIndex && heap[r].first < heap[smallest].first) smallest = r;
            if (smallest == i) break;
            swap(heap[i], heap[smallest]);
            i = smallest;
        }
    }

public:

    bool empty() const { return heap.empty(); }
    int size() const { return (int)heap.size(); }

    void push( const T& value, const K& key){
        heap.emplace_back(key, value);
        sift_up((int)heap.size() - 1);
    }

    pair<T, K> pop() {
        auto ret = heap.front();
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) sift_down(0);
        return ret;
    }
};

struct Edge {  //dijikstra
    int to, w;
};

vector<int> dijkstra(int n, int src, const vector<vector<Edge>>& adj){
    const int INF = 1e9;
    vector<int> dist(n, INF);
    dist[src] = 0;

    BinaryHeap<int, int> pq;
    pq.push(src, 0);

    while(!pq.empty()) {
        auto [u, d] = pq.pop();
        if (d != dist[u]) continue;

        for (auto &e : adj[u]) {
            if (dist[u] + e.w < dist[e.to]){
                dist[e.to] = dist[u] + e.w;
                pq.push(e.to, dist[e.to]);
            }
        }
    }
    return dist;
}

//supostatmente usar o tal do tad prioridade com heap binario
//usar o dijikstra para achar o menor caminho (codigo antigo dijkstra sem usar o binary heap)

class GraphAL {
private:
    uint num_vertices;
    uint num_edges;
    list<Vertex>* adj;

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

class Queue{
    private:
        list<pair<int, int>> data;

    public:
        void enqueue(pair<int, int> value) {
          data.push_back(value);
        }

        pair<int,int> unqueue() {
          pair<int, int> value = data.front();
          data.pop_front();
          return value;
        }

        bool empty(){
          return data.empty();
        }
    };

class Board {
private:
    GraphAL g;

    static bool onLimit(int i, int j) {
        return (i >= 0 && i < 8 && j >= 0 && j < 8);
    }

    static int toVertex(int i, int j) {
        return i * 8 + j;
    }

    static pair<int,int> toCoord(int v) {
        return {v / 8, v % 8};
    }

public:

    Board() : g(64) {
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                int v = toVertex(i, j);
                auto moves = possibleMoves(i, j);
                for (auto [ni, nj] : moves) {
                    int u = toVertex(ni, nj);
                    g.add_edge(v, u);
                }
            }
        }
    }

    GraphAL& getGraph() {return g;}


    static vector<pair<int,int>> possibleMoves(int i, int j) {
        vector<pair<int,int>> moves;
        int deslocs[8][2] = {
            {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
            {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
        };

        for (int k = 0; k < 8; k++) {
            int newI = i + deslocs[k][0];
            int newJ = j + deslocs[k][1];
            if (onLimit(newI, newJ)) {
                moves.push_back({newI, newJ});
            }
        }
        return moves;
    }


    static int coordToVertex(pair<int,int> pos) {
        return toVertex(pos.first, pos.second);
    }

    static pair<int,int> vertexToCoord(int v) {
        return toCoord(v);
    }

};

void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}


// Ele tem que buscar a partir do king/knight -> ordenar os vertices com Bubble Sort
class BFS {
private:
    vector<int> dist;
    vector<int> parent;

public:
    BFS() : dist(64, -1), parent(64, -1) {}

    vector<int> buildTree(Board& board, pair<int,int> start){
        Queue q;
        auto& g = board.getGraph();

        int startV = Board::coordToVertex(start);
        fill(dist.begin(), dist.end(), -1);
        fill(parent.begin(), parent.end(), -1);

        dist[startV] = 0;
        q.enqueue({startV, 0});

        while(!q.empty()) {
            auto [v, d] = q.unqueue();

            for (auto u : g.get_adj(v)) {
                if (dist[u] == -1) {
                    dist[u] = d + 1;
                    parent[u] = v;
                    q.enqueue({u, d + 1});
                }
            }
        }
        return parent;
    }

        vector<int> getPath(int startV, int endV){
            vector<int> path;
            for (int v = endV; v!= -1; v = parent[v]) {
                path.push_back(v);
                if (v == startV) break;
            }
            reverse(path.begin(), path.end());
            return path;
    }
};

class Knight {
private:
    pair<int,int> pos;

public:
    Knight(string position) {
        pos = chessToCoord(position);
    }

    pair<int,int> getPos() const {
        return pos;
    }

    void setPos(int i, int j) {
        pos = {i, j};
    }


    int minMovesTo(Board& board, int i, int j) {
        BFS bfs;
        return bfs.minorPath(board, pos, {i, j});
    }
};

void printVector(const vector<int>& v) {
    if (v.empty()) return;

    int firstPrint = v[0];
    int count = 0;

    while (firstPrint == v[0]) {
        cout << firstPrint << " ";
        count++;
        firstPrint = v[count];
    }
    cout << endl;
}

int main() {
    Board board;
    int testQnt;
    vector<int> moves;
    string position1, position2, position3, position4, king;

    cin >> testQnt;

    for (int i = 0; i < testQnt; i++) {
        cin >> position1 >> position2 >> position3 >> position4 >> king;
        Knight k1(position1);
        Knight k2(position2);
        Knight k3(position3);
        Knight k4(position4);

        pair<int,int> kingPosition = chessToCoord(king);

        moves.push_back(k1.minMovesTo(board, kingPosition.first, kingPosition.second) - 1);
        moves.push_back(k2.minMovesTo(board, kingPosition.first, kingPosition.second) - 1);
        moves.push_back(k3.minMovesTo(board, kingPosition.first, kingPosition.second) - 1);
        moves.push_back(k4.minMovesTo(board, kingPosition.first, kingPosition.second) - 1);


        bubbleSort(moves);

        printVector(moves);

        moves.clear();
    }

    return 0;
}
