/**
 * @file car_parks.h
 * @author ist1109686
 * @brief The header file for the c file that contains the main functions
 * essential for the program functionality 
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
 * @brief Car data.
 */
typedef struct Car{

	char license_plate[9];
	Time entry_time;
	Date entry_date;
	Time exit_time;
	Date exit_date;
	float cost;
	struct Car* next; //Pointer to the next car in the linkedlist.
} Car;

/**
 * @brief The structure for storing each park, in this struture the car 
 * hashtable and the history linkedlist have their pointers stored, 
 * each park has one.
 */
typedef struct {
	int max_capacity;
	float quarter_hourly_rate; // Rate for 15 min blocks
	float quarter_hourly_rate_after_first_hour; // Rate for 1 hour blocks
	float max_daily_cost; // Max daily rate
	int num_cars; // Number of cars in the park
	Car** cars; //Pointer to a pointer of car that is used to create the dynamic
	// array of the hashtable(a array of pointers to car)
	Car* car_history; //Pointer for the head of the linkedlist 
	int hashtable_size; //Size of the cars hashtable
	char *name; 
} Park;

/**
 * @brief Main structure, it holds the number of parks in the system, a fixed
 * size array for the parks structs and a CurrentTime struture
 * 
 */
typedef struct {
	int num_parks;
	Park parks[MAX_PARK];
	CurrentTime current_time;

} ParksData;

/**
 * @brief Temporary structure used to sort the parks in
 * alphabetic order non destructively.
 * 
 */
typedef struct {
		int parknumber;
		char *parkname;
	} ParkSort;


/**
 * @brief 
 * This function is used to validate a license plate with three pairs of
 * letter and numbers.
 * @param temp The licence plate to validate.
 * @return (int) Returns 0 if it is valid and 1 if not.
 */
int number_plate_check (char temp[9]);

/**
 * @brief 
 * Function used to list the parks in the system in the order they were added.
 * @param parksdata The main data struture
 * (where all data for the program is stored).
 */
void list_parking (ParksData* parksdata);

void list_parking_alfa(ParksData* parksdatas);

/**
 * @brief
 * Function used to validate the data entered to create a park.
 * @param temp A park struture that holds the data for the new park if it does
 * not pass teh checks it is deleted.
 * @param parksdata The main data struture
 * (where all data for the program is stored).
 * @return (int) Returns 0 if the park is valid 1 if not.
 */
int create_parking_check(Park temp, ParksData* parksdata);

/**
 * @brief 
 * Function used to delete a park struture and free its allocated memory.
 * @param temp The park to be deleted.
 * @param hashtable_size The size of the hashtable that belongs to that park.
 */
void destry_parking (Park* temp, int hashtable_size);

/**
 * @brief 
 * Function that creates a park it directly receives the data
 * from the main parser functions.
 * @param parkname The name of the park.
 * @param capacity The capacity of the park.
 * @param quarter_hourly_rate The price that every 15 minutes are
 * billed for the first hour.
 * @param quarter_hourly_rate_after_first_hour The price that every 15 minutes
 * are billed for the rest hours.
 * @param max_daily_cost The maximum value that can be billed in a 24h period.
 * @param parksdata The main data struture
 * (where all data for the program is stored).
 * @return (int) Returns 0 if valid 1 if not.
 */
int create_parking (char parkname[], int capacity, float quarter_hourly_rate,

float quarter_hourly_rate_after_first_hour, float max_daily_cost,
ParksData* parksdata);

/**
 * @brief 
 * Function used to delete a park from the system
 * including all of the car entry and exit registries.
 * @param parkname The name of the park.
 * @param parksdata The main data struture
 * (where all data for the program is stored).
 * @return (int) Returns 0 if successful 1 if not.
 */
int remove_parking (char parkname[], ParksData* parksdata);

/**
 * @brief 
 * Function used to free all the memory allocated
 * (it is called on program exit).
 * @param parksdata The main data struture
 * (where all data for the program is stored).
 */
void exit_program(ParksData* parksdata);

int validate_timedate (Date d, Time t, ParksData* parksdata);

/**
 * @brief 
 * Function used to register a car entry to the system.
 * @param parkname The park name to where the car is going.
 * @param license_plate The licence plate of the car.
 * @param data The date struture to validate.
 * @param time The time struture to validate.
 * @param parksdata The main data struture
 * (where all data for the program is stored).
 * @return (int) Returns 0 if valid 1 if not.
 */
int add_car_to_park(char parkname[], char license_plate[], Date di, Time ti,
ParksData* parksdata);

int add_car_to_park_check(char parkname[], char license_plate[], Date d, Time t,
ParksData* parksdata, int* parknumber);

/**
 * @brief 
 * Function used to register a exit of a car in the system.
 * @param parkname The park name to where the car is going.
 * @param license_plate The licence plate of the car.
 * @param data The date struture to validate.
 * @param time The time struture to validate.
 * @param parksdata The main data struture
 * (where all data for the program is stored).
 * @return (int) Return 0 if the data is valid 1 if not.
 */
int car_exit_park(char parkname[], char license_plate[9], Date df, Time tf,
ParksData* parksdata);

float parking_cost(long contatempo, float quarter_hourly_rate,
float quarter_hourly_rate_after_first_hour, float max_daily_cost);

/**
 * @brief
 * Function used to list all of the entries and
 * exits of a car in all of the parks in the system.
 * @param license_plate The licence plate of the car.
 * @param parksdata The main data struture
 * (where all data for the program is stored).
 * @return int
 */
int list_cars_entries_exits (char license_plate[], ParksData* parksdata);

/**
 * @brief
 * Function used to print revenue per day of one park.
 * @param parkname The name of the park.
 * @param parksdata The main data struture.
 * (where all data for the program is stored).
 * @return (int) Returns 0 if successful 1 if not.
 */
int park_revenue_data(char parkname[], ParksData* parksdata);

/**
 * @brief 
 * Function used to check the validity of the parkname
 * for the function park_revenue.
 * @param parkname The name of the park
 * @param parksdata The main data struture
 * (where all data for the program is stored).
 * @param parknumber The index of the park it is -1 if not found.
 * @return (int) Returns 0 if valid 1 if not.
 */
int check_park_revenue_parkname(char parkname[], ParksData* parksdata,
int* parknumber);

/**
 * @brief 
 * Function used to print all the records of one park for one day.
 * @param parkname The name of the park.
 * @param parksdata The main data struture
 * (where all data for the program is stored).
 * @param data The date to which to print the revenue.
 * @return (int) Returns 0 if successful 1 if not.
 */
int park_revenue_car(char parkname[], ParksData* parksdata, Date d);

/**
 * @brief 
 * Function used to validate a date test if it in the past in
 * accordance to the current time of the program.
 * @param data The date to validate.
 * @param parksdata The main data struture
 * (where all data for the program is stored).
 * @return (int) Returns 0 if valid 1 if not.
 */
int check_park_revenue_date(Date d, ParksData* parksdata);

/**
 * @brief 
 * Function used to sort the parks in alphabetic order in a non destructive way.
 * @param parksdata The main data struture
 * (where all data for the program is stored).
 * @return (ParkSort*) Returns a pointer to a struture of ordered parks.
 */
ParkSort* alfabetic_sort_parks (ParksData* parksdata);

#endif
