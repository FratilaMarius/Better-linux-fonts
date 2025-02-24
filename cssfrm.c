/* advfrm.c
call with funcPointers = NULL for MENU to return just input (int)
 both arrays are decced: *arr[] = {... , NULL}, they end with NULL
 the functions Menu() can take are only voids with no arguments
 2 GUI styles available
    EXTRAS::
      -->> text formatting: TEXT_COLOUR(int value) [30-39]
                            TEXT_RESET()
                            TEXT_BOLD()
                            TEXT_UNDERLINE()
      -->> BACKGROUND_COLOUR(int value) [40-49]
      -->> BACKGROUND_RESET()

      ______ Format("") #c -> font color |  asnii color codes: "red", "blu", "cyn", "wht", etc..
                        #b -> bckgr color|  
                        #B -> bold        |
                        #u -> underline   |   "y" or "n"
                        #i -> italic      |
*/
#include "cssfrm.h"

#define TEXT_COLOUR(colourCode) printf("\033[%dm", colourCode)
#define TEXT_RESET() printf("\033[0m")
#define TEXT_BOLD() printf("\033[1m")
#define TEXT_UNDERLINE() printf("\033[4m")
#define BACKGROUND_COLOUR(colourCode) printf("\033[%dm", colourCode)
#define BACKGROUND_RESET() printf("\033[49m")

void __StringToInt_forColors__(char *color) {
  if(!strncmp(color, "blk", 3)) color[0] = 30;
  if(!strncmp(color, "red", 3)) color[0] = 31;
  if(!strncmp(color, "grn", 3)) color[0] = 32;
  if(!strncmp(color, "yel", 3)) color[0] = 33;
  if(!strncmp(color, "blu", 3)) color[0] = 34;
  if(!strncmp(color, "mag", 3)) color[0] = 35;
  if(!strncmp(color, "cyn", 3)) color[0] = 36;
  if(!strncmp(color, "wht", 3)) color[0] = 37;
}

void Format(char *input, ...) {
  va_list lista_arg;
  va_start(lista_arg, input);
  char color[4] = {0}, *background[4] = {0}, bold, underline, itallic;
  // defaults:
  bold = 22;
  itallic = 23;
  underline= 24;
  color[0] = 37;
  background [0] = 49;
// getting the number of characters in the string:
  int counter = 0;
  while (input[counter] != 0) {
    counter++;
    if(counter == 500) {
      printf("Error in reading number of chars in string input");
      return;
    }
  }
// parcurge stringul si ia inputul:
  for (int j = 0; j < counter; j++) {
    if(input[j] == 0x23 && input[j+1] != 0x0) {
      if(input[j+1] == 99) {
        strncpy(color, va_arg(lista_arg, char*), 4);
        __StringToInt_forColors__(color);
      }
      if(input[j+1] == 98) {
        strncpy(background,va_arg(lista_arg, char*), 4);
        __StringToInt_forColors__(background); background[0] += 10;
      }
      if(input[j+1] == 66) {
        strncpy(&bold, va_arg(lista_arg, char*), 1);
        if(bold == 121) bold = 1;
      }
      if(input[j+1] == 117) {
        strncpy(&underline, va_arg(lista_arg, char*), 1);
        if(underline == 121) underline = 4;
      }
      if(input[j+1] == 105) {
        strncpy(&itallic, va_arg(lista_arg, char*), 1);
        if(underline == 121) itallic = 3;
      }
      j++;
    } 
  }
  // color si background[0] sunt seatate cum trebuie si acu mai tre sa faci la celelalte 3 si sq
  // vezi cum faci printarea
  printf("\033[%d;%d;%d;%d;%dm", color[0], background[0], bold, itallic, underline);

  va_end(lista_arg);
}

void Message(const char *message) {
  Format("#c #i", "grn", "y");
  printf("%s", message);
  Format("");
}
void Error(const char *errorMessage) {
  Format("#c#B", "red", "y");
  perror(errorMessage);
  Format("");
}

void GUI_1(char *declaredNames[], int nrOfEntries) {
  printf("\n\n_MENU_:");
  
  for(int i = 0; i < nrOfEntries; i++) {
    printf("\n     [%d]...%s", i+1 ,declaredNames[i]);
  }
  printf("\n[OPTION BY NUMBER] --> ");
}
void GUI_2(char *decNames[], int nr) {
  Format("#c#B", "blu", "y");

  printf("\nCommands:\n=============================================================================");
  Format("#c#B", "cyn", "y");
  for(int i = 0; i < nr; i++) {
    printf("\n     [%d]...%s", i+1 ,decNames[i]);
  }
  Format("#c#B", "blu", "y");
  printf("\n=============================================================================\n[Enter command id] --> ");
  Format("");
}
int Menu(char *declaredNames[], void *funcPointers[], int GUI_style) {
  //asses how many UI and function ellements there are:
  int nrOfEntries_fncP = 0;
  int nrOfEntries = 0;
  if (funcPointers == NULL) 
    for(int i = 0; declaredNames[i] != NULL; i++) {
      nrOfEntries++;
    }
  if (funcPointers != NULL) {
    for(int i = 0; declaredNames[i] != NULL; i++) {
      nrOfEntries++;
    }
    for(int i = 0; funcPointers[i] != NULL; i++) {
      nrOfEntries_fncP++;
    }
    if(nrOfEntries != nrOfEntries_fncP) {
      printf("\n\nIncorect use of menu function");
      exit(1);
    }
  }
  // GUI:
  switch (GUI_style) {
    case 1:
      GUI_1(declaredNames, nrOfEntries);
      break;
    case 2:
      GUI_2(declaredNames, nrOfEntries);
      break;

    default: 
    printf("\nUnrecognised UI element!"); 
    exit(1);
      break;
  }
  // take the input:
  int input = -1;
  scanf("%d", &input);
  if (funcPointers != NULL && input != -1) {
    ((void (*)())funcPointers[input - 1])();  
    return 0;
  }
  if(input != -1) return input;
  else printf("Error in recieving menu input!\n");
}