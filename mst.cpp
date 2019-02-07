#include<bits/stdc++.h>
using namespace std;

// Creating shortcut for an integer pair
typedef pair<int, int> iPair;

// Structure to represent a graph
struct Graph
{
	int V, E;
	vector< pair<int, iPair> > edges;

	// Constructor
	Graph(int V, int E)
	{
		this->V = V;
		this->E = E;
	}

	// Utility function to add an edge
	void addEdge(int u, int v, int w)
	{
		edges.push_back({w, {u, v}});
	}

	// Function to find MST using Kruskal's
	// MST algorithm
	void kruskalMST(int num);
};

// To represent Disjoint Sets
struct DisjointSets
{
	int *parent, *rnk;
	int n;

	// Constructor.
	DisjointSets(int n)
	{
		// Allocate memory
		this->n = n;
		parent = new int[n+1];
		rnk = new int[n+1];

		// Initially, all vertices are in
		// different sets and have rank 0.
		for (int i = 0; i <= n; i++)
		{
			rnk[i] = 0;

			//every element is parent of itself
			parent[i] = i;
		}
	}
	
	int * ranks(){
		return this->rnk; 
	}

	// Find the parent of a node 'u'
	// Path Compression
	int find(int u)
	{
		/* Make the parent of the nodes in the path
		from u--> parent[u] point to parent[u] */
		if (u != parent[u])
			parent[u] = find(parent[u]);
		return parent[u];
	}
	
	int findDepth(int u){
		int depth = 0;
		
		while(u != parent[u]){
			parent[u] = find(parent[u]);
			u = parent[u];
			depth++;
		}
		return depth;
	}

	// Union by rank
	void merge(int x, int y)
	{    
		int ox = x;
		int oy = y;
		x = find(x), y = find(y);
		
		if(x==y)
			return;
			

		/* Make tree with smaller height
		a subtree of the other tree */
		if (rnk[x] < rnk[y])
			parent[x] = y;
		else if (rnk[x] < rnk[y])
			parent[y] = x;

		else{
			if(ox < oy){
				parent[y] = x;
				rnk[x]++;
			} else {
				parent[x] = y;
				rnk[y]++;
			}
		}
	}
};

bool exists(int val, int *arr, int size){
    int i;
    for (i=0; i < size; i++) {
        if (arr[i] == val)
            return true;
    }
    return false;
}

/* Functions returns weight of the MST*/

void Graph::kruskalMST(int num)
{
	
	long mst_wt = 0; // Initialize result
	long max_weight = 0;
	int max_weight_pos  = 1;
	int counter = 0;

	// Sort edges in increasing order on basis of cost
	sort(edges.begin(), edges.end());

	// Create disjoint sets
	DisjointSets ds(V);

	// Iterate through all sorted edges
	vector< pair<int, iPair> >::iterator it;
	int nodes[V];
	
	for (it=edges.begin(); it!=edges.end(); it++)
	{
		int u = it->second.first;
		int v = it->second.second;

		int set_u = ds.find(u);
		int set_v = ds.find(v);

		// Check if the selected edge is creating
		// a cycle or not (Cycle is created if u
		// and v belong to same set)
		if (set_u != set_v)
		{
			// Current edge will be in the MST
			// so print it
			
			//cout << u << " - " << v << " - " << set_u << " - " << set_v << endl;
			
			

			// Update MST weight
			if(it->first >= max_weight){
				max_weight = it->first;
			}
			mst_wt += it->first;

			// Merge two sets
			ds.merge(set_u, set_v);
			
			if(!exists(u, nodes, counter)){
				nodes[counter++] = u;
			}
			
			if(!exists(v, nodes, counter)){
				nodes[counter++] = v;
			}			
		}		
	}
	
	for (it=edges.begin(); it!=edges.end(); it++){
		if(max_weight == it->first){
			break;
		} else {
			max_weight_pos++;
		}
	}
	
	sort(nodes, nodes+V);
	
	int * rank_arr = ds.ranks();
	int n_half_depth = ds.findDepth(*(nodes+(num/2)));
	
/*	cout << "nodes: ";
	for (int k = 0 ; k < V; k++){
		//cout << rank_arr[k] << ", ";
		cout << nodes[k] << ", ";
	}
	cout<<"\nranks: ";
	for (int k = 0 ; k < V; k++){
		cout << rank_arr[k] << ", ";
	
	}
	cout<<"\ndepth: ";	
	for (int k = 0 ; k < V; k++){
		//cout << rank_arr[k] << ", ";
		cout << ds.findDepth(nodes[k]) << ", ";
	}
	cout<<"\n";*/
	
	// Output	
	cout << mst_wt << "\n";
	cout << max_weight << "\n";
	cout << max_weight_pos << "\n";	
	cout << n_half_depth << "\n";	
}

// Driver program to test above functions
int main()
{
	// Taking inputs
	
	int V, E, u, v;
	long w;
	cin >> V;
	cin >> E;
	
	Graph g(V, E);
	
	for (int i = 0; i < E; i++){
		cin >> u;
		cin >> v;
		cin >> w;	
		g.addEdge(u, v, w);
	}
	g.kruskalMST(V);
	return 0;
}

