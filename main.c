#include "car_parks.h"
#include <stdlib.h>


//Usar buffer de leitura com 65535 e para o limite de leitura de scanf para os comandos

void read_command_p(ParksData* parksdata) {
	int capacity;
	float price_15;
    float price_15_1hour;
    float price_dailymax;
    char buffer[65535];
    char *parkname;

    fgets(buffer, sizeof(buffer), stdin);

    parkname = malloc(strlen(buffer)+1);

    if (buffer[1] == '"') { //Fica com um caracter em branco a mais a esquerda 
    	sscanf(buffer, " \"%65534[^\"]\" %d %f %f %f", parkname, &capacity, &price_15, &price_15_1hour, &price_dailymax); //Ler o nome do parque com ""
    	
    } else if (buffer[0] == '\n'){
    	list_parking(parksdata);
		free(parkname);
    	parkname = NULL;
    	return;

    } else {
    	sscanf(buffer, "%65534s %d %f %f %f", parkname, &capacity, &price_15, &price_15_1hour, &price_dailymax); //Ler o nome do parque sem aspas
    }
   

	parkname = realloc(parkname, strlen(parkname) + 1);
    create_parking(parkname, capacity, price_15, price_15_1hour, price_dailymax, parksdata); //Function to create a car parking
    //printf("%s\n%d\n%f\n%f\n%f", parkname, capacity, price_15, price_15_1hour, price_dailymax);
    free(parkname);
    parkname = NULL;

}

void read_command_e_s(char command,ParksData* parksdata) {
	char *parkname;
	char buffer[65535];
	date d;
	time t;
	char mt[9];

	fgets(buffer, sizeof(buffer), stdin);
	parkname = malloc(strlen(buffer)+1);

	if (buffer[1] == '"') {
		sscanf(buffer, " \"%65534[^\"]\" %8s %d-%d-%d %d:%d", parkname, mt, &d.day, &d.month, &d.year, &t.hours, &t.minutes);
	// } else if (buffer[0] == '\n'){ (não tem saida sem argumentos)

	} else {
		sscanf(buffer, "%65534s %8s %d-%d-%d %d:%d", parkname, mt, &d.day, &d.month, &d.year, &t.hours, &t.minutes);
	}

	parkname = realloc(parkname, strlen(parkname) + 1);
	if (command == 'e') {
	 	add_car_to_park(parkname, mt, d, t, parksdata);
	} else {
		// Call function para registar saida de um carro, passar o parksdata
		car_exit_park(parkname, mt, d, t, parksdata);
	}

    free(parkname);
    parkname = NULL;
}


void read_comand_v(ParksData* parksdata) {
	char mt[9];

	scanf(" %8s", mt);
	list_cars_entries_exits(mt, parksdata);
	//Cal function para listar todas as entradas deste veiculo, passar o parksdata
}


void read_comand_f(ParksData* parksdata) {
	date d;
	char buffer[65535];
	char *parkname;
	int tipo = 0;

	fgets(buffer, sizeof(buffer), stdin);
	parkname = malloc(strlen(buffer)+1);

	if (buffer[1] == '"') {
		tipo = sscanf(buffer, " \"%65534[^\"]\" %d-%d-%d", parkname, &d.day, &d.month, &d.year);

	} else {
		tipo = sscanf(buffer, "%65534s %d-%d-%d", parkname, &d.day, &d.month, &d.year);
	}

	parkname = realloc(parkname, strlen(parkname) + 1);

	if (tipo == 1)
		park_revenue_data(parkname, parksdata);
	else if (tipo == 4)
		park_revenue_car(parkname, parksdata, d);

	free(parkname);
	parkname = NULL;
}

void read_comand_r(ParksData* parksdata) {
	char buffer[65535];
	char *parkname;

	fgets(buffer, sizeof(buffer), stdin);
	parkname = malloc(strlen(buffer)+1);

	if (buffer[1] == '"') {
		sscanf(buffer, " \"%65534[^\"]\"", parkname);

	} else {
		sscanf(buffer, "%65534s", parkname);
	}

	parkname = realloc(parkname, strlen(parkname) + 1);
	
	remove_parking(parkname, parksdata); //Call the remove parque function 
	//printf("%s\n", parkname);
	free(parkname);
	parkname = NULL;
}



int main() {
	char command;
	ParksData parksdata;
	parksdata.nparks = 0;
	
	parksdata.ctime.d.day = 0;
	parksdata.ctime.d.month = 0;
	parksdata.ctime.d.year = 0;

	parksdata.ctime.t.hours = 0;
	parksdata.ctime.t.minutes = 0;

	while ((command = getchar())!= 'q') { 

		if (command == 'p') 
			read_command_p(&parksdata);

		if (command == 'e' || command == 's')
			read_command_e_s(command, &parksdata);

		if (command == 'v')
			read_comand_v(&parksdata);

		if (command == 'f')
			read_comand_f(&parksdata);

		if (command == 'r')
			read_comand_r(&parksdata);
	}
	exit_program(&parksdata);
	//printf("Exit program"); // Call exit function
	exit(0);
}
