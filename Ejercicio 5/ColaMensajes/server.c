#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define SERVER_KEY_PATHNAME "mqueue_server_key"
#define PROJECT_ID 'M'
#define QUEUE_PERMISSIONS 0660

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

int main (int argc, char **argv)
{
    key_t msg_queue_key;
    int qid;
    struct message message;

    if ((msg_queue_key = ftok (SERVER_KEY_PATHNAME, PROJECT_ID)) == -1)
    {
        perror ("ftok");
        exit (1);
    }

    if ((qid = msgget (msg_queue_key, IPC_CREAT | QUEUE_PERMISSIONS)) == -1)
    {
        perror ("msgget");
        exit (1);
    }

    while (1)
    {
        // read an incoming message
        if (msgrcv (qid, &message, sizeof (struct message_payload), 0, 0) == -1)
        {
            perror ("msgrcv");
            exit (1);
        }

        // process message
        switch(message.message_payload.opcion)
        {
        case 1:
            ;
            long int rem, temp = 1;
            long int resultado = 0;
            while (message.message_payload.u.datos[0] > 0)
            {
                rem = message.message_payload.u.datos[0]%2;
                message.message_payload.u.datos[0] = message.message_payload.u.datos[0] / 2;
                resultado = resultado + rem*temp;
                temp = temp * 10;
            }
            message.message_payload.u.datos[0] = resultado;
            break;
        case 2:
            ;
            char *to_return;
            to_return = malloc(sizeof(long int));
            long int binaryval = message.message_payload.u.datos[0];
            long int hexadecimalval = 0;
            int i = 1;
            long int remainder;
            while(binaryval != 0)
            {
                remainder = binaryval % 10;
                hexadecimalval = hexadecimalval + remainder * i;
                i = i * 2;
                binaryval = binaryval / 10;
            }
            sprintf(to_return,"%lX",hexadecimalval);
            strcpy(message.message_payload.u.str, to_return);
            break;
        case 3:
            for(int i = 1; i < 4; i++)
            {
                message.message_payload.u.datos[0] += message.message_payload.u.datos[i];
            }
            break;
        case 4:
            for(int i = 1; i < 4; i++)
            {
                message.message_payload.u.datos[0] -= message.message_payload.u.datos[i];
            }
            break;
        case 5:
            message.message_payload.u.datos[0] *= message.message_payload.u.datos[1];
            break;
        case 6:
            ;
            long int dividendo = message.message_payload.u.datos[0];
            message.message_payload.u.datos[0] /= message.message_payload.u.datos[1];
            message.message_payload.u.datos[1] = dividendo % message.message_payload.u.datos[1];
            break;
        default:
            exit(0);
        }

        int client_qid = message.message_payload.qid;
        message.message_payload.qid = qid;

        // send reply message to client
        if (msgsnd (client_qid, &message, sizeof (struct message_payload), 0) == -1)
        {
            perror ("msgget");
            exit (1);
        }

    }
}
