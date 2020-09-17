//
// Created by Vlad on 9/15/2020.
//

#include <stdio.h>
#include "unHeaderDubios.h"



int getX () {
    static int x = 0;
    return x++;
}

int getXAgain() {
    static int x = 30;
    return x++;
}

inline int max (int x, int y) {
    return x > y ? x : y; /// pp ca avem 10 linii
}