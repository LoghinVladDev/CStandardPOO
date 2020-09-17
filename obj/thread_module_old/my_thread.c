//
// Created by Vlad on 9/17/2020.
//

#include <malloc.h>
#include <string.h>
#include "my_thread.h"
#include "my_thread_defs.h"

static toothpaste_t objectCounter = 1;
static TOOTHPASTE_PROPERTIES objects[MAX_OBJ_COUNT];

TOOTHPASTE_RESULT createToothpaste ( toothpaste_t * pID, TOOTHPASTE_PROPERTIES * pProperties ) {
    if ( pProperties == NULL || pProperties->brandName == NULL )
        return TOOTHPASTE_STR_NULLPTR;

    if ( ! TOOTHPASTE_VALID( *pProperties ) )
        return TOOTHPASTE_STR_INVALID;

    objects[objectCounter]              = * pProperties;
    objects[objectCounter].brandName    = (char* ) malloc (strlen(pProperties->brandName) * sizeof(char));

    strcpy( objects[objectCounter].brandName, pProperties->brandName );

    * pID = objectCounter;

    objectCounter++;

    return SUCCESS;
}

TOOTHPASTE_PROPERTIES getProperties ( toothpaste_t ID ) {
    return objects[ID];
}

TOOTHPASTE_COLOUR getColour ( toothpaste_t ID ) {
    return objects[ID].colour;
}

TOOTHPASTE_FLAVOUR getFlavour ( toothpaste_t ID ) {
    return objects[ID].flavour;
}

float getPrice ( toothpaste_t ID ) {
    return objects[ID].price;
}



void destroyToothpaste( toothpaste_t ID ) {
    free( objects[ID].brandName );
}


const char* getColourName ( TOOTHPASTE_COLOUR colour ) {
    switch( colour ) {

        case WHITE:
            return "White";
        case TWO_COLOURS:
            return "Bi-Coloured";
        case THREE_COLOURS:
            return "Rosu Alb Albastru";
        case PARADONTAX_COLOUR:
            return "Caca";
        default :
            return "What are you doing with your life?";
    }
}
