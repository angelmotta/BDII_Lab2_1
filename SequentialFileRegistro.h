//
// Created by Angel Motta on 4/13/20.
//

#ifndef LAB2_1_SEQUENTIALFILEREGISTRO_H
#define LAB2_1_SEQUENTIALFILEREGISTRO_H

#include "Registro.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class SequentialFileRegistro {
private:
    std::string filename;

    /**
     * Generate new DataFile
     */
    void initFiles() {
        std::cout << "** init Files **\n";
        std::ofstream outDataFile;
        outDataFile.open(filename,std::ios::out | std::ios::trunc | std::ios::binary); // trunc 'discard existing content'
        outDataFile.close();
    }

        public:
    // Default Constructor
    SequentialFileRegistro() = default;

    // Custom Constructor
    SequentialFileRegistro(std::string _filename) {
        this->filename = _filename;
        initFiles();
    }

    /**
     * Write Operation
     */
    void insertAll(std::vector<Registro> registros) {
        std::cout << "\n** insertAll method **\n";
        std::sort(registros.begin(), registros.end());  // overload operator '<'
        int numElements = registros.size();
        int nextRecord = 2;
        int row = 1;
        std::fstream outFile;
        outFile.open(filename, std::ios::out |std::ios::app | std::ios::binary);
        for(auto registro : registros){
            if(row != numElements) {     // Update attribute of all records Except the last one
                registro.setNextRecord(nextRecord);     // Update nextRecord attribute
            }
            outFile << registro;    // Write object to file
            nextRecord++;
            row++;
        }
        outFile.close();
    }

    /**
     * Read Operations
     */

    /**
     * ScanFile: internal tool to verify all fields of DataFile
     */
     void scanFileDb(){
        std::cout << "\n** Scan File DB **\n";
        Registro record;
        std::ifstream inFile;
        inFile.open(filename, std::ios::in | std::ios::binary);
        while(inFile >> record) {
            record.showMetaData();
        }
        inFile.close();
     }
};

#endif //LAB2_1_SEQUENTIALFILEREGISTRO_H
