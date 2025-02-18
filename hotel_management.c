// This program is called cs_hotel
//
// This program was written by Raine Wang (z5418629)
// June 2022
//
// This program is a hotel management system for Sydney Harborfront Hotel

#include <stdio.h>

// Additional libraries here

// Provided constants
#define TRUE 1
#define FALSE 0

#define TOTAL_FLOORS 8
#define ROOMS_PER_FLOOR 8
#define INITIAL_PRICE 10
#define NO_BOOKING -1

// Your constants here
#define GROUP 1
#define NOT_GROUP 0
#define ROYAL 1
#define NOT_ROYAL 0

// Provided struct
struct room {
    int booking_id;
    int price;
    int is_group;
    int is_royal;
};

struct position {
    int guests_required;
    int floor;
    int room;
};

// Provided functions use for game setup
// You do not need to use these functions yourself.
void init_hotel(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);

// You will need to use these functions for stage 1.
void print_hotel(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
void print_hotel_with_prices(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);

// Your function prototypes here:
int set_up_phase(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
void check_room_status(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
int add_guests(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int booking_id);
void check_out_room(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
void set_floor_price(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
void vary_price(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
int get_current_revenue(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
void print_profit_or_loss(int current_revenue, int last_revenue);
int book_best_available(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int booking_id);
void swap_rooms(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
int get_available_rooms(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int floor);
int group_booking(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int booking_id);
int book_cheapest_room(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int booking_id);
int book_cheapest_floor(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int guests, int booking_id);
int book_group_budget_floor(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int booking_id);
int add_royal_guest(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int booking_id);
int replace_with_royal(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int floor, int room, int booking_id);
void burglar_alert(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
int check_burglar_access(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR],
    int visited[TOTAL_FLOORS][ROOMS_PER_FLOOR], int floor, int room);
void vivid_display (struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
void vivid_option1 (struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
void vivid_option2 (struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
void scan_in_pattern (char pattern[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int pattern_height, int pattern_width);
int check_display_possible (struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int floor, int room, char pattern[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int pattern_height, int pattern_width);
int check_guests_required (struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int floor, int room, char pattern[TOTAL_FLOORS][ROOMS_PER_FLOOR],
    int pattern_height, int pattern_width);
struct position find_best_position(
    int guests_required[TOTAL_FLOORS][ROOMS_PER_FLOOR], int pattern_height,
    int pattern_width);
    
int main(void) {
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR];
    init_hotel(hotel);

    int booking_id = 0;

    booking_id = set_up_phase(hotel);

    print_hotel(hotel);

    int last_revenue = 0;
    char command;

    while (1) {
        printf("Enter a command: ");
        if (scanf(" %c", &command) == EOF) {
            break;
        } 
        
        if (command == 'p') {
            print_hotel(hotel);
        } else if (command == '?') {
            check_room_status(hotel);
        } else if (command == 'a') {
            booking_id += add_guests(hotel, booking_id);
        } else if (command == 'l') {
            check_out_room(hotel);
        } else if (command == 'c') {
            print_hotel_with_prices(hotel);
        } else if (command == '$') {
            set_floor_price(hotel);
        } else if (command == '^') {
            vary_price(hotel);
        } else if (command == 'r') { 
            int current_revenue = get_current_revenue(hotel);
            print_profit_or_loss(current_revenue, last_revenue);
            last_revenue = current_revenue;
        } else if (command == 'j') {
            booking_id += book_best_available(hotel, booking_id);
        } else if (command == 's') {
            swap_rooms(hotel);
        } else if (command == 'g') {
            if (group_booking(hotel, booking_id) > 0) {
                booking_id++;
            }
        } else if (command == '!') {
            if (add_royal_guest(hotel, booking_id) == 1) {
                booking_id++;
            }
        } else if (command == 'b') {
            burglar_alert(hotel);
        } else if (command == 'v') {
            vivid_display(hotel);
        }
    }
    printf("Bye!\n");
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// ADDITIONAL FUNCTIONS /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Starting phase, scans in initial guests.
int set_up_phase(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]) {
    int guests;
    printf("Welcome to the CS Hotel!!\n\n");
    printf("Add your current guests, how many guests? ");
    scanf("%d", &guests);
    printf("Input in your guests room numbers\n");
    
    int floor, room;
    int booking_id = 0; 
    int i = 0;
    while (i < guests) {
        scanf("%d %d", &floor, &room);
        
        if (floor < 0 || floor >= TOTAL_FLOORS || 
            room < 0 || room >= ROOMS_PER_FLOOR) {
            printf("Invalid room number!\n");
        } else if (hotel[floor][room].booking_id != NO_BOOKING) {
            printf("Room already occupied!\n");
        } else {
            hotel[floor][room].booking_id = booking_id;
            hotel[floor][room].is_group = NOT_GROUP;
            hotel[floor][room].is_royal = NOT_ROYAL;
            booking_id++;
        }
        i++;
    }
    return booking_id;
}

// command '?'
// Prints the booking id of the guest occupying the specified room.
void check_room_status(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]) {
    int floor, room;
    scanf("%d %d", &floor, &room);
    if (floor < 0 || floor >= TOTAL_FLOORS || 
        room < 0 || room >= ROOMS_PER_FLOOR) {
        printf("Invalid room number!\n");
    } else if (hotel[floor][room].booking_id == -1) {
        printf("Floor %d, Room %d is empty.\n", floor, room);
    } else {
        printf("Booking ID at Floor %d, Room %d is : %d\n", 
        floor, room, hotel[floor][room].booking_id);
    } 
}

// command 'a'
// Checks in a guest to the specified room if vacant.
int add_guests(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR],
    int booking_id) {
    int floor, room;
    scanf("%d %d", &floor, &room);
    if (floor < 0 || floor >= TOTAL_FLOORS || 
        room < 0 || room >= ROOMS_PER_FLOOR) {
        printf("Invalid room number!\n");
    } else if (hotel[floor][room].booking_id != NO_BOOKING) {
        printf("Room already occupied!\n");
    } else {
        hotel[floor][room].booking_id = booking_id;
        hotel[floor][room].is_group = NOT_GROUP;
        hotel[floor][room].is_royal = NOT_ROYAL;
        printf("Guest's booking id is: %d\n", booking_id);
        return 1;
    }
    return 0;
}

// command 'l'
// Checks out a guest from a specified and occupied room.
void check_out_room(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]) {
    int floor, room;
    scanf("%d %d", &floor, &room);
    if (floor < 0 || floor >= TOTAL_FLOORS || 
        room < 0 || room >= ROOMS_PER_FLOOR) {
        printf("Invalid room number!\n");
    } else if (hotel[floor][room].booking_id != NO_BOOKING) { 
        printf("Goodbye booking %d!\n", hotel[floor][room].booking_id);
        hotel[floor][room].booking_id = NO_BOOKING;  
    } else {
        printf("This room is already empty.\n");
    }
}

// command '$'
// Changes the price of a specified floor to a specified new price.
void set_floor_price(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]) {
    int floor, new_price;
    scanf("%d %d", &floor, &new_price);
    if (floor >= TOTAL_FLOORS || floor < 0) {
        printf("Invalid floor number!\n");
    } else if (new_price < 0) {
        printf("Invalid price!\n");
    } else {
        int i = 0;
        while (i < ROOMS_PER_FLOOR) {
            hotel[floor][i].price = new_price;
            i++;
        }
    }
}

// command '^'
// Varies the price of every room in the hotel by specified amount.
void vary_price(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]) {
    int amount;
    scanf("%d", &amount);
    int i = 0;
    while (i < TOTAL_FLOORS) {
        int j = 0;
        while (j < ROOMS_PER_FLOOR) {
            int new_price = hotel[i][j].price + amount;
            if (new_price < 0) {
                new_price = 0;
            } 
            hotel[i][j].price = new_price;
            j++;
        }
        i++;
    }
}

// command 'r'
// Loops through the hotel and if the room is occupied, adds the price of that 
// room to the current revenue. Then sets last revenue to this revenue in main.
int get_current_revenue(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]) {
    int current_revenue = 0; 
    int i = 0;
    while (i < TOTAL_FLOORS) {
        int j = 0;
        while (j < ROOMS_PER_FLOOR) {
            if (hotel[i][j].booking_id > NO_BOOKING) {
                current_revenue += hotel[i][j].price;
            }
            j++;
        }
        i++;
    }
    return current_revenue;
}

// 'r'
// Prints out the correct accompanying messagge for the current revenue.
void print_profit_or_loss(int current_revenue, int last_revenue) {
    int diff = current_revenue - last_revenue;
    printf("The total revenue for the night is $%d\n", current_revenue);
    if (diff > 0) {
        printf("This is $%d more than last time :)\n", diff);
    } else if (diff < 0) {
        printf("This is $%d less than last time (haha lol)\n", -diff);
    } else {
        printf("This is the same as the last time you asked :\\\n");
    }
}

// command 'j'
// Loops through the hotel and searches for vacant rooms, then breaks the loop
// when the first available room is found, and books a guest in. If no room is 
// found, print error message.
int book_best_available(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR],
    int booking_id) {
    int room_found = 0;
    int i = 0, j = 0;
    while (i < TOTAL_FLOORS) {
        j = 0;
        while (j < ROOMS_PER_FLOOR) {
            if (hotel[i][j].booking_id == NO_BOOKING) {
                room_found = 1;
                break;  
            } 
            j++;
        }
        if (room_found == 1) {
            break;
        }
        i++;
    }
    if (room_found == 0) {
        printf("The hotel is full!\n");
    } else {
        hotel[i][j].booking_id = booking_id;
        hotel[i][j].is_group = NOT_GROUP;
        hotel[i][j].is_royal = NOT_ROYAL;
    }
    return 1;
}

// command 's'
// Swaps the booking id, group status, and royal status of 2 rooms.
void swap_rooms(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]) {
    int floor1, floor2, room1, room2;
    int temp_booking_id = 0;
    int temp_group_status = 0;
    int temp_royal_status = 0;
    scanf("%d %d %d %d", &floor1, &room1, &floor2, &room2);
    if (floor1 > TOTAL_FLOORS || floor2 > TOTAL_FLOORS) {
        printf("Invalid room number!\n");
    } else if (floor1 < 0 || floor2 < 0) {
        printf("Invalid room number!\n");
    } else if (room1 > ROOMS_PER_FLOOR || room1 < 0 || 
        room2 > ROOMS_PER_FLOOR || room2 < 0) {
        printf("Invalid room number!\n");
    } else {
        temp_booking_id = hotel[floor1][room1].booking_id;
        temp_group_status = hotel[floor1][room1].is_group;
        temp_royal_status = hotel[floor1][room1].is_royal;
        // stores floor1,room1's info in a temp variable so it isn't displaced

        hotel[floor1][room1].booking_id = hotel[floor2][room2].booking_id;
        hotel[floor1][room1].is_group = hotel[floor2][room2].is_group;
        hotel[floor1][room1].is_royal = hotel[floor2][room2].is_royal;

        hotel[floor2][room2].booking_id = temp_booking_id;
        hotel[floor2][room2].is_group = temp_group_status;
        hotel[floor2][room2].is_royal = temp_royal_status;

    }
}

// command 'g'
int group_booking(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int booking_id) {
    char option;
    int guests_booked = 0;
    scanf(" %c", &option);
    if (option == '1') {
        // Option 1 : the lowest cheapest available floor.
        int guests = 0;
        scanf("%d", &guests);
        guests_booked = book_cheapest_floor(hotel, guests, booking_id);
    } else if (option == '2') {
        // Option 2: the highest floor that is also available within 
        // specified budget.
        guests_booked = book_group_budget_floor(hotel, booking_id);
    }
    return guests_booked;
}

// command 'g' option 1
// Counts the number of available rooms per floor
int get_available_rooms(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR],
    int floor) {
    int rooms = 0;
    int i = 0;
    while (i < ROOMS_PER_FLOOR) {
        if (hotel[floor][i].booking_id == NO_BOOKING) {
            rooms++;
        }
        i++;
    }
    return rooms;
}

// 'g' option 1
// Loops through the hotel and finds the floor with the lowest price, then books
// the group of guests into the lowest cheapest floor.
int book_cheapest_floor(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR],
    int guests, int booking_id) {
    int lowest_price = -1;
    int floor = -1;
    int i = 0;
    while (i < TOTAL_FLOORS) {
        int available_rooms = get_available_rooms(hotel, i); 
        if (available_rooms >= guests) {
            int floor_room_price = hotel[i][0].price;
            if (lowest_price == -1 || floor_room_price <= lowest_price) {
                lowest_price = floor_room_price;
                floor = i;
            }
        }
        i++;
    }
    if (floor == -1) {
        printf("No available floor for your group booking!\n");
        // no available floor
        return 0;
    } else {
        int guests_booked = 0;
        int j = 0;
        while (j < ROOMS_PER_FLOOR && guests_booked < guests) {
            if (hotel[floor][j].booking_id == NO_BOOKING) {
                hotel[floor][j].booking_id = booking_id;
                hotel[floor][j].is_group = GROUP;
                hotel[floor][j].is_royal = NOT_ROYAL;
                guests_booked++;
            }
            j++;
        }
        return guests;
    }
}

// 'g' option 2
// Finds rooms that fit within the budget and books in group.
int book_group_budget_floor(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int booking_id) {
    int guests, budget;
    int guests_booked = 0;
    scanf("%d %d", &guests, &budget);
    int floor = -1;
    int i = 0;
    while (i < TOTAL_FLOORS) {
        int available_rooms = get_available_rooms(hotel, i); 
        if (available_rooms >= guests) {
            int floor_room_price = hotel[i][0].price;
            if (floor_room_price <= budget) {
                floor = i;
                break;
            }
        }
        i++;
    }
    if (floor == -1) {
        printf("No available floor for your group booking!\n");
        
    } else {
        int j = 0;
        while (j < ROOMS_PER_FLOOR && guests_booked < guests) {
            if (hotel[floor][j].booking_id == NO_BOOKING) {
                hotel[floor][j].booking_id = booking_id;
                hotel[floor][j].is_group = GROUP;
                hotel[floor][j].is_royal = NOT_ROYAL;
                guests_booked++;
            }
            j++;
        }
    } 
    return guests_booked;
}

// command '!'
// Books guests with royal priority in.
int add_royal_guest(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int booking_id) {
    int success_booking = 0;
    int floor, room;
    scanf("%d %d", &floor, &room);
    //royalty
    if (floor < 0 || floor >= TOTAL_FLOORS || 
        room < 0 || room >= ROOMS_PER_FLOOR) {
        printf("Invalid room number!\n");
    } else if (hotel[floor][room].booking_id == NO_BOOKING) {
        hotel[floor][room].booking_id = booking_id;
        hotel[floor][room].is_group = NOT_GROUP;
        hotel[floor][room].is_royal = ROYAL;
        success_booking = 1;
    } else {
        success_booking = replace_with_royal(hotel, floor, room, booking_id);
    }
    return success_booking;
}

// '!'
// Replaces and then relocates the non-royal guest with a royal guest
int replace_with_royal(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int floor, int room, int booking_id) {
    int success_booking = 0;

    if (hotel[floor][room].is_group == NOT_GROUP && 
        hotel[floor][room].is_royal == NOT_ROYAL) {
        int original_bid = hotel[floor][room].booking_id;
        hotel[floor][room].booking_id = booking_id;
        hotel[floor][room].is_group = NOT_GROUP;
        hotel[floor][room].is_royal = ROYAL;
        success_booking = 1;
        
        book_best_available(hotel, original_bid);
    } else if (hotel[floor][room].is_group == GROUP && 
        hotel[floor][room].is_royal == NOT_ROYAL) {
        int original_bid = hotel[floor][room].booking_id;
        int i = 0;
        int guests_to_move = 0;
        while (i < TOTAL_FLOORS) {
            int j = 0;
            while (j < ROOMS_PER_FLOOR) {   
                if (hotel[i][j].booking_id == original_bid) {
                    guests_to_move++;
                    hotel[i][j].booking_id = NO_BOOKING;
                    hotel[i][j].is_group = NOT_GROUP;
                }
                j++;
            }
            i++;
        }
        hotel[floor][room].booking_id = booking_id;
        hotel[floor][room].is_group = NOT_GROUP;
        hotel[floor][room].is_royal = ROYAL;
        success_booking = 1;

        book_cheapest_floor(hotel, guests_to_move, original_bid);
    } else if (hotel[floor][room].is_royal == ROYAL) {
        printf("Your Royal Highness, this room is not available\n");
    }

    return success_booking;
}

// command 'b'
// Prints out if burglar has access to roof or not.
void burglar_alert(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]) {
    int visited[TOTAL_FLOORS][ROOMS_PER_FLOOR];
    int i = 0;
    while (i < TOTAL_FLOORS) {
        int j = 0;
        while (j < ROOMS_PER_FLOOR) {
            visited[i][j] = 0;
            // marks the room as visited
            j++;
        }
        i++;
    }
    int result = check_burglar_access(hotel, visited, 
        TOTAL_FLOORS - 1, 0);

    if (result == 1) {
        printf("Possible for a burglar to reach the rooftop!\n");
    } else {
        printf("Impossible for a burglar to reach the rooftop!\n");
    }
}

// Recursive; Function repeatedly returns itself and checks if there is an
// available path for the burglar who can only move left, right, and up.
int check_burglar_access(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR],
    int visited[TOTAL_FLOORS][ROOMS_PER_FLOOR], int floor, int room) {
    if (room < 0 || room >= ROOMS_PER_FLOOR) {
        return 0;
    }
    visited[floor][room] = 1;

    // if room occupied then return because path is blocked
    if (hotel[floor][room].booking_id > NO_BOOKING) {
        return 0;
    }

    if (floor == 0) {
        return 1;
    } 

    // check path to the left
    if (room > 0 && visited[floor][room-1] == 0 && 
        check_burglar_access(hotel, visited, floor, room - 1) == 1) {
        return 1;
    } 
    
    // check path to the right
    if (room < ROOMS_PER_FLOOR - 1 && visited[floor][room+1] == 0 && 
        check_burglar_access(hotel, visited, floor, room + 1) == 1) {
        return 1;
    }

    // check path the floor up
    if (visited[floor-1][room] == 0 && 
        check_burglar_access(hotel, visited, floor - 1, room) == 1) {
        return 1;
    }

    return 0;
}

// command 'v'
// 
void vivid_display (struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]) {
    char option;
    scanf(" %c", &option);
    if (option == '1') {
        vivid_option1(hotel);
    } else if (option == '2') {
        vivid_option2(hotel);
    }
}
// 'v' 1
// Prints if the vivid display is possible or not for the first vivid command.
void vivid_option1 (struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]) {
    char pattern[TOTAL_FLOORS][ROOMS_PER_FLOOR];
    int pattern_height, pattern_width;
    scanf("%d %d\n", &pattern_height, &pattern_width);
    scan_in_pattern(pattern, pattern_height, pattern_width);
    
    int match_found = 0;
    int i = 0;
    while (i < TOTAL_FLOORS && match_found == 0) {
        int j = 0;
        while (j < ROOMS_PER_FLOOR && match_found == 0) {
            int pattern_check = check_display_possible(hotel, i, j, 
                pattern, pattern_height, pattern_width);

            if (pattern_check == 1) {
                match_found = 1;
            }
            j++;
        }
        i++;
    }
    if (match_found == 0) {
        printf("Display impossible!\n");
    } else {
        printf("Display possible!\n");
    }
}

// 'v' 2
void vivid_option2 (struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]) {
    char pattern[TOTAL_FLOORS][ROOMS_PER_FLOOR];
    int pattern_height = 0, pattern_width = 0;
    scanf("%d %d\n", &pattern_height, &pattern_width);
    scan_in_pattern(pattern, pattern_height, pattern_width);
    int guests_required[TOTAL_FLOORS][ROOMS_PER_FLOOR];

    int floor = 0, room = 0;
    int i = 0;
    
    int required = 0;
    int match_found = 0;
    while (i < TOTAL_FLOORS - pattern_height + 1 
        && match_found == 0) {
        int j = 0;
        while (j < ROOMS_PER_FLOOR - pattern_width + 1 
            && match_found == 0) {
            required = check_guests_required(
                hotel, i, j, pattern, pattern_height, 
                pattern_width);

            if (required == 0) {
                match_found = 1;
                floor = i;
                room = j;
            } else {
                guests_required[i][j] = required;
            }
            j++;
        }
        i++;
    }

    if (match_found == 0) {
        struct position pos = find_best_position(guests_required, 
            pattern_height, pattern_width);
        required = pos.guests_required;
        floor = pos.floor;
        room = pos.room;
    } 
    // if found (match_found == 1)
    // print 
    // loop 2d array "guests_required" to find the smallest number

    printf("%d guests needed for display starting at ", required);
    printf("position (%d, %d).\n", floor, room);
}

// Scans in the pattern for the vivid display.
void scan_in_pattern (char pattern[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int pattern_height, int pattern_width) {
    int i = 0, j = 0;
    while (i < pattern_height) {
        j = 0;
        while (j < pattern_width) {
            scanf(" %c", &pattern[i][j]);
            j++;
        }
        i++;
    }
} 

// 'v' option 1
// Checks if the scanned in display is possible (0 is impossible, 1 is possible)
// by seeing if there are any '#'s that coincide with vacant rooms.
int check_display_possible (struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int floor, int room, char pattern[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int pattern_height, int pattern_width) {
    int i = 0;
    while (i < pattern_height) {
        int j = 0;
        while (j < pattern_width) {
            if ((floor + i >= TOTAL_FLOORS || room + j >= ROOMS_PER_FLOOR) ||
                (pattern[i][j] == '#' && 
                hotel[floor + i][room + j].booking_id == NO_BOOKING)) {
                return 0;
            } 
            j++;
        }
        i++;
    }
    return 1;
}

// 'v' option 2
// Counts how many rooms are vacant and coincide with a '#' to calculate how 
// many guests are required for the display.
int check_guests_required (struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int floor, int room, char pattern[TOTAL_FLOORS][ROOMS_PER_FLOOR],
    int pattern_height, int pattern_width) {
    int guests_required = 0;
    int i = 0;
    while (i < pattern_height) {
        int j = 0;
        while (j < pattern_width) {
            if ((floor + i < TOTAL_FLOORS && room + j < ROOMS_PER_FLOOR) && 
                (hotel[i + floor][j + room].booking_id == NO_BOOKING && 
                pattern[i][j] == '#')) {
                guests_required++;
            }
            j++;
        }
        i++;
    }
    return guests_required;
}

// 'v' option 2
// Creates a new struct, so that both the position of the room and amount of 
// guests required can be returned.
struct position find_best_position(
    int guests_required[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
    int pattern_height,
    int pattern_width) {
    struct position pos;
    int min = 0;
    int floor = 0;
    int room = 0;
    int i = 0;
    while (i < TOTAL_FLOORS - pattern_height + 1) {
        int j = 0;
        while (j < ROOMS_PER_FLOOR - pattern_width + 1) {
            if (min == 0 || guests_required[i][j] < min) {
                min = guests_required[i][j];
                floor = i;
                room = j;
            }
            j++;
        }
        i++;
    }
    pos.guests_required = min;
    pos.floor = floor;
    pos.room = room;

    return pos;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// PROVIDED FUNCTIONS //////////////////////////////
/////////////////////////// (DO NOT EDIT BELOW HERE) ///////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Provided Function - DO NOT EDIT
// Initalises the booking id and price of each room in the hotel to be
// the default values.
void init_hotel(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]) {
    int row = 0;
    while (row < TOTAL_FLOORS) {
        int col = 0;
        while (col < ROOMS_PER_FLOOR) {
            hotel[row][col].booking_id = NO_BOOKING;
            hotel[row][col].price = INITIAL_PRICE;
            col++;
        }
        row++;
    }
}

// Provided Function - DO NOT EDIT
// Prints out the hotel with the booking number of each room, or empty
// if there is no booking.
void print_hotel(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]) {
    printf(
        "\n"
        "         /--^--\\\n"
        "   _     |  O  |     _\n"
        " _/ \\___/_^___^_\\___/ \\_\n"
        "/                       \\\n");

    int row = 0;
    while (row < TOTAL_FLOORS) {
        int col = 0;
        printf("|--+--+--+--+--+--+--+--|\n");
        while (col < ROOMS_PER_FLOOR) {
            printf("|");
            if (hotel[row][col].booking_id == NO_BOOKING) {
                printf("  ");
            } else {
                printf("%02d", hotel[row][col].booking_id);
            }
            col++;
        }
        printf("|\n");
        row++;
    }
    printf(
        "|--+--+--+--+--+--+--+--|\n"
        "|         /^^^\\         |\n"
        "|_________|_\"_|_________|\n\n");
}

// Provided Function - DO NOT EDIT
// Prints out the hotel with the the price and booking number included.
void print_hotel_with_prices(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]) {
    printf(
        "\n"
        "             /--^--\\\n"
        "   _         |  O  |         _\n"
        " _/ \\_______/_^___^_\\_______/ \\_\n"
        "/                               \\\n");

    int row = 0;
    while (row < TOTAL_FLOORS) {
        int col = 0;
        printf("|---+---+---+---+---+---+---+---|\n");
        while (col < ROOMS_PER_FLOOR) {
            printf("|");
            if (hotel[row][col].booking_id == NO_BOOKING) {
                printf("   ");
            } else {
#ifndef NO_COLORS
                printf("\033[1;35m");
#endif
                printf("%03d", hotel[row][col].booking_id);
#ifndef NO_COLORS
                printf("\033[0m");
#endif
            }
            col++;
        }
        printf("|\n");
        col = 0;
        while (col < ROOMS_PER_FLOOR) {
            printf("|");
#ifndef NO_COLORS
            printf("\033[1;32m");
#endif
            printf("%03d", hotel[row][col].price);
#ifndef NO_COLORS
            printf("\033[0m");
#endif
            col++;
        }
        printf("|\n");
        row++;
    }
    printf(
        "|---+---+---+---+---+---+---+---|\n"
        "|             /^^^\\             |\n"
        "|_____________|_\"_|_____________|\n\n");
}
