#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

typedef void *(*state_func)();

int kbhit();

char str[10];

uint8_t end = 0;

/* -------------FLYING--------------*/
void *flying(void);

void *speedingUp(void);
void *braking(void);
void *rotating(void);
void *prepareEjection(void);

//uint8_t flyingAct = 1;
uint8_t brakeRequest = 0;
uint8_t accelerationRequest = 0;
uint8_t generalRequest = 0;
uint8_t rotateRequest = 0;
int vel = 3000;
uint8_t fatalError = 0;
int angle = 0;
uint8_t okParameters = 0;
uint8_t preparingSystem = 0;
/* -------------------------------*/

/* -------------EJECTING--------------*/
void *ejecting(void);

void *releaseCabin(void);
void *releasePilotSeat(void);
void *releaseParachute(void);

//uint8_t ejectingAct = 0;
uint8_t releasedCabin = 0;
uint8_t releasedPilotSeat = 0;
uint8_t first = 1;
/* -------------------------------*/

void *ejecting(void) {
    if(first) {
        printf("Starting eject protocol...\n");
        first = 0;
    }

    if(!releasedCabin) return releaseCabin;
    if(releasedCabin && !releasedPilotSeat) return releasePilotSeat;
    if(releasedPilotSeat) return releaseParachute;

    return ejecting;
}

void *releaseCabin(void) {

    releasedCabin = 1;

    printf("Ejecting cabin...\n");

    return ejecting;
}

void *releasePilotSeat(void) {
    releasedPilotSeat = 1;

    printf("Ejecting pilot seat...\n");

    return ejecting;
}

void *releaseParachute(void) {
    printf("Opening parachutes...\n");

    end = 1;
}

/* -----------------------------------------------------FLYING-------------------------------------------------------------*/
void *flying(void) {
    printf("Flying... %d km/h -- %dº\n", vel, angle);

    if(accelerationRequest && !preparingSystem) return speedingUp;  //se estiver processando um error, nao deixa acelerar
    if(brakeRequest) return braking;
    if(rotateRequest) return rotating;
    if(fatalError && !preparingSystem) return prepareEjection; //input error
    if(preparingSystem) return prepareEjection;

    return flying;
}

void *speedingUp(void) {
    accelerationRequest = 0;
    
    vel += 500;

    printf("Speeding up...\n");
    printf("%dKm/h\n", vel);

    if(vel > 4000 && vel < 5000) {
        printf("Error -- Turbines overheating!! Reduce your velocity!!\n");
        return flying;
    } else if(vel < 5000) 
        return flying; //max velocity
    else {
        printf("You lost your engine!!!\n");
        fatalError = 1;
        return flying;
    }     
}

void *braking(void) {
    brakeRequest = 0;

    vel =  vel > 1000 ? vel - 500 : vel; //vel min = 1000km/h

    printf("Braking...\n");
    printf("%dKm/h\n", vel);

    if (!preparingSystem) return flying;
    else return prepareEjection;
    //return flying;
}

void *rotating(void){
    rotateRequest = 0;

    if(preparingSystem) 
        angle = (angle == 90 || angle == 270) ? 0 : angle + 90;
    else 
        angle = (angle == 270) ? 0 : angle + 90;

    printf("Rotating...\n");
    printf("%dº\n", angle);

    if(!preparingSystem) return flying;
    else return prepareEjection;
}

void *prepareEjection(void) {
    preparingSystem = 1;

    if(vel >= 1225) printf("EJECTION NOT ALLOWED! You are too fast! Reduce to 1225km/h!\n");
    if(angle != 0) printf("EJECTION NOT ALLOWED! Wrong angle! Adjust your airplane\n");
    if(vel < 1225 && angle == 0) return ejecting;
    else return flying;
}       

/* ------------------------------------------------------------------------------------------------------------------------*/

uint8_t processReq(char input)
{
    char secInput;

    switch (input) {
        case 'r': //request type
            scanf(" %c", &secInput);
            //printf("Debug sec: %c\n", secInput);

            if (secInput == 's')    //acelera
                accelerationRequest = 1;
            else if (secInput == 'b')
                brakeRequest = 1;
            else if (secInput == 'r')
                rotateRequest = 1;
            else 
                return 1; //INPUT ERROR
            break;

        case 'e': //insert error
            fatalError = 1;
            break;
        default:
            break;
    }

    return 0;
}

int main()
{
    state_func current_state = flying;

    char input;

    printf("Commands: \n");
    printf("Send 'r' to request a new action\nThen send:\n's' to acceleration request\n'b' to brake request\n'r' to rotate request.\n");
    printf("\n\npress enter to start...\n");
    
    input = getchar();

    while (!end)
    {
        current_state = (state_func)(current_state)();
        sleep(1);

        if (kbhit())
        {
            input = getchar();
            while(processReq(input)) {
                printf("Input error!\n");
                processReq(input);
            }
        }
    }

    printf("That was close... But I'm still alive!\n");
}