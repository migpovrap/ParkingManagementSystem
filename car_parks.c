/**
 * @file car_parks.c
 * @author ist1109686
 * @brief 
 */

#include "car_parks.h"
#include <stdio.h>
#include <string.h>
#include "car_hashtable.h"
#include <ctype.h>

/**
 * @brief 
 * This function validate a date and time and checks if it is in the past (against the current time data)
 * @param d The date structure to validate
 * @param t The time struture to validate
 * @param parksdata The main data struture (where all data for the program is stored)
 * @return (int) Returns 0 if the input is valid 1 if not
 */
int validate_timedate (date d, time t, ParksData* parksdata) {
		
	if (check_date(d) || check_time(t)) {
		return 1;
	}

	if (d.year < parksdata->ctime.d.year) {
		return 1;
	}

	if (d.year == parksdata->ctime.d.year && d.month < parksdata->ctime.d.month) {
		return 1;
	}

	if (d.year == parksdata->ctime.d.year && d.month == parksdata->ctime.d.month && d.day < parksdata->ctime.d.day) {
		return 1;
	}

	if (parksdata->ctime.d.year == d.year && parksdata->ctime.d.month == d.month && parksdata->ctime.d.day == d.day) {
		if (t.hours < parksdata->ctime.t.hours || (t.hours == parksdata->ctime.t.hours && t.minutes < parksdata->ctime.t.minutes)) {
			return 1;
		}
	}

	parksdata->ctime.d = d;  // Update the current date
	parksdata->ctime.t = t;  // Update the current time
	return 0;
}

/**
 * @brief 
 * This function is used to validate a license plate with three pairs of letter and numbers
 * @param temp The licence plate to validate
 * @return (int) Returns 0 if it is valid and 1 if not 
 */
int number_plate_check (char temp[9]) {
	//Used to track the number of pairs for a licence plate to be valid it can not have more than two pairs of either type (2 pairs of numbers or 2 of letter)
	int number_pair_count = 0; 
	int letter_pair_count = 0;

	if (temp[2] != '-' || temp[5] != '-') {
		printf("%s: invalid licence plate.\n",temp);
		return 1;
	}
    for (int i = 0; i < 9; i+=3) {
		if (isupper(temp[i]) && isupper(temp[i+1])) {
			letter_pair_count++;
		} else if (isdigit(temp[i]) && isdigit(temp[i+1])) {
			number_pair_count++;
		} else {
			printf("%s: invalid licence plate.\n", temp);
			return 1;
		}
	}
	if (number_pair_count > 2 || letter_pair_count > 2){
		printf("%s: invalid licence plate.\n", temp);
		return 1;
	}
	return 0;
}

/**
 * @brief 
 * This function is used to calculate the cost of a car staying in the park for a determined time
 * @param contatempo The number of minutes that the car was in the park
 * @param price_15 The price that every 15 minutes are billed for the first hour
 * @param price_15_1hour The price that every 15 minutes are billed for the rest hours
 * @param price_dailymax The maximum value that can be billed in a 24h period
 * @return (float) Returns the price that the car needs to pay 
 */
float parking_cost(long contatempo, float price_15, float price_15_1hour, float price_dailymax) {
	int days = 0;
	int hours = 0;
	int minutes = 0;
	int hour_15 = 0;
	int min_15 = 0;
	float parking_cost_no_days;

	//Divides the minutes in days hours and minutes
	days = contatempo / 1440;
	contatempo = contatempo % 1440;
	hours = contatempo / 60;
	minutes = contatempo - (60 * hours);
	
	//Calculates the number of billing periods 
	if (hours > 0) { //If it as more than 0 hours then the first hour is billed at the appropriate rate
		min_15 = 4;
		hours--;
		hour_15 = 4 * hours; //The billing periods for the rest of the hours
		hour_15 += (minutes + (BILLING_INTERVAL - 1)) / BILLING_INTERVAL; //The billing periods for the remaining minutes
	} else {
		//If there are no hours it calculates the billing periods of the minutes left
		min_15 += (minutes + (BILLING_INTERVAL - 1)) / BILLING_INTERVAL;
	}

	parking_cost_no_days = (hour_15 * price_15_1hour) + (min_15 * price_15); //Calculates the price without taking into account the number of days

	if (parking_cost_no_days > price_dailymax) { //If the cost of the hours is superior to the max of one day it is replaced with it
		parking_cost_no_days = price_dailymax;
	}

	return (days * price_dailymax) + parking_cost_no_days; //Adds the cost of the 24h periods
}

/**
 * @brief 
 * Function used to list the parks in the system in the order they were added
 * @param parksdata The main data struture (where all data for the program is stored)
 */
void list_parking (ParksData* parksdata) {
	for (int i = 0; i < parksdata->nparks; i++)
		printf("%s %d %d\n", parksdata->parks[i].name, parksdata->parks[i].maxcapacity, (parksdata->parks[i].maxcapacity-parksdata->parks[i].ncars));
}

/**
 * @brief
 * Function used to validate the data entered to create a park
 * @param temp A park struture that holds the data for the new park if it does not pass teh checks it is deleted
 * @param parksdata The main data struture (where all data for the program is stored)
 * @return (int) Returns 0 if the park is valid 1 if not 
 */
int create_parking_check(Park temp, ParksData* parksdata) {

	for (int j = 0; j < parksdata->nparks; j++) {
		if (strcmp(temp.name, parksdata->parks[j].name) == 0) {
		printf("%s: parking already exists.\n", temp.name);
		return 1;
		}
	}

	if (temp.maxcapacity <= 0) {
		printf("%d: invalid capacity.\n", temp.maxcapacity);
		return 1;
	}

	if (temp.price_15 < 0 || temp.price_15_1hour < 0 || temp.price_dailymax < 0) {
		printf("invalid cost.\n");
		return 1;
	}

	if (temp.price_dailymax < temp.price_15_1hour || temp.price_15_1hour < temp.price_15) {
		printf("invalid cost.\n");
		return 1;
	}

	if (parksdata->nparks == MAX_PARK) {
		printf("too many parks.\n");
		return 1;
	}

	return 0;
}

/**
 * @brief 
 * Function used to delete a park struture and free its allocated memory
 * @param temp The park to be deleted 
 * @param s_cars The size of the hashtable that belongs to that park
 */
void destry_parking (Park* temp, int s_cars) {
	destroy_car_hashtable(temp->cars, s_cars);
	clear_logcars_list(&temp->logcars);
	free(temp->cars);
	temp->cars = NULL;
	free(temp->name);
	temp->name = NULL;
}

/**
 * @brief 
 * Function that creates a park it directly receives the data from the main parser functions
 * @param parkname The name of the park
 * @param capacity The capacity of the park
 * @param price_15 The price that every 15 minutes are billed for the first hour
 * @param price_15_1hour The price that every 15 minutes are billed for the rest hours
 * @param price_dailymax The maximum value that can be billed in a 24h period
 * @param parksdata The main data struture (where all data for the program is stored)
 * @return (int) Returns 0 if valid 1 if not 
 */
int create_parking (char parkname[], int capacity, float price_15, float price_15_1hour, float price_dailymax, ParksData* parksdata) {

	Park temp;
	temp.ncars = 0;
	temp.maxcapacity = capacity;
	temp.price_15 = price_15;
	temp.price_15_1hour = price_15_1hour;
	temp.price_dailymax = price_dailymax;
	temp.s_cars = (HASHTABLE_RATIO * capacity);

	temp.name = (char *)malloc((strlen(parkname) + 1) * sizeof(char));

	temp.cars = malloc((HASHTABLE_RATIO * capacity) * sizeof(Car*));
	temp.logcars = NULL; //Initialize the linkedlist logcars in NULL

	for (int i = 0; i < (HASHTABLE_RATIO * capacity); i++) {
		temp.cars[i] = NULL; //Initialize the array of pointers to cars in NULL
	}	

	strcpy(temp.name, parkname);	

	//Checks that the data is valid if not it deletes the park struture
	if (create_parking_check(temp, parksdata)) { 
		destry_parking(&temp, (HASHTABLE_RATIO * capacity));
		return 1;
	} 

	parksdata->parks[parksdata->nparks] = temp; //Puts the new park in the parks array in the next position

	parksdata->nparks++; //Increments the number of parks
	
	return 0;
}

/**
 * @brief 
 * Function that is used to validate the data used to register a car entry to the system
 * @param parkname The park name to where the car is going
 * @param mt The licence plate of the car 
 * @param d The date struture to validate
 * @param t The time struture to validate
 * @param parksdata The main data struture (where all data for the program is stored)
 * @param parknumber The number of the park (it is passed to the main add_car_to_park function)
 * @return (int) Returns 0 if the data is valid 1 if not 
 */
int add_car_to_park_check(char parkname[], char mt[], date d, time t, ParksData* parksdata, int* parknumber) {

	for (int i = 0; i < parksdata->nparks; i++) {
		if (strcmp(parksdata->parks[i].name, parkname) == 0) {
			*parknumber = i; //If the park is found it stores it index in parknumber(it is declared in the main add_car_to_park function)
			break;
		}
	}

	if (*parknumber == -1) {
		printf("%s: no such parking.\n", parkname); 
		return 1;
	}
  
	if (parksdata->parks[*parknumber].ncars == parksdata->parks[*parknumber].maxcapacity) {
    	printf("%s: parking is full.\n", parkname);
    	return 1;
  	}

	if (number_plate_check(mt) == 1)
    	return 1;

	for (int i = 0; i < parksdata->nparks; i++) {
		if ((search_car_hashtable(parksdata->parks[i].cars, mt, parksdata->parks[i].s_cars)) != NULL) {
			printf("%s: invalid vehicle entry.\n", mt);
			return 1;
		}
	}
	
	if (validate_timedate(d, t, parksdata)) {
    	printf("invalid date.\n");
    	return 1;
	}

	return 0;
}

/**
 * @brief 
 * Function used to register a car entry to the system
 * @param parkname The park name to where the car is going
 * @param mt The licence plate of the car 
 * @param d The date struture to validate
 * @param t The time struture to validate
 * @param parksdata The main data struture (where all data for the program is stored)
 * @return int 
 */
int add_car_to_park(char parkname[], char mt[], date di, time ti, ParksData* parksdata) {
	unsigned long i;
	int parknumber = -1;

	if (add_car_to_park_check(parkname, mt, di, ti, parksdata, &parknumber))
		return 1;

	if (parknumber == -1) //If the park is not found it returns
		return 1;

	Car* ncar = malloc(sizeof(Car)); //Allocates memory for the new car

	//Adds the data to the new car
	strcpy(ncar->mt, mt);
	ncar->entrydate = di;
	ncar->entrytime = ti;
	ncar->cost = 0;

	i = hash(mt, parksdata->parks[parknumber].s_cars); //Finds the index to put the car in the hash table

	//Adds the car to the head of the linkedlist in the hashtable (separate chaining)
	ncar->next = parksdata->parks[parknumber].cars[i];
	parksdata->parks[parknumber].cars[i] = ncar;

	parksdata->parks[parknumber].ncars++;

	printf("%s %d\n", parksdata->parks[parknumber].name, (parksdata->parks[parknumber].maxcapacity - parksdata->parks[parknumber].ncars));

	return 0;
}

/**
 * @brief 
 * Function used to delete a park from the system including all of the car entry and exit registries
 * @param parkname The name of the park
 * @param parksdata The main data struture (where all data for the program is stored)
 * @return (int) Returns 0 if successful 1 if not 
 */
int remove_parking (char parkname[], ParksData* parksdata) {

	int parknumber = -1;

	//Checks if the park exist and if so it stores the index
    for (int i = 0; i < parksdata->nparks; i++) {
        if (strcmp(parksdata->parks[i].name, parkname) == 0) {
            parknumber = i;
            break;
        }
    }

    if (parknumber == -1) {
        printf("%s: no such parking.\n", parkname); 
        return 1;
    }

	destry_parking(&parksdata->parks[parknumber], parksdata->parks[parknumber].s_cars);

	//In case the park to delete is the last in array
	if (parknumber == parksdata->nparks) {
		parksdata->nparks--;
		list_parking_alfa(parksdata);
	//In case the park to delete is not the last in array
	} else {
		for (int i = parknumber; i < parksdata->nparks - 1; i ++)
			parksdata->parks[i] = parksdata->parks[i+1];
		parksdata->nparks--;
		list_parking_alfa(parksdata);
	}
	return 0;
}

/**
 * @brief 
 * Function used to sort the parks in alphabetic order in a non destructive way
 * @param parksdata The main data struture (where all data for the program is stored)
 * @return (ParkSort*) Returns a pointer to a struture of ordered parks
 */
ParkSort* alfabetic_sort_parks (ParksData* parksdata) {

	ParkSort* parksort = malloc(parksdata->nparks * sizeof(ParkSort));

	//Copies the relevant data to the new struture from the main parksdata
	for (int i = 0; i < parksdata->nparks; i++) {
		parksort[i].parknumber = i;
		parksort[i].parkname = parksdata->parks[i].name;
	}

	//Uses a bubble sort algorithm
	for (int i = 0; i < parksdata->nparks; i++) {
		for (int j = i+1; j < parksdata->nparks; j++) {
			if (strcmp(parksort[i].parkname, parksort[j].parkname) > 0) {
				ParkSort temp = parksort[i];
				parksort[i] = parksort[j];
				parksort[j] = temp;
			}
		}
	}
	return parksort;
}

/**
 * @brief 
 * Function used to print the parks in alfabetic order
 * @param parksdata The main data struture (where all data for the program is stored)
 */
void list_parking_alfa(ParksData* parksdata) {
	ParkSort* parkssorted = alfabetic_sort_parks(parksdata);

	for (int i = 0; i < parksdata->nparks; i++)
		printf("%s\n", parksdata->parks[parkssorted[i].parknumber].name);
	free(parkssorted);
}

/**
 * @brief 
 * Function used to free all the memory allocated (it is called on program exit)
 * @param parksdata The main data struture (where all data for the program is stored)
 */
void exit_program(ParksData* parksdata) {
	for (int i = 0; i < parksdata->nparks; i++)
		destry_parking(&parksdata->parks[i], parksdata->parks[i].s_cars);
}

/**
 * @brief 
 * Function used to validate the data used to register a exit of a car in the system
 * @param parkname 
 * @param mt 
 * @param d 
 * @param t 
 * @param parksdata The main data struture (where all data for the program is stored)
 * @param parknumber 
 * @return int 
 */
int check_car_exit_park (char parkname[], char mt[], date d, time t, ParksData* parksdata, int* parknumber) {

	for (int i = 0; i < parksdata->nparks; i++) {
		if (strcmp(parksdata->parks[i].name, parkname) == 0) {
			*parknumber = i;
			break;
		}
	}

	if (*parknumber == -1) {
		printf("%s: no such parking.\n", parkname); 
		return 1;
	}

	if (number_plate_check(mt)) {
		//printf("%s: invalid licence plate.\n", mt);
		return 1;
	}

	if (search_car_hashtable(parksdata->parks[*parknumber].cars, mt, parksdata->parks[*parknumber].s_cars) == NULL) {
		printf("%s: invalid vehicle exit.\n", mt);
		return 1;
	}

	if (validate_timedate(d, t, parksdata)) {
		printf("invalid date.\n");
		return 1;
	}

	return 0;
}

/**
 * @brief 
 * Function used to register a exit of a car in the system
 * @param parkname The park name to where the car is going
 * @param mt The licence plate of the car 
 * @param d The date struture to validate
 * @param t The time struture to validate
 * @param parksdata The main data struture (where all data for the program is stored)
 * @return (int) Return 0 if the data is valid 1 if not 
 */
int car_exit_park(char parkname[], char mt[9], date df, time tf, ParksData* parksdata) {

	int parknumber = -1;
	
	float paidvalue;
	if (check_car_exit_park(parkname, mt, df, tf, parksdata, &parknumber))
		return 1;

	if (parknumber == -1)
		return 1;
	
	//Saves a pointer to the car that is removed from the hashtable and adds it to the logcars linkedlist
	Car* exit_car = remove_car_hashtable(parknumber, parksdata, mt);
	add_car_to_end_list(&parksdata->parks[parknumber].logcars, exit_car);

	exit_car->exitdate = df;
	exit_car->exittime = tf;

	//Calls the function to calculate the bill with all the required arguments
	paidvalue = parking_cost(contatempo(exit_car->entrydate, exit_car->entrytime, df,tf),
	parksdata->parks[parknumber].price_15, parksdata->parks[parknumber].price_15_1hour, parksdata->parks[parknumber].price_dailymax);

	exit_car->cost = paidvalue;

	parksdata->parks[parknumber].ncars--;
 
	printf("%s %02d-%02d-%04d %02d:%02d %02d-%02d-%04d %02d:%02d %.2f\n", mt, exit_car->entrydate.day, exit_car->entrydate.month, exit_car->entrydate.year,
	exit_car->entrytime.hours,exit_car->entrytime.minutes, exit_car->exitdate.day, exit_car->exitdate.month, exit_car->exitdate.year,
	exit_car->exittime.hours, exit_car->exittime.minutes, exit_car->cost);

	return 0;
}


/**
 * @brief 
 * Function used to list all of the entries ans exits of a car in all of the parks in the system
 * @param mt The licence plate of the car 
 * @param parksdata The main data struture (where all data for the program is stored)
 * @return int 
 */
int list_cars_entries_exits (char mt[], ParksData* parksdata) {

	int state = -1;

	if (number_plate_check(mt))
		return 1;
	
	ParkSort* parksorted = alfabetic_sort_parks(parksdata);
	
	for (int i = 0; i < parksdata->nparks; i++) {

		//Prints the entries that match the licence plate on the linkedlist logcars
		Car* car_hist = search_logcars_list(mt, &parksdata->parks[parksorted[i].parknumber].logcars);
		while (car_hist != NULL) {
			printf("%s %02d-%02d-%04d %02d:%02d %02d-%02d-%04d %02d:%02d\n",parksdata->parks[parksorted[i].parknumber].name, car_hist->entrydate.day, car_hist->entrydate.month, car_hist->entrydate.year, car_hist->entrytime.hours, car_hist->entrytime.minutes,
			car_hist->exitdate.day, car_hist->exitdate.month, car_hist->exitdate.year, car_hist->exittime.hours, car_hist->exittime.minutes);
			car_hist = search_logcars_list(mt, &car_hist->next);
			state = 0;
		}

		//Prints the entry that match in the hashtable
		Car* car_on_park = search_car_hashtable(parksdata->parks[parksorted[i].parknumber].cars, mt, parksdata->parks[parksorted[i].parknumber].s_cars);
		if (car_on_park != NULL) {
			printf("%s %02d-%02d-%04d %02d:%02d\n", parksdata->parks[parksorted[i].parknumber].name, car_on_park->entrydate.day,  car_on_park->entrydate.month,  car_on_park->entrydate.year, car_on_park->entrytime.hours, car_on_park->entrytime.minutes);
			state = 0;
		}
	}

	//If the state as not changed them the licence plate was not found on the system
	if ( state == -1) {
		printf("%s: no entries found in any parking.\n", mt);
		free(parksorted);
		return 1;
	}
	free(parksorted);
	return 0;
}

/**
 * @brief 
 * Function used to check the validity of the parkname for the function park_revenue
 * @param parkname The name of the park
 * @param parksdata The main data struture (where all data for the program is stored)
 * @param parknumber The index of the park it is -1 if not found
 * @return (int) Returns 0 if valid 1 if not
 */
int check_park_revenue_parkname(char parkname[], ParksData* parksdata, int* parknumber) {
	for (int i = 0; i < parksdata->nparks; i++) {
		if (strcmp(parksdata->parks[i].name, parkname) == 0) {
			*parknumber = i;
			return 0;
		}
	}
	printf("%s: no such parking.\n", parkname);
	return 1;
}

/**
 * @brief 
 * Function used to print revenue per day of one park
 * @param parkname The name of the park
 * @param parksdata The main data struture (where all data for the program is stored)
 * @return (int) Returns 0 if successful 1 if not 
 */
int park_revenue_data(char parkname[], ParksData* parksdata) {
	int parknumber = -1;
	float day_revenue = 0;
	date temp;
	if (check_park_revenue_parkname(parkname, parksdata, &parknumber))
		return 1;

	if (parknumber == -1)
		return 1;

	//Set the car pointer to the head of the linkedlist logcars and sets the date to the one of teh first car (used to some the revenue of each day)
	Car* car = parksdata->parks[parknumber].logcars; 
	temp.day = car->exitdate.day;
	temp.month = car->exitdate.month;
	temp.year = car->exitdate.year;

	//Goes truth the linkedlist and adds the bill of each one that matches the same data
	while (car != NULL) {
		if (car->exitdate.day == temp.day && car->exitdate.month == temp.month && car->exitdate.year == temp.year) {
			day_revenue += car->cost;
		} else {
			printf("%02d-%02d-%04d %.2f\n", temp.day, temp.month, temp.year, day_revenue);
			temp.day = car->exitdate.day;
			temp.month = car->exitdate.month;
			temp.year = car->exitdate.year;
			day_revenue = car->cost;
		}
		car = car->next;
	}
	printf("%02d-%02d-%04d %.2f\n", temp.day, temp.month, temp.year, day_revenue); //Prints the last car in the linkedlist logcars
	return 0;
}

/**
 * @brief 
 * Function used to validate a date test if it in the past in accordance to the current time of the program
 * @param d The date to validate
 * @param parksdata The main data struture (where all data for the program is stored)
 * @return (int) Returns 0 if valid 1 if not 
 */
int check_park_revenue_date(date d, ParksData* parksdata) {
	if (check_date(d))
		return 1;

	if (d.year > parksdata->ctime.d.year)
		return 1;

	if (d.year == parksdata->ctime.d.year && d.month > parksdata->ctime.d.month)
		return 1;

	if (d.year == parksdata->ctime.d.year && d.month == parksdata->ctime.d.month && d.day > parksdata->ctime.d.day)
		return 1;
	
	return 0;
}

/**
 * @brief 
 * Function used to print all the records of one park for one day
 * @param parkname The name of the park
 * @param parksdata The main data struture (where all data for the program is stored)
 * @param d The date to which to print the revenue
 * @return (int) Returns 0 if successful 1 if not 
 */
int park_revenue_car(char parkname[], ParksData* parksdata, date d) {
	int parknumber = -1;
	
	if (check_park_revenue_parkname(parkname, parksdata, &parknumber))
		return 1;

	if (parknumber == -1)
		return 1;
	
	if (check_park_revenue_date(d, parksdata)) {
		printf("invalid date.\n");
		return 1;
	}
	
	//Goes thru the linkedlist and prints every car that matches the same date
	Car* car = parksdata->parks[parknumber].logcars;
	while (car != NULL) {
		if (car->exitdate.day == d.day && car->exitdate.month == d.month && car->exitdate.year == d.year) {
			printf("%s %02d:%02d %.2f\n", car->mt, car->exittime.hours, car->exittime.minutes, car->cost);
		}
		car = car->next;
	}
	return 0;
}