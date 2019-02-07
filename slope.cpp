/*
 * The slope has a lower bound of 0.0 and a possible upper bound when all unknown nodes have 
 * the same height as the lowest known node. The problem now turns into a problem of determining whether
 * there is a solution for a particular slope value k. The following algorithm takes the input, constructs the graph,
 * computes the bounds and does binary search. While doing binary search, we also run 
 * Dijkstra with the source node being the lowest known node and verify the current slope value k.  
 *
 * The constraints are:
 * For every edge (i,j), we have (1) hi - hj <= len(i,j) * k and (2) hj - hi <= len(i,j) * k
 * For every pair of known nodes (k1, k2), we have (1) hk1 - hk2 <= Hk1 - Hk2 and (2) hk2 - hk1 <= Hk2 - Hk1
 * where the uppercase H's are the actual heights for the known nodes.
 * 
 * Since we want to minimize our constraints we only use all the pairs involving the lowest known node (denoted by 
 * lk), which suffices:
 * (1) hki - hlk <= Hki - Hlk and (2) hlk - hki <= Hlk - Hki
 * 
 * We can construct a new graph using these constraints and the graph has a solution iff there is no negative cycle.
 * 
 * By choosing the lowest known node as the source node, the incoming edges into the source node are negative while
 * the other edges are all positive. To use Dijkstra, first ignore the negative edges and compute all of the shortest
 * paths from the source node. Then check if these shortest paths is smaller than the corresponding negative edge;
 * if so, reject; else, accept it as a solution. 
 * 
 * Dijkstra has a running time of O(mlogn) which is in poly time. 
 */
#include <stdio.h>
#include <algorithm>
#include <queue>
using namespace std;

struct UpdateRequest {
    //a request to update a node's distance, used in Dijkstra
    int index;
    double distance;
    UpdateRequest(int index, double distance): index(index), distance(distance) {}
};

struct CompareUpdate {
    //a comparator for a min heap on distance
    bool operator() (const UpdateRequest &thisRequest, const UpdateRequest &anotherRequest) {
        return thisRequest.distance > anotherRequest.distance;
    }
};

//fields
int nodes = 0; //total number of nodes
int knownNodes = 0; //total number of known nodes
int lowestKnownNode = -1; //index of smallest known node
int knownNodeIndex[1023]; //list of known node indexes
int adjNum[1023]; //adjacent node number for each node, remain unchanged once initialized
int adjList[1023][1025]; //adjacent lists for each node, remain unchanged once initialized    
double height[1023]; //heights of nodes. Unknown nodes don't matter
double curSlope = 1.; //current slope
double upperBound = 0.; //slope upper bound, used for binary search.
double adjWeight[1023][1025]; //adjacent weights for each node; change when curSlope changes
bool isKnownNode[1023]; //whether a node is a node with known heights

//initialization. Return number of known nodes.
int init() {
	for (int i = 0; i < 1023; i++) {
	    adjNum[i] = 0;
	}	
    //first do some initialization
    scanf("%d", &nodes);
    int edges;
    scanf("%d", &edges);
    double minHeight = 1e9;
    char tmp[31];
    for (int index = 0; index < nodes; index++) {
        scanf("%s", tmp);
        if (tmp[0] == '*') {
            //unknown node
            isKnownNode[index] = false;
        } else {
            //known node
            isKnownNode[index] = true;
            double nodeHeight = (double)(atoi(tmp));
            height[index] = nodeHeight;
            knownNodeIndex[knownNodes++] = index;
            //update minimum height
            if (nodeHeight < minHeight) {
                minHeight = nodeHeight;
                lowestKnownNode = index;
            }
        }
    }
    if (knownNodes > 1) {
        //if there are at least 2 known nodes
        for (int index = 0; index < knownNodes; index++) {
            height[knownNodeIndex[index]] -= minHeight;
        }
    } else {
        //if there is at most 1 known node, we can return because the answer is always zero
        return knownNodes;
    }
    //construct edges:
    for (int i = 0; i < edges; i++) {
        int node1, node2;
        double length;
        scanf("%d%d%lf", &node1, &node2, &length);
        adjList[node1][adjNum[node1]] = node2;
        adjList[node2][adjNum[node2]] = node1;
        adjWeight[node1][adjNum[node1]] = length;
        adjWeight[node2][adjNum[node2]] = length;
        adjNum[node1]++;
        adjNum[node2]++;
        //update upper bound
        if (isKnownNode[node1]) {
            double slopeValue = height[node1] / length;
            if (slopeValue > upperBound) {
                upperBound = slopeValue;
            }
        } else if (isKnownNode[node2]) {
            double slopeValue = height[node2] / length;
            if (slopeValue > upperBound) {
                upperBound = slopeValue;
            }
        }
    }
    //now add edges from lowest known node to other known nodes.
    //there should be knownNodes - 1 such edges.
    for (int i = 0; i < knownNodes; i++) {
        int index = knownNodeIndex[i];
        if (index != lowestKnownNode) {
            adjList[lowestKnownNode][adjNum[lowestKnownNode]] = index;
            adjWeight[lowestKnownNode][adjNum[lowestKnownNode]] = height[index];
            adjNum[lowestKnownNode]++;
        }
    }
    return knownNodes;
}

//reset the graph's weights according to a new slope value
void resetGraph(double newSlope) {
    double resetRatio = newSlope / curSlope;
    for (int i = 0; i < nodes; i++) {
        if (i != lowestKnownNode) {
            //reset all slope values
            for (int j = 0; j < adjNum[i]; j++) {
                adjWeight[i][j] *= resetRatio;
            }
        } else {
            //for the lowest known node, only reset the actual connection part
            for (int j = 0; j < adjNum[i] - knownNodes + 1; j++) {
                //the last (knownNodes - 1) edges are for connections to other known nodes
                adjWeight[i][j] *= resetRatio;
            }
        }
    }
    //update current slope
    curSlope = newSlope;
}

//run a Dijkstra to verify current slope
//return true iff this slope has a solution
bool dijkstraOnCurrentSlope() {
    double distance[1023]; //the temporary distance
    bool accepted[1023]; //whether a node is accepted
    int AcceptedknownNodes = 0; //number of accepted known nodes. If this attains knownNodes, return true
    for (int i = 0; i < nodes; i++) {
        //set distance to infinity, and accepted to false
        distance[i] = 1e9;
        accepted[i] = false;
    }
    //a priority queue holding all update requests
    priority_queue<UpdateRequest, vector<UpdateRequest>, CompareUpdate> updateRequests;
    //first update lowest known node
    updateRequests.push(UpdateRequest(lowestKnownNode, 0.));
    while (!updateRequests.empty()) {
        //pops out the first one
        UpdateRequest updateRequest = updateRequests.top();
        updateRequests.pop();
        int updateNode = updateRequest.index;
        double updateDistance = updateRequest.distance;
        if (accepted[updateNode]) {
            //this node is already accepted, ignore
            continue;
        }
        //accept this node
        accepted[updateNode] = true;
        distance[updateNode] = updateDistance;
        if (isKnownNode[updateNode]) {
            //if it is a known node
            if (updateDistance < height[updateNode]) {
                //this means a violation! immediately return false
                return false;
            } else {
                AcceptedknownNodes++;
                if (AcceptedknownNodes == knownNodes) {
                    //all known nodes have been accepted, and there is no violation, return true
                    return true;
                }
            }
        }
        //for all its adjacent un-accepted node, update distance and insert them into the priority queue
        for (int i = 0; i < adjNum[updateNode]; i++) {
            int adjNode = adjList[updateNode][i];
            if (accepted[adjNode]) {
                continue;
            }
            double newDistance = updateDistance + adjWeight[updateNode][i];
            if (newDistance < distance[adjNode]) {
                if (isKnownNode[adjNode] && newDistance < height[adjNode]) {
                    //this is also a violation because even a potential shortest path is shorter. return false.
                    return false;
                }
                distance[adjNode] = newDistance;
                updateRequests.push(UpdateRequest(adjNode, newDistance));
            }
        }
    }
    //actually this line should not be executed
    return true;
}

//solve by using binary search
double solve() {
    double lower = 0., upper = upperBound;
    double epsilon = 1e-7; //the condition to terminate the search. Possible adjustment needed
    while (upper - lower > epsilon) {
        double middle = (upper + lower) * .5;
        //reset the graph weights
        resetGraph(middle);
        if (dijkstraOnCurrentSlope()) {
            //middle passed the test.
            upper = middle;
        } else {
            lower = middle;
        }
    }
    return upper;
}

int main() {
    if (init() <= 1) {
        //if there is at most one known node
        printf("slope = 0.000000\n");
    } else {
        //solve it
        printf("slope = %.6lf\n", solve());
    }
    return 0;
}

