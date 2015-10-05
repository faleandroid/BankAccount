//
//  validator.c
//  Bank Account
//
//  Created by Fabio Piazza on 29/09/15.
//  Copyright © 2015 Fabio Piazza. All rights reserved.
//

#include <time.h>

#include "validator.h"

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