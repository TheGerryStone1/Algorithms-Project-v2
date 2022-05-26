#include <iostream>
#include <climits>
#include <vector>
#include <algorithm>

using namespace std;
#define MAX 100

vector<string> cityNames = {"Condesa", "DelValle", "Polanco", "RomaNorte", "Pantitlán", "Juárez", "Tlatelolco", "SantaFe", "Cosmopolita", "Merced"};
vector<string> centralCities;

struct City{
    string cityName;
    int xCoord;
    int yCoord;
    int isCentral;
};

void readArcs(int arr[MAX][MAX], int p[MAX][MAX], int m)
{
    string origin, destiny;
    int cost;
    
    for(int i = 0; i < MAX; i++)
    {
        arr[i][i] = p[i][i] = 0;

        for(int j = 0; j < MAX; j++)
        {
            arr[i][j] = arr[j][i] = INT_MAX; //Represents infinity.
            p[i][j] = p[j][i] = -1; //Respresents non-existance.
        }
    }

    for(int i = 0; i < m; i++)
    {
        cin >> origin >> destiny >> cost;
        int originPos = find(cityNames.begin(), cityNames.end(), origin) - cityNames.begin();
        int destinyPos = find(cityNames.begin(), cityNames.end(), destiny) - cityNames.begin();
        arr[originPos][destinyPos] = arr[destinyPos][originPos] = cost;
    }
}

void Floyd_Warshall(int arr[MAX][MAX], int p[MAX][MAX], int n)
{
    for(int k = 0; k < n; k++)
    {
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < n; j++)
            {
                if(arr[i][k] != INT_MAX && arr[k][j] != INT_MAX && arr[i][k] + arr[k][j] < arr[i][j])
                {
                    arr[i][j] = arr[i][k] + arr[k][j];
                    p[i][j] = k;
                }
            }
        }
    }
}

void checkPath(int p[MAX][MAX], int origin, int destiny)
{
    if (p[origin][destiny] != -1)
    {
        checkPath(p, origin, p[origin][destiny]);
        cout << (cityNames[p[origin][destiny]]) << "->";
        checkPath(p, p[origin][destiny], destiny);
    }
}

void consults(int arr[MAX][MAX], int p[MAX][MAX], int q)
{
    string origin = centralCities[0], destiny;

    for(int i = 1; i < q; i++)
    {
        destiny = centralCities[i];
        int originPos = find(cityNames.begin(), cityNames.end(), origin) - cityNames.begin();
        int destinyPos = find(cityNames.begin(), cityNames.end(), destiny) - cityNames.begin();
        
        cout << origin << " - " << destiny << endl;
        
        if(arr[originPos][destinyPos] == INT_MAX) cout << "No path" << endl;
        else cout << "Cost: " << arr[originPos][destinyPos] << " Path: " << origin << "->";
        checkPath(p, originPos, destinyPos);
        cout << destiny << endl;
    }
}

void printArr(int arr[MAX][MAX], int p[MAX][MAX], int n)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            cout << arr[i][j] << "\t";
        }

        cout << endl;
    }

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            cout << p[i][j] << "\t";
        }
        
        cout << endl;
    }
}

int main()
{
    //p is intermediate node with largest name.
    int arr[MAX][MAX], p[MAX][MAX];
    //n is amount of nodes, m is amount of arcs, q is amount of consults, t is amount of cases.
    int n, m, q = 0;
    string nombre;
    int coordX, coordY, isCentral;

    cin >> n >> m;

    for(int i = 0; i < n; i++)
    {
        cin >> nombre >> coordX >> coordY >> isCentral;
        if(isCentral == 1)
        {
            q++;
            centralCities.push_back(nombre);
        }
    }

    readArcs(arr, p, m);
    Floyd_Warshall(arr, p, n);
    consults(arr, p, q);
    //printArr(arr, p, n);
}