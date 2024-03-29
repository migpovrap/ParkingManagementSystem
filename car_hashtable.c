/**
 * @file car_hashtable.c
 * @author ist1109686
 * @brief The functions that operate the hashtable and the linkedlist, used to clear the memory used
 * by both stutures, the search function for both and some auxialry ones.
 */

#include "car_hashtable.h"

/**
 * @brief 
 * The hash function using the djb2 algorhtum to calculate the hash of the license plate a string.
 * @param mt The license plate of the veichle in question.
 * @param size The size of the hashtable (it is calculated based on the max capacity of the park).
 * @return (unsigned long) Returns the index for the hashtable that is correspodent
 * to that licensep late.
 */
unsigned long hash (char mt[], int size) {
    unsigned long hash = 5381;
    int c;

    while ((c = *mt++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % size;
}

/**
 * @brief 
 * A function to search the hashtable that takes in account the separete channig
 * colision resolution techine.
 * @param cars The array of pointers to cars (the cars hashtable).
 * @param mt The license plate of the veichle in question.
 * @param size The size of the hashtable (it is calculated based on the max capacity of the park).
 * @return Car* Returns a pointer to car if it is found or a pointer to null if it is not found.
 */
Car* search_car_hashtable (Car* cars[], char mt[], int size) {
    unsigned long i;
    i = hash(mt, size);
    Car* car = cars[i];
    while (car != NULL) {
        if (strcmp(car->mt, mt) == 0) 
            return car;
        car = car->next;
    }
    return NULL;
}

/**
 * @brief 
 * This function frees the memory used by the hashtable and all of its cars that are stored
 * in separate chainning.
 * @param cars The array of pointers to cars (the cars hashtable).
 * @param size The size of the hashtable (it is calculated based on the max capacity of the park).
 */
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

/**
 * @brief 
 * This function frees the memory used by the cars in the linkedlist logcars.
 * @param list_head The head of the linkedlist logcars that stores the history of cars in each park.
 */
void clear_logcars_list (Car** list_head) {
    if (*list_head == NULL)
        return;
    Car* current_car = *list_head;
    while (current_car != NULL) {
        Car* next_car = current_car->next;
        free(current_car);
        current_car = next_car;
    }
    *list_head = NULL;//After it clears the linked list the head is set to null
}

/**
 * @brief 
 * This function is used to add the car in the end of the linkedlist logcars, if the list is empty the car is put in the first position
 * if it is not, then it goes thruh the list until the end and add the car there.
 * @param list_head The head of the linkedlist logcars that stores the history of cars in each park.
 * @param new_car The pointer for the car that needs to be stored in the linkedlist.
 */
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

/**
 * @brief 
 * This function is used to search the linkedlist for a specific car via its licence plate.
 * @param mt The license plate of the veichle in question.
 * @param list_head The head of the linkedlist logcars that stores the history of cars in each park.
 * @return Car* Returns a pointer to car if it is found or a pointer to null if it is not found.
 */
Car* search_logcars_list (char mt[], Car** list_head) {
    Car* current_car = *list_head;
    while (current_car != NULL) {
        if (strcmp(mt, current_car->mt) == 0)
            return current_car;
        else
            current_car = current_car->next;
    }
    return NULL;
}

/**
 * @brief 
 * This function removes a car from the hashtable, it takes the car struture from the linkedlist that start in each index of the hashtable.
 * @param parknumber The index of the park that contains the hashtable in question.
 * @param parksdata The struture that contains all the data related to the operation of the parking system.
 * @param mt The license plate of the veichle in question.
 * @return Car* Returns the pointer to the car that was removed from the hashtable, this pointer needs to be saved somewhere (the logcars linkedloist) or freed.
 */
Car* remove_car_hashtable(int parknumber, ParksData* parksdata, char mt[9]) {
	unsigned long i;
	Car* exit_car = search_car_hashtable(parksdata->parks[parknumber].cars, mt, parksdata->parks[parknumber].s_cars); //The car that we want to remove.
	
	i = hash(mt, parksdata->parks[parknumber].s_cars); //The index on the hashtable that contains the head of the linkedlist where the car is located.
	Car* current_car = parksdata->parks[parknumber].cars[i]; //Sets current car to the head of the linked list (separate chainning).
	Car* prev_car = NULL; 

	while (current_car != NULL) {
		if (current_car == exit_car) {
			if (prev_car == NULL) {
				parksdata->parks[parknumber].cars[i] = current_car->next; //If the car removed is the head of the list, moves the head to the next car.
			} else {
				prev_car->next = current_car->next; //If the car removed is not the head of the list, it is byppassed.
			}
			current_car->next = NULL; //The removed car no longer points to the other cars in the hashtable (separate chaining).
			break;
		}
		prev_car = current_car;
		current_car = current_car->next;
	}
	return exit_car;
} 