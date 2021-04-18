#include "Graph.h"
//uncomment this to include your own "heap.h"
//we will assume that you use the same code in your previous assignment
#include "heap.h"

std::ostream& operator<<(std::ostream& os, nodeWeightPair const& n) {
	return os << " (idx:" << n._node << " w:" << n._weight << ")";
}


Graph::Graph(int n)
{
	_al = new List<nodeWeightPair> [n];    // Creating an Array of Lists
	_nv = n;                               // Number of Vertices in the Graph (number of Nodes)
}

// Returns the shortest distance from node S to node D
// Choice of Algorithm will be Dijkstra Algorithm
int Graph::shortestDistance(int s, int d)
{
    /* Characteristics of Graph
     1. Directed
     2. At least one node, but no limitation of number of Edges
     3. Each edge has non-zero positive weight
     4. All computations are in Integers
    */

    Heap <nodeWeightPair> VertexHeap;
    int *distance = new int [_nv];
    int *true_distance = new int [_nv];
    int *parent = new int [_nv];
    int *path = new int [_nv];
    for (int i = 0; i < _nv; i ++)
    {
        distance[i] = -2147483648;
        true_distance[i] = -2147483648;
        parent[i] = -1;
        path[i] = -1;
    }
    distance[s] = 0;
    true_distance[s] = 0;

    // Initialization of Adapted MaxHeap for Dijkstra
    nodeWeightPair original_source;
    for (int i = 0; i < _nv; i++)
    {
        nodeWeightPair initialization = nodeWeightPair(i, -2147483648);
        VertexHeap.insert(initialization);

        if (i == s)
            original_source = initialization;
    }
    nodeWeightPair adjusted_source = nodeWeightPair(s, 0);
    VertexHeap.increaseKey(original_source, adjusted_source);    // Ensuring that Source Vertex is now at top of MaxHeap, with distance 0

    int return_value;

    // Beginning of Dijkstra Algorithm
    while (!VertexHeap.empty())
    {
        nodeWeightPair extracted = VertexHeap.extractMax();      // Extract the nodeWeightPair item in Heap
        int NodeIndexExtracted = extracted.nodeIndex();          // Find out it's associated Node Index

        // Relaxing and Updating of Extracted Node's neighbors
        for (_al[NodeIndexExtracted].start(); !_al[NodeIndexExtracted].end(); _al[NodeIndexExtracted].next())
        {
            nodeWeightPair neighbor= _al[NodeIndexExtracted].current();       // Find out neighbors in Adjacency List
            int neighbor_index = neighbor.nodeIndex();                        // Neighbor Index
            int neighbor_weight = -1 * neighbor.weight();                     // Neighbor Weight

            // Relaxation
            if (distance[NodeIndexExtracted] != -2147483648 && distance[neighbor_index] < distance[NodeIndexExtracted] + neighbor_weight)
            {
                distance[neighbor_index] = distance[NodeIndexExtracted] + neighbor_weight;
                true_distance[neighbor_index] = true_distance[NodeIndexExtracted] + (-1 * neighbor_weight);
                parent[neighbor_index] = NodeIndexExtracted;
                nodeWeightPair adjusted_neighbor = nodeWeightPair(neighbor_index, distance[neighbor_index]);
                VertexHeap.increaseKey(neighbor, adjusted_neighbor);
            }
        }
    }

    return_value = (true_distance[d] == -2147483648) ? -1 : true_distance[d];


    // All of this below is for printing for Path (IF APPLICABLE)
    if (return_value != -1)
    {
        std::cout << "Path: ";
        int traversal = d;
        int i = 0;

        // Everything below is just to print out Shortest Path
        do
        {
            path[i] = traversal;
            traversal = parent[traversal];
            i++;
        } while (traversal != s && traversal != -1);

        path[i] = s;

        int start = 0;
        int end = i;
        while (start < end)
        {
            std::swap(path[start], path[end]);
            start++;
            end--;
        }

        int j = 0;

        // Checking for Edge Case (Single Node isolated by itself)
        if (path[j] == path[j+1])
            std::cout << path[j];


        // Assert that not Single Node
        else
        {
            for (j = 0; path[j] != -1 && j < _nv; j++)
            {
                std::cout << path[j];
                if (path[j+1] != -1)
                    std::cout << " ";
            }
        }

        std::cout << std::endl;
    }

    delete[] distance;
    delete[] true_distance;
    delete[] path;
    delete[] parent;
    return return_value;
}

void Graph::addEdge(int s, int d, int w)
{
    // _al[] creates an Array of Lists (Engine of ListNode)
    // _al[s] decays to a pointer, pointing to the current List Engine at [s] index
    // After accessing the [s] element, we are at the sth List Engine (controlling a single Linked List)
    // We can then proceed to insertHead at this List, by using List Functions
	_al[s].insertHead(nodeWeightPair(d, w));
}

void Graph::printGraph()
{
	for (int i=0; i < _nv; i++)
	{
		cout << "Node " << i << ": ";
		for (_al[i].start(); !_al[i].end(); _al[i].next())
			cout << " (" << _al[i].current().nodeIndex() << "," << _al[i].current().weight() << ")";
		cout << endl;

	}

}
Graph::~Graph()
{

	delete[] _al;

}