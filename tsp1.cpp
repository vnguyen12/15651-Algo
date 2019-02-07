#include <vector>
#include <iostream>
#include <map>
#include <iomanip>

#define INT_MAX 99999
#define MAX_SIZE 80

using namespace std;
int n, m;

vector<vector<int> > tspPath;

void printPath(vector<vector<int> > path, int v, int u)
{
    if (path[v][u] == v)
        return;

    printPath(path, v, path[v][u]);
    cout << path[v][u] << " ";
}


int tsp(const vector<vector<int> >& cities, int pos, int visited, vector<vector<int> >& state, std::vector<int> v)
{
    v.push_back(pos);
    if (visited == ((1 << cities.size()) - 1)) {
        tspPath.push_back(v);

        return cities[pos][0]; // return to starting city
    }
    if (state[pos][visited] != INT_MAX) {
        return state[pos][visited];
    }
    for (int i = 0; i < cities.size(); ++i) {
        // can't visit ourselves unless we're ending & skip if already visited
        if (i == pos || (visited & (1 << i)))
            continue;
        int distance = cities[pos][i] + tsp(cities, i, visited | (1 << i), state, v);
        if (distance < state[pos][visited] && distance > 0) {
            state[pos][visited] = distance;
        }
    }

    return state[pos][visited];
}

int main()
{
    cin >> n >> m;
    int N = n;
    std::vector<std::vector<int> > graph(n, std::vector<int>(n, 0));
    std::vector<std::vector<int> > matrix(n, std::vector<int>(n, 0));
    std::vector<std::vector<int> > cost(n, std::vector<int>(n, 0));
    std::vector<std::vector<int> > path(n, std::vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = INT_MAX;
            graph[i][j] = INT_MAX;
        }
    }
    for (int i = 0; i < m; i++) {
        int a, b, dij, dji;
        cin >> a >> b >> dij >> dji;
        matrix[a][b] = dij;
        matrix[b][a] = dji;
        graph[a][b] = dij;
        graph[b][a] = dji;
    }

    // initialize cost[] and parent[]
    for (int v = 0; v < N; v++) {
        for (int u = 0; u < N; u++) {
            // initally cost would be same as weight
            // of the edge
            cost[v][u] = matrix[v][u];

            if (v == u)
                path[v][u] = 0;
            else if (cost[v][u] != INT_MAX)
                path[v][u] = v;
            else
                path[v][u] = -1;
        }
    }

    // run Floyd-Warshall
    for (int k = 0; k < N; k++) {
        for (int v = 0; v < N; v++) {
            for (int u = 0; u < N; u++) {
                // If vertex k is on the shortest path from v to u,
                // then update the value of cost[v][u], path[v][u]

                if (cost[v][k] != INT_MAX && cost[k][u] != INT_MAX
                    && cost[v][k] + cost[k][u] < cost[v][u]) {
                    cost[v][u] = cost[v][k] + cost[k][u];
                    path[v][u] = path[k][u];
                }
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (matrix[i][j] == INT_MAX) {
                matrix[i][j] = cost[i][j];
            }
        }
    }


    std::vector<int> v;
    vector<vector<int> > state(matrix.size());
    for (auto& neighbors : state)
        neighbors = vector<int>((1 << matrix.size()) - 1, INT_MAX);
    int dis = tsp(matrix, 0, 1, state, v);
    cout << "optimal tour cost = " << dis << endl;

    for (int i = 0; i < tspPath.size(); i++) {
        int sum = 0;
        int sv = 0;
        int ev = 1;
        for (int j = 1; j < tspPath[0].size(); j++) {
            sv = tspPath[i][j - 1];
            ev = tspPath[i][j];
            sum += matrix[sv][ev];
        }
        sum += matrix[ev][0];
        //cout << "sum = " << sum <<endl;

        if (sum == dis) {
            sv = 0;
            cout << "tour: " << sv<< " ";
            for (int j = 1; j < N; j++) {
                sv = tspPath[i][j - 1];
                ev = tspPath[i][j];
                if (graph[sv][ev] == INT_MAX) {
                    printPath(path, sv, ev);

                }
                  cout << ev<< " ";
            }
            sv = ev;
            ev = 0;
            if (graph[sv][ev] == INT_MAX) {
                printPath(path, sv, ev);
            }

                cout << ev << endl;

            cout << endl;
            break;
        }
    }

    return 0;
}
