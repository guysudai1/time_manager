/*
Program uses - PThreads, read, write  
 
This program creates files with a schedule.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EVENTS_PER_DAY 10
#define DAYS_PER_WEEK  7
#define EVENT_NAME_SIZE 20

typedef struct {
	char name[20];
	int day;
	int hour;
	int minute;
} Event;

void initiate_schedule() {
	printf( "=------Schedule------=\n"
		"=    1 New Event     =\n"
		"=  2 Delete Event    =\n"
		"=  3 Save Schedule   =\n"
		"=  4 Load Schedule   =\n"
		"= 5 Print Schedule   =\n"
		"=      6 Exit        =\n"
		"=--------------------=\n");

}
Event* get_new_event() {
	// this function gets a new event and returns the pointer to it.
	
	// Day variable consists of a number from 0-6(including 6) and represents sunday-saturday
	int day, hour, minute;
	char name[EVENT_NAME_SIZE];
	name[0] = 0;
	printf("Day hour minute of event: ");
	scanf("%1d %2d %2d", &day, &hour, &minute);
	if (day < 0 || hour < 0  || minute < 0 ) return NULL;
	if (day > 6 || hour > 24 || minute > 60) return NULL;
	
	// Name of new event
	printf("Name of the event(max %d): ", EVENT_NAME_SIZE);
	scanf("%20s", name);
	if (strlen(name) == 0) return NULL;
	
	Event* event = (Event *) malloc(sizeof(*event));	
	strcpy(event->name, name);
	event->day = day;
	event->hour = hour;
	event->minute = minute;
	
	return event;
}	

int compare_events(Event* event1, Event* event2) {
	// Compares events by their members
	return (strcmp(event1->name, event2->name) && event1->day == event2->day && event1->hour == event2->hour && event1->minute == event2->minute) ? 1 : 0;
}

int binary_search(Event* day[EVENTS_PER_DAY], int low, int high, Event* event) {
	// Finds event in a day and returns the index, if doesn't exist then return -1.

	if (high < low) return -1;
	int middle = (high + low) / 2;
	Event* middle_event = day[middle];
	if (compare_events(middle_event, event)) return middle;
	
	// Check hour
	if (middle_event->hour > event->hour) return binary_search(day, low, middle, event);
	if (middle_event->hour < event->hour) return binary_search(day, middle+1, high, event);

	// Same hour - Check minute
	if (middle_event->minute > event->hour) return binary_search(day, low, middle, event);
	return binary_search(day, middle+1, high, event);
}

int enter_sorted(Event* schedule[DAYS_PER_WEEK][EVENTS_PER_DAY], int schedule_index[DAYS_PER_WEEK], Event* event) {
	int event_day = event->day;
	if (schedule_index[event_day] == 0) {
		schedule[event_day][0] = event;
		return 1;
	}
	// check if array is maxed out
	if (schedule_index[event_day] == EVENTS_PER_DAY) return 0;

	// Find appropriate index to add event in.
	int i;
	for (i=schedule_index[event_day] - 1; i >= 0; i--) {
		Event* event_i = schedule[event_day][i];
		if (event_i->hour <= event->hour && event_i->minute < event->minute) 
			break;
		// shift items to the right
		schedule[event_day][i+1] = schedule[event_day][i];
	}
	schedule[event_day][i+1] = event;
	return 1;
}
int max(int schedule_index[DAYS_PER_WEEK]) {
	int max_day = schedule_index[0];
	for (int i=1; i < DAYS_PER_WEEK; i++) 	{
		int current_value = schedule_index[i];
		if (current_value > max_day) max_day = current_value;
	}
	return max_day;
}

void print_schedule(Event* schedule[DAYS_PER_WEEK][EVENTS_PER_DAY], int schedule_index[DAYS_PER_WEEK]) {
	// Prints out the schedule by order of time, from lowest to heighest. 
	
	char line_break[] = "|====================|====================|====================|====================|====================|====================|====================|";
	printf("\n|       Sunday       |       Monday       |       Teusday      |      Wedensday     |      Thursday      |       Friday       |      Saturday      |\n%s\n",line_break);
	//printf("%s\n", line_break);
	//printf("break before function\n");	
	//printf("Max value: %d\n", max(schedule_index));
	
	for (int i=0; i < max(schedule_index);i++) {
		char events[8+EVENT_NAME_SIZE*7 + 1] = {0};
		char  hours[8+EVENT_NAME_SIZE*7 + 1] = {0};
		strcat(events, "|");
		strcat(hours,  "|");
		for (int day=0; day < DAYS_PER_WEEK; day++) {
			char temp_events[EVENT_NAME_SIZE + 1]= {0}; 
			char temp_hours[EVENT_NAME_SIZE + 1] = {0};
			for (int i=0; i<EVENT_NAME_SIZE;i++) {temp_events[i] = ' '; temp_hours[i] = ' ';}


			//if (schedule_index[day] >= i + 1) printf("\nString: %s, String size: %zu, schedule-index: %d\n",schedule[day][i]->name, strlen(schedule[day][i]->name), schedule_index[day]);
			if (schedule_index[day] >= i + 1) {
				char* event_name = schedule[day][i]->name;
				int event_name_size = strlen(event_name);
				strncpy(&temp_events[(EVENT_NAME_SIZE + 1-event_name_size)/2], event_name, event_name_size);  // names[day]=schedule[day][i];
			
				
				char* temp_str;
				sprintf(temp_str, "%02d:%02d", schedule[day][i]->hour, schedule[day][i]->minute);

				int hour_size = strlen(temp_str);
				strncpy(temp_hours + (EVENT_NAME_SIZE - 4)/2,temp_str, hour_size);
			}
			strcat(events, temp_events);
			strcat(events, "|");

			strcat(hours, temp_hours);
			strcat(hours, "|");
		}

		
		//printf("%d - %s",i,line_break);
		
		printf("%s\n", events);
		printf("%s\n", hours);
		printf("%s\n", line_break);
		
	}
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
				
				if (enter_sorted(schedule, schedule_index, event)) {
					printf("[+] Successfully added new event to schedule!");
					schedule_index[event->day]++;
				} else { printf("Couldn't add new event to schedule!"); }
				
				print_schedule(schedule, schedule_index);
				break;
			case 2:;
				// TODO : Create option to delete events from schedule.
				
				// Get event details

				int event_index, event_day;
				printf("Event Index / Event Day:");
				scanf("%d %d", &event_index, &event_day);				

				// Check if event exists
				int amount_of_events = schedule_index[event_day];
				if (amount_of_events - 1 < event_index) {
					printf("You cannot delete an event which doesn't exist.");
					break;
				}
				
				// Remove event
				int i;
				for (i=event_index; i<amount_of_events-1;i++) {
					schedule[event_day][i] = schedule[event_day][i+1];
				}	
				schedule[event_day][i] = 0;
				schedule_index[event_day]--;
				break;
			case 3:
				// TODO : Add option to save the schedule in a file.
				break;
			case 4:
				// TODO : Add option to load a schedule from a file.
				break;
			case 5:
				// Prints schedule in a pretty table.
				print_schedule(schedule, schedule_index);
				break;
			case 6: 
				running = 0;
				break;
			default:
			       break;	
				
		}
	}
	
	return 0;
}
