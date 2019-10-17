#include <stdlib.h>
#include "ai.h"
#include "all.h"

int LaneCount(CAR *car, int lane)
{
    int count = 0;
    CAR *current;
    for (current = car->next; current != NULL; current = current->next)
        if (current->justment == lane)
            count++;
    return count;
}