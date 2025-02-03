/**
 * @file car_hashtable.h
 * @author ist1109686
 * @brief The header files for the c file that contains the functions
 * that operate the hashtable and the linkedlist used in this program.
 */

#ifndef car_hashtable
#define car_hashtable

#include "car_parks.h"

/**
 * @brief 
 * The hash function using the djb2 algorithm to calculate
 * the hash of the license plate a string.
 * @param license_plate The license plate of the vehicle in question.
 * @param size The size of the hashtable (it is calculated based on
 * the max capacity of the park).
 * @return (unsigned long) Returns the index for the hashtable that
 * is correspondent to that licence plate late.
 */
unsigned long hash(char license_plate[], int size);

/**
 * @brief 
 * This function frees the memory used by the hashtable and all of its cars that are stored
 * in separate chaining.
 * @param cars The array of pointers to cars (the cars hashtable).
 * @param size The size of the hashtable (it is calculated based on the max capacity of the park).
 */
void destroy_car_hashtable(Car* cars[], int size);

/**
 * @brief 
 * A function to search the hashtable that takes in account the separate
 * chaining collision resolution technique.
 * @param cars The array of pointers to cars (the cars hashtable).
 * @param license_plate The license plate of the vehicle in question.
 * @param size The size of the hashtable (it is calculated based on
 * the max capacity of the park).
 * @return Car* Returns a pointer to car if it is found or a pointer to
 * null if it is not found.
 */
Car* search_car_hashtable (Car* cars[], char license_plate[], int size);

/**
 * @brief 
 * This function is used to add the car in the end of the linkedlist car_history,
 * if the list is empty the car is put in the first position.
 * if it is not, then it goes thru the list until the end and add the car there.
 * @param list_head The head of the linkedlist car_history that stores the
 * history of cars in each park.
 * @param new_car The pointer for the car that needs to be stored
 * in the linkedlist.
 */
void add_car_to_end_list (Car** list_head, Car* new_car);

/**
 * @brief 
 * This function frees the memory used by the cars in the
 * linkedlist car_history.
 * @param list_head The head of the linkedlist car_history that
 * stores the history of cars in each park.
 */
void clear_logcars_list (Car** list_head);

/**
 * @brief 
 * This function is used to search the linkedlist for a specific car
 * via its licence plate.
 * @param license_plate The license plate of the vehicle in question.
 * @param list_head The head of the linkedlist car_history that stores
 * the history of cars in each park.
 * @return Car* Returns a pointer to car if it is found or a pointer
 * to null if it is not found.
 */
Car* search_logcars_list (char license_plate[], Car** list_head);

/**
 * @brief 
 * This function removes a car from the hashtable, it takes the car struture
 * from the linkedlist that start in each index of the hashtable.
 * @param parknumber The index of the park that contains the hashtable
 * in question.
 * @param parksdata The struture that contains all the data related to the
 * operation of the parking system.
 * @param license_plate The license plate of the vehicle in question.
 * @return Car* Returns the pointer to the car that was removed from the hashtable,
 * this pointer needs to be saved somewhere (the car_history linkedlist) or freed.
 */
Car* remove_car_hashtable(int parknumber, ParksData* parksdata, char license_plate[9]);

#endif