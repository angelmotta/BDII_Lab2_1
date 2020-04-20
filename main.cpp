#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

#include "Registro.h"
#include "SequentialFileRegistro.h"
#include "RandomDirectFileRegistro.h"

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
     * P1. Sequential File
     */

    /**
     * P1. a)
     * InsertAll(vector<Registro> registros)
     */
//    SequentialFileRegistro myFileRegistroDb("registros.txt");
//    // Insert all records to data File
//    myFileRegistroDb.insertAll(registros);
//    // Check metadata and verify (internal tool)
//    myFileRegistroDb.scanFileDb();

    /**
     * P1. b)
     * binarySearch realiza un numero de Accesos a disco que es lg(n)
     */
//     int res = myFileRegistroDb.binarySearch("004");
//     std::cout << "binarySearch Result: record is located in row " << res << '\n';
     /**
      * P1. c)
      */
    // Insert
//    Registro reg3 = Registro("003", "CS", 5);
//    myFileRegistroDb.add(reg3);
//    myFileRegistroDb.scanFileDb();
//
//    // Insert
//    Registro reg8 = Registro("008", "CS", 5);
//    myFileRegistroDb.add(reg8);
//    myFileRegistroDb.scanFileDb();
//
//    // Insert
//    Registro reg9 = Registro("009", "CS", 5);
//    myFileRegistroDb.add(reg9);
//    myFileRegistroDb.scanFileDb();
//
//    // Insert
//    Registro reg10 = Registro("010", "CS", 5);
//    myFileRegistroDb.add(reg10);
//    myFileRegistroDb.scanFileDb();
//
//    // Insert until reach 10 records
//    Registro reg11 = Registro("011", "CS", 5);
//    Registro reg12 = Registro("012", "CS", 5);
//    Registro reg13 = Registro("013", "CS", 5);
//    Registro reg14 = Registro("014", "CS", 5);
//    Registro reg15 = Registro("015", "CS", 5);
//    Registro reg16 = Registro("016", "CS", 5);
//    myFileRegistroDb.add(reg11);
//    myFileRegistroDb.add(reg12);
//    myFileRegistroDb.add(reg13);
//    myFileRegistroDb.add(reg14);
//    myFileRegistroDb.add(reg15);
//    myFileRegistroDb.scanFileDb();
//    myFileRegistroDb.add(reg16);
//    myFileRegistroDb.scanFileDb();


    /**
     * P2. Random File
     */

    /**
     * P2. a)
     */

    std::cout << "\n****** P2. Random File *******\n";

    // P2. a)
    // Create Random File (disk)
    RandomDirectFileRegistro myRandomFileDb("randomDB.dat");
    myRandomFileDb.insertAll(registros);
    myRandomFileDb.scanFileDb();

    // Load to memory Index File (Create Map)
    std::map<std::string, int> indexMap = myRandomFileDb.loadIndexFile();

    // P2. b)
    // Busqueda usando key-record in index Map
    myRandomFileDb.search(indexMap, "006");     // search codigo '005'
    myRandomFileDb.search(indexMap, "008");     // search codigo '008'

    // P2. c)
    // Agregar nuevo registro
    Registro reg20 = Registro("020", "CS", 4);
    myRandomFileDb.add(reg20, indexMap);    // Update map and index file in disk
    myRandomFileDb.scanFileDb();

    myRandomFileDb.search(indexMap, "020");
    return 0;
}
