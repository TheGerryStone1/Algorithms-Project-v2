#include <iostream>
#include <queue>
#include <climits>
#include <algorithm>
#include <cmath>

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

int main()
{
    //n is amount of nodes, m is amount of arcs.
    string a, b;
    int c, n, m, q;
    cin >> n >> m >> q;

    for(int i = 0; i < n; i++)
    {
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

    for(int i = 0; i < m; i++)
    {
        cin >> a >> b >> c;
        //add reading of citynames
    }

    for(int i = 0; i < q; i++)
    {
        string newCityName;
        int xCoord, yCoord;
        City tempNewCity;
        cin >> newCityName >> xCoord >> yCoord;
        City newTempCity;
        newTempCity.cityName = newCityName;
        newTempCity.xCoord = xCoord;
        newTempCity.yCoord = yCoord;
        newTempCity.isCentral = 0;
        newCities.push_back(newTempCity);
        newCityNames.push_back(newCityName);
    }

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

        cout << tempNewCityName << " debe conectarse con " << tempBestCity << endl;
        minDistance = 0.0;
    }
}