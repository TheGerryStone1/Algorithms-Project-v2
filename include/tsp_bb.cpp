// Francisco Zamora Treviño A01570484
#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <vector>

struct route_info {
  int cost;
  vector<string> visited_cities;
};


using namespace std;
/**
 * @brief represents a Branch and Bound solution tree node
 *
 */
namespace tsp {
struct node_s {
  int level;
  int accumulated_cost;
  int possible_cost;
  int last_vertex;
  int curr_vertex;
  vector<bool> visited;
  bool operator<(const node_s& other) const {
    return possible_cost >= other.possible_cost;
  }
  node_s(unsigned int vertices_count) {
    level = 0;
    accumulated_cost = 0;
    possible_cost = 0;
    last_vertex = 0;
    curr_vertex = 1;
    visited = vector<bool>(vertices_count + 1, false);
    visited[1] = true;
  }
};
/**
 * @brief populates the adjacency with diagonal zeroes and infinite weights
 * where no edge exists O(n^2)
 * @param graph adjacency matrix
 * @param n number of vertices
 */
void initialize_graph(vector<vector<int>>& graph, int n) {
  for (int i = 0; i < n; i++) {
    graph[i][i] = 0;
    for (int j = i + 1; j < n + 1; j++) {
      graph[i][j] = graph[j][i] = INT_MAX;
    }
  }
}

/**
 * @brief reads data from cin and populates the adjacency matrix, expects nodes
 * to be represented by capitalized letters O(n)
 * @param graph adjacency matrix
 * @param n number of vertices
 */
void read_arcs(vector<vector<int>>& graph, int n) {
  int c;
  char a, b;
  for (int i = 0; i < n; i++) {
    cin >> a >> b >> c;
    graph[a - 'A' + 1][b - 'A' + 1] = graph[b - 'A' + 1][a - 'A' + 1] = c;
  }
}
/**
 * @brief calculates the possible cost of a child node in the solution tree
 * O(n^2)
 * @param node prospect part of the solution tree
 * @param graph adjacency matrix
 * @param n number of vertices
 */
void calculate_possible_cost(node_s& node, vector<vector<int>>& graph, int n) {
  node.possible_cost = node.accumulated_cost;
  int obtained_cost = 0;

  for (int i = 1; i <= n; i++) {
    obtained_cost = INT_MAX;
    if (!node.visited[i] || i == node.curr_vertex) {
      if (!node.visited[i]) {
        for (int j = 1; j <= n; j++) {
          if (i != j && (!node.visited[j] || j == 1)) {
            obtained_cost = min(obtained_cost, graph[i][j]);
          }
        }
      } else {
        for (int j = 1; j <= n; j++) {
          if (!node.visited[j]) {
            obtained_cost = min(obtained_cost, graph[i][j]);
          }
        }
      }
    }
    node.possible_cost += obtained_cost;
  }
}
/**
 * @brief Travelling Salesman Branch and Bound implementation
 * O(n^2 * 2^n)
 * @param graph
 * @param n
 * @return int
 */
int tsp(vector<vector<int>>& graph, int n) {
  int min_cost = INT_MAX;
  node_s root(n);
  calculate_possible_cost(root, graph, n);
  priority_queue<node_s> q;
  q.push(root);
  while (!q.empty()) {
    // Pop queue
    node_s node = q.top();
    q.pop();
    // Check if possible cost < optimal cost
    if (node.possible_cost < min_cost) {
      // If so, generate all possible children of that node
      for (int i = 1; i <= n; i++) {
        if (!node.visited[i] && graph[node.curr_vertex][i] != INT_MAX) {
          node_s child = node;
          child.curr_vertex = i;
          child.last_vertex = node.curr_vertex;
          child.accumulated_cost =
              node.accumulated_cost + graph[node.curr_vertex][i];
          child.visited[i] = true;
          child.level = node.level + 1;
          calculate_possible_cost(child, graph, n);
          // If node is on level n-2 then check for missing node and use it to
          // calculate total cost
          if (child.level == n - 2) {
            // Find last non-visited vertex
            int last_vertex;
            for (int j = 1; j <= n; j++) {
              if (!child.visited[j]) {
                last_vertex = j;
                break;
              }
            }
            // Calculate total cost
            if (graph[i][last_vertex] != INT_MAX &&
                graph[last_vertex][1] != INT_MAX) {
              child.accumulated_cost += graph[i][last_vertex];
              child.accumulated_cost += graph[1][last_vertex];
              min_cost = min(child.accumulated_cost, min_cost);
            }
          } else if (child.level < n - 2 && child.possible_cost < min_cost) {
            q.push(child);
          }
        }
      }
    }
  }
  return min_cost;
}
/**
 * @brief prints the adjacency matrix
 * O(n^2)
 * @param graph adjacency matrix
 * @param n number of vertices
 */
void print_graph(vector<vector<int>>& graph, int n) {
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      cout << graph[i][j] << '\t';
    }
    cout << endl;
  }
}
}  
// namespace tsp
// int main() {
//   int nodes, arcs;
//   cin >> nodes >> arcs;
//   vector<vector<int>> graph(
//       vector<vector<int>>(nodes + 1, vector<int>(arcs, 0)));
//   tsp::initialize_graph(graph, nodes);
//   tsp::read_arcs(graph, arcs);
//   // print_graph(graph, nodes);
//   int ans = tsp::tsp(graph, nodes);
//   cout << (ans != INT_MAX ? to_string(ans) : "INF") << endl;
// }
/*
4 5
A B 5
A C 10
A D 8
B C 2
C D 1
*/