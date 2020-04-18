//
// Created by Angel Motta on 4/17/20.
//
#ifndef LAB2_1_RANDOMDIRECTFILEREGISTRO_H
#define LAB2_1_RANDOMDIRECTFILEREGISTRO_H

#include "Registro.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

class RandomDirectFileRegistro{
private:
    std::string indexFile;
    std::string dataFile;

    void updateIndexFile(Registro& registro, int row){
        std::fstream outIndexFile;
        outIndexFile.open(indexFile, std::ios::out |std::ios::app | std::ios::binary);
        if(outIndexFile.is_open()){
            std::cout << "Write Index\n";
            outIndexFile.write(registro.getCodigo(),  5);       // Write Key
            outIndexFile.write((char *)(&row), sizeof(row));    // Write Row number
            outIndexFile << "\n";
            outIndexFile << std::flush;
        }
        outIndexFile.close();
    }

    void initFiles() {
        std::cout << "** init Files **\n";
        std::ofstream outDataFile, outAuxFile;
        outDataFile.open(dataFile,std::ios::out | std::ios::trunc | std::ios::binary); // trunc 'discard existing content'
        outDataFile.close();
        outAuxFile.open(indexFile,std::ios::out | std::ios::trunc | std::ios::binary); // trunc 'discard existing content'
        outAuxFile.close();
    }

    void readRecord(int row){
        std::cout << "- Read record from data File (disk) -\n";
        Registro record;
        int sizeRecord = sizeof(record) + 1;
        std::ifstream inFile;
        inFile.open(dataFile, std::ios::in | std::ios::binary);
        inFile.seekg(sizeRecord * (row - 1), std::ios::beg);
        inFile >> record;
        record.showMetaData();
        inFile.close();
    }

public:
    RandomDirectFileRegistro() = default;

    RandomDirectFileRegistro(std::string _dataFile){
        dataFile = _dataFile;
        indexFile = "index_" + dataFile;
        initFiles();
    }

    void insertAll(std::vector<Registro> registros) {
        std::cout << "\n** insertAll method **\n";
        std::fstream outFile;
        outFile.open(dataFile, std::ios::out |std::ios::app | std::ios::binary);
        std::string key;
        int row = 1;
        for(auto registro : registros){
            // Write object to Data file
            outFile << registro;
            // Update index File
            updateIndexFile(registro, row);
            row++;
        }
        outFile.close();
    }

    /**
     * Load to Memory Random File
     */
    std::map<std::string, int> loadIndexFile(){
        std::cout << "\n** Load Index File Method **\n";
        std::map<std::string, int> indexMap;
        std::ifstream inFile;
        inFile.open(indexFile, std::ios::in | std::ios::binary);
        // Check number of rows
        inFile.seekg(0, std::ios::end);
        int pos = inFile.tellg();
        int sizeRecord = 5 + 4 + 1;         // size of record in Index File
        int numRows =  (pos / (sizeRecord));
        // Start Reading Index File
        inFile.seekg(0, std::ios::beg);
        char codigo[5];
        int rowInDataFile;
        std::string codigoKey;
        for(int i = 1; i <= numRows; i++){
            inFile.read(codigo, 5);
            inFile.read(reinterpret_cast<char *>(&rowInDataFile), sizeof(rowInDataFile));
            codigoKey = codigo;
            indexMap.insert({codigoKey, rowInDataFile});
            inFile.get();   // read endLine character
        }
        inFile.close();
        return indexMap;
    }

    void search(std::map<std::string, int>& indexMap, std::string key){
        std::cout << "\n*** Search method ***\n";
        std::cout << "search key '" << key << "' in map (Memory)\n";
        auto itrResult = indexMap.find(key);
        if(itrResult != indexMap.end()){
            std::cout << "Found row value: " << itrResult->second << "\n";
            readRecord(itrResult->second);
        }
        else{
            std::cout << "Key does not exist\n";
        }
    }

    /**
     * ScanFile: internal tool to verify all fields of DataFile and Index File
     */
    void scanFileDb(){
        std::cout << "\n** Scan File DB **\n";
        Registro record;
        std::ifstream inFile;
        inFile.open(dataFile, std::ios::in | std::ios::binary);
        // Read Data File
        std::cout << "\n** Data File DB **\n";
        while(inFile >> record) {
            record.showMetaData();
        }
        inFile.close();
        // Read Aux File
        inFile.open(indexFile, std::ios::in | std::ios::binary);
        std::cout << "\n** Index File DB **\n";
        inFile.seekg(0, std::ios::end);
        int pos = inFile.tellg();
        int sizeRecord = 5 + 4 + 1;         // size of record in Index File
        int numRows =  (pos / (sizeRecord));
        inFile.seekg(0, std::ios::beg);
        char codigo[5];
        int rowInDataFile;
        for(int i = 1; i <= numRows; i++){
            inFile.read(codigo, 5);
            std::cout << codigo << ", ";
            inFile.read(reinterpret_cast<char *>(&rowInDataFile), sizeof(rowInDataFile));
            std::cout << rowInDataFile << "\n";
            inFile.get();   // read endLine character
        }
        inFile.close();
    }

    void add(Registro registro) {
        std::cout << "\n*** Add method ***\n";
        std::fstream outFile;
        outFile.open(dataFile, std::ios::out |std::ios::app | std::ios::binary);
        // Get row number
        int pos = outFile.tellg();
        int sizeRecord = sizeof(registro) + 1;
        int rowNewRecord = (pos / sizeRecord) + 1;
        outFile << registro;
        // Update Index (Map and File Index)
        updateIndexFile(registro, rowNewRecord);
    }
};

#endif //LAB2_1_RANDOMDIRECTFILEREGISTRO_H
