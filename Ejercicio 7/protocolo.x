struct MiTimeval
{
	long int tv_sec;
	long int tv_usec;
};
typedef struct MiTimeval MiTimeval;
program PROY1EJ7
{
	version PROY1EJ7VERS
	{
		MiTimeval LOCALTIME()=1;
		void PING(void)=2;
	}=1;
}=0x20001004;
