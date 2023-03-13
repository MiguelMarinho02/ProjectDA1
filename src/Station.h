
#ifndef PROJETODA1_STATION_H
#define PROJETODA1_STATION_H

#include <string>

using namespace std;

class Station {
public:
    Station(string name, string district, string municipality, string township, string line);
    Station(string name);
    string name;
    string district;
    string municipality;
    string township;
    string line;
};


#endif //PROJETODA1_STATION_H
