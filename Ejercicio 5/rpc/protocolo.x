struct Cuadrupla
{
	int operandos[4];
};
typedef struct Cuadrupla Cuadrupla;

struct Dupla
{
	int a;
	int b;
};
typedef struct Dupla Dupla;

struct Double
{
	double val;
};
typedef struct Double Double;

struct String
{
	char val[32];
};
typedef struct String String;

program PROY1EJ5 {
	version PROY1EJ5VERS {
		unsigned long int DECABIN(int)=1;
		String BINAHEX(unsigned long int)=2;
		int SUMA(Cuadrupla)=3;
		int RESTA(Cuadrupla)=4;
		int MULTIPLICACION(Dupla)=5;
		Double DIVISION(Dupla)=6;
		
	}=1;
}=0x20001003;
