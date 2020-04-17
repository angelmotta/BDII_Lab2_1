//
// Created by Angel Motta on 4/13/20.
//
#ifndef LAB2_1_REGISTRO_H
#define LAB2_1_REGISTRO_H

#include <iostream>
#include <string>

class Registro {
private:
    char codigo[5];
    char carrera[15];
    int ciclo;
    int nextRecord;
    friend std::istream& operator >>(std::istream & stream, Registro & record);
    friend std::ostream& operator <<(std::ostream & stream, Registro & record);
    friend bool operator <(const Registro &reg, const Registro &other);     // Overload to use Sort of STL
public:
    // Default Constructor
    Registro() = default;

    // Custom Constructor
    Registro(const char *_codigo, const char *_carrera, int _ciclo) {
        strcpy(this->codigo, _codigo);
        strcpy(this->carrera, _carrera);
        this->ciclo = _ciclo;
        this->nextRecord = 0;           // default 0 means -> does not exist Next element
    }

    char* getCodigo(){
        return this->codigo;
    }

    void setNextRecord(int _next){
        this->nextRecord = _next;
    }

    int getNextRecord() {
        return this->nextRecord;
    }

    void showMetaData(){
        std::cout << codigo << ", " << carrera << ", " << ciclo << ", " << nextRecord << '\n';
    }
};

// Overload operator | Write file Operation << object
std::ostream & operator << (std::ostream & stream, Registro & record) {
    stream.write(record.codigo,  5);
    stream.write(record.carrera,  15);
    stream.write((char *)(&record.ciclo), sizeof(record.ciclo));
    stream.write((char *)(&record.nextRecord), sizeof(record.nextRecord));
    stream << "\n";
    stream << std::flush;
    return stream;
}

// Overload operator | Read file operation >> object
std::istream& operator >> (std::istream& stream, Registro& record) {
    stream.read(record.codigo, 5);
    stream.read(record.carrera, 15);
    stream.read(reinterpret_cast<char *>(&record.ciclo), sizeof(record.ciclo));
    stream.read(reinterpret_cast<char *>(&record.nextRecord), sizeof(record.nextRecord));
    stream.get(); //read endLine '\n'
    return stream;
}

bool operator<(const Registro &reg, const Registro &other) {
    return std::stoi(reg.codigo) < std::stoi(other.codigo);
}

#endif //LAB2_1_REGISTRO_H
