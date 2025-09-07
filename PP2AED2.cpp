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
          return front;  //verificar pois está dando erro
        }
    
        bool empty(){
          return data.empty();
        }
    };

class Board {
    private:
        static bool onLimit (int i, int j){
            return (i >= 0 && i < 8 && j >= 0 && j < 8);
        }

        static vector <pair<int,int>>possibleMoves(int i, int j){
            vector <pair<int,int>> moves;
            int deslocs[8][2] =
            {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, 
            {1, 2}, {1, -2}, {1, -2}, {-1, -2}
        };

        for (int i=0; i<8; i++)){
            int newI = i + deslocs[i][0];
            int newJ = j + deslocs[i][1];
            if(onLimit(newI, newJ)){
                moves.push_back(newI, newJ);
            }
        }
            return moves;
    }
};

class BFS{
    private:
        int dist[9][9];

    public:
        BFS(){
            for (int i=1; i<8; i++){
                for (int j=1; j<8; j++){
                    dist[i][j] = -1;
                }

            int minorPath(pair<int,int> start, pair<int,int> end)){
              Queue q;
              dist[start.first][start.second] = 0;
              q.enqueue(start);

              while(!q.empty()){
                pair<int,int> current = q.unqueue();
                int l = current.first;
                int c = current.second;

                vector<pair<int,int>> moves = Board::possibleMoves(l,c);
                for (auto n : neighbors) {
                  if (dist[n.first][n.second] == -1){
                    dist[n.first][n.second] = dist[l][c] + 1;
                    q.enqueue(n);
                  }
                }
              }
              return dist[end.first][end.second];
            }
          };
        }
    };
//falta ----- class Cavaleiro, função auxiliar e main. Verificar se precisa de mais alguma coisa e se o código de cima vai funcionar.
//ordernar os vizinhos com bubblesort
