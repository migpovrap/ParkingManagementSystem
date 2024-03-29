#include "car_hashtable.h"

unsigned long hash (char mt[], int size) {
    unsigned long hash = 5381;
    int c;

    while ((c = *mt++)) {
        hash = ((hash << 5) + hash) + c; //Algoritmo de hash djb2  
    }

    return hash % size;
}

Car* search_car_hashtable (Car* cars[], char mt[], int size) {
    int i;
    i = hash(mt, size);
    Car* car = cars[i];
    while (car != NULL) {
        if (strcmp(car->mt, mt) == 0) 
            return car;
        car = car->next;
    }
    return NULL; //Se o carro não for enceontrado
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
        Car* next_car = current_car->next; //Guardo o ponteiro para o próximo carro em next_car
        free(current_car); //Limpa a memória do carro atual
        current_car = next_car;
    }
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

Car* remove_car_hashtable(int parknumber, ParksData* parksdata, char mt[9]) {
	unsigned long i;
	Car* exit_car = search_car_hashtable(parksdata->parks[parknumber].cars, mt, parksdata->parks[parknumber].s_cars);
	
	i = hash(mt, parksdata->parks[parknumber].s_cars);
	Car* current = parksdata->parks[parknumber].cars[i];
	Car* prev = NULL;

	while (current != NULL) {
		if (current == exit_car) {
			if (prev == NULL) {
				parksdata->parks[parknumber].cars[i] = current->next;
			} else {
				prev->next = current->next;
			}
			current->next = NULL;
			break;
		}
		prev = current;
		current = current->next;
	}
	return exit_car;
} 