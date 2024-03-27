#include "datetime.h"
static int  diasdomes[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // Começa em zero de forma a que os indices correspondam aos meses do ano.
date readdata() {
    date d;
    scanf("%d-%d-%d", &d.day, &d.month, &d.year);
    return d;
}   

time readtime() {
    time h;
    scanf("%d:%d", &h.hours, &h.minutes);
    return h;
}

int check_date (date d) {
    if (d.year < 0 || d.day <= 0) 
        return 1;
        
    if (d.month < 1 || d.month > 12)
        return 1;
        
    if (d.day > diasdomes[d.month])
        return 1;
        
    return 0;
}

int check_time (time t) {
    if (t.hours < 0 || t.hours > 23)
        return 1;
    if (t.minutes < 0 || t.minutes > 59)
        return 1;
    return 0;
}

long contatempo (date di, time ti, date df, time tf) {                   

    long ftime = 0;
    long itime = 0;

    //Converte a data final em unix time
    ftime = df.year * 365 * 24 * 60;
    for (int i = 1; i < df.month; i++)
        ftime += diasdomes[i] * 24 * 60;
    ftime += df.day * 24 * 60;
    ftime += (tf.hours * 60) + tf.minutes;
    
    //Converte a data inicial em unix time
    itime = di.year * 365 * 24 * 60;
    for (int i = 0; i < di.month; i++)
        itime += diasdomes[i] * 24 * 60;
    itime += di.day * 24 * 60;
    itime += (ti.hours * 60) + ti.minutes;

    return (ftime - itime);
}
