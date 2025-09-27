#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <utility>
#include <limits>
#include <algorithm>
#include <climits>
#include <queue>
#include <stdexcept>

using namespace std;
using Vertex = unsigned int;
using uint = unsigned int;

// ---------------- Colors  ----------------

enum Color {
    Amarelo,
    Vermelho,
    Branco,
    Azul,
    Verde,
    Laranja,
    Rosa,
    Desconhecido
};

std::string colorsToString(Color s) {
    switch (s) {
        case Color::Amarelo:     return "amarelo";
        case Color::Vermelho:    return "vermelho";
        case Color::Branco:      return "branco";
        case Color::Azul:        return "azul";
        case Color::Verde:       return "verde";
        case Color::Laranja:     return "laranja";
        case Color::Rosa:        return "rosa";
        default:                 return "Desconhecido";
    }
}

Color stringToColor(const std::string& s) {
    if (s == "amarelo")   return Color::Amarelo;
    if (s == "vermelho")  return Color::Vermelho;
    if (s == "branco")    return Color::Branco;
    if (s == "azul")      return Color::Azul;
    if (s == "verde")     return Color::Verde;
    if (s == "laranja")   return Color::Laranja;
    if (s == "rosa")      return Color::Rosa;
    return Color::Desconhecido;
}

// ---------------- GraphAL ----------------
class GraphAL {
private:
    uint num_vertices;
    uint num_edges;
    vector<list<pair<int,int>>> adj;

public:
    GraphAL(uint n) : num_vertices(n), num_edges(0), adj(n) {}

    uint get_num_vertices() const { return num_vertices; }
    uint get_num_edges() const { return num_edges; }

    void add_edge(Vertex u, Vertex v, int w) {
        if (u >= num_vertices || v >= num_vertices) {
            throw invalid_argument("Vertice invalido");
        }
        if (u == v) return;


        for (auto &e : adj[u]) if (e.first == (int)v && e.second == w) return;

        adj[u].push_back({(int)v, w});
        adj[v].push_back({(int)u, w});
        num_edges++;
    }


    void add_edge(Vertex u, Vertex v) { add_edge(u, v, 1); }

    void remove_edge(Vertex u, Vertex v) {
        if (u >= num_vertices || v >= num_vertices) return;
        adj[u].remove_if([&](const pair<int,int>& e) { return e.first == (int)v; });
        adj[v].remove_if([&](const pair<int,int>& e) { return e.first == (int)u; });
        if (num_edges > 0) num_edges--;
    }

    const list<pair<int,int>>& neighbors(Vertex u) const {
        if (u >= num_vertices) {
            throw invalid_argument("Vertice invalido");
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

    void generate_Graph(int boardSize) {
        int moves[8][2] = {
            {1, 2}, {2, 1}, {2, -1}, {1, -2},
            {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}
        };
        
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                int u = i * boardSize + j;
                
                char alpha_u = 'a' + i;
                int beta_u = j + 1;
                int ascii_alpha_u = (int)alpha_u;
                
                for (auto &m : moves) {
                    int ni = i + m[0];
                    int nj = j + m[1];
                    
                    if (ni >= 0 && ni < boardSize && nj >= 0 && nj < boardSize) {
                        int v = ni * boardSize + nj;
                        
                        char alpha_v = 'a' + ni;
                        int beta_v = nj + 1;
                        int ascii_alpha_v = (int)alpha_v;
                        
                        if (u < v) {
                            int weight = (ascii_alpha_u * beta_u + ascii_alpha_v * beta_v) % 19;
                            add_edge(u, v, weight);
                        }

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
    
    int getEdgeWeight(int u, int v) const {
        if (u < 0 || v < 0 || u >= (int)num_vertices || v >= (int)num_vertices) return -1;
        for (const auto &e : adj[u]) {
            if (e.first == v) return e.second;
        }
        return -1;
    }

};

// ---------------- BinaryHeap (min-heap) ----------------
template<typename KeyT, typename ValT = int>
class BinaryHeap {
private:
    vector<pair<KeyT, ValT>> heap;
    vector<int> pos;

    static inline int parent(int i) { return (i - 1) / 2; }
    static inline int left(int i) { return 2 * i + 1; }
    static inline int right(int i) { return 2 * i + 2; }

    void swapNodes(int i, int j) {
        auto temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;

        pos[(int)heap[i].second] = i;
        pos[(int)heap[j].second] = j;
    }

    void min_heapify(int i) {
        int l = left(i), r = right(i);
        int smallest = i;

        if (l < (int)heap.size() && heap[l].first < heap[smallest].first)
            smallest = l;
        if (r < (int)heap.size() && heap[r].first < heap[smallest].first)
            smallest = r;

        if (smallest != i) {
            swapNodes(i, smallest);
            min_heapify(smallest);
        }
    }

public:
    BinaryHeap(int maxVertices = 0) {
        pos.assign(maxVertices, -1);
    }

    bool empty() const { return heap.empty(); }

    void push(const pair<KeyT, ValT>& p) {
        heap.push_back(p);
        int i = (int)heap.size() - 1;
        if ((int)pos.size() <= (int)p.second) pos.resize((int)p.second + 1, -1);
        pos[(int)p.second] = i;
        while (i != 0 && heap[parent(i)].first > heap[i].first) {
            swapNodes(i, parent(i));
            i = parent(i);
        }
    }

    pair<KeyT, ValT> extractMin() {
        if (heap.empty()) throw runtime_error("Heap vazio");
        pair<KeyT, ValT> root = heap[0];
        if (heap.size() == 1) {
            heap.pop_back();
            pos[(int)root.second] = -1;
            return root;
        }
        swapNodes(0, (int)heap.size() - 1);
        heap.pop_back();
        pos[(int)root.second] = -1;
        min_heapify(0);
        return root;
    }

    void decreaseKey(ValT value, KeyT newKey) {
        if ((int)value < 0 || (int)value >= (int)pos.size()) return;
        int i = pos[(int)value];
        if (i == -1) return;

        if (newKey >= heap[i].first) return;
        heap[i].first = newKey;

        while (i > 0 && heap[parent(i)].first > heap[i].first){
            swapNodes(i, parent(i));
            i = parent(i);
        }
    }
};

// ---------------- Dijkstra (usa BinaryHeap) ----------------
class Dijkstra {
private:
    GraphAL &g;
    vector<int> dist;
    vector<int> parent;

public:
    Dijkstra(GraphAL &graph) : g(graph) {
        dist.assign(g.get_num_vertices(), numeric_limits<int>::max());
        parent.assign(g.get_num_vertices(), -1);
    }

    void shortestPath(int s) {
        int n = (int)g.get_num_vertices();
        dist.assign(n, numeric_limits<int>::max());
        parent.assign(n, -1);

        BinaryHeap<int,int> Q(n);
        for (int i = 0; i < n; i++) {
            Q.push({dist[i], i});
        }

        dist[s] = 0;
        Q.decreaseKey(s, 0);

        while (!Q.empty()) {
            auto [d, u] = Q.extractMin();

            if (d == numeric_limits<int>::max()) break; // remaining vertices inacessiveis

            for (const pair<int,int>& edge : g.neighbors(u)) {
                int v = edge.first;
                int w = edge.second;

                if (dist[u] != numeric_limits<int>::max() && dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    Q.decreaseKey(v, dist[v]);
                }
            }
        }
    }

    vector<int> getPath(int v) const {
    vector<int> path;
    if (v < 0 || v >= (int)dist.size()) return path;
    if (dist[v] == numeric_limits<int>::max()) return path;

    for (int u = v; u != -1; u = parent[u])
        path.push_back(u);


    for (int i = 0, j = (int)path.size() - 1; i < j; i++, j--) {
        int temp = path[i];
        path[i] = path[j];
        path[j] = temp;
    }

    return path;
}


    int getDistance(int v) const { return dist[v]; }
};

// ---------------- Board ----------------
class Board {
private:
    int n;
    GraphAL g;
    
    pair<int,int> castle;
    vector<pair<int,int>> storms;

    bool onLimit(int i, int j) const {
        return (i >= 0 && i < n && j >= 0 && j < n);
    }

    int toVertex(int i, int j) const {
        return i * n + j;
    }

    pair<int,int> toCoord(int v) const {
        return {v / n, v % n};
    }

public:
    Board(int n_) : n(n_), g(n_ * n_) {}

    GraphAL& getGraph() { return g; }

    vector<pair<int,int>> possibleMoves(int i, int j) const {
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

    int coordToVertex(pair<int,int> pos) const {
        return toVertex(pos.first, pos.second);
    }

    pair<int,int> vertexToCoord(int v) const {
        return toCoord(v);
    }

    void setCastle(pair<int,int> c) {
        castle = c;
    }

    pair<int,int> getCastle() const {
        return castle;
    }

    void addStorm(pair<int,int> s) {
        storms.push_back(s);
    }

    const vector<pair<int,int>>& getStorms() const {
        return storms;
    }
    
    void removeStorm(pair<int,int> s) {
    for (auto it = storms.begin(); it != storms.end(); ) {
        if (*it == s) {
            it = storms.erase(it);
        } else {
            ++it;
        }
    }
}

    
    string coordToPosition(pair<int,int> p) const {
        char file = 'a' + p.first;
        return string(1, file) + to_string(p.second + 1);
    }

    string vertexToPosition(int v) const {
        auto c = vertexToCoord(v);
        return coordToPosition(c);
    }

    pair<int,int> position(const string& s) const {
        if (s.size() < 2) return {-1, -1};

        char file = s[0];
        int i = file - 'a';

        int rank = 0;
        for (size_t k = 1; k < s.size(); k++) {
            char c = s[k];
            rank = rank * 10 + (c - '0');
        }
                

        rank -= 1;

        return {i, rank};
    }

};


// ---------------- Knight ----------------
class Knight {
private:
    pair<int,int> pos;
    Color color;
    vector<Color> enemies;
    bool canMove;
    int moves;
    int totalWeight;

public:
    
    Knight(pair<int,int> position, const Color& c)
        : pos(position), color(c), canMove(true), moves(0), totalWeight(0) {}

    pair<int,int> getPos() const { return pos; }
    void setPosition(int i, int j) { pos = {i, j}; }

    Color getColor() const { return color; }

    void addEnemy(Color e) { enemies.push_back(e); }
    const vector<Color>& getEnemies() const { return enemies; }

    bool isEnemy(Color other) const {
    for (auto &c : enemies) {
        if (c == other) {
            return true;
        }
    }
    return false;
}


    bool canMoveNow() const { return canMove; }
    void blockNextRound() { canMove = false; }
    void unblock() { canMove = true; }

    void addMove(int weight) {
        moves++;
        totalWeight += weight;
    }

    int getMoves() const { return moves; }
    int getTotalWeight() const { return totalWeight; }
};


// ---------------- sort ----------------
template <typename Iterator, typename Compare>
void bubbleSort(Iterator first, Iterator last, Compare comp) {
    for (auto i = first; i != last; ++i) {
        for (auto j = first; next(j) != last - (i - first); ++j) {
            auto k = next(j);
            if (comp(*k, *j)) {
                auto temp = *j;
                *j = *k;
                *k = temp;
            }
        }
    }
}


// ---------------- movimentacao ------------------

bool moveOneStep(Knight &knight, Board &board) {
    GraphAL &g = board.getGraph();
    Dijkstra dijkstra(g);

    int source = board.coordToVertex(knight.getPos());
    int target = board.coordToVertex(board.getCastle());

    dijkstra.shortestPath(source);
    vector<int> path = dijkstra.getPath(target);

    int nextVertex = path[1];
    auto [ni, nj] = board.vertexToCoord(nextVertex);

    knight.setPosition(ni, nj);

    return true;
}

// --------------- checagens --------------

bool isStorm(Board& board, pair<int,int> pos) {
    for (auto& s : board.getStorms()) {
        if (s == pos) return true;
    }
    return false;
}

vector<int> knightsAt(const vector<Knight>& knights, pair<int,int> pos) {
    vector<int> ids;
    for (int i = 0; i < (int)knights.size(); i++) {
        if (knights[i].getPos() == pos) ids.push_back(i);
    }
    return ids;
}

// ---------------- main ----------------
int main() {
    int size;
    int numberOfKnights;
    

    cin >> size;

    //obs: o numero de colunas do tabuleiro/mapa e igual ao numero de linhas; o menor numero de linhas e 8 e o maior e 15.
    if (size < 8 || size > 15) {
        throw length_error("tamanho invalido");
    }


    Board board(size);
    board.getGraph().generate_Graph(size);


    cin >> numberOfKnights;


    if (numberOfKnights < 2 || numberOfKnights > 7) {
        throw length_error("numero de exercitos invalido");
    }
    
    vector<Knight> knights;
    vector<Knight> winners;
    
    

    for (int i = 0; i < numberOfKnights; i++) {
        string color, position;
        cin >> color >> position;

        Color c = stringToColor(color);
        
        pair<int,int> startPosition = board.position(position);
        if (startPosition.first == -1) {
            throw invalid_argument("posicao inicial invalida: " + position);
        }
        Knight k(startPosition, c);

        string rest;
        getline(cin, rest);

        string enemy;
        for (size_t j = 0; j < rest.size();) {
            while (j < rest.size() && rest[j] == ' ') j++;
            size_t start = j;
            while (j < rest.size() && rest[j] != ' ') j++;
            if (start < j) {
                enemy = rest.substr(start, j - start);
                k.addEnemy(stringToColor(enemy));
            }
        }

        knights.push_back(k);
    }
    
   
    string castlePosition;
    cin >> castlePosition;
    board.setCastle(board.position(castlePosition));
    

    int stormsNumber;
    cin >> stormsNumber;
    

    for (int i = 0; i < stormsNumber; i++) {
        string stormPosition;
        cin >> stormPosition;
        board.addStorm(board.position(stormPosition));
    }
    
    
    bool finished = false;

    while (!finished) {

        for (int i = 0; i < (int)knights.size(); i++) {
            Knight& k = knights[i];

            if (!k.canMoveNow()) {
                k.unblock();
                continue;
            }

            GraphAL& g = board.getGraph();
            Dijkstra d(g);

            int source = board.coordToVertex(k.getPos());
            int target = board.coordToVertex(board.getCastle());

            d.shortestPath(source);
            auto path = d.getPath(target);

            if (path.size() < 2) continue;

            int proximo = path[1];
            auto [a, b] = board.vertexToCoord(proximo);

            auto ocupantes = knightsAt(knights, {a, b});
            bool blocked = false;
            for (int id : ocupantes) {
                if (k.isEnemy(knights[id].getColor())) {
                    blocked = true;
                    break;
                }
            }

            if (!blocked) {
                int edgeWeight = g.getEdgeWeight(source, proximo);
                if (edgeWeight == -1) {
                    throw runtime_error("Erro: aresta nao encontrada!");
                }
                
                k.setPosition(a, b);
                k.addMove(edgeWeight);
            }
        }


        for (auto& k : knights) {
            if (isStorm(board, k.getPos())) {
                auto ocupantes = knightsAt(knights, k.getPos());
                if (ocupantes.size() == 1) {
                    k.blockNextRound();
                } else if (ocupantes.size() == 2) {
                    board.removeStorm(k.getPos());
                }
            }
        }

        for (auto& k : knights) {
            if (k.getPos() == board.getCastle()) {
                winners.push_back(k);
                finished = true;
            }
        }
    }

    
        bubbleSort(winners.begin(), winners.end(), [](const Knight& a, const Knight& b){
            return colorsToString(a.getColor()) < colorsToString(b.getColor());
        });

        for (auto& k : winners) {
            cout << colorsToString(k.getColor()) << " " << k.getMoves() << " " << k.getTotalWeight() << " ";
        }


    return 0;
}


