#include <iostream>
#include <vector>
#include <algorithm>
#include "Registro.h"
#include "SequentialFileRegistro.h"

using namespace std;

int main() {
    Registro reg1 = Registro("001", "CS", 5);
    Registro reg2 = Registro("002", "CS", 5);
    Registro reg5 = Registro("005", "CS", 5);
    Registro reg6 = Registro("006", "CS", 5);
    vector<Registro> registros;     // vector desordenado
    registros.push_back(reg2);
    registros.push_back(reg1);
    registros.push_back(reg6);
    registros.push_back(reg5);


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
     * P1. b)
     */
     int res = myFileRegistroDb.binarySearch("004");
     std::cout << "binarySearch Result: record is located in row " << res << '\n';
     /**
      * P1. c)
      */
    // Insert
    Registro reg3 = Registro("003", "CS", 5);
    myFileRegistroDb.add(reg3);
    myFileRegistroDb.scanFileDb();

    // Insert
    Registro reg8 = Registro("008", "CS", 5);
    myFileRegistroDb.add(reg8);
    myFileRegistroDb.scanFileDb();

    // Insert
    Registro reg9 = Registro("009", "CS", 5);
    myFileRegistroDb.add(reg9);
    myFileRegistroDb.scanFileDb();

    // Insert
    Registro reg10 = Registro("010", "CS", 5);
    myFileRegistroDb.add(reg10);
    myFileRegistroDb.scanFileDb();

    return 0;
}
