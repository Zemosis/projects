#include "../inc/listnode.h"
#include "../inc/exec.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/*
 * Name: Tur-Erdene Bekhtur
 * GNumber: G01585228
 *
 *
 */
#define MAX_HISTORY 100 
#define MAX_INPUT 10000

// Prototype Functions
void process_command(char *buffer, char *history[], int *history_count, ListNode **head);
void add_to_history(char *history[], int *history_count, const char *cmd);
void clear_history(char *history[], int *history_count);
void display_history(char *history[], int history_count);
ListNode* create_node(char *command, char **arguments, int arguments_length, ListNode *head);
void add_node(ListNode **head, ListNode *node);
int get_next_available_id(ListNode *head);
void free_all_nodes(ListNode **head);

int main() {
    char buffer[MAX_INPUT];
    char *history[MAX_HISTORY] = {NULL};
    int history_count = 0;
    ListNode *head = NULL;

    while (1) {
        printf("262$");
        fflush(stdout);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            // EOF occured
            break;
        }

        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        if (strlen(buffer) == 0) {
            continue;
        }

        process_command(buffer, history, &history_count, &head);
    }

    clear_history(history, &history_count);
    free_all_nodes(&head);
    return 0;
}

void process_command (char *buffer, char *history[], int *history_count, ListNode **head) {
    char buffer_copy[MAX_INPUT];
    strcpy(buffer_copy, buffer);

    char *command = strtok(buffer, " \t\n\r");

    if (command == NULL) {
        return;
    }

    // Check for built-in commands
    if (strcmp(command, "quit") == 0) {
        clear_history(history, history_count);
        exit(0);

    } else if (strcmp(command, "cd") == 0) {
        add_to_history(history, history_count, buffer_copy);

        char *dir = strtok(NULL, " \t\n\r");
        char *extra_arg = strtok(NULL, " \t\n\r");

        if (extra_arg != NULL) {
            fprintf(stderr, "error: too many arguments provided\n");
        } else if (dir != NULL) {
            if (chdir(dir) != 0) {
                fprintf(stderr, "error: %s\n", strerror(errno)); 
            }
        }

    } else if (strcmp(command, "history") == 0) {
        char *arg = strtok(NULL, " \t\n\r");

        if (arg == NULL) { 
            display_history(history, *history_count);
        } else if (strcmp(arg, "-c") == 0) {
            clear_history(history, history_count);
        } else {
            char *endptr;
            long index = strtol(arg, &endptr, 10);

            if (*endptr != '\0' || index < 0 || index >= *history_count) {
                fprintf(stderr, "error: %s\n", "Index in history list does not exist");
            } else {
                char cmd_to_execute[MAX_INPUT];
                strcpy(cmd_to_execute, history[index]);

                process_command(cmd_to_execute, history, history_count, head);
            }
        }
        
    } else if (strcmp(command, "new") == 0) {
        add_to_history(history, history_count, buffer_copy);

        char *cmd = strtok(NULL, " \t\n\r");

        if (cmd == NULL) {
            fprintf(stderr, "error: too few arguments provided\n");
            return;
        } 

        char *args[MAX_INPUT];
        int arg_count = 0;

        args[arg_count++] = strdup(cmd);

        char *arg;
        while ((arg = strtok(NULL, " \t\n\r")) != NULL) {
            args[arg_count++] = strdup(arg);
        }

        char **arguments = malloc((arg_count + 1) * sizeof(char*));
        for (int i = 0; i < arg_count; i++) {
            arguments[i] = args[i];
        }

        arguments[arg_count] = NULL;

        ListNode *node = create_node(strdup(cmd), arguments, arg_count, *head);

        add_node(head, node);

    } else if (strcmp(command, "list") == 0) {
        add_to_history(history, history_count, buffer_copy);

        ListNode *current = *head;
        while (current != NULL) {
            printf("List Node %d\n", current->id);
            printf("\tCommand: %s\n", current->command);
            printf("\tFile Contents:\n");

            if (current->file_contents != NULL) {
                char *contents_copy = strdup(current->file_contents);
                char *line = strtok(contents_copy, "\n");
                while (line != NULL) {
                    printf("\t\t%s\n", line);
                    line = strtok(NULL, "\n");
                }
                free(contents_copy);
            }
            current = current->next;
        }
    } else if (strcmp(command, "open") == 0) {
        add_to_history(history, history_count, buffer_copy);

        char *id_str = strtok(NULL, " \t\n\r");
        char *filename = strtok(NULL, " \t\n\r");
        char *extra_arg = strtok(NULL, " \t\n\r");
        
        if (id_str == NULL || filename == NULL || extra_arg != NULL) {
            fprintf(stderr, "error: incorrect number of arguments\n");
            return;
        }
        
        char *endptr;
        long id = strtol(id_str, &endptr, 10);
        
        ListNode *current = *head;
        ListNode *target = NULL;
        
        while (current != NULL) {
            if (current->id == id) {
                target = current;
                break;
            }
            current = current->next;
        }
        
        if (target == NULL) {
            fprintf(stderr, "error: %s\n", "Id does not exist");
            return;
        }
        
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            fprintf(stderr, "error: file cannot be opened\n");
            return;
        }
        
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        char *contents = malloc(file_size + 1);
        size_t bytes_read = fread(contents, 1, file_size, file);
        contents[bytes_read] = '\0';
        
        fclose(file);
        
        if (target->file_contents != NULL) {
            free(target->file_contents);
        }
        
        target->file_contents = contents;
    } else if (strcmp(command, "execute") == 0) {
        add_to_history(history, history_count, buffer_copy);

        char *id_str = strtok(NULL, "  \t\n\r");
        char *extra_arg = strtok(NULL, " \t\n\r");

        if (id_str == NULL || extra_arg != NULL) {
            fprintf(stderr, "error: incorrect number of arguments\n");
            return;
        }

        char *endptr;
        long id = strtol(id_str, &endptr, 10);

        ListNode *current = *head;
        ListNode *target = NULL;

        while (current != NULL) {
            if (current->id == id) {
                target = current;
                break;
            }

            current = current->next;
        }

        if (target == NULL) {
            fprintf(stderr, "error: %s\n", "Id does not exist");
            return;
        }

        if (target->arguments_length > 128) {
            fprintf(stderr, "error: %s\n", "too many arguments");
            return;
        }

        int status = run_command(target);

        int exit_status = (status >> 8) & 0xFF;

        if (exit_status != 0) {
            fprintf(stderr, "error: %s\n", strerror(exit_status));
        }

    } else if (strcmp(command, "remove") == 0) {
        add_to_history(history, history_count, buffer_copy);

        char *id_str = strtok(NULL, " \t\n\r");
        
        if (id_str == NULL) {
            fprintf(stderr, "error: too few arguments provided\n");
            return;
        }
        
        char *endptr;
        long id = strtol(id_str, &endptr, 10);
        
        ListNode *current = *head;
        ListNode *target = NULL;
        
        while (current != NULL) {
            if (current->id == id) {
                target = current;
                break;
            }
            current = current->next;
        }
        
        if (target == NULL) {
            fprintf(stderr, "error: %s\n", "Id does not exist");
            return;
        }
        
        if (target->prev != NULL) {
            target->prev->next = target->next;
        } else {
            *head = target->next;
        }
        
        if (target->next != NULL) {
            target->next->prev = target->prev;
        }
        
        free(target->command);
        if (target->arguments != NULL) {
            for (int i = 0; i < target->arguments_length; i++) {
                free(target->arguments[i]);
            }
            free(target->arguments);
        }
        if (target->file_contents != NULL) {
            free(target->file_contents);
        }
        free(target);
    } else {
        add_to_history(history, history_count, buffer_copy);
    }
}

// History Functions
void add_to_history(char *history[], int *history_count, const char *cmd) {
    if (*history_count < MAX_HISTORY) {
        history[*history_count] = strdup(cmd);
        (*history_count)++;
    } else {
        free(history[0]);
        for (int i = 0; i < MAX_HISTORY - 1; i++) {
            history[i] = history[i + 1];
        }
        history[MAX_HISTORY - 1] = strdup(cmd);
    }
}

void clear_history(char *history[], int *history_count) {
    for (int i = 0; i < *history_count; i++) {
        free(history[i]);
        history[i] = NULL;
    }

    *history_count = 0;
}

void display_history(char *history[], int history_count) {
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i, history[i]);
    }
}

// ListNode Functions
ListNode *create_node(char *command, char **arguments, int arguments_length, ListNode *head) {
    ListNode *node = malloc(sizeof(ListNode));
    node->command = command;
    node->arguments = arguments;
    node->arguments_length = arguments_length;
    node->next = NULL;
    node->prev = NULL;
    node->id = get_next_available_id(head);
    node->file_contents = NULL;
    return node;
}

void add_node(ListNode **head, ListNode *node) {
    if (*head == NULL) {
        *head = node;
    } else {
        ListNode *current = *head;
        ListNode *prev = NULL;

        while (current != NULL && current->id < node->id) {
            prev = current;
            current = current->next;
        }

        if (prev == NULL) {
            node->next = *head;
            (*head)->prev = node;
            *head = node;
        } else {
            prev->next = node;
            node->prev = prev;
            node->next = current;
            if (current != NULL) {
                current->prev = node;
            }
        }
    }
}

int get_next_available_id(ListNode *head) {
    int used_ids[1000] = {0};

    ListNode *current = head;
    while (current != NULL) {
        if (current->id < 1000) {
            used_ids[current->id] = 1;
        }
        current = current->next;
    }

    for (int i = 0; i < 1000; i++) {
        if (used_ids[i] == 0) {
            return i;
        }
    }

    return 0;
}

void free_all_nodes(ListNode **head) {
    ListNode *current = *head;
    while (current != NULL) {
        ListNode *next = current->next;
        free(current->command);
        if (current->arguments != NULL) {
            for (int i = 0; i < current->arguments_length; i++) {
                free(current->arguments[i]);
            }
            free(current->arguments);
        }
        free(current->file_contents);
        free(current);
        current = next;
    }
    *head = NULL;
}
