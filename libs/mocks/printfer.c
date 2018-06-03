#include "color.h"
#include "printfer.h"

#include <commons/string.h>
#include <commons/process.h>
#include <commons/temporal.h>
#include <stdio.h>
#include <stdlib.h>

void messenger_init(char* logger_route, char* process_name, char* log_level) {
	printf(COLOR_RESET);
}

void messenger_show_method(char* log_level, char* message) {
	char* current_time = temporal_get_string_time();

	printf("PID <%d> %s%-12s - %-7s | %s%s\n", process_getpid(), color_get(), current_time, log_level, message, COLOR_RESET);

	free(current_time);
}

void printfer_next_line() {
	printf("\n");
}

void messenger_destroy() {
	printf(COLOR_RESET);
}
