struct ByteArray{
	char bytes[2048];
};
typedef struct ByteArray ByteArray;

program PROY1EJ3
{
	version PROY1EJ3VERS
	{
		ByteArray ADDBYTEARRAY(ByteArray)=1;
	}=1;
}=0x20001002;
