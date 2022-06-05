#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

#define MAX 21

// Stores visited cities in the order of traversal
namespace tsp_noncentral {
vector<string> record;
/* Nuevo */ vector<int> tspPath;

struct Nodo {
  int niv;
  int costoAcum;
  int costoPos;
  int verticeAnterior;
  int verticeActual;
  bool visitados[MAX] = {false};
  /* Nuevo */ vector<int> camino;
  bool operator<(const Nodo &otro) const {  // Para que la fila priorizada tenga
                                            // prioridad de costo posible menor
    return costoPos >= otro.costoPos;
  }
};

void iniciaMat(int matAdj[MAX][MAX]) {
  for (int i = 0; i < MAX; i++) {
    matAdj[i][i] = 0;
    for (int j = i + 1; j < MAX; j++) {
      matAdj[i][j] = matAdj[j][i] = INT_MAX;
    }
  }
}

// Algoritmo de floyd, no motifique nada de esto lo copie directo de la file que
// hicimos en clase
void floyd(int mat[MAX][MAX], int p[MAX][MAX], int n) {
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (mat[i][k] != INT_MAX && mat[k][j] != INT_MAX &&
            mat[i][k] + mat[k][j] < mat[i][j]) {
          mat[i][j] = mat[i][k] + mat[k][j];
          p[i][j] = k;
        }
      }
    }
  }
}

// Esta funcion llena la matriz de adjecencia nueva con la matriz de adjecencia
// anterior. Pasa por el vector de nombres de ciudades no centrales, saca el
// indice de la matriz original con ese nombre utilizando cityNamesToIdx. Copia
// los elementos a la nueva matriz en la posicion i, j.
void crearNuevaMat(int newMatAdj[MAX][MAX], int matAdj[MAX][MAX], int n,
                   vector<string> non_central_cities,
                   unordered_map<string, int> cityNamesToIdx) {
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      newMatAdj[i][j] = newMatAdj[j][i] =
          matAdj[cityNamesToIdx[non_central_cities[i]]]
                [cityNamesToIdx[non_central_cities[j]]];
    }
  }
}

/* Nuevo */
void printCentrals(int origin, int destiny, int path[MAX][MAX],
                   vector<string> cities) {
  if (path[origin][destiny] != -1) {
    printCentrals(origin, path[origin][destiny], path, cities);
    cout << cities[path[origin][destiny]] << " -> ";
    record.push_back(cities[path[origin][destiny]]);
    printCentrals(path[origin][destiny], destiny, path, cities);
  }
}

/* Nuevo */
void printPath(vector<int> tspP, int path[MAX][MAX],
               vector<string> non_central_cities,
               unordered_map<string, int> cityNamesToIdx,
               vector<string> cities) {
  for (int i = 0; i < tspP.size() - 1; i++) {
    cout << non_central_cities[tspP[i]] << " -> ";
    record.push_back(non_central_cities[tspP[i]]);
    printCentrals(cityNamesToIdx[non_central_cities[tspP[i]]],
                  cityNamesToIdx[non_central_cities[tspP[i + 1]]], path,
                  cities);
  }
  cout << non_central_cities[tspP[tspP.size() - 1]] << endl;
}

// Nada de esta funcion se modifico.
void calculaCostoPosible(Nodo &nodo, int matAdj[MAX][MAX], int n) {
  nodo.costoPos = nodo.costoAcum;
  int costoObtenido;
  for (int i = 1; i <= n; i++) {
    costoObtenido = INT_MAX;
    if (!nodo.visitados[i] || i == nodo.verticeActual) {
      if (!nodo.visitados[i]) {
        for (int j = 1; j <= n; j++) {
          if (i != j && (!nodo.visitados[j] || j == 1)) {
            costoObtenido = min(costoObtenido, matAdj[i][j]);
          }
        }
      } else {
        for (int j = 1; j <= n; j++) {
          if (!nodo.visitados[j]) {
            costoObtenido = min(costoObtenido, matAdj[i][j]);
          }
        }
      }
    }
    nodo.costoPos += costoObtenido;
  }
}

vector<string>& get_record() { return record; }

// Complejidad O(2^n)
// *MODIFICADO A FUNCIONAR COMENZANDO EN 0 EN LUGAR DE  1*
int tsp(int matAdj[MAX][MAX], int n) {
  int costoOptimo = INT_MAX;
  Nodo raiz;
  raiz.niv = 0;
  raiz.costoAcum = 0;
  raiz.verticeActual = 0;
  raiz.verticeAnterior = 0;
  raiz.visitados[0] = true;
  /* Nuevo */ raiz.camino.push_back(0);
  calculaCostoPosible(raiz, matAdj, n);
  priority_queue<Nodo> pq;
  pq.push(raiz);
  while (!pq.empty()) {
    // Sacar de pq;
    Nodo actual = pq.top();
    pq.pop();
    // Ver si el costoPos < costoOptimo
    if (actual.costoPos < costoOptimo) {
      // SI si, generar todos los posibles hijos de este nodo
      for (int i = 0; i < n; i++) {
        if (!actual.visitados[i] &&
            matAdj[actual.verticeActual][i] != INT_MAX) {
          // Para cada hijo generar un nuevo nodo, actualizar los datos
          Nodo hijo = actual;
          hijo.verticeActual = i;
          hijo.verticeAnterior = actual.verticeActual;
          hijo.visitados[i] = true;
          hijo.niv = actual.niv + 1;
          hijo.costoAcum = actual.costoAcum + matAdj[actual.verticeActual][i];
          /* Nuevo */ hijo.camino = actual.camino;
          /* Nuevo */ hijo.camino.push_back(hijo.verticeActual);
          calculaCostoPosible(hijo, matAdj, n);
          // Cuando el nivel sea == n-2 calcular el costo real, si este mejora
          // al costo optimo actualizarlo
          if (hijo.niv == n - 2) {
            int ultimaVertice;
            for (int j = 0; j < n; j++) {
              if (!hijo.visitados[j]) {
                ultimaVertice = j;
              }
            }
            if (matAdj[i][ultimaVertice] != INT_MAX &&
                matAdj[ultimaVertice][0] != INT_MAX) {
              hijo.costoAcum += matAdj[i][ultimaVertice];
              hijo.costoAcum += matAdj[ultimaVertice][0];
              if (hijo.costoAcum < costoOptimo) {
                costoOptimo = hijo.costoAcum;
                /* Nuevo */ tspPath = hijo.camino;
                /* Nuevo */ tspPath.push_back(ultimaVertice);
                /* Nuevo */ tspPath.push_back(0);
              }
            }
          }
          // y cuando el nivel < n-2 ... checar si el costo posible es mejor
          // que el costo optimo y lo metes a la pq
          else if (hijo.niv < n - 2) {
            if (hijo.costoPos < costoOptimo) {
              pq.push(hijo);
            }
          }
        }
      }
    }
  }
  return costoOptimo;
}
}  // namespace tsp_noncentral

// int main() {
//   // n = cantidad de nodos
//   // m = cantidad de arcos
//   int n, m, nc;
//   cin >> n >> m >> nc;
//   int matAdj[MAX][MAX];  // Asumir es base-1 A=1 B=2 ...

//   // Inicializa la matriz de adjecencia poniendo INT_MAX en todos los elementos;
//   tsp_noncentral::iniciaMat(matAdj);

//   // Mapa que tiene un string de cuidad como llave, da el indice da la matriz de
//   // adjecencia original de la cuidad
//   unordered_map<string, int> cityNamesToIdx;

//   // Vector de strings con los nombres de todas las cuidades no centrales
//   vector<string> non_central_cities;

//   /* Nuevo */ vector<string> cities;

//   // arreglo de bool con indice de la cuidad en matriz de adjecencia original
//   // que dice si la cuidad de ese indice es o no central (Creo que no se
//   // utiliza, es posible que se pueda ignorar)
//   bool isCentral[MAX] = {true};

//   // En este for loop, vamos de 0 hasta la cantidad de nodos y llenamos el mapa
//   // de cityNamesToIdx En el caso de ser una cuidad no central, le damos
//   // pushback a non_central_cities con el nombre de la ciudad actual.
//   for (int i = 0; i < n; i++) {
//     int x, y, centralInt;
//     string tempCityName;
//     cin >> tempCityName >> x >> y >> centralInt;
//     cityNamesToIdx[tempCityName] = i;
//     /* Nuevo */ cities.push_back(tempCityName);
//     if (centralInt == 0) {
//       isCentral[i] = false;
//       non_central_cities.push_back(tempCityName);
//     }
//   }

//   // En este for loop se leen las conexiones entre cuidades, los valores se
//   // agregan a la matriz de adjecencia.
//   for (int i = 0; i < m; i++) {
//     string tempCity1, tempCity2;
//     int cost;
//     cin >> tempCity1 >> tempCity2 >> cost;
//     matAdj[cityNamesToIdx[tempCity1]][cityNamesToIdx[tempCity2]] =
//         matAdj[cityNamesToIdx[tempCity2]][cityNamesToIdx[tempCity1]] = cost;
//   }

//     for (int i=0; i<n; i++) {
//         for (int j=0; j<n; j++) {
//             cout << matAdj[i][j] << '\t';
//         }
//         cout << endl;
//     }
//   // Matriz path, es inicializada con todos los elementos en -1
//   int path[MAX][MAX];
//   for (int i = 0; i < MAX; i++) {
//     for (int j = i + 1; j < MAX; j++) {
//       path[i][j] = path[j][i] = -1;
//     }
//   }

//   // Modifica la matriz de adjecencia y llena los datos de matriz path.
//   // matriz de adjecencia ahora tiene la ruta mas corta entre dos nodos.
//   tsp_noncentral::floyd(matAdj, path, n);

//   // Se crea una nueva matriz de adjecencia para el nuevo grafo de solo cuidades
//   // no centrales.
//   int newMatAdj[MAX][MAX];
//   // Comentarios de esta funcion arriba de la funcion
//   tsp_noncentral::crearNuevaMat(newMatAdj, matAdj, non_central_cities.size(),
//                 non_central_cities, cityNamesToIdx);

//     // Print newMatAdj
//     for (int i=0; i<non_central_cities.size(); i++) {
//         for (int j=0; j<non_central_cities.size(); j++) {
//             cout << newMatAdj[i][j] << '\t';
//         }
//         cout << endl;
//     }
//   // TSP Utilizando la nueva matriz de adjecencia con el tamaño de las cuidades
//   // no centrales
//   int costoOptimo = tsp_noncentral::tsp(newMatAdj, non_central_cities.size());
//   if (costoOptimo != INT_MAX) {
//     cout << costoOptimo << endl;
//   } else {
//     cout << "INF" << endl;
//   }
//   /* Nuevo */ tsp_noncentral::printPath(tspPath, path, non_central_cities, cityNamesToIdx,
//                         cities);
// }