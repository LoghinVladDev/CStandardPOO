//
// Created by Vlad on 9/17/2020.
//

#ifndef UNTITLED2_MY_THREAD_H
#define UNTITLED2_MY_THREAD_H


#ifndef NULL
#define NULL ((void*)0)
#endif

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef char sint8;
typedef short sint16;
typedef int sint32;
typedef long long sint64;

typedef uint32 toothpaste_t;

typedef enum {
    NO_MENTHOL = 0x00,
    MENTHOL,
    MILD,
    PARADONTAX_SALT
} TOOTHPASTE_FLAVOUR;

typedef enum {
    WHITE = 0x10,
    TWO_COLOURS,
    THREE_COLOURS,
    PARADONTAX_COLOUR
} TOOTHPASTE_COLOUR;

typedef struct {
    TOOTHPASTE_COLOUR   colour;
    TOOTHPASTE_FLAVOUR  flavour;
    float               price;
    char *              brandName;
} TOOTHPASTE_PROPERTIES;

typedef enum {
    SUCCESS = 0x00,
    TOOTHPASTE_STR_INVALID,
    TOOTHPASTE_STR_NULLPTR,
    OBJ_INVALID
} TOOTHPASTE_RESULT;

/**
 * fct care ret
 * fct care ret prin param id-ul resp
 */

TOOTHPASTE_RESULT createToothpaste( toothpaste_t *, TOOTHPASTE_PROPERTIES * );
TOOTHPASTE_PROPERTIES getProperties ( toothpaste_t );
TOOTHPASTE_COLOUR getColour ( toothpaste_t  );
TOOTHPASTE_FLAVOUR getFlavour ( toothpaste_t  );

float getPrice ( toothpaste_t  );
void destroyToothpaste( toothpaste_t );

const char* getColourName ( TOOTHPASTE_COLOUR );

#endif //UNTITLED2_MY_THREAD_H
