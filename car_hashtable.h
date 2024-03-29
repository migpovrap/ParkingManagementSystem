#ifndef car_hashtable
#define car_hashtable

#include "car_parks.h"

unsigned long hash(char mt[], int size);
void destroy_car_hashtable(Car* cars[], int size);
Car* search_car_hashtable (Car* cars[], char mt[], int size);
void add_car_to_end_list (Car** list_head, Car* new_car);
void clear_logcars_list (Car** list_head);
Car* search_logcars_list (char mt[], Car** list_head);
Car* remove_car_hashtable(int parknumber, ParksData* parksdata, char mt[9]);

#endif