#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define TABLE_SIZE 4

int patched = 0;
char fmt[] = "%d";
uint8_t values[TABLE_SIZE] = {85, 76, 90, 69};

void setup() {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
}

void print_table() {
    printf("Table:\n");
    printf("-------------------------------\n");
    printf("Address         | Value\n");
    printf("-------------------------------\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("%s | %d\n", "REDACTED", values[i]);
    }
    printf("-------------------------------\n");
}

void print_menu() {
    printf("\nOptions:\n");
    printf("1. Inspect an address\n");
    if (patched == 0)
        printf("2. Patch a value\n");
    printf("0. Exit\n");
    printf("Choose an option: ");
}

void inspect_value() {
    unsigned long addr_input;
    unsigned long value;
    printf("Which address would you like to inspect? ");
    scanf("%lu", &addr_input);
    getc(stdin);

    value = *(unsigned long*)addr_input;
    printf("Address: %p | Value: %p\n", (void *)addr_input, (void*)value);
}

void patch_value() {
    unsigned long addr_input;
    printf("Enter address to patch: ");
    scanf("%lu", &addr_input);
    getc(stdin);
    uint8_t *target = (uint8_t *)addr_input;

    printf("Current value at %p: %d\n", (void *)target, *target);
    printf("Enter new value (0-255): ");
    int new_value;
    scanf("%d", &new_value);
    getc(stdin);

    if (new_value < 0 || new_value > 255) {
        printf("Invalid value. Must be between 0 and 255.\n");
        return;
    }

    if(patched == 0) {
        patched = 1;
        *target = (uint8_t)new_value;
        printf("Value patched at %p. New value: %d\n", (void *)target, *target);
    } else {
        printf("Error detected\n");
        exit(0);
    }
}

void main_loop() {
    uint8_t option;

    while (1) {
        print_menu();

        scanf(fmt, &option);
        getc(stdin);

        switch (option) {
            case 0:
                return;
            case 1:
                inspect_value();
                break;
            case 2:
                if (patched == 0) {
                    patch_value();
                } else {
                    printf("Patch limit reached.\n");
                }
                break;
            default:
                printf("Invalid option.\n");
        }
    }
}

int main() {
    setup();
    print_table();
    main_loop();
    return 0;
}

