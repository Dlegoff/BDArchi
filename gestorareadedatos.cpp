#include "gestorareadedatos.h"


Registro *GestorAreaDeDatos::getVectorRegistro() const
{
    return vectorRegistro;
}

void GestorAreaDeDatos::setVectorRegistro(Registro *value)
{
    vectorRegistro = value;
}

indice *GestorAreaDeDatos::getVectorIndice() const
{
    return vectorIndice;
}

void GestorAreaDeDatos::setVectorIndice(indice *value)
{
    vectorIndice = value;
}

GestorAreaDeDatos::GestorAreaDeDatos() //constructor
{
    this->Omax=20;
    this->n=4;
    this->Pmax=11;
    this->Over=this->Pmax+1; //over inicia en 12
    //int *auxVector = new int [cantidad+1];
    Registro *vecAuxReg = new Registro [this->Omax];
    delete [] this->vectorRegistro; //limpio si hay basura
    delete [] this->vectorIndice;
    this->vectorRegistro = vecAuxReg;
    this->tamVecInd=this->Over/this->n;
    indice *vecAuxIndice = new indice [tamVecInd]; //creo un vector de indice de 3 bloques, 12/4. Esto es solo para este caso o cuando sea par y multiplo de n.
    this->vectorIndice = vecAuxIndice;
}

Registro GestorAreaDeDatos::consulta(int claveRecibida)//funcion devuelve un registro buscado segun la clave, si devuelve un registro con -1 es porque no esta.
{
    Registro auxRegistro;
    bool bandera = false;
    for (int i = 0; i < 3; ++i)
    {
        if ((this->vectorIndice[i].getClave()>=claveRecibida)&&
                (this->vectorIndice[i+1].getClave()<=claveRecibida||(i+1>=3)))
            //comparo si es mayor al pos sub "i" y menor que el siguiente, con i+1 salvo el ultimo.
        {
            for (int j = this->vectorIndice[i].getDireccion(); j < this->n; ++j)
            //entra al bloque y lo recorre desde la direc del indice hasta el tamaño n del bloque
            {
                if (vectorRegistro[j].getClave()==claveRecibida)
                {
                    bandera = true;
                    auxRegistro.setClave(vectorRegistro[j].getClave());
                    auxRegistro.setDireccion(vectorRegistro[j].getDireccion());
                    auxRegistro.setValor(vectorRegistro[j].getValor());
                }
            }
        }

    }
    if (!bandera)
    //si no lo encontro en el area de indice, busco en overflow, inicia en over y termina en omax
    {
        for (int i = this->Over; i < this->Omax ; ++i)
        {
            if (this->vectorRegistro[i].getClave()==claveRecibida)
            {
                bandera = true;
                auxRegistro.setClave(vectorRegistro[i].getClave());
                auxRegistro.setDireccion(vectorRegistro[i].getDireccion());
                auxRegistro.setValor(vectorRegistro[i].getValor());
            }
        }
    }
    if(!bandera)//No se encontro el registro y retorno un registro vacio "con ceros"
    {
        auxRegistro.setClave(-1); auxRegistro.setDireccion(-1); auxRegistro.setValor(-1);
        return auxRegistro;
    }
    else
    {
        return auxRegistro;
    }

}

void GestorAreaDeDatos::insercion(int claveRecibida, int valor, int direccion)
//supongo que el dato que ingresan no se encuentra en el vector.
{
    Registro auxRegistro;
    auxRegistro.setClave(claveRecibida);
    auxRegistro.setValor(valor);
    auxRegistro.setDireccion(direccion);
    //busco en indice
    if (this->vectorIndice[0].getClave()!= NULL) //si la clave del primer elemento del vector de indice no existe, esta todo vacio.
    {
        this->vectorIndice[0].setClave(claveRecibida);//creo el indice
        this->vectorIndice[0].setDireccion(0); //inicia en 0 el bloque
        this->vectorRegistro[0]=auxRegistro;//meto el registro en la primer pos del area principal
    }else if(claveRecibida < this->vectorIndice[0].getClave())//si la clave es menor al primer elemento del indice va en el primer bloque
    {
        for (int i = 0; i < this->tamVecInd; ++i) //va de 0 a 2 y el tamaño del vecInd es 3.
        {
            if (this->vectorIndice[i+1].getClave() != NULL) //si la clave que le sigue existe // ignorar:y si el i+1 osea el siguiente no exedio al tamaño del vector de indice
            {
                if (claveRecibida > this->vectorIndice[i].getClave()
                        && claveRecibida < this->vectorIndice[i+1].getClave()) //significa que es un bloque intermedio y no debo crear otro bloque si tiene la mitad ocupada
                { //si esto se cumple la meto en el bloque sub i, si este tiene lugar, sino va al over y si el over esta lleno va un cartel de error
                    if (!this->bloqueLleno(this->vectorIndice[i].getDireccion()))//si el bloque no esta lleno
                    {
                        int auxPos = this->posicionLibreEnBloque(this->vectorIndice[i].getDireccion());
                        this->vectorRegistro[auxPos] = auxRegistro; //meto el registro en la posicion libre
                        this->ordenarBloque(this->vectorIndice[i].getDireccion()); //despues de insertar ordeno el bloque y actualiza el indice esta funcion.
                    } //****** aca meter en overflow si ta lleno
                    else
                    {
                        if (!this->insercionEnOverFlow(claveRecibida,valor,direccion)) //sino lo metio
                        {
                            cout<<"no se tiene más lugar en el área de Overflow";
                        }
                    }
                }
            }
            else if(i+1 < this->tamVecInd) //si la clave siguiente no existe significa que es un bloque final o el ultimo bloque antes (hacer un if para ver si es el ultimo sino no lo es)
            {//en este caso entra si es 0/1; aca verificar si la mitad de este bloque esta ocupada debo crear un nuevo bloque.
                int CantOcupada = this->cantOcupadaBloque(this->vectorIndice[i].getDireccion());
                if (CantOcupada < (this->n/2))
                {
                    int posLibre = this->posicionLibreEnBloque(this->vectorIndice[i].getDireccion());
                    this->vectorRegistro[posLibre] = auxRegistro; //meto el registro en la posicion libre
                    this->ordenarBloque(this->vectorIndice[i].getDireccion());
                }
                else //creo un nuevo bloque
                {
                    this->vectorIndice[i+1].setClave(claveRecibida);
                    this->vectorIndice[i+1].setDireccion(this->vectorIndice[i].getDireccion()+this->n); //si la dir del anterior inicia en 0 le sumo n=4
                    this->vectorRegistro[this->vectorIndice[i+1].getDireccion()] = auxRegistro;
                }
            }
            else//sino es pos 2 osea el bloque final
            {
                if (!this->bloqueLleno(this->vectorIndice[i].getDireccion())) // si el bloque no esta lleno
                {
                    int posLibre = this->posicionLibreEnBloque(this->vectorIndice[i].getDireccion());
                    this->vectorRegistro[posLibre] = auxRegistro; //meto el registro en la posicion libre
                    this->ordenarBloque(this->vectorIndice[i].getDireccion());
                }
                else
                {
                    cout<<"reorganizar el archivo, se supero PMAX";
                    if (!this->insercionEnOverFlow(claveRecibida,valor,direccion)) //sino lo metio
                    {
                        cout<<"no se tiene más lugar en el área de Overflow";
                    }
                }
            }

        }
    }


}

void GestorAreaDeDatos::ordenarBloque(int dirInicial) //ordenamiento dentro de cada bloque, por clave
{
    Registro auxRegistro;
    Registro auxRegistroCache = this->vectorRegistro[dirInicial];
    for (int i = dirInicial; i < this->n; ++i) //this N es el numero de registros por bloque
    {
        for (int j = i+1; j < this->n-1; ++j)
        {
            if (this->vectorRegistro[i].getClave()==NULL
                    ||this->vectorRegistro[i+1].getClave()==NULL)//si en esa pos o el siguiente esta vacio, corto porque no hay mas para comparar. en el bloque
            {
                i = this->n;//corto ambos for
            }
            else
            {
            if (this->vectorRegistro[i].getClave()>this->vectorRegistro[i+1].getClave())
            {
                auxRegistro = this->vectorRegistro[i+1];
                this->vectorRegistro[i+1] = this->vectorRegistro[i];
                this->vectorRegistro[i] = this->vectorRegistro[i+1];
            }
            }
        }

    }
    //actualizar el valor del indice cuando ordeno el bloque porque puede cambiar el valor menor
    for (int i = 0; i < this->tamVecInd; ++i)
    {
        if (this->vectorIndice[i].getClave() == auxRegistroCache.getClave())
        {
            this->vectorIndice[i].setClave(this->vectorRegistro[dirInicial].getClave()); //actualice la clave en el indice y la direccion sigue siendo la misma, que apunta al inicio del bloque
        }
    }

}

bool GestorAreaDeDatos::bloqueLleno(int dirInicial)
{

    return (this->vectorRegistro[dirInicial+(this->n-1)].getClave()!=NULL);

}

int GestorAreaDeDatos::cantOcupadaBloque(int dirInicial)
{
    int aux=0;
    for (int i = dirInicial; i < this->n; ++i)
    {
        if (this->vectorRegistro[i].getClave()!=NULL)
        {
            aux++;
        }
    }
    return aux;
}

int GestorAreaDeDatos::posicionLibreEnBloque(int dirInicial)
{
    int aux=0;
    for (int i = dirInicial; i < this->n; ++i)
    {
        if (this->vectorRegistro[i].getClave()!=NULL)
        {
            aux = i;
        }
    }
    return aux;
}

bool GestorAreaDeDatos::insercionEnOverFlow(int claveRecibida, int valor, int direccion)
{
    bool bandera = false;
    Registro auxRegistro;
    auxRegistro.setClave(claveRecibida);
    auxRegistro.setValor(valor);
    auxRegistro.setDireccion(direccion);
    for (int i = this->Over; i < this->Omax; ++i)
    {
        if (this->vectorRegistro[i].getClave()==NULL)//busco un lugar libre y lo pongo
        {
            this->vectorRegistro[i] = auxRegistro;
            bandera = true;
        }

    }
    return bandera;
}
