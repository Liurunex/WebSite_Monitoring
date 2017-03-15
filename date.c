/* 
 * Author: Zhixin Liu
 * Duck ID: zhixinl
 * UO ID: 951452405
 * Authorship Statement:
 * This is my own work.
*/

#include "date.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct date {
	int day;
	int month;
	int year;
};

/*
 * date_create creates a Date structure from `datestr`
 * `datestr' is expected to be of the form "dd/mm/yyyy"
 * returns pointer to Date structure if successful,
 *         NULL if not (syntax error)
 */
Date *date_create(char *datestr) {
	
	Date *to_user;
	to_user = (Date *) malloc (sizeof(Date));

	if( to_user != NULL) {
		//memset(to_user, 0, sizeof(Date));
		int timeformat[3] = {0,0,0};
		int index = 0;
		int len = strlen(datestr);
		const char delimit[1] = "/";
		char *token;
		int illegal_date = 0;
		
		/* check the date format */
		if(len != 10)
			return NULL;
		if(datestr[2] != delimit[0] || datestr[5] != delimit[0])
			return NULL;
		for(index = 0; index < len; index++) {
			if(index == 2 || index == 5)
				continue;
			if(datestr[index] < '0' || datestr[index] > '9')
				return NULL;
		}
		/* check time illegality */
		index = 0;
		char split[] = "DD/MM/YYYY";
		strcpy(split, datestr);
		token = strtok(split, delimit);
		while(token != NULL) {
			timeformat[index] = atoi(token);
			index++;
			token = strtok(NULL, delimit);
		}

		if(timeformat[2] < 1)
			return NULL;
		if(timeformat[1] < 1 || timeformat[1] > 12)
			return NULL;

		switch(timeformat[1]) {
			case 2:
				/* leap year */
				if((timeformat[2] % 400 == 0) || ((timeformat[2] % 4 == 0 ) && 
					(timeformat[2] % 100 != 0))) {
					
					if(timeformat[0] > 29 || timeformat[0] < 1)
						illegal_date = 1;
				}
				/* common year */
				else {
					if(timeformat[0] > 28 || timeformat[0] < 1)
						illegal_date = 1;
				}
				break;
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
				if(timeformat[0] > 31 || timeformat[0] < 1)
					illegal_date = 1;
				break;
			default:
				if(timeformat[0] > 30 || timeformat[0] < 1)
					illegal_date = 1;
		}
		if(illegal_date == 1)
			return NULL;
		/* add date into Date structure */
		to_user->day = timeformat[0];
		to_user->month = timeformat[1];
		to_user->year = timeformat[2];

		return to_user;
	}
	else 
		return NULL;
}

/*
 * date_duplicate creates a duplicate of `d'
 * returns pointer to new Date structure if successful,
 *         NULL if not (memory allocation failure)
 */
Date *date_duplicate(Date *d) {
	
	Date *to_user;
	to_user = (Date *) malloc (sizeof(Date));

	if(to_user != NULL) {
		
		to_user->day = d->day;
		to_user->month = d->month;
		to_user->year = d->year;

		return to_user;
	}
	else 
		return NULL;
}

/*
 * date_compare compares two dates, returning <0, 0, >0 if
 * date1<date2, date1==date2, date1>date2, respectively
 */
int date_compare(Date *date1, Date *date2) {
	
	int intCompare = (date1->day - date2->day) 
		+ (date1->month - date2->month) * 100 
		+ (date1->year - date2->year) * 10000;
	
	if(intCompare > 0) {
		return 1;
	}
	else if (intCompare < 0) {
		return -1;
	}
	else 
		return 0;
}

/*
 * date_destroy returns any storage associated with `d' to the system
 */
void date_destroy(Date *d) {
	if(d != NULL)
		free(d);
}