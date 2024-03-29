/**
 * @file datetime.h
 * @author ist1109686
 * @brief The header file for the datetime c file, 
 * contains all the data structures related to date and time used in the program.
 */

#ifndef datetime_h
#define  datetime_h

#include <stdio.h>

/**
 * @brief The strucure that holds the time, hours and minutes.
 * 
 */
typedef struct {
	int hours;
	int minutes;
} time;

/**
 * @brief The struture that holds the date, year month and day.
 * 
 */
typedef struct {
	int year;
	int month;
	int day;
} date;

/**
 * @brief The struture that holds the currenttime, so date and time (day-month-year hh:mm).
 * 
 */
typedef struct {
	time t;
	date d;
} CurrentTime;

long contatempo (date di, time ti, date df, time tf);
int check_date(date d);
int check_time (time t);

#endif