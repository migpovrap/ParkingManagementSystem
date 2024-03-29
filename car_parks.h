/**
 * @file car_parks.h
 * @author ist1109686
 * @brief The header file for the c file that contains the main functions essential for the program functionality 
 */
#ifndef car_parks_h
#define car_parks_h

#include "datetime.h"
#include <string.h>
#include <stdlib.h>

//The maximum number of parks in the system
#define MAX_PARK 20
//The ratio for creating the hashtable that stores the cars in the system
#define HASHTABLE_RATIO 2
//The interval that the park bills the cars
#define BILLING_INTERVAL 15

/**
 * @brief The structure for cars in the system
 * 
 */
typedef struct Car{

	char mt[9];
	time entrytime;
	date entrydate;
	time exittime;
	date exitdate;
	float cost;
	struct Car* next; //Pointer to the next car in the linkedlist
} Car;

/**
 * @brief The structure for storing each park, in this struture the car hashtable and the history linkedlist have their pointers stored, each park has one
 * 
 */
typedef struct {
	int maxcapacity;
	float price_15;
	float price_15_1hour;
	float price_dailymax;
	int ncars; // Number of cars in the park
	Car** cars; //Pointer to a pointer of car that is used to create the dynamic array of the hashtable(a array of pointers to car)
	Car* logcars; //Pointer for the head of the linkedlist 
	int s_cars; //Size of the cars hashtable
	char *name; 
} Park;

/**
 * @brief Main structure, it holds the number of parks in the system, a fixed size array for the parks structs and a CurrentTime struture
 * 
 */
typedef struct {
	int nparks;
	Park parks[MAX_PARK];
	CurrentTime ctime;

} ParksData;

/**
 * @brief A sort of temporary structure that is used to sort the parks in alfabetic order in a non destructive way
 * 
 */
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
