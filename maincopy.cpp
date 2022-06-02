#include <algorithm>
#include <map>
#include <climits>
#include <cmath>
#include "include/tsp_bb.cpp"
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <map>
#include <unordered_set>
#include <vector>

using namespace std;

#define MAX 21
#define MAX2 100

struct City {
  string cityName;
  int xCoord;
  int yCoord;
  int isCentral;
};
vector<string> centralCities;
vector<City> cities;
vector<string> cityNames;
vector<City> newCities;
vector<string> newCityNames;
ofstream outfile;
vector<string> citiesAndCoordinates;
vector<string> citiesConnectionCosts;
vector<string> newCitiesAndCoordinates;
unordered_set<string> centralCitiesSet;
map<pair<string, string>, route_info> routes;

typedef pair<int, int> iPair;

vector<string> getPath(int p[MAX2][MAX2], int origin, int destiny) {
  vector<string> path;
  if (p[origin][destiny] != -1) {
    getPath(p, origin, p[origin][destiny]);
    path.push_back(cityNames[p[origin][destiny]]);
    getPath(p, p[origin][destiny], destiny);
  }
  return path;
}

// PART 2 IMPLEMENTATION BEGIN.
struct Graph {
  // V is amount of vertex, E is amount of edges.
  int V, E, costMSTKruskal;

  vector<pair<int, pair<string, string>>> edges;  // Kruskal.
  vector<vector<pair<string, int>>> adjacenceList;
  vector<pair<int, pair<string, string>>> selectedEdgesKruskal;

  Graph(int V, int E) {
    this->V = V;
    this->E = E;
    adjacenceList.resize(V);
    costMSTKruskal = 0;
  }

  void addEdge(string U, string V, int W) {
    int uPos = find(cityNames.begin(), cityNames.end(), U) - cityNames.begin();
    int vPos = find(cityNames.begin(), cityNames.end(), V) - cityNames.begin();
    edges.push_back({W, {U, V}});  // Costo, {Conexión de U -> V}.
    adjacenceList[uPos].push_back({V, W});
  }

  static bool cmp(const pair<int, pair<string, string>>& a,
                  const pair<int, pair<string, string>>& b) {
    return a.first < b.first;
  }

  void Load();
  void KruskalMST();
  void PrintEdgesKruskal();
};

/**
 * @brief populates route map used in part 3.
 *
 * @param adj_matrix
 * @param p
 * @param n_cities
 */
void populateRouteMap(int adj_matrix[MAX2][MAX2], int paths[MAX2][MAX2],
                      int n_cities) {
  for (int i = 0; i < n_cities; i++) {
    for (int j = i + 1; j < n_cities; j++) {
      string a_name = cityNames[i];
      string b_name = cityNames[j];
      int cost = adj_matrix[i][j];
      route_info info;
      info.cost = adj_matrix[i][j];
      info.visited_cities = getPath(paths, i, j);
      routes.insert({{a_name, b_name}, info});
    }
  }
}

// Disjoint sets
struct DisjointSets {
  int *parent, *rnk;
  int n;

  DisjointSets(int n) {
    this->n = n;
    parent = new int[n + 1];
    rnk = new int[n + 1];

    for (int i = 0; i <= n; i++) {
      rnk[i] = 0;
      parent[i] = i;
    }
  }

  // Find parent of node u.
  int find(int u) {
    if (u != parent[u]) parent[u] = find(parent[u]);

    return parent[u];
  }

  // Union using rank.
  void merge(int x, int y) {
    x = find(x);
    y = find(y);

    if (rnk[x] > rnk[y])
      parent[y] = x;
    else
      parent[x] = y;

    if (rnk[x] == rnk[y]) rnk[y]++;
  }
};

void Graph::Load() {
  string a, b;
  int c;
  int j = 0;

  for (int i = 0; i < E; i++) {
    a = citiesConnectionCosts[j];
    j++;
    b = citiesConnectionCosts[j];
    j++;
    c = stoi(citiesConnectionCosts[j]);
    j++;

    // add reading of citynames
    addEdge(a, b, c);
    addEdge(b, a, c);
  }
}

void Graph::KruskalMST() {
  costMSTKruskal = 0;
  sort(edges.begin(), edges.end(), cmp);
  DisjointSets disjointSet(V);

  for (auto it : edges) {
    string u = it.second.first;
    string v = it.second.second;
    int uPos = find(cityNames.begin(), cityNames.end(), u) - cityNames.begin();
    int vPos = find(cityNames.begin(), cityNames.end(), v) - cityNames.begin();
    int set_u = disjointSet.find(uPos);
    int set_v = disjointSet.find(vPos);

    if (set_u != set_v) {
      disjointSet.merge(uPos, vPos);
      costMSTKruskal += it.first;
      selectedEdgesKruskal.push_back({it.first, {u, v}});
    }
  }
}

void Graph::PrintEdgesKruskal() {
  // cout << "Selected edges Kruskal: ";

  for (auto it : selectedEdgesKruskal) {
    outfile << it.second.first << " - " << it.second.second << " " << it.first
            << endl;
  }

  outfile << endl;
}
// PART 2 IMPLEMENTATION END.

// PART 4 IMPLEMENTATION BEGIN.
void readArcs(int arr[MAX2][MAX2], int p[MAX2][MAX2], int m) {
  string origin, destiny;
  int cost;

  for (int i = 0; i < MAX; i++) {
    arr[i][i] = p[i][i] = 0;

    for (int j = 0; j < MAX; j++) {
      arr[i][j] = arr[j][i] = INT_MAX;  // Represents infinity.
      p[i][j] = p[j][i] = -1;           // Respresents non-existance.
    }
  }

  int j = 0;
  for (int i = 0; i < m; i++) {
    origin = citiesConnectionCosts[j];
    j++;
    destiny = citiesConnectionCosts[j];
    j++;
    cost = stoi(citiesConnectionCosts[j]);
    j++;

    int originPos =
        find(cityNames.begin(), cityNames.end(), origin) - cityNames.begin();
    int destinyPos =
        find(cityNames.begin(), cityNames.end(), destiny) - cityNames.begin();
    arr[originPos][destinyPos] = arr[destinyPos][originPos] = cost;
  }
}

/**
 * @brief Create a Non Central Graph object
 * 
 * @param adj_matrix graph that contains both central and non-central nodes
 * @param new_graph reference to graph that will be populated with only non-central nodes
 * @param n number of nodes on original graph 
 */
vector<vector<int>> createNonCentralGraph(int adj_matrix[MAX2][MAX2], int n) {
  // Get number of non-central nodes.
  int non_central_nodes = 0;
  vector<vector<int>> new_graph(n+1, vector<int>(n, 0));
  vector<string> non_central_names;

  // Count non-central nodes.
  for (int i = 0; i < n; i++) {
    if (centralCitiesSet.find(cityNames[i]) == centralCitiesSet.end()) {
      non_central_nodes++;
      non_central_names.push_back(cityNames[i]);
    }
  }

  // Create new graph with only non-central nodes.
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == j)
        new_graph[i][i] = INT_MAX;
      else
        new_graph[i][j] = routes[{cityNames[i], cityNames[j]}].cost;
    }
  }
  return new_graph;
}

// pair<vector<string>, int> non_central_tsp(vector<vector<int>>& non_central_g) {
//   vector<string> visits;
//   int cost = 0;
  
// }

/**
 * @brief Get pair's shortest routes
 *
 * @param arr adjacency matrix
 * @param p path
 * @param n
 */
void Floyd_Warshall(int arr[MAX2][MAX2], int p[MAX2][MAX2], int n) {
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (arr[i][k] != INT_MAX && arr[k][j] != INT_MAX &&
            arr[i][k] + arr[k][j] < arr[i][j]) {
          arr[i][j] = arr[i][k] + arr[k][j];
          p[i][j] = k;
        }
      }
    }
  }
}


void checkPath(int p[MAX2][MAX2], int origin, int destiny) {
  if (p[origin][destiny] != -1) {
    checkPath(p, origin, p[origin][destiny]);
    outfile << (cityNames[p[origin][destiny]]) << "->";
    checkPath(p, p[origin][destiny], destiny);
  }
}

void consults(int arr[MAX2][MAX2], int p[MAX2][MAX2], int q) {
  string origin, destiny;
  for (int i = 0; i < q; i++) {
    origin = centralCities[i];
    for (int j = i + 1; j < q; j++) {
      destiny = centralCities[j];
      int originPos =
          find(cityNames.begin(), cityNames.end(), origin) - cityNames.begin();
      int destinyPos =
          find(cityNames.begin(), cityNames.end(), destiny) - cityNames.begin();

      outfile << origin << " - " << destiny << endl;

      if (arr[originPos][destinyPos] == INT_MAX)
        outfile << "No path" << endl;
      else
        outfile << "Cost: " << arr[originPos][destinyPos] << " Path: " << origin
                << "->";
      checkPath(p, originPos, destinyPos);
      outfile << destiny << endl << endl;
    }
  }
}
// PART 4 IMPLEMENTATION END.

int main() {
  int consultsAmount = 0;
  string x;
  fstream newFile;

  outfile.open("checking.txt");

  // Read input file
  newFile.open("in02.txt");

  // n is amount of nodes, m is amount of arcs, q is amount of new cities.
  int n, m, q;

  newFile >> n >> m >> q;

  int k = -1;
  // Read all cities
  for (int i = 0; i < n * 4; i++) {
    string lastCity;
    newFile >> x;
    if (i % 4 == 0) {
      cityNames.push_back(x);
      k++;
    }

    if (i % 4 == 3 && x == "1") {
      centralCities.push_back(cityNames[k]);
      centralCitiesSet.insert(cityNames[k]);
      consultsAmount++;
    }
    citiesAndCoordinates.push_back(x);
  }
  cout << "Finished 1" << endl;

  for (int i = 0; i < m * 3; i++) {
    newFile >> x;
    citiesConnectionCosts.push_back(x);
  }
  cout << "Finished 2" << endl;

  for (int i = 0; i < q * 3; i++) {
    newFile >> x;
    newCitiesAndCoordinates.push_back(x);
  }
  cout << "Finished 3" << endl;
  newFile.close();

  // PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1
  // PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1
  // PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1
  // PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1 PARTE 1
  Graph G(n, m);
  outfile << "1 - Cableado óptimo de nueva conexión." << endl << endl;
  G.Load();
  G.KruskalMST();
  G.PrintEdgesKruskal();
  outfile << "Costo total: " << G.costMSTKruskal << endl;
  outfile << endl;
  outfile << "================================================================="
             "========"
          << endl;
  outfile << "================================================================="
             "========"
          << endl;
  outfile << endl;

  // PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2
  // PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2
  // PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2
  // PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2 PARTE 2
  string a, b;
  int c;
  int j = 0;
  for (int i = 0; i < n; i++) {
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
  for (int i = 0; i < m; i++) {
    a = citiesConnectionCosts[j];
    j++;
    b = citiesConnectionCosts[j];
    j++;
    c = stoi(citiesConnectionCosts[j]);
    j++;
  }

  j = 0;
  for (int i = 0; i < q; i++) {
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

  // PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3
  // PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3
  // PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3
  // PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3
  outfile << "3 - Caminos más cortos entre centrales." << endl << endl;

  // p is intermediate node with largest name.
  int arr2[MAX2][MAX2], p2[MAX2][MAX2];



  readArcs(arr2, p2, m);
  Floyd_Warshall(arr2, p2, n);
  consults(arr2, p2, consultsAmount);

  outfile << "================================================================="
             "========"
          << endl;
  outfile << "================================================================="
             "========"
          << endl;
  outfile << endl;
  // Registers cities between routes to later obscure central cities without losing record of them
  populateRouteMap(arr2, p2, n);
  // Graph with non-central cities only
  vector<vector<int>> new_graph = createNonCentralGraph(arr2, n);
  
  int noncentral_cost = tsp::tsp(new_graph, n, routes, cityNames);
  cout << "Finished 4" << endl;
  cout << "Hamiltonian path cost of non central nodes: " << noncentral_cost << endl;


  // PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4
  // PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4
  // PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4
  // PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4
  outfile << "4 - Conexión de nuevas colonias." << endl << endl;

  for (int i = 0; i < newCities.size(); i++) {
    float minDistance = INT_MAX;
    string tempBestCity = "";
    string tempNewCityName = newCityNames[i];
    float tempNewCityX = newCities[i].xCoord;
    float tempNewCityY = newCities[i].yCoord;

    for (int j = 0; j < cities.size(); j++) {
      string tempCityName = cityNames[j];
      float tempCityX = cities[j].xCoord;
      float tempCityY = cities[j].yCoord;

      float tempCost = sqrt(pow((tempCityX - tempNewCityX), 2) +
                            pow((tempCityY - tempNewCityY), 2));

      if (tempCost < minDistance) {
        minDistance = tempCost;
        tempBestCity = tempCityName;
      }
    }

    outfile << tempNewCityName << " debe conectarse con " << tempBestCity
            << endl;
    minDistance = INT_MAX;
  }
  return 0;
}
