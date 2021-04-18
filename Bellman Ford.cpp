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

    // Initialization
    int *predecessor = new int[_nv];     // "Remembering" the Shortest Path in Reverse (Destination to Source)
    int *distance=  new int [_nv];       // Shortest Distance to each Vertex
    auto *path = new double[_nv];        // Holder for outputting Shortest Path
    bool cycle = false;                  // Check for Cycle (only used if negative weights present)
    for (int i = 0; i < _nv; i ++)
    {
        predecessor[i] = -1;
        distance[i] = 2147483647;
        path[i] = 0.1;
    }
    distance[s] = 0;    // Initialize Source to have 0 distance

    // Let's use Bellman-Ford Algorithm (run V times)
    for (int i = 0; i < _nv; i ++)
    {
        for (int i = 0; i < _nv; i++)                               // For every Vertex in the Graph
        {
            for ( _al[i].start(); !(_al[i].end()); _al[i].next() )  // For every Edge in the that Linked List
            {
                int node = (_al[i].current()).nodeIndex();
                int weight = (_al[i].current()).weight();

                // Relax all the possible nodeWeightPairs that exist inside
                if (distance[i] != 2147483647 && distance[node] > distance[i] + weight)
                {
                    distance[node] = distance[i] + weight;
                    predecessor[node] = i;
                }
            }
        }
    }

    // Run Bellman-Ford Algorithm one more time
    for (int i = 0; i < _nv; i++)                               // For every Vertex in the Graph
    {
        for ( _al[i].start(); !(_al[i].end()); _al[i].next() )  // For every Edge in the that Linked List
        {
            int node = (_al[i].current()).nodeIndex();
            int weight = (_al[i].current()).weight();

            // Relax all the possible nodeWeightPairs that exist inside
            if (distance[i] != 2147483647 && distance[node] > distance[i] + weight)
            {
                distance[node] = distance[i] + weight;
                predecessor[node] = i;
                cycle = true;
            }
        }
    }

    if (cycle)
        return -1;

    int value;
    value = (distance[d] == 2147483647) ? -1 : distance[d];  // If destination has not been relaxed yet, then it must be impossible for Source to reach Destination

    // Assert that Source and Destination are connected
    if (value != -1)
    {
        std::cout << "Path: ";
        int traversal = d;
        int i = 0;

        // Everything below is just to print out Shortest Path
        do
        {
            path[i] = traversal;
            traversal = predecessor[traversal];
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
            for (j = 0; path[j] != 0.1 && j < _nv; j++)
            {
                std::cout << path[j];
                if (path[j+1] != 0.1)
                    std::cout << " ";
            }
        }

        std::cout << std::endl;
    }

    delete[] distance;
    delete[] predecessor;
    delete[] path;
    return value;
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
