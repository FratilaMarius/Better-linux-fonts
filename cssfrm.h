#ifndef CSSFRM_H
#define CSSFRM_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define TEXT_COLOUR(colourCode) printf("\033[%dm", colourCode)
#define TEXT_RESET() printf("\033[0m")
#define TEXT_BOLD() printf("\033[1m")
#define TEXT_UNDERLINE() printf("\033[4m")
#define BACKGROUND_COLOUR(colourCode) printf("\033[%dm", colourCode)
#define BACKGROUND_RESET() printf("\033[49m")

void Message(const char *message);  // has to be used with _IONBUF to work, otherwise dies
void Error(const char *errorMessage); // has to be used with _IONBUF to work, otherwise dies
void Format(char *input, ...);
int Menu(char *declaredNames[], void *funcPointers[], int GUI_style);

#endif