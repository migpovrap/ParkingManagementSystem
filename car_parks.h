#ifndef car_parks_h
#define car_parks_h

#include "datetime.h"
#include <string.h>
#include <stdlib.h>

#define MAX_PARK 20
#define BILLING_INTERVAL 15
#define INCREMENT 256
#define HASHTABLE_RATIO 2


typedef struct Car{

	char mt[9];
	time entrytime;
	date entrydate;
	time exittime;
	date exitdate;
	float cost;
	struct Car* next; //Ponteiro para o próximo carro, forma de resolver colisões
} Car;

typedef struct {
	int maxcapacity;
	float price_15;
	float price_15_1hour;
	float price_dailymax;
	int ncars; // Numero de carros que estão atualmente no parque
	Car** cars;
	Car* logcars; //Tem de ser uma linked list
	int s_cars; //Tamanho da hashtable dos carros
	char *name; 
	
} Park;

typedef struct {
	int nparks;
	Park parks[MAX_PARK];
	CurrentTime ctime;

} ParksData;

typedef struct {
		int parknumber;
		char *parkname;
	} ParkSort;

int number_plate_check (char temp[9]);
void list_parking (ParksData* parksdata);
void list_parking_alfa(ParksData* parksdatas);
int create_parking_check(Park temp, ParksData* parksdata);
void destry_parking (Park* temp, int s_cars);
int create_parking (char parkname[], int capacity, float price_15, float price_15_1hour, float price_dailymax, ParksData* parksdata);
int remove_parking (char parkname[], ParksData* parksdata);
void exit_program(ParksData* parksdata);
int validate_timedate (date d, time t, ParksData* parksdata);
int add_car_to_park(char parkname[], char mt[], date di, time ti, ParksData* parksdata);
int add_car_to_park_check(char parkname[], char mt[], date d, time t, ParksData* parksdata, int* parknumber);
int car_exit_park(char parkname[], char mt[9], date df, time tf, ParksData* parksdata);
float parking_cost(long contatempo, float price_15, float price_15_1hour, float price_dailymax);
int list_cars_entries_exits (char mt[], ParksData* parksdata);
int park_revenue_data(char parkname[], ParksData* parksdata);
int check_park_revenue_parkname(char parkname[], ParksData* parksdata, int* parknumber);
int park_revenue_car(char parkname[], ParksData* parksdata, date d);
int check_park_revenue_date(date d, ParksData* parksdata);
ParkSort* alfabetic_sort_parks (ParksData* parksdata);

#endif
