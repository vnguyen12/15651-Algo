#include <iostream>
#include <stack>
using namespace std;

const int max_vertices = 1 << 20;
const int max_edges = 20;

int dij_dist[max_edges][max_edges];
int dij_tour[max_edges][max_edges];
int temp_dist[max_edges][max_edges][2];
int temp_tour[max_edges][max_edges][2];
int tsp_dist[max_edges][max_vertices];
int tsp_tour[max_edges][max_vertices];

int n;
const int MAXINT = (int) 1e9;
const int ZERO = 0;

void take_input(){
	int m;
	cin >> n;
	cin >> m;

	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(i == j) {
				// setting zero distance to itself
				temp_dist[i][j][0] = 0;
				dij_dist[i][j] = 0;
			}
			else {
				//set distance to infinity
				temp_dist[i][j][0] = MAXINT;
				dij_dist[i][j] = MAXINT;
			}
		}
	}
	int i,j,dij,dji;
	while(m--){
		cin.ignore();
		cin.clear();
		cin >> i;
		cin >> j;
		cin >> dij;
		cin >> dji;
		if(i==j) continue;//ignore self loop
		if(dij < temp_dist[i][j][0]){
			temp_dist[i][j][0] = dij;
			dij_dist[i][j] = dij;
			temp_tour[i][j][0] = i;
		}
		if(dji < temp_dist[j][i][0]){
			temp_dist[j][i][0] = dji;
			temp_tour[j][i][0] = j;
		}
	}
}

void floyd_warshall(){
	//use floyd-warshall 
	for(int k = 0; k < n; k++){
		int from, to;
		if(k%2==0){
			from = 0, to = 1;
		}
		else{
			from = 1, to = 0;
		}
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				if(i==j) continue;//assume no negative weights
				int newDist = temp_dist[i][k][from] + temp_dist[k][j][from];
				if(newDist < temp_dist[i][j][from]){
					temp_dist[i][j][to] = newDist;
					temp_tour[i][j][to] = temp_tour[k][j][from];
				}
				else{
					temp_dist[i][j][to] = temp_dist[i][j][from];
					temp_tour[i][j][to] = temp_tour[i][j][from];
				}
			}
		}
	}
	int target_index = n%2;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			dij_dist[i][j] = temp_dist[i][j][target_index];
			dij_tour[i][j] = temp_tour[i][j][target_index];
		}
	}
}

void tsp(){
	//init
	tsp_dist[0][1] = 0;

	int stUb = 1 << n;

	for(int state = 3; state < stUb; state += 2){
		tsp_dist[0][state] = MAXINT;
	}

	for(int state = 3; state < stUb; state += 2){
		for(int i = 1; i < n; i++){
			if(!(state & (1 << i))) continue;
			int last = state ^ (1 << i);
			int thisMin = MAXINT, arg = -1;
			for(int j = 0; j < n; j++){
				if(i == j || !(state & (1 << j))) continue;
				int newMin = tsp_dist[j][last] + dij_dist[j][i];
				if(newMin < thisMin){
					thisMin = newMin;
					arg = j;
				}
			}
			tsp_dist[i][state] = thisMin;
			tsp_tour[i][state] = arg;
		}
	}
}

void push_path(int from, int to, stack<int> &Tour){
	int cur = to;
	while(cur != from){
		int last = dij_tour[from][cur];
		Tour.push(last);
		cur = last;
	}
}

void print_tour(){
	int target_index = (1 << n) - 1;
	int minDist = MAXINT, arg = -1;
	for(int i = 1; i < n; i++){
		int thisDist = dij_dist[i][0] + tsp_dist[i][target_index];
		if(thisDist < minDist) {
			minDist = thisDist;
			arg = i;
		}
	}
	cout << "optimal tour cost = "<< minDist << endl << "tour:";
	stack<int> Tour;
	Tour.push(0);
	int r = 0;
	while(1){
		r = dij_tour[arg][r];
		Tour.push(r);
		if(r == arg) break;
	}
	int cur = arg;
	int state = target_index;
	while(1){
		int last = tsp_tour[cur][state];
		state = state ^ (1 << cur);
		//push the vertexes from cur to last to the stack
		push_path(last, cur, Tour);
		cur = last;
		if(!cur) break;
	}
	while(!Tour.empty()){
		cout << " " << Tour.top();

		Tour.pop();
	}
	cout << endl;
}


int main() {
	//take_input and initialize
	take_input();
	if(n == 1){
		cout << "optimal tour cost = 0\ntour: 0\n";
		return 0;
	}

	//floyd-warshall
	floyd_warshall();

	//tsp
	tsp();

	//trace back to print the path
	print_tour();
	return 0;
}