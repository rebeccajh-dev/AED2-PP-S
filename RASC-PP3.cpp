//
//  main.cpp
//  AED-PP2
//
//  Created by Jessica Rodrigues on 07/09/25.
//

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <utility>
#include <limits>
#include <unordered_set>
#include <algorithm>
#include <climits>

using namespace std;
using Vertex = unsigned int;
using uint = unsigned int;


class GraphAL {
private:
    uint num_vertices;
    uint num_edges;
    vector<list<pair<int,int>>> adj;

public:
    GraphAL(uint num_vertices) {
        this->num_vertices = num_vertices;
        this->num_edges = 0;
        adj.resize(num_vertices);
    }

    ~GraphAL() { //como criei um vector, o cleanup é feito automaticamente
    }

    uint get_num_vertices() const {
        return num_vertices;
    }

    uint get_num_edges() const {
        return num_edges;
    }

    void add_edge(Vertex u, Vertex v, int w) {
        if (u >= num_vertices || v >= num_vertices || u == v) {
            throw invalid_argument("Vértice inválido");
        }
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); // se for não-direcionado
        num_edges++;
    }

    void remove_edge(Vertex u, Vertex v) {
        adj[u].remove_if([&](const pair<int,int>& e) { return e.first == v; });
        adj[v].remove_if([&](const pair<int,int> &e) { return e.first == u; });
        num_edges--;
    }

    const list<pair<int,int>>& neighbors(Vertex u) const {
        if (u >= num_vertices) {
            throw invalid_argument("Vértice inválido");
        }
        return adj[u];
    }


    uint positionToVertex(const string& pos, int boardSize){
        char alpha = pos[0];
        int i = alpha - 'a';
        int j = stoi(pos.substr(1)) - 1;

        return i * boardSize + j;
    }

    string vertexToPosition(uint v, int boardSize){
        int i = v / boardSize;
        int j = v % boardSize;
        char alpha = 'a' + i;
        string beta = to_string(j+1);
        return string(1, alpha) + beta;
    }

    void generate_Graph(int boardSize){
        int moves[8][2] = {
        {1, 2}, {2, 1}, {2, -1}, {1, -2},
        {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}
        };
        for (int i = 0; i < boardSize; i++){
            for (int j = 0; j < boardSize; j++){
                int u = i*boardSize + j;

                for (auto &m : moves) {
                    int ni = i + m[0];
                    int nj = j + m[1];

                    if (ni >= 0 && ni < boardSize && nj >= 0 && nj < boardSize){
                        int v = ni*boardSize + nj;

                        char alpha_u = 'a' + i;
                        char alpha_v = 'a' + ni;
                        int beta_u = j + 1;
                        int beta_v = nj + 1;

                        int weight = ((int)alpha_u * beta_u + (int)alpha_v * beta_v) % 19;
                        add_edge(u, v, weight);
                        
                    }
                }
            }
        }
    }

    void print_adjacency_list() const {
        cout << "num_vertices: " << num_vertices << endl;
        cout << "num_edges: " << num_edges << endl;

        for (uint u = 0; u < num_vertices; u++) {
            cout << u << ": ";
            for (auto e : adj[u]) {
                cout << "(" << e.first << ", w=" << e.second << ") ";
            }
            cout << endl;
        }
    }
};


//supostamente o tad prioridade com heap binario ANNND dijkstra

template<typename T, typename K = int>
class BinaryHeap {
private:
    vector<pair<T, K>> heap; // key = distância, value = vértice
    vector<int> pos;

    static inline int parent(int i) { return (i - 1) / 2; }
    static inline int left(int i) { return 2 * i + 1; }
    static inline int right(int i) { return 2 * i + 2; }

    void swapNodes(int i, int j) {
        // Atualiza as posições no vetor pos
        int tempPos = pos[heap[i].second];
        pos[heap[i].second] = pos[heap[j].second];
        pos[heap[j].second] = tempPos;

        // Troca os elementos do heap manualmente
        pair<T, K> temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
    }



    void min_heapify(int i) {
        int l = left(i), r = right(i);
        int smallest;

        if (l < heap.size() && heap[l].first < heap[i].first)
            smallest = l;
        else
            smallest = i;

        if (r < heap.size() && heap[r].first < heap[smallest].first)
            smallest = r;

        if (smallest != i) {
            swapNodes(i, smallest);
            min_heapify(smallest);
        }
    }



public:
    BinaryHeap(int maxVertices) {
        pos.resize(maxVertices, -1);
    }

    bool empty() const { return heap.empty(); } //feito

    void push(const pair<T,K>& p) {
        heap.push_back(p);
        int i = heap.size() - 1;
        pos[p.second] = i;
        while (i != 0 && heap[parent(i)].first > heap[i].first) {
            swapNodes(i, parent(i));
            i = parent(i);
        }
    }

    pair<T,K> extractMin() {
        if (heap.empty()) throw runtime_error("Heap vazio");
        pair<T,K> root = heap[0];
        swapNodes(0, heap.size() -1);
        heap.pop_back();
        pos[root.second] = -1;
        if (!heap.empty())
            min_heapify(0);
        return root;
    }

    // decreaseKey precisa de um mapeamento de índice para posição do heap
    // para simplicidade, aqui podemos reconstruir o heap
    void decreaseKey(K value, T newKey) { //feito
        int i = pos[value];
        if (i == -1) return;

        if (newKey >= heap[i].first) return;
        heap[i].first = newKey;

        while (i > 0 && heap[parent(i)].first > heap[i].first){
            swapNodes(i, parent(i));
            i = parent(i);
        }



class Dijkstra {
private:
    GraphAL &g;
    vector<int> dist;
    vector<int> parent;

public:
    Dijkstra(GraphAL &graph) : g(graph) {
        dist.resize(g.get_num_vertices(), numeric_limits<int>::max());
        parent.resize(g.get_num_vertices(), -1);
    }

    void shortestPath(int s) {
        BinaryHeap<int> Q(g.get_num_vertices());
        for (int i = 0; i < g.get_num_vertices(); i++) {
            Q.push({dist[i], i});
        }
        dist[s] = 0;
        Q.decreaseKey(s, 0);

        while (!Q.empty()) {
            auto [d, u] = Q.extractMin();

            // Itera sobre a lista de adjacência do vértice u
            for (const pair<int,int>& edge : g.neighbors(u)) {
                int v = edge.first;   // vértice vizinho
                int w = edge.second;  // peso da aresta

                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    Q.decreaseKey(v, dist[v]);
                }
            }
        }

    }

    vector<int> getPath(int v) const {
        vector<int> path;
        if (dist[v] == numeric_limits<int>::max()) return path;
        for (int u = v; u != -1; u = parent[u])
            path.push_back(u);
        reverse(path.begin(), path.end());
        return path;
    }

    int getDistance(int v) const { return dist[v]; }
};
    
//supostatmente usar o tal do tad prioridade com heap binario
//usar o dijikstra para achar o menor caminho (codigo antigo dijkstra sem usar o binary heap)
//oq falta de classe: army(puvlic->(Army, getColor, getPosition, setPosition, isEnemy, allyWith, moveOneStep), storm(public -> storm(), getPosition()), simulation(simulation(boardSize), addArmy, addStorm, setCastle, run, resolveEncounters, checkEnd) e o main

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
