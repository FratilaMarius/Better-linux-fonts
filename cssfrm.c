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

int GetColorCode(const char *color) {
    if(!color) return 37;
    if(!strncmp(color, "blk", 3)) return 30;
    if(!strncmp(color, "red", 3)) return 31;
    if(!strncmp(color, "grn", 3)) return 32;
    if(!strncmp(color, "yel", 3)) return 33;
    if(!strncmp(color, "blu", 3)) return 34;
    if(!strncmp(color, "mag", 3)) return 35;
    if(!strncmp(color, "cyn", 3)) return 36;
    if(!strncmp(color, "wht", 3)) return 37;
    return 37; // Default to white
}

void Format(char *input, ...) {
    if (!input) return;

    va_list lista_arg;
    va_start(lista_arg, input);
    
    // ANSI Defaults
    int color = 37;
    int background = 49;
    int bold = 22;
    int itallic = 23;
    int underline = 24;

    int counter = strlen(input);

    // Loop stops at counter - 1 to safely check j+1
    for (int j = 0; j < counter - 1; j++) {
        if(input[j] == '#') {
            if(input[j+1] == 'c') {
                color = GetColorCode(va_arg(lista_arg, char*));
            }
            else if(input[j+1] == 'b') {
                background = GetColorCode(va_arg(lista_arg, char*)) + 10;
            }
            else if(input[j+1] == 'B') {
                char *val = va_arg(lista_arg, char*);
                if(val && val[0] == 'y') bold = 1;
            }
            else if(input[j+1] == 'u') {
                char *val = va_arg(lista_arg, char*);
                if(val && val[0] == 'y') underline = 4;
            }
            else if(input[j+1] == 'i') {
                char *val = va_arg(lista_arg, char*);
                if(val && val[0] == 'y') itallic = 3;
            }
            j++; // Skip the format character so it isn't parsed again
        } 
    }
    
    printf("\033[%d;%d;%d;%d;%dm", color, background, bold, itallic, underline);
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
        printf("\n     [%d]...%s", i+1, declaredNames[i]);
    }
    printf("\n[OPTION BY NUMBER] --> ");
}

void GUI_2(char *decNames[], int nr) {
    Format("#c#B", "blu", "y");
    printf("\nCommands:\n=============================================================================");
    Format("#c#B", "cyn", "y");
    for(int i = 0; i < nr; i++) {
        printf("\n     [%d]...%s", i+1, decNames[i]);
    }
    Format("#c#B", "blu", "y");
    printf("\n=============================================================================\n[Enter command id] --> ");
    Format("");
}

int Menu(char *declaredNames[], void *funcPointers[], int GUI_style) {
    int nrOfEntries_fncP = 0;
    int nrOfEntries = 0;
    
    if (funcPointers == NULL) {
        for(int i = 0; declaredNames[i] != NULL; i++) nrOfEntries++;
    } else {
        for(int i = 0; declaredNames[i] != NULL; i++) nrOfEntries++;
        for(int i = 0; funcPointers[i] != NULL; i++) nrOfEntries_fncP++;
        
        if(nrOfEntries != nrOfEntries_fncP) {
            printf("\n\nIncorrect use of menu function: Name and Function arrays must match in size.\n");
            exit(1);
        }
    }

    switch (GUI_style) {
        case 1: GUI_1(declaredNames, nrOfEntries); break;
        case 2: GUI_2(declaredNames, nrOfEntries); break;
        default: 
            printf("\nUnrecognised UI element!\n"); 
            exit(1);
    }

    // Secure input gathering
    char inputBuffer[16];
    int input = -1;
    
    if (fgets(inputBuffer, sizeof(inputBuffer), stdin) != NULL) {
        input = strtol(inputBuffer, NULL, 10);
    }

    if (funcPointers != NULL && input > 0 && input <= nrOfEntries) {
        ((void (*)())funcPointers[input - 1])();  
        return 0;
    }
    
    if(input > 0 && input <= nrOfEntries) return input;
    
    printf("Error: Invalid menu input!\n");
    return -1;
}
