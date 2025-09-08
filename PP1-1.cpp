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

using namespace std;

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
    static bool onLimit(int i, int j) {
        return (i >= 0 && i < 8 && j >= 0 && j < 8);
    }

public:
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


class BFS {
private:
    int dist[8][8];

public:
    BFS() {
        for (int i=0; i<8; i++) {
            for (int j=0; j<8; j++) {
                dist[i][j] = -1;
            }
        }
    }

    int minorPath(pair<int,int> start, pair<int,int> end) {
        Queue q;
        dist[start.first][start.second] = 0;
        q.enqueue(start);

        while(!q.empty()) {
            pair<int,int> current = q.unqueue();
            int l = current.first;
            int c = current.second;

            vector<pair<int,int>> moves = Board::possibleMoves(l, c);
            for (auto n : moves) {
                if (dist[n.first][n.second] == -1) {
                    dist[n.first][n.second] = dist[l][c] + 1;
                    q.enqueue(n);
                }
            }
        }
        return dist[end.first][end.second];
    }
};

pair<int,int> chessToCoord(string pos) {
    int col = pos[0] - 'a';
    int row = pos[1] - '1';
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

    void setPos(int i, int j) {
        pos = {i, j};
    }


    int minMovesTo(int i, int j) {
        BFS bfs;
        return bfs.minorPath(pos, {i, j});
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
        
        moves.push_back(k1.minMovesTo(kingPosition.first, kingPosition.second) - 1);
        moves.push_back(k2.minMovesTo(kingPosition.first, kingPosition.second) - 1);
        moves.push_back(k3.minMovesTo(kingPosition.first, kingPosition.second) - 1);
        moves.push_back(k4.minMovesTo(kingPosition.first, kingPosition.second) - 1);
        
        bubbleSort(moves);

        printVector(moves);
        
        moves.clear();
    }

    return 0;
}
