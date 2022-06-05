#include <algorithm>
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

#include "include/tsp_noncentral.cpp"

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
void readArcs(int arr[MAX][MAX], int p[MAX][MAX], int m) {
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

// pair<vector<string>, int> non_central_tsp(vector<vector<int>>& non_central_g)
// {
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

void checkPath(int p[MAX][MAX], int origin, int destiny) {
  if (p[origin][destiny] != -1) {
    checkPath(p, origin, p[origin][destiny]);
    outfile << (cityNames[p[origin][destiny]]) << "->";
    checkPath(p, p[origin][destiny], destiny);
  }
}

/**
 * @brief Reads input file and creates a graph. I really did not want to mess
 * with previous code.
 *
 * @param filename string pointing to file
 * @param mat int[MAX][MAX] representing a graph
 * @param n number of items in the graph
 * @param arcs number of arcs
 * @param city_map map relating city names to their positions in the graph
 */
void read_data(string filename, int mat[MAX][MAX], int n, int arcs,
               unordered_map<string, int>& city_map,
               unordered_set<string>& central_cities) {
  ifstream file(filename);
  int num, m, c;
  file >> num >> m >> c;
  for (int i = 0; i < num; i++) {
    string name;
    int x, y, central;
    file >> name >> x >> y >> central;
    if (central == 1) {
      central_cities.insert(name);
    }
  }

  for (int i = 0; i < arcs; i++) {
    string origin, destiny;
    int cost;
    file >> origin >> destiny >> cost;
    mat[city_map[origin]][city_map[destiny]] = cost;
    mat[city_map[destiny]][city_map[origin]] = cost;
  }
}

void init_path_mat(int p[MAX][MAX], int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      p[i][j] = -1;
    }
  }
}

void consults(int arr[MAX][MAX], int p[MAX][MAX], int q) {
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

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <input_file>" << endl;
    return 1;
  }
  string filename = argv[1];

  int consultsAmount = 0;
  string x;
  fstream newFile;

  outfile.open("checking.txt");

  // Read input file
  newFile.open(filename);

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
  outfile << "2 - La ruta mas optima." << endl << endl;

  int matrix[MAX][MAX], paths[MAX][MAX];
  tsp_noncentral::iniciaMat(matrix);
  unordered_map<string, int> city_name_index;
  vector<string> non_centrals;

  for (int i = 0; i < n; i++) {
    city_name_index.insert({cityNames[i], i});
  }

  init_path_mat(paths, n);
  read_data(filename, matrix, n, m, city_name_index, centralCitiesSet);

  for (int i = 0; i < n; i++) {
    if (centralCitiesSet.find(cityNames[i]) == centralCitiesSet.end()) {
      non_centrals.push_back(cityNames[i]);
    }
  }

  tsp_noncentral::floyd(matrix, paths, n);
  int new_matrix[MAX][MAX];
  tsp_noncentral::crearNuevaMat(new_matrix, matrix, non_centrals.size(),
                                non_centrals, city_name_index);

  int optimal_cost = tsp_noncentral::tsp(new_matrix, non_centrals.size());
  tsp_noncentral::printPath(tsp_noncentral::tspPath, paths, non_centrals,
                            city_name_index, cityNames);

  // Write shortest path to file
  for (string city : tsp_noncentral::get_record()) {
    outfile << city << " -> ";
  }
  outfile << non_centrals[0];
  outfile << " (" << optimal_cost << ")" << endl;
  outfile << endl;

  cout << "Optimal cost: " << optimal_cost << endl;

  outfile << endl;
  outfile << "================================================================="
             "========"
          << endl;
  outfile << "================================================================="
             "========"
          << endl;
  outfile << endl;
  // PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3
  // PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3
  // PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3
  // PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3 PARTE 3
  outfile << "3 - Caminos más cortos entre centrales." << endl << endl;
  consults(matrix, paths, consultsAmount);

  outfile << "================================================================="
             "========"
          << endl;
  outfile << "================================================================="
             "========"
          << endl;
  outfile << endl;
  // PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4
  // PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4
  // PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4
  // PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4 PARTE 4
  outfile << "4 - Conexión de nuevas colonias." << endl << endl;
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
  cout << "Done!" << endl;
  return 0;
}
