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
 */
typedef struct {
	int hours;
	int minutes;
} Time;

/**
 * @brief The struture that holds the date, year month and day.
 */
typedef struct {
	int year;
	int month;
	int day;
} Date;

/**
 * @brief The struture that holds the currenttime,
 * so date and time (day-month-year hh:mm).
 */
typedef struct {
	Time time;
	Date date;
} CurrentTime;

/**
 * @brief 
 * This function calculates the time in minutes that passed between to dates.
 * @param di The initial date.
 * @param ti The initial hours and minutes.
 * @param df The final date.
 * @param tf The final hours and minutes.
 * @return (long) Returns the time passed in minutes.
 */
long contatempo (Date di, Time ti, Date df, Time tf);

/**
 * @brief 
 * This function checks if a time is valid.
 * @param t The struture for an hour (it stores hours and minutes).
 * @return (int) Returns zero if valid, one if invalid. 
 */
int check_date(Date d);

/**
 * @brief 
 * This function checks if a determined date is valid (does not contemplate leap years).
 * @param d The date struture to check
 * @return (int) Returns zero if valid, one if invalid.
 */
int check_time (Time t);

#endif