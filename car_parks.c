#include "car_parks.h"
#include <stdio.h>
#include <string.h>
#include "car_hashtable.h"
#include <ctype.h>


int validate_timedate (date d, time t, ParksData* parksdata) {
	//printf("%d-%d-%d %d:%d\n", parksdata->ctime.d.day, parksdata->ctime.d.month, parksdata->ctime.d.year, parksdata->ctime.t.hours, parksdata->ctime.t.minutes);
	
	if (check_date(d) || check_time(t)) {
		//printf("1\n");
		return 1;
	}

	if (d.year < parksdata->ctime.d.year) {
		//printf("2\n");
		return 1;
	}

	if (d.year == parksdata->ctime.d.year && d.month < parksdata->ctime.d.month) {
		//printf("3\n");
		return 1;
	}

	if (d.year == parksdata->ctime.d.year && d.month == parksdata->ctime.d.month && d.day < parksdata->ctime.d.day) {
		//printf("4\n");
		return 1;
	}


	if (parksdata->ctime.d.year == d.year && parksdata->ctime.d.month == d.month && parksdata->ctime.d.day == d.day) {
		if (t.hours < parksdata->ctime.t.hours || (t.hours == parksdata->ctime.t.hours && t.minutes < parksdata->ctime.t.minutes)) {
			//printf("5\n");
			return 1;
		}
	}

	parksdata->ctime.d = d;  // Update the current date
	parksdata->ctime.t = t;  // Update the current time
	return 0;
}

int number_plate_check (char temp[9]) {
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
//mt AA-00-AA\0

//Não parece estar a calcular o preco de forma correta
float parking_cost(long contatempo, float price_15, float price_15_1hour, float price_dailymax) {
	int days = 0;
	int hours = 0;
	int minutes = 0;
	int hour_15 = 0;
	int min_15 = 0;
	float parking_cost_no_days;

	days = contatempo / 1440;
	contatempo = contatempo % 1440;
	hours = contatempo / 60;
	minutes = contatempo - (60 * hours);
	
	if (hours > 0) {
		min_15 = 4;
		hours--;
		hour_15 = 4* hours;
		hour_15 += (minutes +14) / 15;
	} else {
		min_15 += (minutes +14) / 15;
	}

	parking_cost_no_days = (hour_15 * price_15_1hour) + (min_15 * price_15);

	if (parking_cost_no_days > price_dailymax) {
		parking_cost_no_days = price_dailymax;
	}
	return (days*price_dailymax) + parking_cost_no_days;
}


void list_parking (ParksData* parksdata) {
	for (int i = 0; i < parksdata->nparks; i++)
		printf("%s %d %d\n", parksdata->parks[i].name, parksdata->parks[i].maxcapacity, (parksdata->parks[i].maxcapacity-parksdata->parks[i].ncars));
}


int create_parking_check(Park temp, ParksData* parksdata) {

	for (int j = 0; j < parksdata->nparks; j++) {
		if (strcmp(temp.name, parksdata->parks[j].name) == 0) {
		printf("%s: parking already exists.\n",temp.name);
		return 1;
		}
	}

	if (temp.maxcapacity <= 0) {
		printf("%d: invalid capacity.\n",temp.maxcapacity);
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


void destry_parking (Park* temp, int s_cars) {
	destroy_car_hashtable(temp->cars, s_cars);
	clear_logcars_list(&temp->logcars);
	free(temp->cars);
	free(temp->name);
	temp->name = NULL;
}


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
	temp.logcars = NULL; //Iniciar a linkedlist logcars em NULL

	for (int i = 0; i < (HASHTABLE_RATIO * capacity); i++) {
		temp.cars[i] = NULL; //Inicia o array de ponteiros para carros em NULL
	}	

	strcpy(temp.name, parkname);	

	if (create_parking_check(temp, parksdata)) {
		destry_parking(&temp, (HASHTABLE_RATIO * capacity));
		return 1;
	} 

	//Copia o conteudo de temp para parks, se calhar usar o memcopy
	parksdata->parks[parksdata->nparks] = temp;

	parksdata->nparks++;
	
	return 0;
}

int add_car_to_park_check(char parkname[], char mt[], date d, time t, ParksData* parksdata, int* parknumber) {


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

int add_car_to_park(char parkname[], char mt[], date di, time ti, ParksData* parksdata) {
	unsigned long i;
	int parknumber = -1;

	if (add_car_to_park_check(parkname, mt, di, ti, parksdata, &parknumber))
		return 1;

	if (parknumber == -1)
		return 1;

	Car* ncar = malloc(sizeof(Car));

	strcpy(ncar->mt, mt);
	ncar->entrydate = di;
	ncar->entrytime = ti;
	ncar->cost = 0;

	i = hash(mt, parksdata->parks[parknumber].s_cars);

	ncar->next = parksdata->parks[parknumber].cars[i];
	parksdata->parks[parknumber].cars[i] = ncar;

	parksdata->parks[parknumber].ncars++;

	printf("%s %d\n", parksdata->parks[parknumber].name, (parksdata->parks[parknumber].maxcapacity - parksdata->parks[parknumber].ncars));

	//printf("%s\n", parksdata->parks[parknumber].cars[i]->mt); //Testes
	return 0;
}


int remove_parking (char parkname[], ParksData* parksdata) {

	int parknumber = -1;

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

	//Caso o parque a apagar seja o ultimo da lista
	if (parknumber == parksdata->nparks) {

		parksdata->nparks--;
		list_parking_alfa(parksdata);
	//Caso o parque a apagar não esteja no final da lista
	} else {
		for (int i = parknumber; i < parksdata->nparks - 1; i ++)
			parksdata->parks[i] = parksdata->parks[i+1];
		parksdata->nparks--;
		list_parking_alfa(parksdata);
	}
	return 0;
}


void list_parking_alfa(ParksData* parksdata) {
	typedef struct {
		int parknumber;
		char *parkname;
	} ParkSort;

	ParkSort parksort[MAX_PARK];

	for (int i = 0; i < parksdata->nparks; i++) {
		parksort[i].parknumber = i;
		parksort[i].parkname = parksdata->parks[i].name;
	}
  
  for (int i = 0; i < parksdata->nparks; i++) {
    for (int j = i+1; j < parksdata->nparks; j++) {
       if (strcmp(parksort[i].parkname, parksort[j].parkname) > 0) {
        ParkSort temp = parksort[i];
        parksort[i] = parksort[j];
        parksort[j] = temp;
      }
    }
  }
  
  for (int i = 0; i < parksdata->nparks; i++)
	  printf("%s\n", parksdata->parks[parksort[i].parknumber].name);
	

}

void exit_program(ParksData* parksdata) {

	for (int i = 0; i < parksdata->nparks; i++)
		destry_parking(&parksdata->parks[i], parksdata->parks[i].s_cars);
}

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
		printf("%s: invalid license plate.\n", mt);
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

int car_exit_park(char parkname[], char mt[9], date df, time tf, ParksData* parksdata) {

	int parknumber = -1;
	unsigned long i;
	float paidvalue;	//Se calhar guardar na estrutura carro??
	if (check_car_exit_park(parkname, mt, df, tf, parksdata, &parknumber))
		return 1;

	if (parknumber == -1)
		return 1;

	Car* exit_car = search_car_hashtable(parksdata->parks[parknumber].cars, mt, parksdata->parks[parknumber].s_cars); //Guarda o ponteiro do carro que queremos mover
	//Remover os ponteiros da hash table para o carro pertendido e passa-lo para a linked list
	i = hash(mt, parksdata->parks[parknumber].s_cars);

	if (parksdata->parks[parknumber].cars[i] == exit_car)
		parksdata->parks[parknumber].cars[i] = exit_car->next; //Se o carro que queremos é o primeiro da lista, pomos o ponteiro da hashtable para o proximo carro (ou seja o ponteiro da car->next)
	else {
		Car* prev_car = parksdata->parks[parknumber].cars[i];
		while (prev_car->next != exit_car) {
			prev_car = prev_car->next;
		}
		prev_car->next = exit_car->next;
	}
  
	add_car_to_end_list (&parksdata->parks[parknumber].logcars, exit_car);
	
	exit_car->exitdate = df;
	exit_car->exittime = tf;

	paidvalue = parking_cost(contatempo(exit_car->entrydate, exit_car->entrytime, df,tf),
	parksdata->parks[parknumber].price_15, parksdata->parks[parknumber].price_15_1hour, parksdata->parks[parknumber].price_dailymax);

	exit_car->cost = paidvalue;

	parksdata->parks[parknumber].ncars--;
 
	printf("%s %02d-%02d-%04d %02d:%02d %02d-%02d-%04d %02d:%02d %.2f\n", mt, exit_car->entrydate.day, exit_car->entrydate.month, exit_car->entrydate.year,
	exit_car->entrytime.hours,exit_car->entrytime.minutes, exit_car->exitdate.day, exit_car->exitdate.month, exit_car->exitdate.year,
	exit_car->exittime.hours, exit_car->exittime.minutes, exit_car->cost);

	return 0;

}

int check_list_cars_entries_exits (char mt[]) {
	if (number_plate_check(mt))
		return 1;
	
	return 0;
}

int list_cars_entries_exits (char mt[], ParksData* parksdata) {

	int state = -1;

	if (check_list_cars_entries_exits(mt))
		return 1;
	
	typedef struct {
		int parknumber;
		char *parkname;
	} ParkSort;

	ParkSort parksort[MAX_PARK];

	for (int i = 0; i < parksdata->nparks; i++) {
		parksort[i].parknumber = i;
		parksort[i].parkname = parksdata->parks[i].name;
	}
  
	for (int i = 0; i < parksdata->nparks; i++) {
		for (int j = i+1; j < parksdata->nparks; j++) {
			if (strcmp(parksort[i].parkname, parksort[j].parkname) > 0) {
				ParkSort temp = parksort[i];
				parksort[i] = parksort[j];
				parksort[j] = temp;
			}
		}
	}
	
	for (int i = 0; i < parksdata->nparks; i++) {

		Car* car_hist = search_logcars_list(mt, &parksdata->parks[parksort[i].parknumber].logcars);
		while (car_hist != NULL) {
			printf("%s %02d-%02d-%04d %02d:%02d %02d-%02d-%04d %02d:%02d\n",parksdata->parks[parksort[i].parknumber].name, car_hist->entrydate.day, car_hist->entrydate.month, car_hist->entrydate.year, car_hist->entrytime.hours, car_hist->entrytime.minutes,
			car_hist->exitdate.day, car_hist->exitdate.month, car_hist->exitdate.year, car_hist->exittime.hours, car_hist->exittime.minutes);
			car_hist = search_logcars_list(mt, &car_hist->next);
			state = 0;
		}

		Car* car_on_park = search_car_hashtable(parksdata->parks[parksort[i].parknumber].cars, mt, parksdata->parks[parksort[i].parknumber].s_cars);
		if (car_on_park != NULL) {
			printf("%s %02d-%02d-%04d %02d:%02d\n", parksdata->parks[parksort[i].parknumber].name, car_on_park->entrydate.day,  car_on_park->entrydate.month,  car_on_park->entrydate.year, car_on_park->entrytime.hours, car_on_park->entrytime.minutes);
			state = 0;
		}
	}

	if ( state == -1) {
		printf("%s: no entries found in any parking.\n", mt);
		return 1;
	}
	return 0;
}

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

int park_revenue_data(char parkname[], ParksData* parksdata) {
	int parknumber = -1;
	float day_revenue = 0;
	date temp;
	if (check_park_revenue_parkname(parkname, parksdata, &parknumber))
		return 1;

	if (parknumber == -1)
		return 1;

	Car* car = parksdata->parks[parknumber].logcars;
	temp.day = car->exitdate.day;
	temp.month = car->exitdate.month;
	temp.year = car->exitdate.year;

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
	printf("%02d-%02d-%04d %.2f\n", temp.day, temp.month, temp.year, day_revenue); // Da print ao ultimo carro da lista se este tiver uma data diferente
	return 0;
}

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
	
	Car* car = parksdata->parks[parknumber].logcars;
	while (car != NULL) {
		if (car->exitdate.day == d.day && car->exitdate.month == d.month && car->exitdate.year == d.year) {
			printf("%s %02d:%02d %.2f\n", car->mt, car->exittime.hours, car->exittime.minutes, car->cost);
		}
		car = car->next;
	}
	return 0;
}