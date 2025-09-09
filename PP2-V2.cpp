#include <strings.h>
#include <limits>
#include <list>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


class Queue {
private:
    list<pair<int,int>> q;
public:
    void enqueue(pair<int,int> val) {
        q.push_back(val);
    }
    pair<int,int> unqueue() {
        auto val = q.front();
        q.pop_front();
        return val;
    }
    bool empty() const {
        return q.empty();
    }
};


class Graph {
private:
    vector<vector<int>> adj;
public:
    Graph() : adj(64) {}
    void add_edge(int v, int u) {
        adj[v].push_back(u);
    }
    const vector<int>& get_adj(int v) const {
        return adj[v];
    }
};

class Board {
private:
    Graph g;
public:
    Board() {

        vector<pair<int,int>> moves = {
            {2,1},{1,2},{-1,2},{-2,1},
            {-2,-1},{-1,-2},{1,-2},{2,-1}
        };
//mudei aqui
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                int v = coordToVertex({i,j});
                for (auto [dx,dy] : moves) {
                    int ni = i + dx, nj = j + dy;
                    if (ni >= 0 && ni < 8 && nj >= 0 && nj < 8) {
                        int u = coordToVertex({ni,nj});
                        g.add_edge(v, u);
                    }
                }
            }
        }
    }

    static int coordToVertex(pair<int,int> p) {
        return p.first * 8 + p.second;
    }
//mudei aqui
    static pair<int,int> vertexToCoord(int v) {
        return {v/8, v%8};
    }

    const Graph& getGraph() const {
        return g;
    }
};

//mudei o formato do bubbleSort
void bubbleSort(vector<int>& v) {
    int n = v.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (v[j] > v[j + 1]) {
                swap(v[j], v[j + 1]);
            }
        }
    }
}

//adicionei o vector parent
class BFS {
private:
    vector<int> dist;
    vector<int> parent;

public:
    BFS() : dist(64, -1), parent(64, -1) {}

    void buildTree(Board& board, pair<int,int> start) {
        Queue q;
        auto& g = board.getGraph();

        int startV = Board::coordToVertex(start);

        fill(dist.begin(), dist.end(), -1);
        fill(parent.begin(), parent.end(), -1);

        dist[startV] = 0;
        q.enqueue({startV, 0});

        while(!q.empty()) {
            auto [v, d] = q.unqueue();


            vector<int> neighbors = g.get_adj(v);
            bubbleSort(neighbors);

            for (auto u : neighbors) {
                if (dist[u] == -1) {
                    dist[u] = d + 1;
                    parent[u] = v;
                    q.enqueue({u, d + 1});
                }
            }
        }
    }

    int minorPath(Board& board, pair<int,int> start, pair<int,int> end) {
        buildTree(board, start);
        int endV = Board::coordToVertex(end);
        return dist[endV];
    }

    vector<int> getPath(int startV, int endV) {
        vector<int> path;
        for (int v = endV; v != -1; v = parent[v]) {
            path.push_back(v);
            if (v == startV) break;
        }
        reverse(path.begin(), path.end());
        return path;
    }
};


pair<int,int> chessToCoord(string pos) {
    int col = pos[0] - 'a';
    int row = 8 - (pos[1] - '0'); 
    return {row, col};
}


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

    int minMovesTo(Board& board, int i, int j) {
        BFS bfs;
        return bfs.minorPath(board, pos, {i, j});
    }
};



void printVector(const vector<int>& v) {
    if (v.empty()) return;
    int minVal = *min_element(v.begin(), v.end());
    for (int val : v) {
        if (val == minVal) cout << val << " ";
    }
    cout << endl;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Board board;
    int testQnt;
    cin >> testQnt;

    for (int i = 0; i < testQnt; i++) {
        string position1, position2, position3, position4, king;
        cin >> position1 >> position2 >> position3 >> position4 >> king;

        Knight k1(position1), k2(position2), k3(position3), k4(position4);
        pair<int,int> kingPosition = chessToCoord(king);

        vector<int> moves;
        moves.push_back(k1.minMovesTo(board, kingPosition.first, kingPosition.second) - 1);
        moves.push_back(k2.minMovesTo(board, kingPosition.first, kingPosition.second) - 1);
        moves.push_back(k3.minMovesTo(board, kingPosition.first, kingPosition.second) - 1);
        moves.push_back(k4.minMovesTo(board, kingPosition.first, kingPosition.second) - 1);

        printVector(moves);
    }
    return 0;
}
