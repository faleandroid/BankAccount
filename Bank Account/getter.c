//
//  getter.c
//  Bank Account
//
//  Created by Fabio Piazza on 29/09/15.
//  Copyright © 2015 Fabio Piazza. All rights reserved.
//

#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "getter.h"
#include "validator.h"

#define STRLEN 1024

time_t getDate()
{
    struct tm * date= (struct tm *) calloc(1, sizeof(struct tm));
    
    
    do {
        
        printf("inserisci ora : minuti , giorno/mese/anno della transazione\n");
        
        printf("ore\n");
        
        scanf("%d", &date->tm_hour);
        getchar();
        
        printf("minuti\n");
        
        scanf("%d", &date->tm_min);
        getchar();
        
        printf("giorno\n");
        
        scanf("%d", &date->tm_mday);
        getchar();
        
        printf("mese\n");
        
        scanf("%d", &date->tm_mon);
        getchar();
        
        printf("anno\n");
        
        scanf("%d", &date->tm_year);
        
        date->tm_year -= 1900;
        
        getchar();
        
    } while (!dateValidator(*date));
    
    time_t compact_date = mktime(date);
    
    return compact_date;
}

char  * getString()
{
    char * string = calloc(1, STRLEN);
    
    fgets(string, STRLEN, stdin);
    
    if ((strlen(string) > 0) && string[strlen(string)-1] == '\n')
        string[strlen(string)-1] = '\0';
    
    return string;
}