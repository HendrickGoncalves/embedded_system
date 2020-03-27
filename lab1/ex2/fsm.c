#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>

typedef void *(*state_func)();

int req = 0, trava = 0, vel = 0;

int kbhit();
void *open(void);
void *closed(void);

char str[7];

void *closed(void) {
    memset(str, 0, sizeof(str));
    sprintf(str, "CLOSED");

    if (req && !trava && vel < 30) {
        req = 0;
        return open;
    } else {
        return closed;
    }
}

void *open(void) {
    memset(str, 0, sizeof(str));
    sprintf(str, "OPEN");
    return req ? closed : open; 
}

void processReq(char input) {
    char in;
    char aux[3];
    
    switch (input)
    {
        case 't': 
            printf("Travando..\n");
            trava ^= 1;
            break;
        case '1':           
            printf("Quero sair!\n");
            req = 1;
            break;
        case 'v': 
            vel = vel > 30 ? 10 : 31; //reduz ou aumenta a velocidade apenas para ver a funcionalidade 
            break;
        default:
            req = 0;
            break;
    }
}

int main()
{
    state_func current_state = closed;

    char input;
    int aux;

    while (1)
    {
        current_state = (state_func)(current_state)();

        printf("DOOR %s \n", str);
        sleep(1);

        if(kbhit())
        {
            input = getchar();
            processReq(input);
        }
    }

}