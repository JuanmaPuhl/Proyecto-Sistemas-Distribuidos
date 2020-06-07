#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>

#define SERVER_KEY_PATHNAME "mqueue_server_key"
#define PROJECT_ID 'M'

struct message_payload
{
    int qid;
    int opcion;
    union
    {
        long int datos[4];
        char str[32];
    } u;
};

struct message
{
    long message_type;
    struct message_payload message_payload;
};

void texto()
{
    printf("Seleccione una opcion:\n");
    printf("1) Conversion de numero decimal a binario.\n");
    printf("2) Conversion de numero binario a hexadecimal.\n");
    printf("3) Suma de hasta cuatro operandos.\n");
    printf("4) Resta de hasta cuatro operandos.\n");
    printf("5) Multiplicacion de dos operandos.\n");
    printf("6) Division de hasta dos operandos.\n");
}

int main (int argc, char **argv)
{

    int opcion;
    printf("Mini Operaciones\n");
    printf("-----------------\n");
    texto();
    fflush(stdin);
    scanf("%d",&opcion);

    while(!((opcion>=1)&&(opcion<=6)))
    {
        printf("Elija una opcion valida.\n");
        printf("%d",opcion);
        texto();
        fflush(stdin);
        scanf("%d",&opcion);
    }

    key_t server_queue_key;
    int server_qid, myqid;
    struct message my_message, return_message;

    // create my client queue for receiving messages from server
    if ((myqid = msgget (IPC_PRIVATE, 0660)) == -1)
    {
        perror ("msgget: myqid");
        exit (1);
    }

    if ((server_queue_key = ftok (SERVER_KEY_PATHNAME, PROJECT_ID)) == -1)
    {
        perror ("ftok");
        exit (1);
    }

    if ((server_qid = msgget (server_queue_key, 0)) == -1)
    {
        perror ("msgget: server_qid");
        exit (1);
    }

    my_message.message_type = 1;
    my_message.message_payload.qid = myqid;
    my_message.message_payload.opcion = opcion;
    switch(opcion)
    {
    case 1:
        printf("Ingrese un numero decimal para convertir a binario.\n");
        fflush(stdin);
        scanf("%ld",&(my_message.message_payload.u.datos[0]));
        break;
    case 2:
        printf("Ingrese un numero binario para convertir a hexadecimal.\n");
        fflush(stdin);
        scanf("%ld",&(my_message.message_payload.u.datos[0]));
        break;
    case 3:
        printf("Ingrese hasta 4 numeros para sumarlos.\n");
        for(int i = 0; i < 4; i++)
        {
            printf("Ingrese el operando %d: \n",(i+1));
            fflush(stdin);
            scanf("%ld",&(my_message.message_payload.u.datos[i]));
        }
        break;
    case 4:
        printf("Ingrese hasta 4 numeros para restarlos.\n");
        for(int i = 0; i < 4; i++)
        {
            printf("Ingrese el operando %d: \n",(i+1));
            fflush(stdin);
            scanf("%ld",&(my_message.message_payload.u.datos[i]));
        }
        break;
    case 5:
        printf("Ingrese hasta 2 numeros para multiplicarlos.\n");
        for(int i = 0; i < 2; i++)
        {
            printf("Ingrese el operando %d: \n",(i+1));
            fflush(stdin);
            scanf("%ld",&(my_message.message_payload.u.datos[i]));
        }
        break;
    case 6:
        printf("Ingrese hasta 2 numeros para dividirlos.\n");
        for(int i = 0; i < 2; i++)
        {
            printf("Ingrese el operando %d: \n",(i+1));
            fflush(stdin);
            scanf("%ld",&(my_message.message_payload.u.datos[i]));
        }
        break;
    default:
        exit(0);
    }
    struct timeval begin, end;

    gettimeofday(&begin,NULL);
    // send message to server
    if (msgsnd (server_qid, &my_message, sizeof (struct message_payload), 0) == -1)
    {
        perror ("client: msgsnd");
        exit (1);
    }

    // read response from server
    if (msgrcv (myqid, &return_message, sizeof (struct message_payload), 0, 0) == -1)
    {
        perror ("client: msgrcv");
        exit (1);
    }
    gettimeofday(&end,NULL);
    unsigned int t = end.tv_usec - begin.tv_usec;
    printf("tiempo en microsegundos: %u\n",t);

    // process return message from server
    switch(opcion)
    {
        case 2:
            printf ("Resultado: %s\n",return_message.message_payload.u.str);
            break;
        case 6:
            printf ("Cociente: %ld, resto: %ld\n", return_message.message_payload.u.datos[0],return_message.message_payload.u.datos[1]);
            break;
        default:
            printf ("Resultado: %ld\n", return_message.message_payload.u.datos[0]);

    }

    // remove message queue
    if (msgctl (myqid, IPC_RMID, NULL) == -1)
    {
        perror ("client: msgctl");
        exit (1);
    }

    exit (0);
}
