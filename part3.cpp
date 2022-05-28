#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>

using namespace std;

#define MAX 21

struct City {
  string cityName;
  int xCoord;
  int yCoord;
  int isCentral;
};

struct Node {
  int level;
  int accumCost;
  int possibleCost;
  int previousVertex;
  int actualVertex;
  bool visited[MAX] = {false};
  bool operator<(const Node &other) const {
    return possibleCost >= other.possibleCost;
  }
};

vector<City> cities;
vector<string> cityNames;

void rootMatrix(int arr[MAX][MAX]) {
  for (int i = 0; i < MAX; i++) {
    arr[i][i] = 0;

    for (int j = i + 1; j < MAX; j++) {
      arr[i][j] = arr[j][i] = INT_MAX;
    }
  }
}

void readArcs(int arr[MAX][MAX], int m) {
  int cost;
  string origin, destination;
  int originIndex, destinationIndex;

  for (int i = 0; i < m; i++) {
    cin >> origin >> destination >> cost;
    originIndex =
        find(cityNames.begin(), cityNames.end(), origin) - cityNames.begin();
    destinationIndex = find(cityNames.begin(), cityNames.end(), destination) -
                       cityNames.begin();
    arr[originIndex + 1][destinationIndex + 1] =
        arr[destinationIndex + 1][originIndex + 1] = cost;
  }
}

void calcPossCost(Node actualNode, int arr[MAX][MAX], int n) {
  actualNode.possibleCost = actualNode.accumCost;
  int obtainedCost;

  for (int i = 1; i <= n; i++) {
    obtainedCost = INT_MAX;

    if (!actualNode.visited[i] || i == actualNode.actualVertex) {
      if (!actualNode.visited[i]) {
        for (int j = 1; j <= n; j++) {
          if (i != j && (!actualNode.visited[j] || j == 1))
            obtainedCost = min(obtainedCost, arr[i][j]);
        }
      }

      else {
        for (int j = 1; j <= n; j++) {
          if (!actualNode.visited[j])
            obtainedCost = min(obtainedCost, arr[i][j]);
        }
      }
    }

    actualNode.possibleCost += obtainedCost;
  }
}

int BranchAndBoundTSP(int arr[MAX][MAX], int n) {
  int optimalCost = INT_MAX;
  Node root;
  root.level = 0;
  root.accumCost = 0;
  root.possibleCost = 0;
  root.actualVertex = 1;
  root.previousVertex = 0;
  root.visited[1] = true;
  calcPossCost(root, arr, n);
  priority_queue<Node> priorityQueue;
  priorityQueue.push(root);

  while (!priorityQueue.empty()) {
    Node curr = priorityQueue.top();
    priorityQueue.pop();

    if (curr.possibleCost < optimalCost) {
      for (int i = 1; i <= n; i++) {
        if (!curr.visited[i] && arr[curr.actualVertex][i] != INT_MAX) {
          Node child = curr;
          child.actualVertex = i;
          child.previousVertex = curr.actualVertex;
          child.visited[i] = true;
          child.level = curr.level + 1;
          child.accumCost = curr.accumCost + arr[curr.actualVertex][i];
          calcPossCost(child, arr, n);

          if (child.level == n - 2) {
            int lastVertex;

            for (int j = 1; j <= n; j++) {
              if (!child.visited[j]) lastVertex = j;
            }

            if (arr[i][lastVertex] != INT_MAX &&
                arr[lastVertex][1] != INT_MAX) {
              child.accumCost += arr[i][lastVertex];
              child.accumCost += arr[lastVertex][1];

              if (child.accumCost < optimalCost) optimalCost = child.accumCost;
            }
          }

          else if (child.level < n - 2) {
            if (child.possibleCost < optimalCost) priorityQueue.push(child);
          }
        }
      }
    }
  }

  return optimalCost;
}

int main() {
  // n is amount of nodes, m is amount of arcs.
  int n, m, newCities;
  cin >> n >> m >> newCities;

  for (int i = 0; i < n; i++) {
    string cityName;
    int xCoord, yCoord, isCentral;
    cin >> cityName >> xCoord >> yCoord >> isCentral;
    City tempCity;
    tempCity.cityName = cityName;
    tempCity.xCoord = xCoord;
    tempCity.yCoord = yCoord;
    tempCity.isCentral = isCentral;
    cities.push_back(tempCity);
    cityNames.push_back(cityName);
  }

  int arr[MAX][MAX];
  rootMatrix(arr);
  readArcs(arr, m);
  cout << BranchAndBoundTSP(arr, n) << endl;
}