struct Mensaje{
	long int datos[4];
};
typedef struct Mensaje Mensaje;

program PROY1EJ5
{
	version PROY1EJ5VERS
	{
		Mensaje DECTOBIN(Mensaje)=1;
		string BINTOHEX(Mensaje)=2;
		Mensaje SUMA(Mensaje)=3;
		Mensaje RESTA(Mensaje)=4;
		Mensaje MULTIPLICACION(Mensaje)=5;
		Mensaje DIVISION(Mensaje)=6;
	}=1;
}=0x20001003;
