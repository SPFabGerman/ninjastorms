#include "demo_button.h"

#include "kernel/drivers/button.h"
#include <stdio.h>

void
button_test(void)
{
    printf("Start the button test.\n");
    button_state lastR, lastL, lastT, lastB, lastC, lastBack;
    while (1)
    {
        button_state newState;
        if ((newState = button_get_state(BUTTON_RIGHT)) != lastR)
        {
            lastR = newState;
            printf("Right, new Button State: %i\n", newState);
        }

        if ((newState = button_get_state(BUTTON_LEFT)) != lastL)
        {
            lastL = newState;
            printf("Left, new Button State: %i\n", newState);
        }

        if ((newState = button_get_state(BUTTON_TOP)) != lastT)
        {
            lastT = newState;
            printf("Top, new Button State: %i\n", newState);
        }

        if ((newState = button_get_state(BUTTON_BOTTOM)) != lastB)
        {
            lastB = newState;
            printf("Bottom, new Button State: %i\n", newState);
        }

        if ((newState = button_get_state(BUTTON_CENTER)) != lastC)
        {
            lastC = newState;
            printf("Center, new Button State: %i\n", newState);
        }

        if ((newState = button_get_state(BUTTON_BACK)) != lastBack)
        {
            lastBack = newState;
            printf("Back, new Button State: %i\n", newState);
        }
    }
}