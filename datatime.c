/**
 * Functions related to handeling dates and hours, cheking if a date or a hour is valid.
 * @file datatime.c
 * @author ist1109686
*/

#include "datetime.h"

/**
 * @brief 
 * The number of days that every month has.
 * Starts at zero, so that the index of the days correspondt to months.
 */
static int  diasdomes[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/**
 * @brief 
 * This function checks if a determinated date is valid (does not contemplate leap years).
 * @param d The date struture to check
 * @return (int) Returns zero if valid, one if invalid.
 */
int check_date (date d) {
    if (d.year < 0 || d.day <= 0) 
        return 1;
        
    if (d.month < 1 || d.month > 12)
        return 1;
        
    if (d.day > diasdomes[d.month])
        return 1;
        
    return 0;
}

/**
 * @brief 
 * This function checks if a time is valid.
 * @param t The struture for an hour (it stores hours and minutes).
 * @return (int) Returns zero if valid, one if invalid. 
 */
int check_time (time t) {
    if (t.hours < 0 || t.hours > 23)
        return 1;
    if (t.minutes < 0 || t.minutes > 59)
        return 1;
    return 0;
}

/**
 * @brief 
 * This function calculates the time in minutes that passed between to dates.
 * @param di The inicial date.
 * @param ti The inicial hours and minutes.
 * @param df The final date.
 * @param tf The final hours and minutes.
 * @return (long) Returns the time passed in minutes.
 */
long contatempo (date di, time ti, date df, time tf) {                   

    long ftime = 0;
    long itime = 0;

    //Converts the final date in unixtime
    ftime = df.year * 365 * 24 * 60;
    for (int i = 1; i < df.month; i++)
        ftime += diasdomes[i] * 24 * 60;
    ftime += df.day * 24 * 60;
    ftime += (tf.hours * 60) + tf.minutes;
    
    //Converts the inicial date in unixtime
    itime = di.year * 365 * 24 * 60;
    for (int i = 0; i < di.month; i++)
        itime += diasdomes[i] * 24 * 60;
    itime += di.day * 24 * 60;
    itime += (ti.hours * 60) + ti.minutes;

    return (ftime - itime);
}
