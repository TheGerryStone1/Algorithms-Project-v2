#include <vector>
#include <algorithm>
#include <queue>


vector<string> cityNames = {"Purisima", "Tecnologico", "Roma", "AltaVista", "LindaVista"};

struct Graph{
    //V is amount of vertex, E is amount of edges.
    int V, E, costMSTKruskal;

    vector<pair<int, pair<string, string>>> edges; //Kruskal.
    vector<vector<pair<string, int>>> adjacenceList;
    vector<pair<int, pair<string, string>>> selectedEdgesKruskal;

    Graph(int V, int E){
        this->V = V;
        this->E = E;
        adjacenceList.resize(V);
        costMSTKruskal = 0;
    }

    void addEdge(string U, string V, int W)
    {
        int uPos = find(cityNames.begin(), cityNames.end(), U) - cityNames.begin();
        int vPos = find(cityNames.begin(), cityNames.end(), V) - cityNames.begin();
        edges.push_back({W, {U, V}}); //Costo, {Conexión de U -> V}.
        adjacenceList[uPos].push_back({V, W});
    }

    static bool cmp(const pair<int, pair<string, string>>& a, const pair<int, pair<string, string>>& b) 
    {
        return a.first < b.first;
    }

    void Load();
    void Print();
    void KruskalMST();
    void PrintEdgesKruskal();
};

//Disjoint sets
struct DisjointSets{
    int *parent, *rnk;
    int n;

    DisjointSets(int n){
        this->n = n;
        parent = new int[n + 1];
        rnk = new int[n + 1];

        for(int i = 0; i <= n; i++)
        {
            rnk[i] = 0;
            parent[i] = i;
        }
    }

    //Find parent of node u.
    int find(int u)
    {
        if(u != parent[u]) parent[u] = find(parent[u]);

        return parent[u];
    }

    //Union using rank.
    void merge(int x, int y)
    {
        x = find(x);
        y = find(y);

        if(rnk[x] > rnk[y]) parent[y] = x;
        else parent[x] = y;

        if(rnk[x] == rnk[y]) rnk[y]++;

    }
};

void Graph::Load()
{
    string a, b;
    int c;

    for(int i = 0; i < E; i++)
    {
        cin >> a >> b >> c;
        //add reading of citynames
        addEdge(a, b, c);
        addEdge(b, a, c);
    }
}

void Graph::Print()
{
    cout << "Adjacence list: " << endl;
    for(int i = 0; i < V; i++)
    {
        cout << " " << i << ": ";
        for(int j = 0; j < adjacenceList[i].size(); j++)
        {
            cout << "(" << adjacenceList[i][j].first << ", " << adjacenceList[i][j].second << ") ";
        }

        cout << endl;
    }
}

void Graph::KruskalMST()
{
    costMSTKruskal = 0;
    sort(edges.begin(), edges.end(), cmp);
    DisjointSets disjointSet(V);

    for(auto it:edges)
    {
        string u = it.second.first;
        string v = it.second.second;
        int uPos = find(cityNames.begin(), cityNames.end(), u) - cityNames.begin();
        int vPos = find(cityNames.begin(), cityNames.end(), v) - cityNames.begin();
        int set_u = disjointSet.find(uPos);
        int set_v = disjointSet.find(vPos);

        if(set_u != set_v)
        {
            disjointSet.merge(uPos, vPos);
            costMSTKruskal += it.first;
            selectedEdgesKruskal.push_back({it.first, {u, v}});
        }
    }
}

void Graph::PrintEdgesKruskal()
{
    //cout << "Selected edges Kruskal: ";

    for(auto it:selectedEdgesKruskal)
    {
        cout << it.second.first << " - " << it.second.second << " " << it.first << endl;
    }
    
    cout << endl;
}