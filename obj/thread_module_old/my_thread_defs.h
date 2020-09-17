//
// Created by Vlad on 9/17/2020.
//

#ifndef UNTITLED2_MY_THREAD_DEFS_H
#define UNTITLED2_MY_THREAD_DEFS_H

#define NULL_OBJECT (toothpaste_t)0
#define MAX_OBJ_COUNT 100
#define MAX_PRICE 500.0f

#define FLAVOUR_VALID(flavour) (flavour >= NO_MENTHOL && flavour <= PARADONTAX_SALT)
#define COLOUR_VALID(colour) (colour >= WHITE && colour <= PARADONTAX_COLOUR)
#define PRICE_VALID(price) (price <= MAX_PRICE)

#define TOOTHPASTE_VALID(toothpasteStr) \
(                                       \
    COLOUR_VALID((toothpasteStr).colour) && \
    FLAVOUR_VALID((toothpasteStr).flavour) && \
    PRICE_VALID((toothpasteStr).price) \
)

#endif //UNTITLED2_MY_THREAD_DEFS_H
