//Kruskal and Prim examples of MST.

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <climits>

using namespace std;

typedef pair<int, int> iPair;


int main()
{
    int V, E, newCities;
    cin >> V >> E >> newCities;
    Graph G(V, E);
    
    G.Load();
    //G.Print();
    G.KruskalMST();
    G.PrintEdgesKruskal();
    cout << "Costo total: " << G.costMSTKruskal << endl;

    return 0;
}