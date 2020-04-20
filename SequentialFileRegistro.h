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
    std::string dataFile;
    std::string auxFile;
    int rowsDataFile;
    int rowsAuxFile;
    /**
     * Generate new DataFile
     */
    void initFiles() {
        std::cout << "** init Files **\n";
        std::ofstream outDataFile, outAuxFile;
        outDataFile.open(dataFile,std::ios::out | std::ios::trunc | std::ios::binary); // trunc 'discard existing content'
        outDataFile.close();
        outAuxFile.open(auxFile,std::ios::out | std::ios::trunc | std::ios::binary); // trunc 'discard existing content'
        outAuxFile.close();
    }

    void getCodigoFromRow(int row, char* codigo) {       // used by BinarySearch
        Registro record;
        int sizeRecord = sizeof(record);
        std::ifstream inFile;
        inFile.open(dataFile, std::ios::in | std::ios::binary);
        inFile.seekg((sizeRecord * (row)) + (1 * row), std::ios::beg);
        inFile.read(codigo, 5);
        inFile.close();
        //std::cout << "Read from disk....getCodigoFromRow: " << codigo << '\n';
    }

    void getCodigoFromRowAux(int row, char* codigo) {       // used by BinarySearch
        Registro record;
        int sizeRecord = sizeof(record);
        std::ifstream inFile;
        inFile.open(auxFile, std::ios::in | std::ios::binary);
        inFile.seekg((sizeRecord * (row - 1)) + (1 * (row - 1)), std::ios::beg);
        inFile.read(codigo, 5);
        inFile.close();
        //std::cout << "Read from disk....getCodigoFromRowAux: " << codigo << '\n';
    }

    int getNextRecordFromRow(int row, int typeFile) {
        Registro record;
        int sizeRecord = sizeof(record);
        std::ifstream inFile;
        if(typeFile == 1){
            inFile.open(dataFile, std::ios::in | std::ios::binary);
            // Locate File pointer in that row and read nextRecord field
            inFile.seekg((sizeRecord * (row - 1)) + (1 * (row - 1)) + (24), std::ios::beg);
        }
        else {
            inFile.open(auxFile, std::ios::in | std::ios::binary);
            // Locate File pointer in that row and read nextRecord field
            inFile.seekg((sizeRecord * (row - 1)) + (1 * (row - 1)) + (24), std::ios::beg);
        }
        int nextRecord;
        inFile.read(reinterpret_cast<char *>(&nextRecord), sizeof(nextRecord));
        //std::cout << "getNextRecord: " << nextRecord << '\n';
        inFile.close();
        return nextRecord;
    }

    void updateNextRecord(int row, int nextValue, int typeFile) {
        //std::cout << "update row: " << row << ", nextValue: " << nextValue << '\n';
        Registro record;
        int sizeRecord = sizeof(record);
        std::fstream outFile;
        if(typeFile == 1){
            outFile.open(dataFile, std::ios::out | std::ios::in | std::ios::binary);
        }
        else {
            outFile.open(auxFile, std::ios::out | std::ios::in | std::ios::binary);
        }

        if(outFile.is_open()){
            outFile.seekp((sizeRecord * (row - 1)) + (1 * (row - 1)) + (24), std::ios::beg);
            outFile.write((char *)(&nextValue), sizeof(nextValue));
        }
        outFile.close();
    }

    int getWhereToInsert(std::string codigo){
        std::ifstream inFile;
        inFile.open(dataFile, std::ios::in | std::ios::binary);
        Registro record;
        std::string codigoFromFile;
        int row = 0;
        while(inFile >> record) {
            row++;
            codigoFromFile = record.getCodigo();
            if(codigoFromFile > codigo) {
                //std::cout << "codigoFromFile '" << codigoFromFile << "' > " << codigo << '\n';
                row--;
                break;
            }
        }
        inFile.close();
        //std::cout << "Insert after row: " << row << '\n';
        return row;
    }

    void setNewDataFile(std::string dataFileNew){
        this->dataFile = dataFileNew;
    }

    void MergeFiles(){
        std::cout << "\n** Merge Files method **\n";
        std::fstream newDataFile;
        std::string dataFileNew = "new_"+dataFile;
        newDataFile.open(dataFileNew, std::ios::out | std::ios::in | std::ios::trunc | std::ios::binary);
        newDataFile.close();
        newDataFile.open(dataFileNew, std::ios::out | std::ios::in | std::ios::app | std::ios::binary);
        int rowNewDataFile = 1;
        Registro record;
        int sizeRecord = sizeof(record);
        std::ifstream inFile;
        inFile.open(dataFile, std::ios::in | std::ios::binary);
        int next;
        while(true){
            // Read data File
            std::cout << "\n** Reading from Data File **\n";
            while(inFile >> record) {
                rowNewDataFile++;
                record.showMetaData();
                next = record.getNextRecord();
                // Update next field of object before write to Data File
                record.setNextRecord(rowNewDataFile);
                newDataFile << record;  // write operation
                if(next < 0){
                    break;
                }
            }
            inFile.close();
            // Read Aux File
            std::cout << "\n** Reading from Aux File DB **\n";
            inFile.open(auxFile, std::ios::in | std::ios::binary);
            inFile.seekg((sizeRecord * ((next * -1) - 1)) + (1 * ((next * -1) - 1)), std::ios::beg);
            while(inFile >> record) {
                rowNewDataFile++;
                record.showMetaData();
                next = record.getNextRecord();
                // Update next field of object before write to Data File
                record.setNextRecord(rowNewDataFile);
                newDataFile << record;  // write operation
                if(next >= 0) {
                    break;
                }
            }
            inFile.close();
            if(next == 0){
                break;
            }
            // update file pointer
            inFile.open(dataFile, std::ios::in | std::ios::binary);
            inFile.seekg((sizeRecord * ((next) - 1)) + (1 * ((next) - 1)), std::ios::beg);
        }
        inFile.close();
        // Merge job finished
        // Init aux file
        std::ofstream outAuxFile;
        outAuxFile.open(auxFile,std::ios::out | std::ios::trunc | std::ios::binary); // trunc 'discard existing content'
        outAuxFile.close();
        setNewDataFile(dataFileNew);
        std::cout << "\n** Merge finished **\n";
    }

public:
    // Default Constructor
    SequentialFileRegistro() = default;

    // Custom Constructor
    SequentialFileRegistro(std::string _filename) {
        this->dataFile = _filename;
        this->auxFile = "aux_" + _filename;
        this->rowsDataFile = 0;
        this->rowsAuxFile = 0;
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
        outFile.open(dataFile, std::ios::out |std::ios::app | std::ios::binary);
        for(auto registro : registros){
            if(row != numElements) {     // Update attribute of all records Except the last one
                registro.setNextRecord(nextRecord);     // Update nextRecord attribute
            }
            outFile << registro;    // Write object to file
            nextRecord++;
            row++;
            rowsDataFile++;
        }
        outFile.close();
    }

    void add(Registro registro) {
        std::cout << "\n** add Method **\n";
        // get codigo from object
        std::string codigo = registro.getCodigo();
        std::cout << "add registro codigo: " << codigo << '\n';
        // Identificar despues de que fila del data file insertar el nuevo registro
        int afterRow = getWhereToInsert(codigo);    // row = {1,2,3...}
        // Get Next value from that row
        int nextRecord = getNextRecordFromRow(afterRow, 1);
        // If next value is positive write directly in Aux File
        if(nextRecord >= 0){
            // Update nextRecord field of new object
            registro.setNextRecord(nextRecord);
            // Insert new record in Aux File (using Next value previously obtained)
            std::fstream outAuxFile;
            outAuxFile.open(auxFile, std::ios::out | std::ios::in | std::ios::app | std::ios::binary);
            int pos = outAuxFile.tellg();
            int rowNumAuxFile = (pos / (sizeof(registro) + 1)) + 1;
            rowNumAuxFile = rowNumAuxFile * -1;     // Negative Value for Data File
            //std::cout << "Row number in Aux: " << rowNumAuxFile << '\n';
            outAuxFile << registro;                 // Write in AuxFile
            rowsAuxFile++;
            outAuxFile.close();
            // Update nextRecord in DataFile
            updateNextRecord(afterRow, rowNumAuxFile, 1);
        }
        //Other case continue checking in Aux File and identify where to place inside Aux File
        else {
            while(true){
                char codAux[5];
                getCodigoFromRowAux(nextRecord * -1, codAux);        // codAux: read Codigo from file
                int next = getNextRecordFromRow(nextRecord * - 1, 0);   // next: read Next from Aux File
                // check where to insert in Aux File
                if(codAux > codigo){
                    //std::cout << "Write pointing before row: " << nextRecord << '\n';
                    break;
                }
                if(next == 0){
                    //std::cout << "Write pointing after row: " << nextRecord << '\n';
                    std::fstream outAuxFile;
                    outAuxFile.open(auxFile, std::ios::out | std::ios::in | std::ios::app | std::ios::binary);
                    int pos = outAuxFile.tellg();
                    int rowNumAuxFile = (pos / (sizeof(registro) + 1)) + 1;
                    // Update NextRecord field in Aux File
                    rowNumAuxFile = rowNumAuxFile * -1;     // Negative Value for Aux File
                    updateNextRecord(nextRecord * -1, rowNumAuxFile, 0);
                    // Write record in AuxFile
                    outAuxFile << registro;
                    rowsAuxFile++;
                    outAuxFile.close();
                    break;
                }
                // Avanza al siguiente record considerando el Next value
                nextRecord = next;
            }
        }
        // if we have more than 10 records in aux File Call Merge
        if(rowsAuxFile == 10) {
            MergeFiles();               // Merge method
        }
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
        inFile.open(dataFile, std::ios::in | std::ios::binary);
        std::cout << "\n** Data File DB **\n";
        while(inFile >> record) {
            record.showMetaData();
        }
        inFile.close();
        // Scan Aux File
        inFile.open(auxFile, std::ios::in | std::ios::binary);
        std::cout << "\n** Aux File DB **\n";
        while(inFile >> record) {
            record.showMetaData();
        }
        inFile.close();
     }

     /**
      * BinarySearch
      */
     int binarySearchUtil(int l, int r, std::string key) {
         //std::cout << "\n**Search Util**\n";
         if(r >= l) {
             int midRow = l + (r - l) / 2;
             // check if the element is present at the middle
             char codigoMidRow[5];
             getCodigoFromRow(midRow, codigoMidRow);        // read from disk
             if(codigoMidRow == key){
                 return midRow;
             }
             // if key is smaller than codigo at mid Row
             if(codigoMidRow > key){
                return binarySearchUtil(l, midRow - 1, key);
             }
             return binarySearchUtil(midRow + 1, r, key);
         }
         return -1;
     }

      int binarySearch(std::string key) {
         std::cout << "\n** BinarySearch method **\n";
         int rowRes = binarySearchUtil(0, rowsDataFile - 1, key);
         if(rowRes == -1){
             std::cout << "binarySearch Result: key '" << key << "' no existe en ninguna fila\n";
             return -1;
         }
         else{  // Muestra el resultado de la primera fila como fila 1
             std::cout << "binarySearch Result: key '" << key << "' posición logica en fila numero: " << ++rowRes << '\n';
             return rowRes;
         }
      }
};

#endif //LAB2_1_SEQUENTIALFILEREGISTRO_H
