#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>

typedef void *(*state_func)();

typedef enum
{
    idle = 0,
    request 
} button;

int open_door1_act = 0, open_door2_act = 0, close_door1_act = 0, close_door2_act = 0;

button buttonDoor1 = idle;
button buttonDoor2 = idle;

int kbhit(void);
void *door1closed();
void *door1open();
void *door2closed();
void *door2open();

char door1State[7], door2State[7];

void *door1closed()
{
    memset(door1State, 0, sizeof(door1State));
    sprintf(door1State, "CLOSED");

    if (buttonDoor1 && !buttonDoor2 && !open_door1_act && !open_door2_act) {
        open_door1_act = 1;
        buttonDoor1 = idle;
        return door1open;
    } else if (buttonDoor1 && !buttonDoor2 && open_door1_act && !open_door2_act) {
        open_door1_act = 1;
        buttonDoor1 = idle;
        return door1open;
    } else if (buttonDoor1 && !buttonDoor2 && !open_door1_act && open_door2_act) {
        open_door1_act = 0;
        buttonDoor1 = idle;
        return door1closed;
    } else if (buttonDoor1 && buttonDoor2 && !open_door1_act && !open_door2_act) { //1 tem preferência
        open_door1_act = 1;
        buttonDoor1 = idle;
        return door1open;
    } else if (buttonDoor1 && buttonDoor2 && !open_door1_act && open_door2_act) { 
        open_door1_act = 0;
        buttonDoor1 = idle;
        return door1closed;
    } else if (buttonDoor1 && buttonDoor2 && open_door1_act && !open_door2_act) { 
        open_door1_act = 1;
        buttonDoor1 = idle;
        return door1open;
    } else {
        buttonDoor1 = idle;
        return door1closed;
    }
}

void *door2closed()
{
    memset(door2State, 0, sizeof(door2State));
    sprintf(door2State, "CLOSED");

    if (!buttonDoor1 && buttonDoor2 && !open_door1_act && !open_door2_act)
    {
        open_door2_act = 1;
        buttonDoor2 = idle;
        return door2open;
    } else if (!buttonDoor1 && buttonDoor2 && !open_door1_act && open_door2_act) {
        open_door2_act = 1;
        buttonDoor2 = idle;
        return door2open;
    } else if (!buttonDoor1 && buttonDoor2 && open_door1_act && !open_door2_act) {
        open_door2_act = 0;
        buttonDoor2 = idle;
        return door2closed;
    } else if (buttonDoor1 && buttonDoor2 && !open_door1_act && !open_door2_act) {
        open_door2_act = 0;
        buttonDoor2 = idle;
        return door2closed;
    } else if (buttonDoor1 && buttonDoor2 && !open_door1_act && open_door2_act) {
        open_door2_act = 1;
        buttonDoor2 = idle;
        return door2open;
    } else if (buttonDoor1 && buttonDoor2 && open_door1_act && !open_door2_act) {
        open_door2_act = 0;
        buttonDoor2 = idle;
        return door2closed;
    } else {
        buttonDoor2 = idle;
        return door2closed;
    }
}

void *door1open()
{
    memset(door1State, 0, sizeof(door1State));
    sprintf(door1State, "OPEN");

    open_door1_act = 0;
    //open_door2_act = 0;
    buttonDoor1 = idle;

    return door1closed;
}

void *door2open()
{
    memset(door2State, 0, sizeof(door2State));
    sprintf(door2State, "OPEN");

    //open_door1_act = 0;
    open_door2_act = 0;
    buttonDoor2 = idle;

    return door2closed;
}

void processButtonRequest(char input)
{
    int aux = 0;
    
    aux = input - '0';

    switch (aux)
    {
        case 0:
            buttonDoor1 = 1;
            buttonDoor2 = 0;
            break;
        case 1:
            buttonDoor1 = 0;
            buttonDoor2 = 1;
            break;
        case 2: 
            buttonDoor1 = 1;
            buttonDoor2 = 1;
            break;
        default:
            buttonDoor1 = 0;
            buttonDoor2 = 0;
            break;
    } 
}

int main()
{
    state_func door1_state = door1closed;
    state_func door2_state = door2closed;

    char input;
    int aux = 2;

    while (1)
    {
        door1_state = (state_func)(*door1_state)();
        //sleep(1);
        door2_state = (state_func)(*door2_state)();

        printf("DOOR 1: %s -- DOOR 2: %s\n", door1State, door2State);
        sleep(1);

        if(kbhit() && !open_door1_act && !open_door2_act)
        {
            input = getchar();
            processButtonRequest(input);
        }
    }

}
