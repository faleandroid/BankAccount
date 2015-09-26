//
//  main.c
//  Bank Account
//
//  Created by Fabio Piazza on 04/09/15.
//  Copyright (c) 2015 Fabio Piazza. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <jansson.h>

#define REVENUE "/Users/fabio/Desktop/revenue.json"
#define LOST "/Users/fabio/Desktop/lost.json"

#define STRLEN 1024

struct lost
{
    char * title;
    float value;
    char * description;
    time_t date;
    
    struct lost * next;
};

struct revenue
{
    char * title;
    float value;
    char * description;
    time_t date;

    struct revenue * next;
} ;

struct revenue * firstRevenueMovement = NULL;
struct lost * firstLostMovement = NULL;

float total = 0;

void getTotal(char * revenue)
{
    json_error_t * error;
    
    FILE * revenueFile = fopen(strcat(revenue, "/revenue.json"), "r");
    
    json_t * json_total = json_loadf(revenueFile, 0, error);
    
    json_total = json_object_get(json_total, "total");
    
    total = json_real_value(json_total);
    
    free(json_total);
    
    fclose(revenueFile);
}

int dateValidator(struct tm date)
{
    int valid = 1;
    
    if (date.tm_year < 0)
    {
        valid = 0;
        
        return valid;
    }
    
    if (date.tm_mon < 0 || date.tm_mon > 11)
    {
        valid = 0;
        
        return valid;
    }
    
    switch (date.tm_mon) {
        case 9:
        case 3:
        case 5:
        case 8:
            
            if (date.tm_mday < 0 || date.tm_mday > 30)
            {
                valid = 0;
                return valid;
            }
            break;
            
        case 1:
            
            if (date.tm_mday < 0 || date.tm_mday > 28)
            {
                valid = 0;
                return valid;
            }
            break;
            
        default:
            
            if (date.tm_mday < 0 || date.tm_mday > 31)
            {
                valid = 0;
                return valid;
            }
            
            break;
    }
    
    if (date.tm_hour < 0 || date.tm_hour > 23)
    {
        valid = 0;
        
        return valid;
    }
    
    if (date.tm_min < 0 || date.tm_min > 59)
    {
        valid = 0;
        
        return valid;
    }
    
    return valid;
}

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

int createRevenueMovementList(FILE * revenue)
{
    json_error_t * error;
    
    size_t index;
    json_t *value;
    
    json_t * json_revenue = json_loadf(revenue, 0, error);
    
    json_revenue = json_object_get(json_revenue, "revenue");
    
    if (json_revenue == NULL)
    {
        return 0;
    }
    
    json_array_foreach(json_revenue, index, value)
    {
        struct revenue * newRevenue;
        
        newRevenue = calloc(1, sizeof(struct revenue));
        
        json_t * json_title = json_object_get(value, "title");
        
        json_t * json_value = json_object_get(value, "value");
        
        json_t * json_date = json_object_get(value, "date");
        
        json_t * json_desc = json_object_get(value, "description");
        
        char * title = strdup(json_string_value(json_title));
        
        char * description = strdup(json_string_value(json_desc));
        
        float value  = json_real_value(json_value);
        
        time_t date = json_integer_value(json_date);
        
        newRevenue->title = malloc(sizeof(title));
        newRevenue->title = title;
        
        newRevenue->description = malloc(sizeof(description));
        newRevenue->description = description;
        
        newRevenue->value = value;
        newRevenue->date = date;
        
        newRevenue->next = firstRevenueMovement;
        
        firstRevenueMovement = newRevenue;
        
    }
    
    return 1;
    
}

int createLostMovementList(FILE * lost)
{
    json_error_t * error;
    
    size_t index;
    json_t *value;
    
    json_t * json_lost = json_loadf(lost, 0, error);
    
    json_lost = json_object_get(json_lost, "lost");
    
    if (json_lost == NULL)
    {
        return 0;
    }
    
    json_array_foreach(json_lost, index, value)
    {
        struct lost * newLost;
        
        newLost = calloc(1, sizeof(struct lost));
        
        json_t * json_title= json_object_get(value, "title");
        
        json_t * json_value= json_object_get(value, "value");
        
        json_t * json_date= json_object_get(value, "date");
        
        json_t * json_desc = json_object_get(value, "description");
        
        char * title = strdup(json_string_value(json_title));
        
        char * description = strdup(json_string_value(json_desc));
        
        float value  = json_real_value(json_value);
        
        time_t date = json_integer_value(json_date);
        
        newLost->title = malloc(sizeof(title));
        newLost->title = title;
        
        newLost->description = malloc(sizeof(description));
        newLost->description = description;
        
        newLost->value = value;
        newLost->date = date;
        
        newLost->next = firstLostMovement;
        
        firstLostMovement = newLost;
        
    }
    
    return 1;
    
}

json_t * createRevenueArray(struct revenue * revenue){
    
    json_t * json_revenue = json_object();
        
    json_t * title = json_string(revenue->title);
        
    json_t * desc = json_string(revenue->description);
        
    json_t * value = json_real(revenue->value);
        
    json_t * date = json_integer(JSON_INTEGER_IS_LONG_LONG);
        
    json_integer_set(date, revenue->date);
        
    json_object_set(json_revenue, "title", title);
    json_object_set(json_revenue, "description", desc);
    json_object_set(json_revenue, "value", value);
    json_object_set(json_revenue, "date", date);
        
    return json_revenue;
    
}

json_t * createLostArray(struct lost * lost)
{
    json_t * json_lost = json_object();
    
    json_t * title = json_string(lost->title);
    
    json_t * desc = json_string(lost->description);
    
    json_t * value = json_real(lost->value);
    
    json_t * date = json_integer(JSON_INTEGER_IS_LONG_LONG);
    
    json_integer_set(date, lost->date);
    
    json_object_set(json_lost, "title", title);
    json_object_set(json_lost, "description", desc);
    json_object_set(json_lost, "value", value);
    json_object_set(json_lost, "date", date);
    
    return json_lost;
}

void addNewRevenueMovement()
{
    float value;
    char * title = calloc(1, STRLEN), * description = calloc(1, STRLEN);
    
    printf("adding a new Revenue Movement\n");
    
    title = strdup(getString());
    
    printf("inserisci una descrizione\n");
    
    description = strdup(getString());
    
    printf("inserisci il valore della transazione\n");
    
    scanf("%f", &value);
    getchar();
    
    time_t compact_date = getDate();
    
    struct revenue * newRvenue = malloc(sizeof(struct revenue));
    
    newRvenue->title = strdup(title);
    
    newRvenue->description = strdup(description);
    
    newRvenue->value = value;
    
    newRvenue->date = compact_date;
    
    newRvenue->next = firstRevenueMovement;
    
    firstRevenueMovement = newRvenue;
    
}

void addNewLostMovement()
{
    
    float value;
    char * title = calloc(1, STRLEN), * description = calloc(1, STRLEN);
    
    printf("adding a new Lost Movement:\n");
    
    title = strdup(getString());
    
    printf("inserisci una descrizione\n");
    
    description = strdup(getString());
    
    printf("inserisci il valore della transazione\n");
    
    scanf("%f", &value);
    getchar();
    
    time_t compact_date = getDate();
    
    struct lost * newLost = malloc(sizeof(struct lost));
    
    newLost->title = strdup(title);
    
    newLost->description = strdup(description);
    
    newLost->value = (value * -1);
    
    newLost->date = compact_date;
    
    newLost->next = firstLostMovement;
    
    firstLostMovement = newLost;
    
}

void saveRevenueMovements(char * save_path)
{
    json_t * json_revenue = json_object();
    
    json_t * array = json_array();

    json_t * json_total = json_real(total);
    
    for (struct revenue * revenue = firstRevenueMovement; revenue!=0; revenue=revenue->next)
    {
        json_array_append(array, createRevenueArray(revenue));
    }
    
    int i  = json_object_set(json_revenue, "total", json_total);
    
    int j  = json_object_set(json_revenue, "revenue", array);
    
    if (i == -1 || j == -1)
    {
        printf("errore non è possibile scrivere il file");
    }
    
    json_dump_file(json_revenue, strcat(save_path, "/revenue.json"), JSON_INDENT(3));
    
}

void saveLostMovements(char * save_path)
{
    json_t * json_lost = json_object();
    
    json_t * array = json_array();
    
    for (struct lost * lost = firstLostMovement; lost!=0; lost=lost->next)
    {
        json_array_append(array, createLostArray(lost));
    }
    
    int i  = json_object_set(json_lost, "lost", array);
    
    if (i == -1)
    {
        printf("errore non è possibile scrivere il file");
    }
    
    json_dump_file(json_lost, strcat(save_path, "/lost.json"), JSON_INDENT(3));
}

void listAllMovements()
{
    struct revenue * revenue;
    struct lost * lost;
    
    printf("entrate:\n");
    
    if (firstRevenueMovement == NULL)
    {
        printf("non ci sono entrate da visualizzare\n");
        
    } else {
        
        for (revenue = firstRevenueMovement; revenue != NULL; revenue = revenue->next) {
            
            printf("titolo: %s \n", revenue->title);
            printf("descrizione: %s \n", revenue->description);
            printf("value: %f \n", revenue->value);
            printf("data: %s \n", ctime(&revenue->date));
            
            printf("\n");
            
        }
    }

    printf("\n");
    
    printf("uscite:\n");
    
    if (firstLostMovement == NULL)
    {
        printf("non ci sono uscite da visualizzare\n");
        
    } else {
        
        for (lost = firstLostMovement; lost != NULL; lost = lost->next) {
            
            printf("titolo: %s \n", lost->title);
            printf("descrizione: %s \n", lost->description);
            printf("value: %f \n", lost->value);
            printf("data: %s \n", ctime(&lost->date));
            
            
            printf("\n");
        }
        
    }
    
    printf("\n");
    
}

struct revenue * findRevenueMovement(char * string)
{
    struct revenue * revenue;
    
    for (revenue = firstRevenueMovement ; revenue!=NULL; revenue=revenue->next)
    {
        if (strcmp(string, revenue->title) == 0)
        {
            return revenue;
        }
    }
    
    return NULL;
}

struct revenue * findPrevRevenueMovement(char * string)
{
    struct revenue * revenue = firstRevenueMovement;
    
    if (strcmp(string, revenue->title) == 0)
    {
        return revenue;
    }
    
    for (revenue = firstRevenueMovement ; revenue!=NULL; revenue=revenue->next)
    {
        if (strcmp(string, revenue->next->title) == 0)
        {
            return revenue;
        }
    }
    
    return NULL;
}

struct lost * findLostMovement(char * string)
{
    struct lost * lost;
    
    for (lost = firstLostMovement ; lost!=NULL; lost=lost->next)
    {
        if (strcmp(string, lost->title) == 0)
        {
            return lost;
        }
    }
    
    return NULL;
}

struct lost * findPrevLostMovement(char * string)
{
    struct lost * lost = firstLostMovement;
    
    if (strcmp(string, lost->title) == 0)
    {
        return lost;
    }
    
    for (lost = firstLostMovement; lost!=NULL; lost=lost->next)
    {
        if (strcmp(string, lost->next->title) == 0)
        {
            return lost;
        }
    }
    
    return NULL;
}

char * searchTitle()
{
    
    char * title = calloc(1, STRLEN);
    
    while (1)
    {
        printf("inserire il titolo della transazione che si vuole modificare o eliminare:\n");
        
        title = strdup(getString());
        
        if (*title == '\0')
        {
            printf("titolo non inserito vuoi riprovare?\n");
            
            char prova;
            
            scanf("%c", &prova);
            
            if (prova == 'N' || prova == 'n')
            {
                *title = '\0';
                
                return title;
            }
            
        } else return title;
    }
    
}

void changeRevenueField(struct revenue * revenue)
{
    
    float value;
    char * title = malloc(STRLEN), * description = malloc(STRLEN);
    
    char ch;
    
    printf("vuoi aggiornare il titolo? y on n\n");
    
    scanf("%c", &ch);
    getchar();
    
    if (ch == 'y' || ch == 'Y')
    {
        printf("inserisci un titolo per la transazione!\n");
        
        title = getString();
        
        revenue->title = strdup(title);
    }
    
    printf("vuoi aggiornare la descrizione? y on n\n");
    
    scanf("%c", &ch);
    getchar();
    
    if (ch == 'y' || ch == 'Y')
    {
        printf("inserisci una descrizione\n");
        
        description = strdup(getString());
        
        revenue->description = strdup(description);
    }
    
    printf("vuoi aggiornare il valore? y on n\n");
    
    scanf("%c\n", &ch);
    getchar();
    
    if (ch == 'y' || ch == 'Y')
    {
        printf("inserisci il valore della transazione\n");
        
        scanf("%f", &value);
        
        revenue->value = value;
    }
    
    printf("vuoi aggioranre la data? y on n\n");
    
    scanf("%c\n", &ch);
    getchar();
    
    if (ch == 'y' || ch == 'Y')
    {
        time_t compact_date = getDate();
        
        revenue->date = compact_date;
    }
}

void changeLostField(struct lost * lost)
{
    float value;
    char * title = malloc(STRLEN), * description = malloc(STRLEN);
    
    char ch;
    
    printf("vuoi aggiornare il titolo? y on n\n");
    
    scanf("%c", &ch);
    getchar();
    
    if (ch == 'y' || ch == 'Y')
    {
        printf("inserisci un titolo per le entrate!\n");
        
        title = getString();
        
        lost->title = strdup(title);
    }
    
    printf("vuoi aggiornare la descrizione? y on n\n");
    
    scanf("%c", &ch);
    getchar();
    
    if (ch == 'y' || ch == 'Y')
    {
        printf("inserisci una descrizione\n");
        
        description = strdup(getString());
        
        lost->description = strdup(description);
    }
    
    printf("vuoi aggiornare il valore? y on n\n");
    
    scanf("%c\n", &ch);
    getchar();
    
    if (ch == 'y' || ch == 'Y')
    {
        printf("inserisci il valore della transazione\n");
        
        scanf("%f", &value);
        
        lost->value = value;
    }
    
    printf("vuoi aggioranre la data? y on n\n");
    
    scanf("%c\n", &ch);
    getchar();
    
    if (ch == 'y' || ch == 'Y')
    {
        time_t compact_date = getDate();
        
        lost->date = compact_date;
    }
}

int updateRevenue()
{
    int stop;
    
    struct revenue * revenue;
    
    char * title = calloc(1, STRLEN);
    
    do
    {
        title = strdup(searchTitle());
        
        if (title == '\0') return stop = 0;
        
        revenue = findRevenueMovement(title);
        
        if (revenue == NULL)
        {
            printf("nessuna transazione trovata\n");
        
            char prova = '\0';
        
            scanf("%c", &prova);
        
            if (prova == 'N' || prova == 'n')
            {
                stop = 0;
                break;
            }
        
        }
        
    } while (revenue == NULL);
    
    
    if (stop == 0) return stop;
    
    changeRevenueField(revenue);
    
    stop = 1;
    
    return stop;
    
}

int updateLost()
{
    int stop = 1;
    
    struct lost * lost;
    
    char * title = calloc(1, STRLEN);
    
    do
    {
        title = strdup(searchTitle());
        
        if (title == '\0') return stop = 0;
        
        lost = findLostMovement(title);
        
        if (lost == NULL)
        {
            printf("nessuna transazione trovata\n");
            
            char prova = '\0';
            
            scanf("%c", &prova);
            getchar();
            
            if (prova == 'N' || prova == 'n')
            {
                stop = 0;
                break;
            }
            
        }
        
    } while (lost == NULL);
    
    
    if (stop == 0) return stop;
    
    changeLostField(lost);
    
    return stop;
    
}

void updateTotal()
{
    struct revenue * revenue;
    
    struct lost * lost;
    
    for (revenue = firstRevenueMovement; revenue != NULL; revenue = revenue->next)
    {
        total += revenue->value;
    }
    
    for (lost = firstLostMovement; lost != NULL; lost = lost->next)
    {
        total += lost->value;
    }
}

int deleteRevenue()
{
    int stop;
    
    struct revenue * revenue;
    
    char * title = calloc(1, STRLEN);
    
    do
    {
        title = strdup(searchTitle());
        
        if (title == '\0') return stop = 0;
        
        revenue = findPrevRevenueMovement(title);
        
        if (revenue == NULL)
        {
            printf("nessuna transazione trovata\n");
            
            char prova = '\0';
            
            scanf("%c", &prova);
            
            if (prova == 'N' || prova == 'n')
            {
                stop = 0;
                break;
            }
            
        }
        
    } while (revenue == NULL);
    
    
    if (stop == 0) return stop;
    
    struct revenue * p;
    
    if (revenue == firstRevenueMovement)
    {
        p = firstRevenueMovement;
        
        firstRevenueMovement = firstRevenueMovement->next;

    } else {
        
        p  = revenue->next;
        
        revenue->next = revenue->next->next;
    }
    
    free(p);
    
    stop = 1;
    
    return stop;
}

int deleteLost()
{
    int stop;
    
    struct lost * lost;
    
    char * title = calloc(1, STRLEN);
    
    do
    {
        title = strdup(searchTitle());
        
        if (title == '\0') return stop = 0;
        
        lost = findPrevLostMovement(title);
        
        if (lost == NULL)
        {
            printf("nessuna transazione trovata\n");
            
            char prova = '\0';
            
            scanf("%c", &prova);
            
            if (prova == 'N' || prova == 'n')
            {
                stop = 0;
                break;
            }
            
        }
        
    } while (lost == NULL);
    
    
    if (stop == 0) return stop;
    
    struct lost * p;
    
    if (lost == firstLostMovement)
    {
        p = firstLostMovement;
        
        firstLostMovement = firstLostMovement->next;
        
    } else {
        
        p  = lost->next;
        
        lost->next = lost->next->next;
    }
    
    free(p);
    
    stop = 1;
    
    return stop;
}

void freeRevenue()
{
    struct revenue * revenue;
    
    for (revenue = firstRevenueMovement; revenue!=NULL; )
    {
        struct revenue * bk = revenue->next;
        
        free(revenue);
        
        revenue = bk;
    }
}

void freeLost()
{
    struct lost * lost;
    
    for (lost = firstLostMovement; lost!=NULL; lost = lost->next)
    {
        struct lost * bk = lost->next;
        
        free(lost);
        
        lost = bk;
    }
}

void updateSavePath()
{
    FILE *save;
    
    char path[STRLEN];
    
    DIR * revenue = NULL;
    
    do
    {
        printf("Where should i save your docs?\n");
        
        fgets(path, STRLEN, stdin);
        
        if ((strlen(path) > 0) && path[strlen(path)-1] == '\n')
            path[strlen(path)-1] = '\0';
        
        if (*path != '\0')
        {
            revenue = opendir(path);
        }
        
        if (revenue == NULL) printf("riprova path non valido\n");
        
    } while (revenue == NULL);
    
    save = fopen(".save.txt", "w");
    
    fwrite(path, 1, strlen(path), save);
    
    fclose(save);
}

char * firstOpen_update(FILE * save)
{
    char * path = calloc(1, STRLEN);
    
    save = fopen(".save.txt", "r");
    
    if (save != NULL)
    {
        fgets(path, STRLEN, save);
        
        if (*path != '\0') return path;
    }
    
    fclose(save);
    
    return '\0';
}

int selection(char * save_path)
{
    
    char c;
    
    char * bk = calloc(1, STRLEN);
    
    while (1)
    {
     printf("dimmi cosa vuoi fare: 'a' per aggiungere, 'u' per aggiornare,"
            " 'd' per eliminare, 'l' per visualizzare i movimenti,"
            " 's' per modficare la cartella di salvataggio e 'e' per uscire\n");
     
     scanf("%c", &c);
     getchar();
     
     switch (c) {
         case 's' :
             
             updateSavePath();
             
             break;
             
         case 'a':
     
             printf("vuoi aggiungere un positivo (p) o un negativo (n)? \n");
     
             scanf("%c", &c);
             getchar();
     
             if (c == 'P' || c == 'p')
             {
                 addNewRevenueMovement();
             }
     
             if (c == 'n' || c == 'N')
             {
                 addNewLostMovement();
             }
     
             break;
         case 'u':
     
             printf("vuoi aggiornare un positivo (p) o un negativo (n)? \n");
     
             scanf("%c", &c);
             getchar();
     
             if (c == 'P' || c == 'p')
             {
                 updateRevenue();
             }
     
             if (c == 'n' || c == 'N')
             {
                 updateLost();
             }
     
             break;
         case 'd':
     
             printf("vuoi cancellare un positivo (p) o un negativo (n)? \n");
     
             scanf("%c", &c);
             getchar();
     
             if (c == 'P' || c == 'p')
             {
                 deleteRevenue();
             }
     
             if (c == 'n' || c == 'N')
             {
                 deleteLost();
             }
     
             break;
         case 'l':
     
             listAllMovements();
     
             break;
         case 'e':
     
             updateTotal();
             
             bk = strdup(save_path);
             
             saveLostMovements(save_path);
             
             saveRevenueMovements(bk);
             
             //freeRevenue();
             //freeLost();
             
             printf("goodBye\n");
             
             return 0;
             
             break;
     
         default:
     
             printf("comando non riconosciuto riprovare!!\n");
     
             break;
     }
     
     
     }
}



int main(int argc, const char * argv[])
{
    FILE * revenue = NULL, * lost = NULL, * save = NULL;
    
    char * save_path  = calloc(1, STRLEN), * bk = calloc(1, STRLEN);
    
    save_path = firstOpen_update(save);
    
    bk = strdup(save_path);

    if (save_path == '\0') updateSavePath();
    
    else
    {
        revenue = fopen(strcat(save_path, "/revenue.json"), "r");
        
        strcpy(save_path, bk);
        
        lost = fopen(strcat(bk, "/lost.json"), "r");
    }
    
    createRevenueMovementList(revenue);
    createLostMovementList(lost);
    
    fclose(revenue);
    fclose(lost);
    
    strcpy(bk, save_path);
    
    getTotal(bk);
    
    selection(save_path);

    return 0;
}
