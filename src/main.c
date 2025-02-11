/**
 * @file main.c
 * @author ist1109686
 * @brief In the main file are all functions responsible for reading the
 * commands from the command line (stdin) and parsing them, calling
 * the respective functions with the correct variables and values.
 */

#include "car_parks.h"
#include <stdlib.h>

/**
 * @brief 
 * Function responsible for parsing the command p and calling the respective
 * function, listing the parks no arguments or creating a park with arguments.
 * @param parksdata The main data struture
 * (where all data for the program is stored).
 */
void read_command_p(ParksData* parksdata) {
	int capacity;
	float quarter_hourly_rate;
    float quarter_hourly_rate_after_first_hour;
    float max_daily_cost;
    char buffer[65535];
    char *parkname;

    fgets(buffer, sizeof(buffer), stdin);

    parkname = malloc(strlen(buffer)+1);

    if (buffer[1] == '"') { //The buffer start with a space
			//Parses the command if the parkname has (" "),
			//it uses a regex expression in the sscanf function.
    	sscanf(buffer, " \"%65534[^\"]\" %d %f %f %f",
			parkname, &capacity, &quarter_hourly_rate,
			&quarter_hourly_rate_after_first_hour, &max_daily_cost);
    	//If the buffer only has a new line the command p as 0
			//arguments do it calls the function to list parks.
    } else if (buffer[0] == '\n'){ 
    	list_parking(parksdata);
		free(parkname);
    	parkname = NULL;
    	return;

    } else {
		//Parses the command when the parkname is not enclosed im (" ").
    	sscanf(buffer, "%65534s %d %f %f %f",
			parkname, &capacity, &quarter_hourly_rate,
			&quarter_hourly_rate_after_first_hour, &max_daily_cost);
    }
   

	parkname = realloc(parkname, strlen(parkname) + 1);
	//Calls the function to create a car parking.
	create_parking(parkname, capacity, quarter_hourly_rate,
	quarter_hourly_rate_after_first_hour, max_daily_cost, parksdata);
	
	free(parkname);
	parkname = NULL;

}

/**
 * @brief 
 * Function responsible for parsing the command e or s and calling the
 * respective function with their arguments.
 * @param command The command that was used either 'e' or 's'.
 * @param parksdata The main data struture
 * (where all data for the program is stored).
 */
void read_command_e_s(char command,ParksData* parksdata) {
	char *parkname;
	char buffer[65535];
	Date d;
	Time t;
	char license_plate[9];

	fgets(buffer, sizeof(buffer), stdin);
	parkname = malloc(strlen(buffer)+1);

	if (buffer[1] == '"') {
		//Parses the command if the parkname has (" "),
		//it uses a regex expression in the sscanf function.
		sscanf(buffer, " \"%65534[^\"]\" %8s %d-%d-%d %d:%d",
		parkname, license_plate, &d.day, &d.month, &d.year, &t.hours, &t.minutes);
	} else {
		//Parses the command when the parkname is not enclosed im (" ")
		sscanf(buffer, "%65534s %8s %d-%d-%d %d:%d",
		parkname, license_plate, &d.day, &d.month, &d.year, &t.hours, &t.minutes);
	}

	parkname = realloc(parkname, strlen(parkname) + 1);

	//If the command called was 'e' it calls the function to add a car.
	if (command == 'e') {
	 	add_car_to_park(parkname, license_plate, d, t, parksdata);
	} else { //If the command called was 's' it calls the function to exit the car.
		car_exit_park(parkname, license_plate, d, t, parksdata);
	}

	free(parkname);
	parkname = NULL;
}

/**
 * @brief 
 * Function responsible for parsing the command v and calling the function
 * to list a specific cars entries and exits, with the correct arguments.
 * @param parksdata The main data struture
 * (where all data for the program is stored).
 */
void read_comand_v(ParksData* parksdata) {
	char license_plate[9];

	//Parses the licence plate from the stdin buffer
	//it is protected by only reading 8 char.
	scanf(" %8s", license_plate);
	list_cars_entries_exits(license_plate, parksdata); 
}

/**
 * @brief 
 * Function responsible for parsing the command f and calling the revenue
 * listing functions, with the correct arguments.
 * @param parksdata The main data struture
 * (where all data for the program is stored).
 */
void read_comand_f(ParksData* parksdata) {
	Date d;
	char buffer[65535]; //Buffer for reading the command from stdin.
	char *parkname;
	//Stores the number of arguments that sscanf parses from the buffer.
	int tipo = 0; 

	fgets(buffer, sizeof(buffer), stdin);
	parkname = malloc(strlen(buffer)+1);

	if (buffer[1] == '"') {
		//Parses the command if the parkname has (" "), it uses a
		//regex expression in the sscanf function.
		tipo = sscanf(buffer, " \"%65534[^\"]\" %d-%d-%d",
		parkname, &d.day, &d.month, &d.year); 
	} else {
		//Parses the command when the parkname is not enclosed im (" ").
		tipo = sscanf(buffer, "%65534s %d-%d-%d",
		parkname, &d.day, &d.month, &d.year);
	}

	parkname = realloc(parkname, strlen(parkname) + 1);

	//If it reed one argument from the buffer it calls the
	//function to show the revenue of a park by date.
	if (tipo == 1)
		park_revenue_data(parkname, parksdata);
	else if (tipo == 4) 
		park_revenue_car(parkname, parksdata, d);
	//If it read 4 arguments, it call the function
	//to show the cars exits from a park in a date.
	//Frees the memory used by the parkname.
	free(parkname);
	parkname = NULL;
}

/**
 * @brief 
 * Function responsible for parsing the command r and calling the
 * remove park function, with the correct arguments.
 * @param parksdata The main data struture.
 * (where all data for the program is stored).
 */
void read_comand_r(ParksData* parksdata) {
	char buffer[65535]; //Buffer for reading the command from stdin.
	char *parkname;

	fgets(buffer, sizeof(buffer), stdin);
	parkname = malloc(strlen(buffer)+1);

	if (buffer[1] == '"') {
		//Parses the command if the parkname has (" "),
		//it uses a regex expression in the sscanf function.
		sscanf(buffer, " \"%65534[^\"]\"", parkname); 

	} else {
		//Parses the command when the parkname is not enclosed im (" ").
		sscanf(buffer, "%65534s", parkname);
	}

	parkname = realloc(parkname, strlen(parkname) + 1);
	
	remove_parking(parkname, parksdata); //Call the remove park function.
	free(parkname); //Frees the memory used by parkname.
	parkname = NULL;
}

/**
 * @brief 
 * Main function declares the Parksdata structure where all the data for the
 * program is stored. It read the stdin for the commands the
 * first letter (char) whichindicates the command and call the pretended
 * function that parses the command.
 * @return (int) Always returns 0. (exit code 0)
 */
int main() {
	//Stores the command used for functions that have the same inputs.
	char command; 
	ParksData parksdata;

	//Initialize some variables two zero that are stored in the parksdata struture.
	parksdata.num_parks = 0;
	parksdata.current_time.date.day = 0;
	parksdata.current_time.date.month = 0;
	parksdata.current_time.date.year = 0;
	parksdata.current_time.time.hours = 0;
	parksdata.current_time.time.minutes = 0;

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
	exit_program(&parksdata); //Exit program call.
	exit(0); //Terminates the program with code 0.
}
