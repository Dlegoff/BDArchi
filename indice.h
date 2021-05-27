#ifndef INDICE_H
#define INDICE_H


class indice
{
private:
    int clave;
    int direccion;
public:
    indice();
    int getClave() const;
    void setClave(int value);
    int getDireccion() const;
    void setDireccion(int value);
};

#endif // INDICE_H
