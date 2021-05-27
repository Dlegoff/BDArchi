#ifndef REGISTRO_H
#define REGISTRO_H


class Registro
{
private:
    int clave;
    int valor;
    int direccion;
public:
    Registro();
    int getClave() const;
    void setClave(int value);
    int getValor() const;
    void setValor(int value);
    int getDireccion() const;
    void setDireccion(int value);
};

#endif // REGISTRO_H
