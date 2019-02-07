/*
 * The problem is reformulated as a bipartite matching problem that uses the Ford-Fulkerson algorithm to 
 * find the max flow. Consider the board as a chessboard with alternating black and white squares. 
 * A domino can only be placed on two different colored squares. So the max-flow network has the following 
 * architecture: source -> black squares -> white squares -> sink and each edge has weight 1. If 
 * an edge between the black and white squares layers is cut, then a domino can be placed on those two 
 * squares. Since this is a bipartite matching problem, Ford-Fulkerson algorithm will return the 
 * max number of matching. Since Ford-Fulkerson has a runtime of O(mF) and the max flow here is 1, 
 * the run time is just O(m).
 */
#include <iostream>
#include <string.h>

#define MAX 1000
using namespace std;
char matrix[100][100];
int adjMatrix[MAX][MAX];
int M = MAX;
int N = MAX;



bool bpm(int u, bool seen[], int matchR[])
{
    for (int v = 0; v < N; v++)
    {
        if (adjMatrix[u][v] && !seen[v])
        {
            seen[v] = true; // Mark v as visited

            if (matchR[v] < 0 || bpm(matchR[v], seen, matchR))
            {
                matchR[v] = u;
                return true;
            }
        }
    }
    return false;
}


int maxBPM()
{

    int matchR[N];

    memset(matchR, -1, sizeof(matchR));

    int result = 0; 
    for (int u = 0; u < M; u++)
    {
        
        bool seen[N];
        memset(seen, 0, sizeof(seen));

        if (bpm(u, seen, matchR))
            result++;
    }
    return result;
}

bool isBlack(int i, int j)
{
    if ((i + j) % 2 == 0) {
        return true;
    }
    return false;
}

void fillAdjMatrix(int row,int col)
{
    int total_vertices = row*col;
    for (int i = 0; i < total_vertices; i++) {
        for (int j = 0; j < total_vertices; j++) {
            adjMatrix[i][j] = 0;
        }
    }
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (i + 1 < row && isBlack(i, j)) {
                if (matrix[i][j] == '.' && matrix[i + 1][j] == '.') {
                    adjMatrix[col * i + j][col * (i + 1) + j] = 1;
                }
            }

            if (i - 1 >= 0 && isBlack(i, j)) {
                if (matrix[i][j] == '.' && matrix[i - 1][j] == '.') {
                    adjMatrix[col * i + j][col * (i - 1) + j] = 1;
                }
            }

            if (j + 1 < col && isBlack(i, j)) {
                if (matrix[i][j] == '.' && matrix[i][j + 1] == '.') {
                    adjMatrix[col * i + j][col * i + j + 1] = 1;
                }
            }

            if (j - 1 >= 0 && isBlack(i, j)) {
                if (matrix[i][j] == '.' && matrix[i][j - 1] == '.') {
                    adjMatrix[col * i + j][col * i + j - 1] = 1;
                }
            }
        }
    }
}

int main()
{
    int row, col,M,N;
    cin >> row;
    cin >> col;
    M = N = row*col;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            cin >> matrix[i][j];
        }
    }
    fillAdjMatrix(row,col);
    int dominos = maxBPM();
    if(dominos == 1){
      cout << dominos << " domino"<<endl;
    }
    else{
      cout << dominos << " dominos"<<endl;
    }
}

