#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <climits>
#include <queue>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;

#define MAX 21

struct City{
    string cityName;
    int xCoord;
    int yCoord;
    int isCentral;
};

vector<City> cities;
vector<string> cityNames;
vector<City> newCities;
vector<string> newCityNames;
ofstream outfile;
vector<string> citiesAndCoordinates;
vector<string> citiesConnectionCosts;
vector<string> newCitiesAndCoordinates;

typedef pair<int, int> iPair;

//PART 2 IMPLEMENTATION BEGIN.
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
    int j = 0;

    for(int i = 0; i < E; i++)
    {
        a = citiesConnectionCosts[j];
        j++;
        b = citiesConnectionCosts[j];
        j++;
        c = stoi(citiesConnectionCosts[j]);
        j++;

        //add reading of citynames
        addEdge(a, b, c);
        addEdge(b, a, c);
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
        outfile << it.second.first << " - " << it.second.second << " " << it.first << endl;
    }
    
    outfile << endl;
}
//PART 2 IMPLEMENTATION END.

int main()
{
    string x;
    fstream newFile;

    outfile.open("checking.txt");

    //Read input file
    newFile.open("in01.txt");

    //n is amount of nodes, m is amount of arcs, q is amount of new cities.
    int c, n, m, q;

    newFile >> n >> m >> q;

    //Read all cities
    for(int i = 0; i < n * 4; i++)
    {
        newFile >> x;
        if(i % 4 == 0) cityNames.push_back(x);
        //getline(newFile, x);
        citiesAndCoordinates.push_back(x);   
    }

    cout << "Finished 1" << endl;
    for(int i = 0; i < m * 3; i++)
    {
        newFile >> x;
        //getline(newFile, x);
        citiesConnectionCosts.push_back(x);   
    }
    cout << "Finished 2" << endl;
    for(int i = 0; i < q * 3; i++)
    {
        newFile >> x;
        //getline(newFile, x);
        newCitiesAndCoordinates.push_back(x);   
    }
    cout << "Finished 3" << endl;
    newFile.close();

    //PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 
    //PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1
    //PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1
    Graph G(n, m);
    outfile << "1 - Cableado óptimo de nueva conexión." << endl << endl;
    G.Load();
    G.KruskalMST();
    G.PrintEdgesKruskal();
    outfile << "Costo total: " << G.costMSTKruskal << endl;
    outfile << endl;
    outfile << "=========================================================================" << endl;
    outfile << "=========================================================================" << endl;
    outfile << endl;

    //PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2
    //PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2
    //PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2
    string a, b;
    int j = 0;
    for(int i = 0; i < n; i++)
    {
        string cityName;
        int xCoord, yCoord, isCentral;

        cityName = citiesAndCoordinates[j];
        j++;
        xCoord = stoi(citiesAndCoordinates[j]);
        j++;
        yCoord = stoi(citiesAndCoordinates[j]);
        j++;
        isCentral = stoi(citiesAndCoordinates[j]);
        j++;

        City tempCity;
        tempCity.xCoord = xCoord;
        tempCity.yCoord = yCoord;
        tempCity.isCentral = isCentral;
        cities.push_back(tempCity);
    }

    j = 0;
    for(int i = 0; i < m; i++)
    {
        a = citiesConnectionCosts[j];
        j++;
        b = citiesConnectionCosts[j];
        j++;
        c = stoi(citiesConnectionCosts[j]);
        j++;

        //add reading of citynames
    }

    j = 0;
    for(int i = 0; i < q; i++)
    {
        string newCityName;
        int xCoord, yCoord;
        City tempNewCity;

        newCityName = newCitiesAndCoordinates[j];
        j++;
        xCoord = stoi(newCitiesAndCoordinates[j]);
        j++;
        yCoord = stoi(newCitiesAndCoordinates[j]);
        j++;

        City newTempCity;
        newTempCity.cityName = newCityName;
        newTempCity.xCoord = xCoord;
        newTempCity.yCoord = yCoord;
        newTempCity.isCentral = 0;
        newCities.push_back(newTempCity);
        newCityNames.push_back(newCityName);
    }

    outfile << "4 - Conexión de nuevas colonias." << endl << endl;

    for(int i = 0; i < newCities.size(); i++)
    {
        float minDistance = INT_MAX;
        string tempBestCity = "";
        string tempNewCityName = newCityNames[i];
        float tempNewCityX = newCities[i].xCoord;
        float tempNewCityY = newCities[i].yCoord;

        for(int j = 0; j < cities.size(); j++)
        {
            string tempCityName = cityNames[j];
            float tempCityX = cities[j].xCoord;
            float tempCityY = cities[j].yCoord;
            
            float tempCost = sqrt(pow((tempCityX - tempNewCityX), 2) + pow((tempCityY - tempNewCityY), 2));
            
            if(tempCost < minDistance)
            {
                minDistance = tempCost;
                tempBestCity = tempCityName;
            }
        }

        outfile << tempNewCityName << " debe conectarse con " << tempBestCity << endl;
        minDistance = 0.0;
    }
    return 0;
}