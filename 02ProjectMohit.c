
/*
                        < Hostel Management System >
    Create a C-based system to track hostel room allocations, resident details, and fee payments.

    # The program should include:
                * An admin login for security
                * Ability to add, update, or remove student records
                * Support for room availability checks
                * Generate reports on hostel occupancy

    # My Assumtion:
        - 101 room's in hostel ( Room 000 is reserved for admin )
        - User name  and Pass for login is <a> <a>
        - Individual User and pass  is <a> <a>
        ** Password and Username is of limit 10 + 1 characters

        - From 1 to 50 - Single Bed Room (Type 1) and 1000 INR
        - From 51 to 80 - Double Bed Room (Type 2) and 2000 INR
        - From 81 to 100 - Triple Bed Room (Type 3) and 3000 INR
        ** Per Bedroom Rent is 1000 INR

        - Roomno -> %03d
        - Room Type -> %d
        - Rent -> %7.2f
        - Date -> %02d-%02d-%04d
        - Name -> %s
        - Mobile Number -> %s

        -flag0 -> Main Menu
        -flag01 -> Report Menu
        -flag02 -> Allocation/Re-Allocation/Leave Menu
        -flag03 -> Edit Menu
        -flag04 -> Edit Room Menu
        -flag05 -> Edit Room Details Menu
    */

#include <stdio.h>
#include <string.h>
#include <unistd.h>  // usleep
#include <stdlib.h>  // system("clear")
#include <time.h>    // For time(), localtime(), and strftime()
#include <ctype.h>   // For isdigit()

#define nRoom 101          // Number of rooms
#define Rent 1000          // Rent per Bedroom in INR
#define DATA_FILE "hostel_data.bin" // Data file name
#define UserName "a"       // Admin username
#define Password "a"       // Admin password

#define RESET "\033[0m"    // Reset color
#define RED "\033[31m"     // Red for errors
#define GREEN "\033[32m"   // Green for success
#define YELLOW "\033[33m"  // Yellow for warnings/info
#define BLUE "\033[34m"    // Blue for headers
#define CYAN "\033[36m"    // Cyan for data
#define BOLD "\033[1m"     // Bold text

#define REPEAT 22          // Number of times to repeat loading dots
#define DELAY_DOT 200000   // Delay in microseconds for loading dots
#define DELAY_TEXT 10000   // Delay in microseconds for text printing
#define MAX_LEN 100        // Max length for string inputs
#define MOBILE_LEN 20      // Max length for mobile number
#define DATE_LEN 12        // Max length for date string (DD-MM-YYYY)

// Structure to hold date information
typedef struct date {
    int day;
    int month;
    int year;
} DATE;

// Structure to hold room information
typedef struct roomInfo {
    int roomNumber;
    char guestName[30];
    char guestMobileNumber[20];
    char guestAddress[100];
    float guestRent;
    float balance;
    int roomType;
    char guestPassword[20], guestUsername[20];
    DATE date;
} ROOM;

ROOM R[nRoom]; // Array of structure to store all room details

// Function prototypes
void gotoxy(int row, int col);
void clearScreen();
void clearLine(int row, int col, int len);
void clearInputBuffer();
void slowPrint(const char* text, useconds_t delay);
void printlayout();
void pressEnterToContinue();
int adminCheck(char* a, char* b);
int validateRoomNumber(int room);
void reNullRoom(int i);
void nullRoom(struct roomInfo *ptr);
void findVacancy(struct roomInfo* ptr);
void allDetails(struct roomInfo* ptr);
void saveDataToFile(const char *filename, struct roomInfo *ptr);
void loadDataFromFile(const char *filename, struct roomInfo *ptr);
void fillSampleData(int count, int mode);
int isValidMobile(const char* mobile);
int isValidFloat(const char* str, float* value);
int isValidDate(const char* dateStr, int* day, int* month, int* year);

// Move cursor to specific row and column
void gotoxy(int row, int col) {
    printf("\033[%d;%dH", row, col);
    fflush(stdout);
}

// Clear console screen
void clearScreen() {
    printf("\033[2J\033[H");
    fflush(stdout);
}

// Clear a specific line or area
void clearLine(int row, int col, int len) {
    gotoxy(row, col);
    for (int i = 0; i < len; i++) printf(" ");
    fflush(stdout);
}

// Clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Slow print for typewriter effect
void slowPrint(const char* text, useconds_t delay) {
    while (*text) {
        printf("%c", *text++);
        usleep(DELAY_TEXT);
    }
}

// Print login page banner
void printlayout() {
    printf(BOLD GREEN "\n\t\t\tL O A D I N G - P R O J E C T - 2 . O\n\t" RESET);
    for (int i = 0; i < REPEAT; i++) {
        printf(BOLD " * " RESET);
        usleep(DELAY_DOT);
    }
    printf("\n\n\n\n");
    printf("\t|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
    printf("\t|||||||||||||| HOSTEL MANAGEMENT SERVICE 2025 |||||||||||||||\n");
    printf("\t||||||||||||||||||||||| LOGIN PAGE ||||||||||||||||||||||||||\n");
    printf("\t|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
    printf("\n\n");
}

// Pause and wait for Enter
void pressEnterToContinue() {
    printf(BOLD "\n\tApplying changes " RESET);

    for (int i = 0; i < REPEAT; i++) {
        printf(BOLD " * " RESET);
        usleep(DELAY_DOT);
    }

    printf(BOLD "\n\tPress Enter to CONTINUE " RESET);
    clearInputBuffer();
    char ch = getchar();
}

// Validate admin credentials
int adminCheck(char* a, char* b) {
    char x[11], y[11];
    printf(YELLOW "\n\tEnter the Username: " RESET);
    scanf(" %10[^\n]", x);
    printf(YELLOW "\tEnter the Password: " RESET);
    scanf(" %10[^\n]", y);
    return (!(strcmp(a, x) || strcmp(b, y)));
}

// Validate room number
int validateRoomNumber(int room) {
    return (room > 0 && room < nRoom);
}

// Nullify a room
void reNullRoom(int i) {
    strcpy(R[i].guestName, "NULL");
    strcpy(R[i].guestMobileNumber, "NULL");
    strcpy(R[i].guestAddress, "NULL");
    R[i].balance = 0.0;
    strcpy(R[i].guestUsername, "b");
    strcpy(R[i].guestPassword, "b");
    R[i].date.day = 0;
    R[i].date.month = 0;
    R[i].date.year = 0;
}

// Initialize rooms
void nullRoom(struct roomInfo *ptr) {
    ptr[0].roomNumber = 0;
    strcpy(ptr[0].guestName, "Mohit Singh");
    strcpy(ptr[0].guestMobileNumber, "9105770414");
    strcpy(ptr[0].guestAddress, "Dehradun");
    ptr[0].guestRent = 0.0;
    strcpy(ptr[0].guestUsername, "b");
    strcpy(ptr[0].guestPassword, "b");
    ptr[0].date.day = 5;
    ptr[0].date.month = 11;
    ptr[0].date.year = 2005;
    ptr[0].balance = 0.0;
    ptr[0].roomType = 5;

    for (int i = 1; i < nRoom; i++) {
        ptr[i].roomNumber = i;
        strcpy(ptr[i].guestName, "NULL");
        strcpy(ptr[i].guestMobileNumber, "NULL");
        strcpy(ptr[i].guestAddress, "NULL");
        strcpy(ptr[i].guestUsername, "b");
        strcpy(ptr[i].guestPassword, "b");
        ptr[i].date.day = 0;
        ptr[i].date.month = 0;
        ptr[i].date.year = 0;
        ptr[i].balance = 0.0;
        if (i <= 50) ptr[i].roomType = 1;
        else if (i <= 80) ptr[i].roomType = 2;
        else ptr[i].roomType = 3;
        ptr[i].guestRent = Rent * ptr[i].roomType;
        ptr[i].balance = ptr[i].guestRent;
    }
}

// Display vacant rooms
void findVacancy(struct roomInfo* ptr) {
    printf(CYAN "\n\t------------------------------------------------------------\n" RESET);
    printf(CYAN "\t|                  Available Rooms                         |\n" RESET);
    printf(CYAN "\t------------------------------------------------------------\n" RESET);
    for (int i = 1; i < nRoom; i++) {
        if (strcmp(ptr[i].guestName, "NULL") == 0) {
            printf(CYAN "\t| Room No: %03d | Type: %dB 1K | Rent: %7.2f      |\n" RESET, ptr[i].roomNumber, ptr[i].roomType, ptr[i].guestRent);
        }
    }
    printf(CYAN "\t------------------------------------------------------------\n\n" RESET);
}

// Display all room details
void allDetails(struct roomInfo* ptr) {
    printf(GREEN "\n\t-------------------------------------------------------------------------------------------------\n" RESET);
    printf(GREEN "\t|                         All Room Details                                                      |\n" RESET);
    printf(GREEN "\t-------------------------------------------------------------------------------------------------\n" RESET);
    printf(GREEN "\t| R.No |      CO Name       |    Ph.No    |      Address         |  Balance  |    Date      |\n" RESET);
    printf(GREEN "\t+------+--------------------+-------------+----------------------+-----------+--------------+\n" RESET);
    for (int i = 1; i < nRoom; i++) {
        if (strcmp(ptr[i].guestName, "NULL")) {
            printf(CYAN "\t| %03d  | %-18s | %-11s | %-20s | %9.2f | %02d-%02d-%4d |\n" RESET, 
                   ptr[i].roomNumber, ptr[i].guestName, ptr[i].guestMobileNumber, ptr[i].guestAddress, 
                   ptr[i].balance, ptr[i].date.day, ptr[i].date.month, ptr[i].date.year);
        }
    }
    printf(GREEN "\t+------+--------------------+-------------+----------------------+-----------+--------------+\n" RESET);
    printf(CYAN "\n\t------------------------------------------------------------------------\n\n" RESET);
}

// Save room data to binary file
void saveDataToFile(const char *filename, struct roomInfo *ptr) {
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf(RED "\n\tError: Could not open file to save data.\n" RESET);
        return;
    }
    for (int i = 1; i < nRoom; i++) {
        if (strcmp(ptr[i].guestName, "NULL")) {
            if (fwrite(&ptr[i], sizeof(ROOM), 1, fp) != 1) {
                printf(RED "\n\tError writing data for room %d to file.\n" RESET, ptr[i].roomNumber);
            }
        }
    }
    fclose(fp);
    printf(GREEN "\n\tData saved successfully to binary file (%s)!\n" RESET, filename);
}

// Load room data from binary file
void loadDataFromFile(const char *filename, struct roomInfo *ptr) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf(YELLOW "\n\tWarning: No existing data file found (%s). Initializing new hostel setup...\n" RESET, filename);
        return;
    }
    ROOM tempRoom;
    while (fread(&tempRoom, sizeof(ROOM), 1, fp) == 1) {
        if (validateRoomNumber(tempRoom.roomNumber))
            ptr[tempRoom.roomNumber] = tempRoom;
        else
            printf(RED "\n\tWarning: Invalid room number %d found in data file. Record skipped.\n" RESET, tempRoom.roomNumber);
    }
    if (ferror(fp))
        printf(RED "\n\tError reading from data file (%s).\n" RESET, filename);
    fclose(fp);
}

// Fill sample data
void fillSampleData(int count, int mode) {
    srand(time(NULL));
    int filled = 0;
    for (int i = 1; i < nRoom && filled < count; i++) {
        int roomIndex = (mode == 1) ? (1 + rand() % (nRoom - 1)) : i;
        if (strcmp(R[roomIndex].guestName, "NULL") == 0) {
            sprintf(R[roomIndex].guestName, "Student_%d", filled + 1);
            sprintf(R[roomIndex].guestMobileNumber, "900000%04d", filled + 1);
            sprintf(R[roomIndex].guestAddress, "Address_%d", filled + 1);
            sprintf(R[roomIndex].guestUsername, "user%d", filled + 1);
            sprintf(R[roomIndex].guestPassword, "pass%d", filled + 1);
            R[roomIndex].date.day = 28;
            R[roomIndex].date.month = 5;
            R[roomIndex].date.year = 2025;
            R[roomIndex].balance = 0.0;
            filled++;
        }
    }
    printf(GREEN "\n\t✔ %d Sample Students Successfully Inserted using %s Allocation.\n" RESET,
           filled, (mode == 1 ? "Random" : "Sequential"));
}

// Validate mobile number (10 digits, numeric)
int isValidMobile(const char* mobile) {
    if (strlen(mobile) != 10) return 0;
    for (int i = 0; i < 10; i++) {
        if (!isdigit((unsigned char)mobile[i])) return 0;
    }
    return 1;
}

// Validate float input
int isValidFloat(const char* str, float* value) {
    char *endptr;
    *value = strtof(str, &endptr);
    if (endptr == str || *endptr != '\0') return 0;
    if (*value < 0) return 0;
    return 1;
}

// Validate date (DD-MM-YYYY)
int isValidDate(const char* dateStr, int* day, int* month, int* year) {
    int d, m, y;
    if (sscanf(dateStr, "%d-%d-%d", &d, &m, &y) != 3) return 0;
    if (d < 1 || d > 31 || m < 1 || m > 12 || y < 1900 || y > 2025) return 0;
    *day = d; *month = m; *year = y;
    return 1;
}

int main() {
    system("cls");
    printlayout();

    char adminUserName[] = UserName;
    char adminPassword[] = Password;
    int loginAttempts = 2;
    int loginSuccessful = 0;

    while (loginAttempts >= 0) {
        if (adminCheck(adminUserName, adminPassword)) {
            printf(GREEN "\n\t--------------------------------------------------\n" RESET);
            printf(GREEN "\t|          Login Successfully !!                 |\n" RESET);
            printf(GREEN "\t--------------------------------------------------\n\n" RESET);
            pressEnterToContinue();
            loginSuccessful = 1;
            break;
        }
        if (loginAttempts > 0) {
            printf(RED "\n\t--------------------------------------------------\n" RESET);
            printf(RED "\t| Incorrect Password or Username (%d try left)   |\n" RESET, loginAttempts);
            printf(RED "\t--------------------------------------------------\n\n" RESET);
        }
        loginAttempts--;
    }

    if (!loginSuccessful) {
        printf(RED "\n\t--------------------------------------------------\n" RESET);
        printf(RED "\t| Repeated Wrong Password, System Closed ....   |\n" RESET);
        printf(RED "\t--------------------------------------------------\n\n" RESET);
        return 0;
    }

    nullRoom(R);
    loadDataFromFile(DATA_FILE, R);

    while (1) {
        int flag0;
        printf(BLUE "\n\t---------------------------------------------------------------------------------\n" RESET);
        printf(BLUE "\t| Commands: [1] Report  [2] Allocation/Re-Allocation/Leave  [3] Edit  [0] Exit |\n" RESET);
        printf(BLUE "\t|                                                                              |\n" RESET);
        printf(BLUE "\t---------------------------------------------------------------------------------\n" RESET);
        printf(YELLOW "\n\tEnter the command: " RESET);
        if (scanf("%d", &flag0) != 1) {
            printf(RED "\n\tInvalid input. Please enter a number.\n" RESET);
            clearInputBuffer();
            continue;
        }
        printf("\n");

        if (flag0 == 0) {
            pressEnterToContinue();
            break;
        } else if (flag0 == 1) {
            while (1) {
                int flag01;
                printf(BLUE "\n\t--------------------------------------------------\n" RESET);
                printf(BLUE "\t|                 * REPORT MENU * |\n" RESET);
                printf(BLUE "\t|------------------------------------------------|\n" RESET);
                printf(BLUE "\t| [1] All Occupied Details  [2] Room Vacancy     |\n" RESET);
                printf(BLUE "\t| [3] Total Occupants       [0] Back to Main Menu|\n" RESET);
                printf(BLUE "\t--------------------------------------------------\n" RESET);
                printf(YELLOW "\n\tEnter your command: " RESET);
                if (scanf("%d", &flag01) != 1) {
                    printf(RED "\n\tInvalid input. Please enter a number.\n" RESET);
                    clearInputBuffer();
                    continue;
                }
                printf("\n");

                if (flag01 == 0) break;
                else if (flag01 == 1) allDetails(R);
                else if (flag01 == 2) findVacancy(R);
                else if (flag01 == 3) {
                    int sum = 0;
                    for (int k = 1; k < nRoom; k++) {
                        if (strcmp("NULL", R[k].guestName) != 0)
                            sum += R[k].roomType;
                    }
                    printf(CYAN "\n\t-------------------------\n" RESET);
                    printf(CYAN "\t| Total Occupants = %3d   |\n", sum);
                    printf(CYAN "\t-------------------------\n\n" RESET);
                } else {
                    printf(RED "\n\t---------------------------------\n" RESET);
                    printf(RED "\t| Incorrect command, Try Again  |\n" RESET);
                    printf(RED "\t---------------------------------\n\n" RESET);
                }
                pressEnterToContinue();
            }
        } else if (flag0 == 2) {
            while (1) {
                int flag02;
                printf(BLUE "\n\t--------------------------------------------------\n" RESET);
                printf(BLUE "\t|     * ROOM MANAGEMENT MENU * |\n" RESET);
                printf(BLUE "\t|------------------------------------------------|\n" RESET);
                printf(BLUE "\t| [1] New Allocation   [2] Re-Allocation (Move)  |\n" RESET);
                printf(BLUE "\t| [3] Guest Leaves     [0] Back to Main Menu     |\n" RESET);
                printf(BLUE "\t|               [100] Sample Fill Data           |\n" RESET);
                printf(BLUE "\t--------------------------------------------------\n" RESET);
                printf(YELLOW "\n\tEnter your command: " RESET);
                if (scanf("%d", &flag02) != 1) {
                    printf(RED "\n\tInvalid input. Please enter a number.\n" RESET);
                    clearInputBuffer();
                    continue;
                }
                printf("\n");

                if (flag02 == 0) break;
                else if (flag02 == 1) { // New Allocation
                    findVacancy(R);
                    int tRoomNo;
                    float tPay;
                    char tempStr[MAX_LEN], mobile[MOBILE_LEN];
                    int base_row = 3, label_col = 5, input_col = 30, error_row = 15, input_row = base_row + 1;
                    clearScreen();
                    printf(BLUE "\n\tNEW ROOM ALLOCATION\n\n" RESET);
                    // Display available rooms first
                    findVacancy(R);
                    printf(YELLOW "\n\tFill the details for new allocation:\n" RESET);
                    gotoxy(base_row, label_col); printf("+---------------------+--------------------------------+");
                    gotoxy(++base_row, label_col); printf("| Room Number         |                                |");
                    gotoxy(++base_row, label_col); printf("+---------------------+--------------------------------+");
                    gotoxy(++base_row, label_col); printf("| Payment (INR)       |                                |");
                    gotoxy(++base_row, label_col); printf("+---------------------+--------------------------------+");
                    gotoxy(++base_row, label_col); printf("| Guest Name          |                                |");
                    gotoxy(++base_row, label_col); printf("+---------------------+--------------------------------+");
                    gotoxy(++base_row, label_col); printf("| Mobile Number       |                                |");
                    gotoxy(++base_row, label_col); printf("+---------------------+--------------------------------+");
                    gotoxy(++base_row, label_col); printf("| Address             |                                |");
                    gotoxy(++base_row, label_col); printf("+---------------------+--------------------------------+");

                    // Room Number
                    do {
                        clearLine(error_row, label_col, 70); // Clear previous error
                        clearLine(input_row, input_col, MAX_LEN);
                        gotoxy(input_row, input_col);
                        fgets(tempStr, MAX_LEN, stdin);
                        tempStr[strcspn(tempStr, "\n")] = '\0';
                        if (sscanf(tempStr, "%d", &tRoomNo) != 1 || !validateRoomNumber(tRoomNo) || tRoomNo == 0) {
                            clearLine(error_row, label_col, 70);
                            gotoxy(error_row, label_col); printf(RED "Invalid Room Number or Room 000 not allowed." RESET);
                            continue;
                        }
                        if (strcmp(R[tRoomNo].guestName, "NULL")) {
                            clearLine(error_row, label_col, 70);
                            gotoxy(error_row, label_col); printf(RED "Room %d is already occupied." RESET, tRoomNo);
                            continue;
                        }
                        clearLine(error_row, label_col, 70);
                        break;
                    } while (1);
                    input_row += 2;

                    // Payment
                    do {
                        clearLine(error_row, label_col, 70); // Clear previous error
                        clearLine(input_row, input_col, MAX_LEN);
                        gotoxy(input_row, input_col);
                        fgets(tempStr, MAX_LEN, stdin);
                        tempStr[strcspn(tempStr, "\n")] = '\0';
                        if (!isValidFloat(tempStr, &tPay) || tPay != R[tRoomNo].guestRent) {
                            clearLine(error_row, label_col, 70);
                            gotoxy(error_row, label_col); printf(RED "Payment must be exactly %.2f INR." RESET, R[tRoomNo].guestRent);
                            continue;
                        }
                        clearLine(error_row, label_col, 70);
                        break;
                    } while (1);
                    input_row += 2;

                    // Guest Name
                    gotoxy(input_row, input_col);
                    fgets(R[tRoomNo].guestName, sizeof(R[tRoomNo].guestName), stdin);
                    R[tRoomNo].guestName[strcspn(R[tRoomNo].guestName, "\n")] = '\0';
                    input_row += 2;

                    // Mobile Number
                    do {
                        clearLine(error_row, label_col, 70); // Clear previous error
                        clearLine(input_row, input_col, MOBILE_LEN);
                        gotoxy(input_row, input_col);
                        fgets(mobile, MOBILE_LEN, stdin);
                        mobile[strcspn(mobile, "\n")] = '\0';
                        if (!isValidMobile(mobile)) {
                            clearLine(error_row, label_col, 70);
                            gotoxy(error_row, label_col); printf(RED "Invalid mobile number (10 digits required)." RESET);
                            continue;
                        }
                        strcpy(R[tRoomNo].guestMobileNumber, mobile);
                        clearLine(error_row, label_col, 70);
                        break;
                    } while (1);
                    input_row += 2;

                    // Address
                    gotoxy(input_row, input_col);
                    fgets(R[tRoomNo].guestAddress, sizeof(R[tRoomNo].guestAddress), stdin);
                    R[tRoomNo].guestAddress[strcspn(R[tRoomNo].guestAddress, "\n")] = '\0';

                    // Set check-in date
                    time_t now = time(NULL);
                    struct tm *tm = localtime(&now);
                    R[tRoomNo].date.day = tm->tm_mday;
                    R[tRoomNo].date.month = tm->tm_mon + 1;
                    R[tRoomNo].date.year = tm->tm_year + 1900;
                    R[tRoomNo].balance = R[tRoomNo].guestRent - tPay;

                    clearLine(error_row, label_col, 70); // Clear any previous error
                    gotoxy(error_row, label_col); printf(GREEN "Room allocated. Check-in: %02d-%02d-%04d. Balance: %.2f" RESET, 
                                                         R[tRoomNo].date.day, R[tRoomNo].date.month, R[tRoomNo].date.year);
                    printf(GREEN "\n\t-----------------------------------------\n" RESET);
                    printf(GREEN "\t| Room %03d Successfully Allocated!      |\n" RESET, tRoomNo);
                    printf(GREEN "\t-----------------------------------------\n\n" RESET);
                    pressEnterToContinue();
                } else if (flag02 == 2) { // Re-Allocation
                    int tRoomNo1, tRoomNo2;
                    float tPay;
                    char tempStr[MAX_LEN];
                    // Display available rooms first, before drawing the form for re-allocation
                    clearScreen();
                    printf(YELLOW "\n\tDisplaying currently available rooms:\n" RESET);
                    findVacancy(R);
                    printf(YELLOW "\n\tNote down the desired new room number for re-allocation.\n" RESET);
                    pressEnterToContinue();

                    clearScreen();
                    printf(BLUE "\n\tROOM RE-ALLOCATION\n" RESET);
                    int base_row = 3, label_col = 5, input_col = 30, error_row = 11, input_row = base_row + 1;
                    gotoxy(base_row, label_col); printf("+---------------------+--------------------------------+");
                    gotoxy(++base_row, label_col); printf("| Current Room No.    |                                |");
                    gotoxy(++base_row, label_col); printf("+---------------------+--------------------------------+");
                    gotoxy(++base_row, label_col); printf("| New Room No.        |                                |");
                    gotoxy(++base_row, label_col); printf("+---------------------+--------------------------------+");
                    gotoxy(++base_row, label_col); printf("| Payment (INR)       |                                |");
                    gotoxy(++base_row, label_col); printf("+---------------------+--------------------------------+");

                    // Clear input buffer before reading the first input
                    clearInputBuffer();

                    // Current Room Number
                    do {
                        clearLine(error_row, label_col, 70); // Clear previous error
                        clearLine(input_row, input_col, MAX_LEN);
                        gotoxy(input_row, input_col);
                        fgets(tempStr, MAX_LEN, stdin);
                        tempStr[strcspn(tempStr, "\n")] = '\0';
                        if (sscanf(tempStr, "%d", &tRoomNo1) != 1 || !validateRoomNumber(tRoomNo1) || tRoomNo1 == 0) {
                            clearLine(error_row, label_col, 70);
                            gotoxy(error_row, label_col); 
                            printf(RED "Invalid Current Room Number." RESET);
                            continue;
                        }
                        if (!strcmp(R[tRoomNo1].guestName, "NULL")) {
                            clearLine(error_row, label_col, 70);
                            gotoxy(error_row, label_col); printf(RED "Room %d is not occupied." RESET, tRoomNo1);
                            continue;
                        }
                        clearLine(error_row, label_col, 70);
                        break;
                    } while (1);
                    input_row += 2;

                    // New Room Number
                    do {
                        clearLine(error_row, label_col, 70); // Clear previous error
                        clearLine(input_row, input_col, MAX_LEN);
                        gotoxy(input_row, input_col);
                        fgets(tempStr, MAX_LEN, stdin);
                        tempStr[strcspn(tempStr, "\n")] = '\0';
                        if (sscanf(tempStr, "%d", &tRoomNo2) != 1 || !validateRoomNumber(tRoomNo2) || tRoomNo2 == 0) {
                            clearLine(error_row, label_col, 70);
                            gotoxy(error_row, label_col); printf(RED "Invalid New Room Number." RESET);
                            continue;
                        }
                        if (strcmp(R[tRoomNo2].guestName, "NULL")) {
                            clearLine(error_row, label_col, 70);
                            gotoxy(error_row, label_col); printf(RED "New Room %d is already occupied." RESET, tRoomNo2);
                            continue;
                        }
                        if (tRoomNo1 == tRoomNo2) {
                            clearLine(error_row, label_col, 70);
                            gotoxy(error_row, label_col); printf(RED "Current and new room cannot be the same." RESET);
                            continue;
                        }
                        clearLine(error_row, label_col, 70);
                        break;
                    } while (1);
                    input_row += 2;

                    // Payment
                    do {
                        clearLine(error_row, label_col, 70); // Clear previous error
                        clearLine(input_row, input_col, MAX_LEN);
                        gotoxy(input_row, input_col);
                        fgets(tempStr, MAX_LEN, stdin);
                        tempStr[strcspn(tempStr, "\n")] = '\0';
                        if (!isValidFloat(tempStr, &tPay) || tPay != R[tRoomNo2].guestRent) {
                            clearLine(error_row, label_col, 70);
                            gotoxy(error_row, label_col); printf(RED "Payment must be exactly %.2f INR." RESET, R[tRoomNo2].guestRent);
                            continue;
                        }
                        clearLine(error_row, label_col, 70);
                        break;
                    } while (1);

                    // Transfer details
                    strcpy(R[tRoomNo2].guestName, R[tRoomNo1].guestName);
                    strcpy(R[tRoomNo2].guestMobileNumber, R[tRoomNo1].guestMobileNumber);
                    strcpy(R[tRoomNo2].guestAddress, R[tRoomNo1].guestAddress);
                    strcpy(R[tRoomNo2].guestUsername, R[tRoomNo1].guestUsername);
                    strcpy(R[tRoomNo2].guestPassword, R[tRoomNo1].guestPassword);
                    time_t now = time(NULL);
                    struct tm *tm = localtime(&now);
                    R[tRoomNo2].date.day = tm->tm_mday;
                    R[tRoomNo2].date.month = tm->tm_mon + 1;
                    R[tRoomNo2].date.year = tm->tm_year + 1900;
                    R[tRoomNo2].balance = R[tRoomNo2].guestRent - tPay;
                    reNullRoom(tRoomNo1);

                    clearLine(error_row, label_col, 70); // Clear any previous error
                    gotoxy(error_row, label_col); printf(GREEN "Room re-allocated. New Check-in: %02d-%02d-%04d" RESET, 
                                                         R[tRoomNo2].date.day, R[tRoomNo2].date.month, R[tRoomNo2].date.year);
                    printf(GREEN "\n\t-----------------------------------------\n" RESET);
                    printf(GREEN "\t| Successfully Re-Allocated to Room %03d! |\n" RESET, tRoomNo2);
                    printf(GREEN "\t-----------------------------------------\n\n" RESET);
                    pressEnterToContinue();
                } else if (flag02 == 3) { // Guest Leaves
                    int tRoomNo3;
                    char reason[MAX_LEN];
                    clearScreen();
                    printf(BLUE "\n\tGUEST LEAVES\n" RESET);
                    int base_row = 3, label_col = 5, input_col = 30, error_row = 9, input_row = base_row + 1;
                    gotoxy(base_row, label_col); printf("+---------------------+--------------------------------+");
                    gotoxy(++base_row, label_col); printf("| Room Number         |                                |");
                    gotoxy(++base_row, label_col); printf("+---------------------+--------------------------------+");
                    gotoxy(++base_row, label_col); printf("| Reason for Leaving  |                                |");
                    gotoxy(++base_row, label_col); printf("+---------------------+--------------------------------+");

                    // Room Number
                    do {
                        clearLine(error_row, label_col, 70); // Clear previous error
                        clearLine(input_row, input_col, MAX_LEN);
                        gotoxy(input_row, input_col);
                        fgets(reason, MAX_LEN, stdin);
                        reason[strcspn(reason, "\n")] = '\0';
                        if (sscanf(reason, "%d", &tRoomNo3) != 1 || !validateRoomNumber(tRoomNo3) || tRoomNo3 == 0) {
                            clearLine(error_row, label_col, 70);
                            gotoxy(error_row, label_col); printf(RED "Invalid Room Number." RESET);
                            continue;
                        }
                        if (strcmp(R[tRoomNo3].guestName, "NULL") == 0) {
                            clearLine(error_row, label_col, 70);
                            gotoxy(error_row, label_col); printf(RED "Room %d is already vacant." RESET, tRoomNo3);
                            continue;
                        }
                        if (R[tRoomNo3].balance > 0) {
                            clearLine(error_row, label_col, 70);
                            gotoxy(error_row, label_col); printf(RED "Room %d has outstanding balance of %.2f INR." RESET, tRoomNo3, R[tRoomNo3].balance);
                            continue;
                        }
                        clearLine(error_row, label_col, 70);
                        break;
                    } while (1);
                    input_row += 2;

                    // Reason
                    gotoxy(input_row, input_col);
                    fgets(reason, MAX_LEN, stdin);
                    reason[strcspn(reason, "\n")] = '\0';

                    clearLine(error_row, label_col, 70); // Clear any previous error
                    reNullRoom(tRoomNo3);
                    gotoxy(error_row, label_col); printf(GREEN "Room %03d Successfully Vacated!" RESET, tRoomNo3);
                    printf(GREEN "\n\t-----------------------------------------\n" RESET);
                    printf(GREEN "\t| Room %03d Successfully Vacated!        |\n" RESET, tRoomNo3);
                    printf(GREEN "\t-----------------------------------------\n\n" RESET);
                    pressEnterToContinue();
                } else if (flag02 == 100) {
                    int n, mode;
                    printf(YELLOW "\n\tEnter how many sample student records to generate (max 100): " RESET);
                    scanf("%d", &n);
                    if (n < 1 || n > 100) {
                        printf(RED "\n\tInvalid number. Returning to main menu...\n" RESET);
                        clearInputBuffer();
                        continue;
                    }
                    printf(YELLOW "\n\tChoose mode: [1] Random Room Allocation  [2] Sequential Room Allocation: " RESET);
                    scanf("%d", &mode);
                    if (mode != 1 && mode != 2) {
                        printf(RED "\n\tInvalid mode. Returning to main menu...\n" RESET);
                        clearInputBuffer();
                        continue;
                    }
                    fillSampleData(n, mode);
                    pressEnterToContinue();
                } else {
                    printf(RED "\n\t---------------------------------------------\n" RESET);
                    printf(RED "\t| Incorrect command, Please Input Valid Command |\n" RESET);
                    printf(RED "\t---------------------------------------------\n\n" RESET);
                    pressEnterToContinue();
                }
            }
        } else if (flag0 == 3) { // Edit Menu
            int editRoomNo;
            char tempStr[MAX_LEN], mobile[MOBILE_LEN], dateStr[DATE_LEN];
            float tempFloat;
            int day, month, year;
            printf(BLUE "\n\tEDIT ROOM DETAILS\n" RESET);
            // Form ends at row 21. current_details_row = 22, error_row = 23
            int base_row = 3, label_col = 5, input_col = 30;
            int input_row = base_row + 1;
            int current_details_display_row = 22; // Row to display current details
            int error_row = 23;                   // Row for error messages, below form and details
            gotoxy(base_row, label_col); printf("+---------------------+--------------------------------+");
            gotoxy(++base_row, label_col); printf("| Room Number         |                                |");
            gotoxy(++base_row, label_col); printf("+---------------------+--------------------------------+");
            gotoxy(++base_row, label_col); printf("| Guest Name          |                                |");
            gotoxy(++base_row, label_col); printf("+---------------------+--------------------------------+");
            gotoxy(++base_row, label_col); printf("| Mobile Number       |                                |");
            gotoxy(++base_row, label_col); printf("+---------------------+--------------------------------+");
            gotoxy(++base_row, label_col); printf("| Address             |                                |");
            gotoxy(++base_row, label_col); printf("+---------------------+--------------------------------+");
            gotoxy(++base_row, label_col); printf("| Rent (INR)          |                                |");
            gotoxy(++base_row, label_col); printf("+---------------------+--------------------------------+");
            gotoxy(++base_row, label_col); printf("| Balance (INR)       |                                |");
            gotoxy(++base_row, label_col); printf("+---------------------+--------------------------------+");
            gotoxy(++base_row, label_col); printf("| Username            |                                |");
            gotoxy(++base_row, label_col); printf("+---------------------+--------------------------------+");
            gotoxy(++base_row, label_col); printf("| Password            |                                |");
            gotoxy(++base_row, label_col); printf("+---------------------+--------------------------------+");
            gotoxy(++base_row, label_col); printf("| Date (DD-MM-YYYY)   |                                |");
            gotoxy(++base_row, label_col); printf("+---------------------+--------------------------------+");

            // Room Number
            do {
                clearLine(error_row, label_col, 70); // Clear previous error
                clearLine(input_row, input_col, MAX_LEN);
                gotoxy(input_row, input_col);
                fgets(tempStr, MAX_LEN, stdin);
                tempStr[strcspn(tempStr, "\n")] = '\0';
                if (sscanf(tempStr, "%d", &editRoomNo) != 1 || !validateRoomNumber(editRoomNo) || editRoomNo == 0) {
                    clearLine(error_row, label_col, 70);
                    gotoxy(error_row, label_col); printf(RED "Invalid Room Number." RESET);
                    continue;
                }
                clearLine(error_row, label_col, 70);
                break;
            } while (1);
            input_row += 2;

            // Display current details
            clearLine(current_details_display_row, 1, 120); // Clear the line for current details
            gotoxy(current_details_display_row, label_col - 2); // Adjusted column for visibility
            printf(CYAN "Current: N:%s M:%s Ad:%s R:%.2f B:%.2f U:%s P:%s D:%02d-%02d-%04d" RESET,
                   R[editRoomNo].guestName, R[editRoomNo].guestMobileNumber, R[editRoomNo].guestAddress,
                   R[editRoomNo].guestRent, R[editRoomNo].balance, R[editRoomNo].guestUsername, R[editRoomNo].guestPassword,
                   R[editRoomNo].date.day, R[editRoomNo].date.month, R[editRoomNo].date.year);
            printf(YELLOW "\n\tEnter new details (leave blank and press Enter to keep current value):\n" RESET);

            // Guest Name
            gotoxy(input_row, input_col);
            fgets(tempStr, MAX_LEN, stdin);
            tempStr[strcspn(tempStr, "\n")] = '\0';
            clearLine(error_row, label_col, 70); // Clear error line after successful or skipped input
            if (strlen(tempStr) > 0) strcpy(R[editRoomNo].guestName, tempStr);
            input_row += 2;

            // Mobile Number
            do {
                clearLine(input_row, input_col, MOBILE_LEN);
                gotoxy(input_row, input_col);
                fgets(mobile, MOBILE_LEN, stdin);
                mobile[strcspn(mobile, "\n")] = '\0';
                if (strlen(mobile) > 0 && !isValidMobile(mobile)) { // Validate only if input is given
                    clearLine(error_row, label_col, 70);
                    gotoxy(error_row, label_col); printf(RED "Invalid mobile number (10 digits required)." RESET);
                    continue;
                }
                if (strlen(mobile) > 0) strcpy(R[editRoomNo].guestMobileNumber, mobile);
                clearLine(error_row, label_col, 70);
                break;
            } while (1);
            input_row += 2;

            // Address
            gotoxy(input_row, input_col);
            fgets(tempStr, MAX_LEN, stdin);
            tempStr[strcspn(tempStr, "\n")] = '\0';
            clearLine(error_row, label_col, 70);
            if (strlen(tempStr) > 0) strcpy(R[editRoomNo].guestAddress, tempStr);
            input_row += 2;

            // Rent
            do {
                clearLine(input_row, input_col, MAX_LEN);
                gotoxy(input_row, input_col);
                fgets(tempStr, MAX_LEN, stdin);
                tempStr[strcspn(tempStr, "\n")] = '\0';
                if (strlen(tempStr) > 0 && !isValidFloat(tempStr, &tempFloat)) { // Validate only if input is given
                    clearLine(error_row, label_col, 70);
                    gotoxy(error_row, label_col); printf(RED "Invalid rent amount." RESET);
                    continue;
                }
                if (strlen(tempStr) > 0) R[editRoomNo].guestRent = tempFloat;
                clearLine(error_row, label_col, 70);
                break;
            } while (1);
            input_row += 2;

            // Balance
            do {
                clearLine(input_row, input_col, MAX_LEN);
                gotoxy(input_row, input_col);
                fgets(tempStr, MAX_LEN, stdin);
                tempStr[strcspn(tempStr, "\n")] = '\0';
                if (strlen(tempStr) > 0 && !isValidFloat(tempStr, &tempFloat)) { // Validate only if input is given
                    clearLine(error_row, label_col, 70);
                    gotoxy(error_row, label_col); printf(RED "Invalid balance amount." RESET);
                    continue;
                }
                if (strlen(tempStr) > 0) R[editRoomNo].balance = tempFloat;
                clearLine(error_row, label_col, 70);
                break;
            } while (1);
            input_row += 2;

            // Username
            gotoxy(input_row, input_col);
            fgets(tempStr, MAX_LEN, stdin);
            tempStr[strcspn(tempStr, "\n")] = '\0';
            clearLine(error_row, label_col, 70);
            if (strlen(tempStr) > 0) strcpy(R[editRoomNo].guestUsername, tempStr);
            input_row += 2;

            // Password
            gotoxy(input_row, input_col);
            fgets(tempStr, MAX_LEN, stdin);
            tempStr[strcspn(tempStr, "\n")] = '\0';
            clearLine(error_row, label_col, 70);
            if (strlen(tempStr) > 0) strcpy(R[editRoomNo].guestPassword, tempStr);
            input_row += 2;

            // Date
            do {
                clearLine(input_row, input_col, DATE_LEN);
                gotoxy(input_row, input_col);
                fgets(dateStr, DATE_LEN, stdin);
                dateStr[strcspn(dateStr, "\n")] = '\0';
                if (strlen(dateStr) > 0 && !isValidDate(dateStr, &day, &month, &year)) { // Validate only if input is given
                    clearLine(error_row, label_col, 70);
                    gotoxy(error_row, label_col); printf(RED "Invalid date (use DD-MM-YYYY, e.g., 01-01-2020)." RESET);
                    continue;
                }
                if (strlen(dateStr) > 0) {
                    R[editRoomNo].date.day = day;
                    R[editRoomNo].date.month = month;
                    R[editRoomNo].date.year = year;
                }
                clearLine(error_row, label_col, 70);
                break;
            } while (1);

            clearLine(error_row, label_col, 70); // Clear any final error message
            gotoxy(error_row, label_col); printf(GREEN "Room %03d Details Updated Successfully!" RESET, editRoomNo);
            printf(GREEN "\n\t--------------------------------------------------\n" RESET);
            printf(GREEN "\t| Room %03d Details Updated Successfully!        |\n" RESET, editRoomNo);
            printf(GREEN "\t--------------------------------------------------\n\n" RESET);
            pressEnterToContinue();
        } else {
            printf(RED "\n\t---------------------------------\n" RESET);
            printf(RED "\t| Incorrect command, Input Valid Command Please |\n" RESET);
            printf(RED "\t---------------------------------\n\n" RESET);
            pressEnterToContinue();
        }
        clearInputBuffer();
    }

    saveDataToFile(DATA_FILE, R);
    printf(YELLOW "\n\t--------------------------------------------------\n" RESET);
    printf(YELLOW "\t| Terminating program with Saving Data. Cleaning data !         |\n" RESET);
    const char* t1 = "\t* * * * * * * * * * * * * * * * * * * * * \n";
    printf(BLUE);
    slowPrint(t1, 500000);
    printf(RESET);
    printf(YELLOW "\t--------------------------------------------------\n\n" RESET);

return 0;}