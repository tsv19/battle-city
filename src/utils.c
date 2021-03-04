
#include "utils.h"

void mustInit(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}


int between(int lower, int higher)
{
    return lower + (rand() % (higher - lower));
}


float between_f(float lower, float higher)
{
    return lower + ((float)rand() / (float)RAND_MAX) * (higher - lower);
}


bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
{
    if(ax1 > bx2) return false;
    if(ax2 < bx1) return false;
    if(ay1 > by2) return false;
    if(ay2 < by1) return false;

    return true;
}
