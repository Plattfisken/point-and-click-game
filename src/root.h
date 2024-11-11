#pragma once

#ifndef ROOT_H
#define ROOT_H


/*---------------- libraries ---------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "../include/raylib.h"
#include "../include/raymath.h"


/*---------------- defines -----------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------*/
#define global static
#define persistent static
#define global static


/*---------------- types -------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------*/
#define i8 int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define f32 float
#define f64 double

typedef Vector2 vec2;


/*---------------- macros ------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ARRAY_LENGTH(array) (sizeof((array)) / sizeof((array)[0]))


/*---------------- source files ------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "game_math.h"
#include "game.h"

#include "game_math.c"
#include "levels.c"
#include "game.c"

#endif
