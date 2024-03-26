#ifndef datetime_h
#define  datetime_h

#include <stdio.h>

typedef struct {
	int hours;
	int minutes;
} time;

typedef struct {
	int year;
	int month;
	int day;
} date;

typedef struct {
	time t;
	date d;
} CurrentTime;

date readdata();
time readtime();
long contatempo (date di, time ti, date df, time tf);
int check_date(date d);
int check_time (time t);

#endif