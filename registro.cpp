#include "registro.h"

int Registro::getClave() const
{
    return clave;
}

void Registro::setClave(int value)
{
    clave = value;
}

int Registro::getValor() const
{
    return valor;
}

void Registro::setValor(int value)
{
    valor = value;
}

int Registro::getDireccion() const
{
    return direccion;
}

void Registro::setDireccion(int value)
{
    direccion = value;
}

Registro::Registro()
{

}
