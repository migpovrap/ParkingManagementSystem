/**
 * @file car_hashtable.c
 * @author ist1109686
 * @brief The functions that operate the hashtable and the linkedlist,
 * used to clear the memory used by both stuctures, the search function
 * for both and some auxilary ones.
 */

#include "car_hashtable.h"

unsigned long hash (char license_plate[], int size) {
    unsigned long hash = 5381;
    int c;

    while ((c = *license_plate++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % size;
}

Car* search_car_hashtable (Car* cars[], char license_plate[], int size) {
    unsigned long i;
    i = hash(license_plate, size);
    Car* car = cars[i];
    while (car != NULL) {
        if (strcmp(car->license_plate, license_plate) == 0) 
            return car;
        car = car->next;
    }
    return NULL;
}

void destroy_car_hashtable (Car* cars[], int size) {
    for (int i = 0; i < size; i++) {
        Car* car = cars[i];
        while (car != NULL) {
            Car* next = car->next;
            free(car);
            car = next;
        }
    }
}

void clear_logcars_list (Car** list_head) {
    if (*list_head == NULL)
        return;
    Car* current_car = *list_head;
    while (current_car != NULL) {
        Car* next_car = current_car->next;
        free(current_car);
        current_car = next_car;
    }
    //After it clears the linked list the head is set to null
    *list_head = NULL;
}

void add_car_to_end_list (Car** list_head, Car* new_car) {
    if (*list_head == NULL) 
        *list_head = new_car;
    else {
        Car* current_car = *list_head;
        while (current_car->next != NULL) {
            current_car = current_car->next;
        }
        current_car->next = new_car;
        new_car->next = NULL;
    }
}

Car* search_logcars_list (char license_plate[], Car** list_head) {
    Car* current_car = *list_head;
    while (current_car != NULL) {
        if (strcmp(license_plate, current_car->license_plate) == 0)
            return current_car;
        else
            current_car = current_car->next;
    }
    return NULL;
}

Car* remove_car_hashtable(int parknumber, ParksData* parksdata,
char license_plate[9]) {
	unsigned long i;
    //The car that we want to remove.
	Car* exit_car = search_car_hashtable(parksdata->parks[parknumber].cars,
    license_plate, parksdata->parks[parknumber].hashtable_size);
	
    //The index on the hashtable that contains the head of the
    //linkedlist where the car is located.
	i = hash(license_plate, parksdata->parks[parknumber].hashtable_size);
    //Sets current car to the head of the linked list (separate chaining).
	Car* current_car = parksdata->parks[parknumber].cars[i];
	Car* prev_car = NULL; 

	while (current_car != NULL) {
		if (current_car == exit_car) {
			if (prev_car == NULL) {
                //If the car removed is the head of the list,
                //moves the head to the next car.
				parksdata->parks[parknumber].cars[i] = current_car->next;
			} else {
                //If the car removed is not the head of the list,
                //it is bypassed.
				prev_car->next = current_car->next;
			}
            //The removed car no longer points to the other cars
            //in the hashtable (separate chaining).
			current_car->next = NULL; 
			break;
		}
		prev_car = current_car;
		current_car = current_car->next;
	}
	return exit_car;
} 