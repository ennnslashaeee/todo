#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TODO_LENGTH 256
#define FILENAME_LENGTH 50

void get_current_date(char *date_str) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date_str, "%04d-%02d-%02d.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

void add_todo(const char *todo) {
    char filename[FILENAME_LENGTH];
    get_current_date(filename);

    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file for appending");
        return;
    }
    fprintf(file, "%s\n", todo);
    fclose(file);
    printf("Added: %s\n", todo);
}

void get_todos() {
    char filename[FILENAME_LENGTH];
    get_current_date(filename);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No to-dos for today.\n");
        return;
    }

    char todo[MAX_TODO_LENGTH];
    printf("To-dos for today:\n");
    while (fgets(todo, sizeof(todo), file)) {
        printf("- %s", todo);
    }
    fclose(file);
}

void delete_todo(int line_number) {
    char filename[FILENAME_LENGTH];
    get_current_date(filename);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No to-dos for today.\n");
        return;
    }

    FILE *temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        perror("Error opening temporary file");
        fclose(file);
        return;
    }

    char todo[MAX_TODO_LENGTH];
    int current_line = 1;
    while (fgets(todo, sizeof(todo), file)) {
        if (current_line != line_number) {
            fprintf(temp_file, "%s", todo);
        }
        current_line++;
    }

    fclose(file);
    fclose(temp_file);

    remove(filename);
    rename("temp.txt", filename);
    printf("Deleted to-do at line %d.\n", line_number);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command> [args]\n", argv[0]);
        printf("Commands:\n");
        printf("  add-todo <todo>   : Add a new to-do item\n");
        printf("  get-todo          : Get today's to-do items\n");
        printf("  todo-delete <line>: Delete a to-do item by line number\n");
        return 1;
    }

    if (strcmp(argv[1], "add-todo") == 0 && argc == 3) {
        add_todo(argv[2]);
    } else if (strcmp(argv[1], "get-todo") == 0) {
        get_todos();
    } else if (strcmp(argv[1], "todo-delete") == 0 && argc == 3) {
        int line_number = atoi(argv[2]);
        delete_todo(line_number);
    } else {
        printf("Invalid command or arguments.\n");
    }

    return 0;
}
