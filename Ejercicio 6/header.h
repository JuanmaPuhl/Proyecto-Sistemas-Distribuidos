typedef struct Cabecera
{
    int identificador;
} Cabecera; 

typedef struct Funcion
{
    int Idfuncion;
} Funcion;

typedef struct Datos
{
    char datos[128];
} Datos;

typedef  enum Identificadores 
{
    Idpedirdato,
    IdCerrar,
    IdpedirFuncion,
    Iddardatos
} Identificadores; 
