#ifndef GESTORAREADEDATOS_H
#define GESTORAREADEDATOS_H
#include "registro.h"
#include "indice.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <ostream>

using namespace std;

class GestorAreaDeDatos
{
private:
    Registro *vectorRegistro; //ej con 20 datos
    indice *vectorIndice; //3 bloques
    int tamVecInd;
    int n; //numero re registros por bloques
    int Over; //inicia el overflow
    int Pmax; //+1 es over
    int Omax; //vale 20
public:
    GestorAreaDeDatos();
    Registro consulta(int claveRecibida); //ej 1. recibe como entrada la CLAVE que desea consultar y como salida los datos del Registro.
    void insercion(int claveRecibida, int valor, int direccion);
    void ordenarBloque(int dirInicial);
    bool bloqueLleno(int dirInicial);
    int cantOcupadaBloque(int dirInicial);
    int posicionLibreEnBloque(int dirInicial);
    bool insercionEnOverFlow(int claveRecibida, int valor, int direccion);

    Registro *getVectorRegistro() const;
    void setVectorRegistro(Registro *value);
    indice *getVectorIndice() const;
    void setVectorIndice(indice *value);
};

#endif // GESTORAREADEDATOS_H
