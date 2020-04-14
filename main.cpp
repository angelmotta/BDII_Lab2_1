#include <iostream>
#include <vector>
#include <algorithm>
#include "Registro.h"
#include "SequentialFileRegistro.h"

using namespace std;

int main() {
    Registro reg1 = Registro("001", "CS", 5);
    Registro reg2 = Registro("002", "CS", 5);
    Registro reg3 = Registro("003", "CS", 5);
    Registro reg4 = Registro("004", "CS", 5);
    Registro reg5 = Registro("005", "CS", 5);
    Registro reg6 = Registro("006", "CS", 5);

    vector<Registro> registros;     // vector desordenado
    registros.push_back(reg3);
    registros.push_back(reg2);
    registros.push_back(reg1);
    registros.push_back(reg6);
    registros.push_back(reg5);
    registros.push_back(reg4);

    /**
     * P1. a)
     * InsertAll(vector<Registro> registros)
     */
    SequentialFileRegistro myFileRegistroDb("registros.txt");
    // Insert all records to data File
    myFileRegistroDb.insertAll(registros);
    // Check metadata and verify (internal tool)
    myFileRegistroDb.scanFileDb();

    /**
     * P2. b)
     */
     myFileRegistroDb.binarySearch("004");
    return 0;
}
