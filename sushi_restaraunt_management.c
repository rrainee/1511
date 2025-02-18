// cs_sushi.c cs_sushi
//
// This program was written by Raine Wang (z5418629)
// July 2022
//
// This program is a restaraunt management system for a sushi train restaurant

//  + thanks tutors for marking everyones work :) 
// hope everyone has a good holiday <3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Additional libraries here

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////  CONSTANTS  /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Provided constants
#define TRUE 1
#define FALSE 0

#define MAX_STRING_LENGTH 100
#define EMPTY_CUSTOMER "EMPTY"

// Command constants
#define COMMAND_PRINT_RESTAURANT 'p'
#define COMMAND_ADD_TABLE 'a'
#define COMMAND_ADD_CUSTOMER 'c'
#define COMMAND_ADD_PLATE 'r'
#define COMMAND_PRINT_TRAIN 't'
#define COMMAND_ORDER_FOOD 'o'
#define COMMAND_CALCULATE_BILL 'b'
#define COMMAND_CLOSE_RESTAURANT 'q'
#define COMMAND_REVERSE_TRAIN 's'
#define COMMAND_ROTATE_HEAD 'm'

// Other constants
#define IS_REVERSED 1
#define NOT_REVERSED 0

// Provided Enums
// the numerical value is the cost of that plate color
enum plate_colour { RED = 3, GREEN = 4, BLUE = 5, PURPLE = 6, ORANGE = 7 };

enum sushi_type { VEGETARIAN, SEAFOOD, CHICKEN, TEMPURA };

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////  STRUCTS  //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Provided structs
struct table {
    char customer[MAX_STRING_LENGTH];
    struct plate *orders;
    struct table *next;
};

struct plate {
    char roll_name[MAX_STRING_LENGTH];
    enum plate_colour colour;
    enum sushi_type type;
    struct plate *next;
};

struct restaurant {
    struct table *tables;
    struct plate *plates;
};

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  FUNCTION PROTOTYPES  ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Helper Functions
void print_restaurant(struct restaurant *restaurant);
int scanf_command(char *command);
void interpret_line(
    char buffer[MAX_STRING_LENGTH], char name[MAX_STRING_LENGTH],
    enum plate_colour *colour, enum sushi_type *type);

void interpret_order(
    char buffer[MAX_STRING_LENGTH], char customer[MAX_STRING_LENGTH],
    char roll[MAX_STRING_LENGTH]);

enum plate_colour string_to_colour(char colour[MAX_STRING_LENGTH]);
enum sushi_type string_to_type(char type[MAX_STRING_LENGTH]);
void remove_newline(char input[MAX_STRING_LENGTH]);
char color_to_char(enum plate_colour colour);
void to_type(char types[MAX_STRING_LENGTH], enum sushi_type type);
void to_colour(char cols[MAX_STRING_LENGTH], enum plate_colour colour);

// TODO: Your function prototypes here
void initialise_restaurant (struct restaurant *restaurant);
void add_table (struct restaurant *restaurant);
void add_customer (struct restaurant *restaurant);
struct plate *scan_in_plate();
void add_plate (struct restaurant *restaurant, int reverse_status);
void print_train (struct plate *plate);
void add_order (struct restaurant *restaurant);
void add_to_table (struct table *table, struct plate *plate);
void calculate_bill (struct restaurant *restaurant);
void close_restaurant (struct restaurant *restaurant);
void reverse_train (struct restaurant *restaurant);
void rotate_head (struct restaurant *restaurant);

////////////////////////////////////////////////////////////////////////////////
//////////////////////////  FUNCTION IMPLEMENTATIONS  //////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(void) {
    struct restaurant *sushi_restaurant = malloc(sizeof(struct restaurant));
    sushi_restaurant->tables = NULL; 
    sushi_restaurant->plates = NULL;

    initialise_restaurant(sushi_restaurant);

    int reverse_status = NOT_REVERSED;

    char command;
    while (scanf_command(&command) == 1) {
        if (command == COMMAND_PRINT_RESTAURANT) {
            print_restaurant(sushi_restaurant);
        } else if (command == COMMAND_ADD_TABLE) {
            add_table(sushi_restaurant);
        } else if (command == COMMAND_ADD_CUSTOMER) {
            add_customer(sushi_restaurant);
        } else if (command == COMMAND_ADD_PLATE) {
            add_plate(sushi_restaurant, reverse_status);
        } else if (command == COMMAND_PRINT_TRAIN) {
            print_train(sushi_restaurant->plates);
        } else if (command == COMMAND_ORDER_FOOD) {
            add_order(sushi_restaurant);
        } else if (command == COMMAND_CALCULATE_BILL) {
            calculate_bill(sushi_restaurant);
        } else if (command == COMMAND_CLOSE_RESTAURANT) {
            break;
        } else if (command == COMMAND_REVERSE_TRAIN) {
            reverse_status++;
            if (reverse_status > IS_REVERSED) {
                reverse_status = NOT_REVERSED;
            }
            reverse_train(sushi_restaurant);
        } else if (command == COMMAND_ROTATE_HEAD) {
            rotate_head(sushi_restaurant);
        }
        printf("Enter command: ");
    }
    
    printf("Thank you for dining with CSE Sushi Restaurant!\n");

    close_restaurant(sushi_restaurant);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// ADDITIONAL FUNCTIONS /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Starts up and initialises the restaurant, then prompts for a command.
void initialise_restaurant (struct restaurant *restaurant) {
    printf("Welcome to CSE Sushi Restaurant!\n");
    print_restaurant(restaurant);
    printf("Enter command: ");
}

// Adds a table to the restaurant.
// 'COMMAND_ADD_TABLE'
void add_table (struct restaurant *restaurant) {
    struct table *table = malloc(sizeof(struct table));
    strcpy(table->customer, EMPTY_CUSTOMER);
    table->next = NULL;
    table->orders = NULL;

    if (restaurant->tables == NULL) {
        restaurant->tables = table;
    } else {
        struct table *p = restaurant->tables;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = table;
    }
}
   
// Adds customer and books them into best available table.
// 'COMMAND_ADD_CUSTOMER'
void add_customer (struct restaurant *restaurant) {
    char customer[MAX_STRING_LENGTH];

    printf("Enter customer name: ");
    fgets(customer, MAX_STRING_LENGTH, stdin);
    remove_newline(customer);

    struct table *p = restaurant->tables;
    while (p != NULL) {
        if (strcmp(p->customer, EMPTY_CUSTOMER) == 0) {
            strcpy(p->customer, customer);
            return;
            // if successful, end function here.
        }
        p = p->next;
    }
    printf("No empty tables!\n");
    // unsuccessful
}

// Scans in a new plate with all necessary details.
// add_plate
// add_order
struct plate *scan_in_plate() {
    struct plate *plate = malloc(sizeof(struct plate));
    plate->next = NULL;
    
    char remaining_input[MAX_STRING_LENGTH];
    fgets(remaining_input, MAX_STRING_LENGTH, stdin);

    char roll_name[MAX_STRING_LENGTH];
    enum plate_colour colour;
    enum sushi_type type;

    interpret_line(remaining_input, roll_name, &colour, &type);

    strcpy(plate->roll_name, roll_name);
    plate->colour = colour;
    plate->type = type;

    return plate;
}

// Adds a plate to the train.
// 'COMMAND_ADD_PLATE'
void add_plate (struct restaurant *restaurant, int reverse_status) {
    if (reverse_status == NOT_REVERSED) {
        // normal order
        struct plate *plate = scan_in_plate();

        struct plate *p = restaurant->plates;
        if (p == NULL) {
            restaurant->plates = plate;
            return;
        }

        while (p != NULL && p->type != plate->type) {
            p = p->next;
        }

        if (p != NULL) {
            while (p != NULL && 
                p->next != NULL && p->next->type == plate->type) {
                p = p->next;
            }

            // p is the last of the same type
            struct plate *next = p->next;
            p->next = plate;
            plate->next = next;
        } else {
            p = restaurant->plates;
            while (p->type <= plate->type && p->next != NULL && 
                p->next->type <= plate->type) {
                // keep going through the loop if the types of existing plates 
                // are less or equal to type of added plate 
                p = p->next;
            }
            
            if (p->type > plate->type) {
                plate->next = p;
                restaurant->plates = plate;
            } else {
                // p is the last node of the same type
                struct plate *temp = p->next;
                p->next = plate;
                plate->next = temp;
            }
        }
    } else {
        // reversed order
        struct plate *plate = scan_in_plate();

        struct plate *p = restaurant->plates;
        if (p == NULL) {
            restaurant->plates = plate;
            return;
        }

        while (p != NULL && p->type != plate->type) {
            p = p->next;
        }

        if (p != NULL) {
            while (p != NULL && 
                p->next != NULL && p->next->type == plate->type) {
                p = p->next;
            }

            // p is the last of the same type
            struct plate *next = p->next;
            p->next = plate;
            plate->next = next;
        } else {
            p = restaurant->plates;
            while (p->type >= plate->type && (p->next != NULL && 
                p->next->type >= plate->type)) {
                p = p->next;
            }

            if (p->type < plate->type) {
                plate->next = p;
                restaurant->plates = plate;
            } else {
                struct plate *temp = p->next;
                p->next = plate;
                plate->next = temp;
            }
        }   
    }
}

// Prints out the current sushi train.
// 'COMMAND_PRINT_TRAIN'
void print_train (struct plate *plate) {
    struct plate *p = plate;
    char colour[MAX_STRING_LENGTH];
    char type[MAX_STRING_LENGTH];

    if (p == NULL) {
        printf("    empty :(\n");
        return;
    } 

    while (p != NULL) {
        to_colour(colour, p->colour);
        to_type(type, p->type);
        printf("    %s-%s-%s\n", p->roll_name, type, colour);
        printf("      |\n      v\n"); 
        p = p->next;
    }  
}

// Adds an order to a customers table.
// 'COMMAND_ORDER_FOOD'
void add_order (struct restaurant *restaurant) {
    char remaining_input[MAX_STRING_LENGTH];
    fgets(remaining_input, MAX_STRING_LENGTH, stdin);

    char customer[MAX_STRING_LENGTH];
    char roll_name[MAX_STRING_LENGTH];

    interpret_order(remaining_input, customer, roll_name);

    struct table *table = restaurant->tables;
    struct plate *plate = restaurant->plates;
    struct plate *prev_plate = NULL;

    while (table != NULL) {
        if (strcmp(table->customer, customer) == 0) {
            break;
        } 
        table = table->next; 
    }
    if (table == NULL) {
        printf("There is no customer with that name!\n");
        return;
    } 

    while (plate != NULL) {
        if (strcmp(plate->roll_name, roll_name) == 0) {
            break;
        } 
        prev_plate = plate;
        plate = plate->next;
    } 

    if (plate == NULL) {
        printf("Enter dish details: ");
        plate = scan_in_plate();
    } else {
        if (prev_plate == NULL) {
            // the plate is the first in the train
            restaurant->plates = plate->next;
        } else {
            // the plate has another plate before it
            prev_plate->next = plate->next;
        }
        plate->next = NULL;
    }
    add_to_table(table, plate);
}

// Moves plate from sushi train to customer's table.
// add_order
void add_to_table (struct table *table, struct plate *plate) {
    if (table->orders == NULL) {
        // no existing orders
        table->orders = plate;
    } else {
        struct plate *p = table->orders;
        while (p->next != NULL) {
            // continues looping until last order
            p = p->next;
        }
        p->next = plate;
    }

}

// Calculates bill of the specified customer and frees their table.
// 'COMMAND_CALCULATE_BILL'
void calculate_bill (struct restaurant *restaurant) {
    int bill = 0;
    char customer[MAX_STRING_LENGTH];
    struct table *p = restaurant->tables;

    printf("Enter customer name: ");
    fgets(customer, MAX_STRING_LENGTH, stdin);
    remove_newline(customer);

    while (p != NULL) {
        if (strcmp(p->customer, customer) == 0) {
            break;
        }
        p = p->next;    
    }
    
    if (p == NULL) {
        printf("There is no customer with that name!\n");
    } else {
        struct plate *temp = p->orders;
        struct plate *plate = p->orders;

        while (plate != NULL) {
            bill += plate->colour;
            plate = plate->next;
            free(temp);
            temp = plate;
        }

        p->orders = NULL;
        strcpy(p->customer, EMPTY_CUSTOMER);

        printf("Thank you for dining with CSE Sushi! ");
        printf("Your total is: $%d\n", bill);
    }
}

// Closes restaurant and frees up all malloc'd space created.
// 'COMMAND_CLOSE_RESTAURANT'
// Function established outside of command loop so that mallocs are still freed
// when the restaurant is closed through other means.
void close_restaurant (struct restaurant *restaurant) {
    struct plate *plate = restaurant->plates;
    while (plate != NULL) {
        struct plate *temp_plate = plate;
        plate = plate->next;
        free(temp_plate);
    }

    struct table *table = restaurant->tables;
    while (table != NULL) {
        if (table->orders != NULL) {
            struct plate *order = table->orders;
            while (order != NULL) {
                struct plate *temp_order = order;
                order = order->next;
                free(temp_order);
            }
        }
        struct table *temp_table = table;
        table = table->next;
        free(temp_table);
    }

    free(restaurant);
}

// Reverses the current order of the sushi train.
// 'COMMAND_REVERSE_TRAIN'
void reverse_train (struct restaurant *restaurant) {
    struct plate *p = restaurant->plates;
    struct plate *next = NULL;
    
    struct plate *reversed_list = NULL;

    while (p != NULL) {
        next = p->next;
        p->next = reversed_list;
        reversed_list = p;
        p = next;
    }

    restaurant->plates = reversed_list;
}

// Rotates the head plate of the train by a specified amount.
// 'COMMAND_ROTATE_HEAD'
void rotate_head (struct restaurant *restaurant) {
    int rotations = 0;
    char rotation_string[MAX_STRING_LENGTH];

    fgets(rotation_string, MAX_STRING_LENGTH, stdin);

    struct plate *p = restaurant->plates;
    int plates_on_train = 0;
    while (p != NULL) {
        // counts how many plates are currently on the train.
        plates_on_train++;
        p = p->next;
    }

    p = restaurant->plates;

    if (plates_on_train == 0) {
        // if the train is empty then end function.
        return;
    }

    rotations = atoi(rotation_string) % plates_on_train;

    if (rotations < 0) {
        rotations += plates_on_train;
    }

    if (rotations == 0) {
        return;
    } else {
        struct plate *temp = restaurant->plates;
        //saves temp as head of the list
        int i = 1;
        while (i < rotations) {
            p = p->next;
            i++;
        }
        restaurant->plates = p->next;
        // p->next is the new head    
        p->next = NULL;
        // severs the front plates from rest of list

        p = restaurant->plates;
        while (p->next != NULL) {
            p = p->next;
        }

        p->next = temp;
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// PROVIDED FUNCTIONS //////////////////////////////
/////////////////////////// (DO NOT EDIT BELOW HERE) ///////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Provided Function - DO NOT EDIT
void print_restaurant(struct restaurant *restaurant) {
    int i;
    if (restaurant == NULL) {
        printf("ERROR: No restaurant!\n");
        return;
    };

    printf("CSE Sushi Restaurant\n");
    struct table *table = restaurant->tables;
    int table_num = 0;
    while (table != NULL) {
        int table_length = 5;
        if (strcmp(table->customer, EMPTY_CUSTOMER) != 0) {
            table_length = strlen(table->customer) + 4;
        }
        // calculate number of plates
        int number_of_plates = 0;
        struct plate *curr = table->orders;
        while (curr != NULL) {
            number_of_plates++;
            curr = curr->next;
        }

        printf(" ");
        for (i = 0; i < table_length; i++) {
            printf("_");
        }
        printf(" ");

        // print out the top row
        for (i = 0; i < number_of_plates; i++) {
            printf("    _____ ");
        }

        printf("\n");

        // print out the data
        int occupied = FALSE;
        if (strcmp(table->customer, "EMPTY") != 0) {
            occupied = TRUE;
        }
        if (occupied) {
            printf("|  %s  |-->", table->customer);
        } else {
            printf("|  %d  |-->", table_num);
        }

        // print out the plates
        struct plate *order = table->orders;
        while (order != NULL) {
            printf("|  %c  |-->", color_to_char(order->colour));

            order = order->next;
        }

        printf("\n");
        printf(".");
        for (i = 0; i < table_length; i++) {
            printf("-");
        }
        printf(".");
        // print out the top row
        for (i = 0; i < number_of_plates; i++) {
            printf("   |-----|");
        }

        printf("\n");
        table = table->next;
        table_num++;
    }
}

// Helper Function
// scans a single character from the user and returns the value of scanf;
//
// Parameters:
//  - command: a pointer to a character to store the command
//
// Returns:
//  - the value that scanf returns (this will be 1 if it was successfully read);
//
// Usage:
//  ```
//  char command;
//  while (scanf_command(&command) == 1) {
//  ```
// this function does some wizadry that you **do not need to understand**
// for you to ensure that subsequent
// calls to fgets works correctlly.
int scanf_command(char *command) {
    // read in character + newline + null-terminator
    char buffer[3];
    char *result = fgets(buffer, 3, stdin);
    if (result == NULL) {
        return EOF;
    }

    // a non a-z character probably means not a char
    if (buffer[0] < 'a' || buffer[0] > 'z') {
        return EOF;
    }

    // set the character
    *command = buffer[0];
    return 1;
}

// Helper Function
// You DO NOT NEED TO UNDERSTAND THIS FUNCTION, and will not need to change it.
//
// Given a raw string in the format: [string] [enum1] [enum2]
// This function will extract the relevant values into the given variables.
// The function will also remove any newline characters.
//
// For example, if given: "salmon red seafood"
// The function will copy the string:
//     "salmon" into the 'name' array
// And will copy the enums:
//     red      into the colour pointer
//     seafood  into the type pointer
//
// If you are interested, `strtok` is a function which takes a string,
// and splits it up into before and after a "token" (the second argument)
//
// Parameters:
//     buffer  = A null terminated string in the following format
//               [string] [enum1] [enum2]
//     name    = An array for the [string] to be copied into
//     colour  = A pointer to where [enum1] should be stored
//     type    = A pointer to where [enum2] should be stored
// Returns:
//     None
void interpret_line(
    char buffer[MAX_STRING_LENGTH], char name[MAX_STRING_LENGTH],
    enum plate_colour *colour, enum sushi_type *type) {
    // Remove extra newline
    remove_newline(buffer);

    // Extract value 1 as string (sushi roll name)
    char *name_str = strtok(buffer, " ");
    if (name_str != NULL) {
        // Extract word
        strcpy(name, name_str);
    }

    // Extract value 2 as str, then enum
    char *colour_str = strtok(NULL, " ");
    if (colour_str != NULL) {
        *colour = string_to_colour(colour_str);
    }

    // Extract value 2 as str, then enum
    char *type_str = strtok(NULL, " ");
    if (type_str != NULL) {
        *type = string_to_type(type_str);
    }

    if (name_str == NULL || colour_str == NULL || type_str == NULL) {
        // If any of these are null, there were not enough words.
        printf("Could not properly interpret line: %s.\n", buffer);
    }
}

// Helper Function
// You DO NOT NEED TO UNDERSTAND THIS FUNCTION, and will not need to change it.
//
// Given a raw string in the following foramt: [string1] [string2]
// This function will extract the relevant values into the given variables.
// The function will also remove any newline characters.
//
// For example, if given: "Gab salmon"
// The function will put:
//     "Gab" into the 'customer' array
//     "salmon" into the 'roll' array
//
// Parameters:
//     buffer   = A null terminated string in the following format
//                [string1] [string2]
//     customer = An array for the [string1] to be copied into
//     roll     = An array for the [string2] to be copied into
// Returns:
//     None
void interpret_order(
    char buffer[MAX_STRING_LENGTH], char customer[MAX_STRING_LENGTH],
    char roll[MAX_STRING_LENGTH]) {
    // Remove extra newline
    remove_newline(buffer);

    // Extract value 1 as string
    char *customer_str = strtok(buffer, " ");
    if (customer_str != NULL) {
        strcpy(customer, customer_str);
    }

    // Extract value 2 as string
    char *roll_str = strtok(NULL, " ");
    if (roll_str != NULL) {
        strcpy(roll, roll_str);
    }

    if (customer_str == NULL || roll_str == NULL) {
        // If any of these are null, there were not enough words.
        printf("Could not properly interpret line: %s.\n", buffer);
    }
}

// Helper Function
// You likely do not need to change this function.
//
// Given a raw string will remove and first newline it sees.
// The newline character wil be replaced with a null terminator ('\0')
void remove_newline(char input[MAX_STRING_LENGTH]) {
    // Find the newline or end of string
    int index = 0;
    while (input[index] != '\n' && input[index] != '\0') {
        index++;
    }
    // Goto the last position in the array and replace with '\0'
    // Note: will have no effect if already at null terminator
    input[index] = '\0';
}

// Helper Function
// You likely do not need to change this function.
//
// Given an enum plate_colour will return the first letter of enum, or ? if the
// value is not one of the plate_colour values.
// Parameters:
//     colour  = enum plate_colour for a plate
// Returns:
//     char    = character represtning the first letter of the plate colour
char color_to_char(enum plate_colour colour) {
    if (colour == RED) {
        return 'r';
    } else if (colour == GREEN) {
        return 'g';
    } else if (colour == BLUE) {
        return 'b';
    } else if (colour == PURPLE) {
        return 'p';
    } else if (colour == ORANGE) {
        return 'o';
    } else {
        return '?';
    }
}

// Helper Function
// You likely do not need to change this function.
//
// Given a raw string will return the corresponding enum plate_colour, or FALSE
// if the string doesn't correspond with the enums.
// Parameters:
//     colour  = string representing the corresponding enum plate_colour value
// Returns:
//     enum plate_colour
enum plate_colour string_to_colour(char colour[MAX_STRING_LENGTH]) {
    if (strcmp(colour, "red") == 0) {
        return RED;
    } else if (strcmp(colour, "green") == 0) {
        return GREEN;
    } else if (strcmp(colour, "blue") == 0) {
        return BLUE;
    } else if (strcmp(colour, "purple") == 0) {
        return PURPLE;
    } else if (strcmp(colour, "orange") == 0) {
        return ORANGE;
    }
    return FALSE;
}

// Helper Function
// You likely do not need to change this function.
//
// Given a raw string will return the corresponding enum sushi_type, or FALSE
// if the string doesn't correspond with the enums.
// Parameters:
//     type  = string representing the corresponding enum sushi_type value
// Returns:
//     enum sushi_type
enum sushi_type string_to_type(char type[MAX_STRING_LENGTH]) {
    if (strcmp(type, "vegetarian") == 0) {
        return VEGETARIAN;
    } else if (strcmp(type, "seafood") == 0) {
        return SEAFOOD;
    } else if (strcmp(type, "chicken") == 0) {
        return CHICKEN;
    } else if (strcmp(type, "tempura") == 0) {
        return TEMPURA;
    }
    return FALSE;
}

// Helper Function
// You likely do not need to change this function.
//
// Given an enum sushi_type will return the enum name as a string, or "ERROR" if
// the value was not one of the given enums.
// Parameters:
//     types  = string to store the name of the enum in
//     type   = sushi_type enum to have the name stored in types.
// Returns:
//     None
void to_type(char types[MAX_STRING_LENGTH], enum sushi_type type) {
    if (type == VEGETARIAN) {
        strcpy(types, "vegetarian");
    } else if (type == SEAFOOD) {
        strcpy(types, "seafood");
    } else if (type == CHICKEN) {
        strcpy(types, "chicken");
    } else if (type == TEMPURA) {
        strcpy(types, "tempura");
    } else {
        strcpy(types, "ERROR");
    }
}

// Helper Function
// You likely do not need to change this function.
//
// Given an enum plate_colour will return the enum name as a string, or "ERROR"
// if the value was not one of the given enums.
// Parameters:
//     cols   = string to store the name of the enum in
//     colour = plate_colour enum to have the name stored in cols
// Returns:
//     None
void to_colour(char cols[MAX_STRING_LENGTH], enum plate_colour colour) {
    if (colour == RED) {
        strcpy(cols, "red");
    } else if (colour == BLUE) {
        strcpy(cols, "blue");
    } else if (colour == GREEN) {
        strcpy(cols, "green");
    } else if (colour == PURPLE) {
        strcpy(cols, "purple");
    } else if (colour == ORANGE) {
        strcpy(cols, "orange");
    } else {
        strcpy(cols, "ERROR");
    }
}
