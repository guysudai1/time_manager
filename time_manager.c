/*
Program uses - PThreads, read, write  
 
This program creates files with a schedule.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EVENTS_PER_DAY 10
#define DAYS_PER_WEEK  7

typedef struct {
	char name[20];
	int day;
	int hour;
	int minute;
} Event;

void initiate_schedule() {
	printf( "=------Schedule------=\n"
		"=                    =\n"
		"=    1 New Event     =\n"
		"=  2 Delete Event    =\n"
		"=  3 Save Schedule   =\n"
		"=  4 Load Schedule   =\n"
		"=      5 Exit        =\n"
		"=--------------------=\n");

}
Event* get_new_event() {
	// this function gets a new event and returns the pointer to it.
	
	// Day variable consists of a number from 0-6(including 6) and represents sunday-saturday
	int day, hour, minute;
	char name[20];
	name[0] = 0;
	printf("Day hour minute of event: ");
	scanf("%1d %2d %2d", &day, &hour, &minute);
	if (day < 0 || hour < 0  || minute < 0 ) return NULL;
	if (day > 6 || hour > 24 || minute > 60) return NULL;
	
	// Name of new event
	printf("Name of the event(max 20): ");
	scanf("%20s", name);
	if (strlen(name) == 0) return NULL;
	
	Event* event = (Event *) malloc(sizeof(*event));	
	strcpy(event->name, name);
	event->day = day;
	event->hour = hour;
	event->minute = minute;
	
	return event;
}	

void print_schedule(Event* schedule[DAYS_PER_WEEK][EVENTS_PER_DAY], int schedule_index[DAYS_PER_WEEK]) {
	// this program prints out the schedule by order of time, from lowest to heighest.
	char* days = {"Sunday", "Monday", "Tuesday", "Wedensday", "Thursday", "Friday", "Saturday"};
	char* day_bar[23*sizeof(days)+ 4] = "DAY |";
	for (int i=0; i<sizeof(days); i++) {
		char* day = days[i];
		int spaces_before_and_after = (strlen(day) % 2 == 0) ? (22 - strlen(day)) / 2: (23 - strlen(day)) / 2;
		// TODO : Add string like "         Sunday          " to days bar.
	}											      
	printf("DAY |                      |"
}	


int main() {
	int running = 1;
	// schedule of sunday-saturday, not ordered.
	Event* schedule[DAYS_PER_WEEK][EVENTS_PER_DAY];
	int schedule_index[DAYS_PER_WEEK] = {0};

	while (running) {
		initiate_schedule();
		int choice;
		Event* event;
		scanf("%1d", &choice);
		switch (choice) {
			case 1:
				// This case creates a new event and appends it to the schedule.
				
				event = get_new_event();
				if (event == NULL) {
					printf("Something went wrong with creating your event.");
					break;
				}

				printf("Event details: \n"
				       "Name: %s\n"
				       "Day/Hour/Minute: %d/%d/%d\n"
				      , event->name, event->day, event->hour, event->minute);
				
				// Index of last event in schedule array
				int event_num = schedule_index[event->day];
				if (event_num == EVENTS_PER_DAY - 1) {
					printf("You have exceeded the amount of events per day");
					free(event);
					break;
				}
				schedule[event->day + DAYS_PER_WEEK * event_num] = *event;
				schedule_index[event->day]++;
				break;
			case 2:
				// TODO : Create option to delete events from schedule.
				break;
			case 3:
				// TODO : Add option to save the schedule in a file.
				break;
			case 4:
				// TODO : Add option to load a schedule from a file.
				break;
			case 5: 
				running = 0;
				break;
			default:
			       break;	
				
		}
	}
	
	return 0;
}
