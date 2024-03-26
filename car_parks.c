#include "car_parks.h"
#include <stdio.h>
#include <string.h>
#include "car_hashtable.h"

//Isto não funciona que cena chata
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
	if (temp[2] != '-' || temp[5] != '-') {
		printf("%s: invalid licence plate.\n",temp);
		return 1;
	}
    
	if (temp[0] >= 'A' && temp[1] >= 'A' && temp[0] <= 'Z' && temp[1] <= 'Z')
		if (temp[3] >= '0' && temp[4] >= '0' && temp[3] <=  '9' && temp[4] <= '9') 
			if (temp[6] >= '0' && temp[7] >= '0' && temp[6] <= '9' && temp[7] <= '9')
				return 0;

	if (temp[0] >= '0' && temp[1] >= '0' && temp[0] <= '9' && temp[1] <= '9')
		if (temp[3] >= '0' && temp[4] >= '0' && temp[3] <= '9' && temp[4] <= '9')
			if (temp[6] >= 'A' && temp[7] >= 'A' && temp[6] <= 'Z' && temp[7] <= 'Z')
				return 0;

	if (temp[0] >= '0' && temp[1] >= '0' && temp[0] <= '9' && temp[1] <= '9')
		if (temp[3] >= 'A' && temp[4] >= 'A' && temp[3] <= 'Z' && temp[4] <= 'Z')
			if (temp[6] >= '0' && temp[7] >= '0' && temp[6] <= '9' && temp[7] <= '9')
				return 0;

	if (temp[0] >= 'A' && temp[1] >= 'A' && temp[0] <= 'Z' && temp[1] <= 'Z')
    	if (temp[3] >= '0' && temp[4] >= '0' && temp[3] <= '9' && temp[4] <= '9')
      		if (temp[6] >= 'A' && temp[7] >= 'A' && temp[6] <= 'Z' && temp[7] <= 'Z')
      			return 0;

	printf("%s: invalid licence plate.\n",temp);    
	return 1;
}
//mt AA-00-AA\0

//Não parece estar a calcular o preco de forma correta
float parking_cost(long contatempo, float price_15, float price_15_1hour, float price_dailymax) {
	int days = contatempo/1440;
	int hours = (contatempo - (days*1440)) / 60;
	contatempo = (contatempo - (days*1440));
	int minutes = (contatempo - (hours*60));
	int min_15 = (minutes + 14)/15;

	return (days*price_dailymax) + (hours*price_15_1hour) + (min_15*price_15);
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

	for (int i = 0; i < INCREMENT; i++) {
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
	ncar->entrytime =ti;

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
	  printf("%s %d %d\n", parksdata->parks[parksort[i].parknumber].name, parksdata->parks[parksort[i].parknumber].maxcapacity, (parksdata->parks[parksort[i].parknumber].maxcapacity-parksdata->parks[parksort[i].parknumber].ncars));
	

}

void exit_program(ParksData* parksdata) {

	for (int i = 0; i < parksdata->nparks; i++)
		destry_parking(&parksdata->parks[i], parksdata->parks[i].s_cars);
}

int check_car_exit_park (char parkname[], char mt[], date d, time t, ParksData* parksdata, int* parknumber) {
	for (int i = 0; i < parksdata->nparks; i++)
		if (strcmp(parkname, parksdata->parks[i].name) == 0)
			*parknumber = i;
		else {
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
	//Pode ser necessario alterar a forma como é referenciado o carro, nas proximas linhas
	exit_car->exitdate = df;
	exit_car->exittime = tf;

	paidvalue = parking_cost(contatempo(exit_car->entrydate, exit_car->entrytime, df,tf),
	parksdata->parks[parknumber].price_15, parksdata->parks[parknumber].price_15_1hour, parksdata->parks[parknumber].price_dailymax);

	parksdata->parks[parknumber].ncars--;
 
	printf("%s %02d-%02d-%04d %02d:%02d %02d-%02d-%04d %02d:%02d %.2f\n", mt, exit_car->entrydate.day, exit_car->entrydate.month, exit_car->entrydate.year,
	exit_car->entrytime.hours,exit_car->entrytime.minutes, exit_car->exitdate.day, exit_car->exitdate.month, exit_car->exitdate.year,
	exit_car->exittime.hours, exit_car->exittime.minutes,paidvalue);

	return 0;

}



