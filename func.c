#include <stdio.h>
#include "robot/log.h"
#include "robot/vector.h"

int main() {
    Vector vec = vcreate(3, sizeof(int));
    vpush(vec, 1);
    vpush(vec, 2);
    vpush(vec, 3);
    for (unsigned i = 0; i < vec->size; i++) printf("%d ", vget(vec, int, i));
    printf("\n");
    vpush(vec, 4);
    for (unsigned i = 0; i < vec->size; i++) printf("%d ", vget(vec, int, i));
    return 0;
}