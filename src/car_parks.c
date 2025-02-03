/**
 * @file car_parks.c
 * @author ist1109686
 * @brief  This file contains the implementation of the car parks management system.
 * This module provides functions to manage car parks, including adding, removing,
 * and querying car parks. It also handles the allocation and deallocation of parking
 * spaces within the car parks.
 */

#include "car_parks.h"
#include <stdio.h>
#include <string.h>
#include "car_hashtable.h"
#include <ctype.h>

/**
 * @brief 
 * This function validate a date and time and checks if it is in the past
 * (against the current time data).
 * @param date The date structure to validate.
 * @param time The time struture to validate.
 * @param parksdata The main data struture
 * (where all data for the program is stored).
 * @return (int) Returns 0 if the input is valid 1 if not.
 */
int validate_timedate (Date date, Time time, ParksData* parksdata) {
		
	if (check_date(date) || check_time(time))
		return 1;

	if (date.year < parksdata->current_time.date.year)
		return 1;

	if (date.year == parksdata->current_time.date.year &&
	date.month < parksdata->current_time.date.month)
		return 1;

	if (date.year == parksdata->current_time.date.year &&
	date.month == parksdata->current_time.date.month &&
	date.day < parksdata->current_time.date.day)
		return 1;

	if (parksdata->current_time.date.year == date.year &&
	parksdata->current_time.date.month == date.month &&
	parksdata->current_time.date.day == date.day)
		if (time.hours < parksdata->current_time.time.hours ||
		(time.hours == parksdata->current_time.time.hours &&
		time.minutes < parksdata->current_time.time.minutes))
			return 1;

	parksdata->current_time.date = date;  // Update the current date.
	parksdata->current_time.time = time;  // Update the current time.
	return 0;
}

int number_plate_check (char temp[9]) {
	//Used to track the number of pairs for a licence plate to be valid it 
	// can not have more than two pairs of either type
	// (2 pairs of numbers or 2 of letter).
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
 * This function is used to calculate the cost of a car staying in the park for
 * a determined time.
 * @param contatempo The number of minutes that the car was in the park.
 * @param quarter_hourly_rate The price that every 15 minutes are billed for
 * the first hour.
 * @param quarter_hourly_rate_after_first_hour The price that every 15 minutes
 * are billed for the rest hours.
 * @param max_daily_cost The maximum value that can be billed in a 24h period.
 * @return (float) Returns the price that the car needs to pay.
 */
float parking_cost(long contatempo, float quarter_hourly_rate,
float quarter_hourly_rate_after_first_hour, float max_daily_cost) {
	int days = 0;
	int hours = 0;
	int minutes = 0;
	int hour_15 = 0;
	int min_15 = 0;
	float parking_cost_no_days;

	//Divides the minutes in days hours and minutes.
	days = contatempo / 1440;
	contatempo = contatempo % 1440;
	hours = contatempo / 60;
	minutes = contatempo - (60 * hours);
	
	//Calculates the number of billing periods.
	//If it as more than 0 hours then the first hour is billed at the appropriate rate.
	if (hours > 0) {
		min_15 = 4;
		hours--;
		//The billing periods for the rest of the hours.
		hour_15 = 4 * hours;
		//The billing periods for the remaining minutes.
		hour_15 += (minutes + (BILLING_INTERVAL - 1)) / BILLING_INTERVAL; 
	} else {
		//If there are no hours it calculates the billing periods of the minutes left.
		min_15 += (minutes + (BILLING_INTERVAL - 1)) / BILLING_INTERVAL;
	}

	 //Calculates the price without taking into account the number of days.
	parking_cost_no_days =
	(hour_15 * quarter_hourly_rate_after_first_hour) + (min_15 * quarter_hourly_rate);

	//If the cost of the hours is superior to the max of one day it is replaced with it.
	if (parking_cost_no_days > max_daily_cost)
		parking_cost_no_days = max_daily_cost;

	//Adds the cost of the 24h periods
	return (days * max_daily_cost) + parking_cost_no_days;
}

void list_parking (ParksData* parksdata) {
	for (int i = 0; i < parksdata->num_parks; i++)
		printf("%s %d %d\n",
		parksdata->parks[i].name, parksdata->parks[i].max_capacity,
		(parksdata->parks[i].max_capacity - parksdata->parks[i].num_cars));
}

int create_parking_check(Park temp, ParksData* parksdata) {

	for (int j = 0; j < parksdata->num_parks; j++) {
		if (strcmp(temp.name, parksdata->parks[j].name) == 0) {
		printf("%s: parking already exists.\n", temp.name);
		return 1;
		}
	}

	if (temp.max_capacity <= 0) {
		printf("%d: invalid capacity.\n", temp.max_capacity);
		return 1;
	}

	if (temp.quarter_hourly_rate < 0 || temp.quarter_hourly_rate_after_first_hour < 0
	|| temp.max_daily_cost < 0) {
		printf("invalid cost.\n");
		return 1;
	}

	if (temp.max_daily_cost < temp.quarter_hourly_rate_after_first_hour ||
	temp.quarter_hourly_rate_after_first_hour < temp.quarter_hourly_rate) {
		printf("invalid cost.\n");
		return 1;
	}

	if (parksdata->num_parks == MAX_PARK) {
		printf("too many parks.\n");
		return 1;
	}

	return 0;
}

void destry_parking (Park* temp, int hashtable_size) {
	destroy_car_hashtable(temp->cars, hashtable_size);
	clear_logcars_list(&temp->car_history);
	free(temp->cars);
	temp->cars = NULL;
	free(temp->name);
	temp->name = NULL;
}

int create_parking (char parkname[], int capacity, float quarter_hourly_rate,
float quarter_hourly_rate_after_first_hour, float max_daily_cost,
ParksData* parksdata) {

	Park temp;
	temp.num_cars = 0;
	temp.max_capacity = capacity;
	temp.quarter_hourly_rate = quarter_hourly_rate;
	temp.quarter_hourly_rate_after_first_hour =
	quarter_hourly_rate_after_first_hour;
	temp.max_daily_cost = max_daily_cost;
	temp.hashtable_size = (HASHTABLE_RATIO * capacity);

	temp.name = (char *)malloc((strlen(parkname) + 1) * sizeof(char));

	temp.cars = malloc((HASHTABLE_RATIO * capacity) * sizeof(Car*));
	temp.car_history = NULL; //Initialize the linkedlist car_history in NULL

	for (int i = 0; i < (HASHTABLE_RATIO * capacity); i++)
		temp.cars[i] = NULL; //Initialize the array of pointers to cars in NULL

	strcpy(temp.name, parkname);	

	//Checks that the data is valid if not it deletes the park struture
	if (create_parking_check(temp, parksdata)) { 
		destry_parking(&temp, (HASHTABLE_RATIO * capacity));
		return 1;
	} 

	//Puts the new park in the parks array in the next position
	parksdata->parks[parksdata->num_parks] = temp;

	//Increments the number of parks
	parksdata->num_parks++;
	
	return 0;
}

/**
 * @brief 
 * Function that is used to validate the data used to register a car entry
 * to the system.
 * @param parkname The park name to where the car is going.
 * @param license_plate The licence plate of the car.
 * @param data The date struture to validate.
 * @param time The time struture to validate.
 * @param parksdata The main data struture
 * (where all data for the program is stored).
 * @param parknumber The number of the park
 * (it is passed to the main add_car_to_park function).
 * @return (int) Returns 0 if the data is valid 1 if not.
 */
int add_car_to_park_check(char parkname[], char license_plate[], Date data, Time time, ParksData* parksdata, int* parknumber) {

	for (int i = 0; i < parksdata->num_parks; i++) {
		if (strcmp(parksdata->parks[i].name, parkname) == 0) {
			//If the park is found it stores it index in
			//parknumber(it is declared in the main add_car_to_park function)
			*parknumber = i;
			break;
		}
	}

	if (*parknumber == -1) {
		printf("%s: no such parking.\n", parkname); 
		return 1;
	}
  
	if (parksdata->parks[*parknumber].num_cars ==
	parksdata->parks[*parknumber].max_capacity) {
    	printf("%s: parking is full.\n", parkname);
    	return 1;
  	}

	if (number_plate_check(license_plate) == 1)
    	return 1;

	for (int i = 0; i < parksdata->num_parks; i++) {
		if ((search_car_hashtable(parksdata->parks[i].cars,
		license_plate, parksdata->parks[i].hashtable_size)) != NULL) {
			printf("%s: invalid vehicle entry.\n", license_plate);
			return 1;
		}
	}
	
	if (validate_timedate(data, time, parksdata)) {
    	printf("invalid date.\n");
    	return 1;
	}

	return 0;
}

int add_car_to_park(char parkname[], char license_plate[], Date di, Time ti, ParksData* parksdata) {
	unsigned long i;
	int parknumber = -1;

	if (add_car_to_park_check(parkname, license_plate, di, ti, parksdata, &parknumber))
		return 1;

	//If the park is not found it returns
	if (parknumber == -1)
		return 1;

	//Allocates memory for the new car
	Car* ncar = malloc(sizeof(Car));

	//Adds the data to the new car
	strcpy(ncar->license_plate, license_plate);
	ncar->entry_date = di;
	ncar->entry_time = ti;
	ncar->cost = 0;

	//Finds the index to put the car in the hash table
	i = hash(license_plate, parksdata->parks[parknumber].hashtable_size); 

	//Adds the car to the head of the linkedlist in the hashtable (separate chaining)
	ncar->next = parksdata->parks[parknumber].cars[i];
	parksdata->parks[parknumber].cars[i] = ncar;

	parksdata->parks[parknumber].num_cars++;

	printf("%s %d\n", parksdata->parks[parknumber].name,
	(parksdata->parks[parknumber].max_capacity - parksdata->parks[parknumber].num_cars));

	return 0;
}

int remove_parking (char parkname[], ParksData* parksdata) {

	int parknumber = -1;

	//Checks if the park exist and if so it stores the index
    for (int i = 0; i < parksdata->num_parks; i++) {
        if (strcmp(parksdata->parks[i].name, parkname) == 0) {
            parknumber = i;
            break;
        }
    }

    if (parknumber == -1) {
        printf("%s: no such parking.\n", parkname); 
        return 1;
    }

	destry_parking(&parksdata->parks[parknumber],
	parksdata->parks[parknumber].hashtable_size);

	//In case the park to delete is the last in array
	if (parknumber == parksdata->num_parks) {
		parksdata->num_parks--;
		list_parking_alfa(parksdata);
	//In case the park to delete is not the last in array
	} else {
		for (int i = parknumber; i < parksdata->num_parks - 1; i ++)
			parksdata->parks[i] = parksdata->parks[i+1];
		parksdata->num_parks--;
		list_parking_alfa(parksdata);
	}
	return 0;
}

ParkSort* alfabetic_sort_parks (ParksData* parksdata) {

	ParkSort* parksort = malloc(parksdata->num_parks * sizeof(ParkSort));

	//Copies the relevant data to the new struture from the main parksdata
	for (int i = 0; i < parksdata->num_parks; i++) {
		parksort[i].parknumber = i;
		parksort[i].parkname = parksdata->parks[i].name;
	}

	//Uses a bubble sort algorithm
	for (int i = 0; i < parksdata->num_parks; i++) {
		for (int j = i+1; j < parksdata->num_parks; j++) {
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
 * @param parksdata The main data struture
 * (where all data for the program is stored).
 */
void list_parking_alfa(ParksData* parksdata) {
	ParkSort* parkssorted = alfabetic_sort_parks(parksdata);

	for (int i = 0; i < parksdata->num_parks; i++)
		printf("%s\n", parksdata->parks[parkssorted[i].parknumber].name);
	free(parkssorted);
}

void exit_program(ParksData* parksdata) {
	for (int i = 0; i < parksdata->num_parks; i++)
		destry_parking(&parksdata->parks[i],
		parksdata->parks[i].hashtable_size);
}

/**
 * @brief 
 * Function used to validate the data used to register
 * a exit of a car in the system.
 * @param parkname The name of the park
 * @param license_plate The license plate of the car
 * @param data Exit date
 * @param time Exit time
 * @param parksdata The main data struture
 * (where all data for the program is stored).
 * @param parknumber 
 * @return int 
 */
int check_car_exit_park (char parkname[], char license_plate[], Date data,
Time time, ParksData* parksdata, int* parknumber) {

	for (int i = 0; i < parksdata->num_parks; i++) {
		if (strcmp(parksdata->parks[i].name, parkname) == 0) {
			*parknumber = i;
			break;
		}
	}

	if (*parknumber == -1) {
		printf("%s: no such parking.\n", parkname); 
		return 1;
	}

	if (number_plate_check(license_plate)) {
		//printf("%s: invalid licence plate.\n", license_plate);
		return 1;
	}

	if (search_car_hashtable(parksdata->parks[*parknumber].cars,
	license_plate, parksdata->parks[*parknumber].hashtable_size) == NULL) {
		printf("%s: invalid vehicle exit.\n", license_plate);
		return 1;
	}

	if (validate_timedate(data, time, parksdata)) {
		printf("invalid date.\n");
		return 1;
	}

	return 0;
}

int car_exit_park(char parkname[], char license_plate[9],
Date df, Time tf, ParksData* parksdata) {

	int parknumber = -1;
	
	float paidvalue;
	if (check_car_exit_park(parkname, license_plate, df, tf, parksdata, &parknumber))
		return 1;

	if (parknumber == -1)
		return 1;
	
	//Saves a pointer to the car that is removed from the hashtable and adds
	//it to the car_history linkedlist
	Car* exit_car = remove_car_hashtable(parknumber, parksdata, license_plate);
	add_car_to_end_list(&parksdata->parks[parknumber].car_history, exit_car);

	exit_car->exit_date = df;
	exit_car->exit_time = tf;

	//Calls the function to calculate the bill with all the required arguments
	paidvalue =
	parking_cost(contatempo(exit_car->entry_date, exit_car->entry_time, df,tf),
	parksdata->parks[parknumber].quarter_hourly_rate,
	parksdata->parks[parknumber].quarter_hourly_rate_after_first_hour,
	parksdata->parks[parknumber].max_daily_cost);

	exit_car->cost = paidvalue;

	parksdata->parks[parknumber].num_cars--;
 
	printf("%s %02d-%02d-%04d %02d:%02d %02d-%02d-%04d %02d:%02d %.2f\n",
	license_plate, exit_car->entry_date.day, exit_car->entry_date.month,
	exit_car->entry_date.year, exit_car->entry_time.hours,
	exit_car->entry_time.minutes, exit_car->exit_date.day,
	exit_car->exit_date.month, exit_car->exit_date.year, exit_car->exit_time.hours,
	 exit_car->exit_time.minutes, exit_car->cost);

	return 0;
}

int list_cars_entries_exits (char license_plate[], ParksData* parksdata) {

	int state = -1;

	if (number_plate_check(license_plate))
		return 1;
	
	ParkSort* parksorted = alfabetic_sort_parks(parksdata);
	
	for (int i = 0; i < parksdata->num_parks; i++) {

		//Prints the entries that match the licence plate on
		// the linkedlist car_history
		Car* car_hist = search_logcars_list(license_plate,
		&parksdata->parks[parksorted[i].parknumber].car_history);
		while (car_hist != NULL) {
			printf("%s %02d-%02d-%04d %02d:%02d %02d-%02d-%04d %02d:%02d\n",
			parksdata->parks[parksorted[i].parknumber].name, car_hist->entry_date.day,
			car_hist->entry_date.month, car_hist->entry_date.year,
			car_hist->entry_time.hours, car_hist->entry_time.minutes, car_hist->exit_date.day,
			car_hist->exit_date.month, car_hist->exit_date.year,
			car_hist->exit_time.hours, car_hist->exit_time.minutes);
			car_hist = search_logcars_list(license_plate, &car_hist->next);
			state = 0;
		}

		//Prints the entry that match in the hashtable
		Car* car_on_park = search_car_hashtable(parksdata->parks[parksorted[i].parknumber].cars,
		license_plate, parksdata->parks[parksorted[i].parknumber].hashtable_size);
		if (car_on_park != NULL) {
			printf("%s %02d-%02d-%04d %02d:%02d\n",
			parksdata->parks[parksorted[i].parknumber].name, car_on_park->entry_date.day,
			car_on_park->entry_date.month,  car_on_park->entry_date.year,
			car_on_park->entry_time.hours, car_on_park->entry_time.minutes);
			state = 0;
		}
	}

	//If the state as not changed them the licence plate was not found on the system
	if ( state == -1) {
		printf("%s: no entries found in any parking.\n", license_plate);
		free(parksorted);
		return 1;
	}
	free(parksorted);
	return 0;
}

int check_park_revenue_parkname(char parkname[], ParksData* parksdata,
int* parknumber) {
	for (int i = 0; i < parksdata->num_parks; i++) {
		if (strcmp(parksdata->parks[i].name, parkname) == 0) {
			*parknumber = i;
			return 0;
		}
	}
	printf("%s: no such parking.\n", parkname);
	return 1;
}

int park_revenue_data(char parkname[], ParksData* parksdata) {
	int parknumber = -1;
	float day_revenue = 0;
	Date temp;
	if (check_park_revenue_parkname(parkname, parksdata, &parknumber))
		return 1;

	if (parknumber == -1)
		return 1;

	//Set the car pointer to the head of the linkedlist car_history and
	//sets the date to the one of teh first car
	//(used to some the revenue of each day)
	Car* car = parksdata->parks[parknumber].car_history; 
	temp.day = car->exit_date.day;
	temp.month = car->exit_date.month;
	temp.year = car->exit_date.year;

	//Goes truth the linkedlist and adds the bill of
	//each one that matches the same data
	while (car != NULL) {
		if (car->exit_date.day == temp.day && car->exit_date.month == temp.month
		&& car->exit_date.year == temp.year) {
			day_revenue += car->cost;
		} else {
			printf("%02d-%02d-%04d %.2f\n", temp.day, temp.month, temp.year,
			day_revenue);
			temp.day = car->exit_date.day;
			temp.month = car->exit_date.month;
			temp.year = car->exit_date.year;
			day_revenue = car->cost;
		}
		car = car->next;
	}
	//Prints the last car in the linkedlist car_history
	printf("%02d-%02d-%04d %.2f\n", temp.day, temp.month, temp.year,
	day_revenue);
	return 0;
}

int check_park_revenue_date(Date date, ParksData* parksdata) {
	if (check_date(date))
		return 1;

	if (date.year > parksdata->current_time.date.year)
		return 1;

	if (date.year == parksdata->current_time.date.year &&
	date.month > parksdata->current_time.date.month)
		return 1;

	if (date.year == parksdata->current_time.date.year &&
	date.month == parksdata->current_time.date.month &&
	date.day > parksdata->current_time.date.day)
		return 1;
	
	return 0;
}

int park_revenue_car(char parkname[], ParksData* parksdata, Date data) {
	int parknumber = -1;
	
	if (check_park_revenue_parkname(parkname, parksdata, &parknumber))
		return 1;

	if (parknumber == -1)
		return 1;
	
	if (check_park_revenue_date(data, parksdata)) {
		printf("invalid date.\n");
		return 1;
	}
	
	//Goes truth the linkedlist and prints every car that matches the same date
	Car* car = parksdata->parks[parknumber].car_history;
	while (car != NULL) {
		if (car->exit_date.day == data.day &&
		car->exit_date.month == data.month && car->exit_date.year == data.year) {
			printf("%s %02d:%02d %.2f\n",
			car->license_plate, car->exit_time.hours, car->exit_time.minutes,
			car->cost);
		}
		car = car->next;
	}
	return 0;
}