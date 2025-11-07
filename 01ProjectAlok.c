#include <stdio.h>    // For standard input/output functions (printf, scanf, fopen, etc.)
#include <stdlib.h>   // For general utility functions (exit, system, etc.)
#include <string.h>   // For string manipulation functions (strcpy, strcmp, strlen, etc.)
#include <ctype.h>    // For character type functions (isdigit, tolower, toupper, etc.)

// Constants for file names
#define FILENAME "students.txt"
#define TEMP_FILENAME "temp_students.txt"

// Constants for admin credentials (Hardcoded for simplicity in this example)
#define USERNAME "a"
#define PASSWORD "a"

// Max lengths for string fields to improve readability, maintainability, and prevent buffer overflows
#define NAME_LEN 50
#define MOTHER_LEN 50
#define FATHER_LEN 50
#define MOBILE_LEN 15       // Max length for mobile number (e.g., +91 and 10 digits)
#define PERCENT_LEN 10      // Max length for percentage string (e.g., "99.99")
#define DOMICILE_LEN 30
#define COURSE_LEN 50
#define DOB_LEN 20          // Max length for Date of Birth string (e.g., "DD/MM/YYYY")
#define MAX_LINE_LEN 512    // Buffer size for reading lines from the student data file

// Structure to hold student form data
typedef struct {
    char name[NAME_LEN];
    char mother[MOTHER_LEN];
    char father[FATHER_LEN];
    char mobile[MOBILE_LEN];
    char percent[PERCENT_LEN]; // Store as string for easier input and initial validation
    char domicile[DOMICILE_LEN];
    char course[COURSE_LEN];
    char dob[DOB_LEN];
    float totalFee, discount, domicileDiscount, finalFee;  
    /*    
    float totalFee;         // Calculated total fee for the course
    float discount;         // Calculated discount based on percentage
    float domicileDiscount; // Calculated discount based on domicile
    float finalFee;         // Final fee after all discounts
    */     
} StudentForm;

// Function Prototypes
void mainMenu();
void login();
void adminMenu();
void studentRegistration();
void displayStudents();
void searchStudent();
void modifyStudent();
void deleteStudent();

float getTotalFee(char course[]);
float getPercentDiscount(float percent);
float getDomicileDiscount(char dom[]);

int isValidMobile(const char* mobile);
int isValidPercentage(const char* percentStr, float* percentage); // Validates and converts percentage string
int parseStudentLine(const char* line, StudentForm* s); // Helper to parse a line from the student file

void clearInputBuffer();
void gotoxy(int row, int col);
void clearScreen();
void clearLine(int row, int startCol, int length);
void str_to_lower(char* str);

// Main function - entry point of the program
int main() {
    mainMenu(); // Navigate to the main menu
    return 0;   // Indicate successful execution
}

// Clears the standard input buffer.
// This is useful after scanf to consume any leftover characters, especially newline.
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Moves the console cursor to a specified row and column.
// Uses ANSI escape codes, common on Unix-like systems and modern Windows terminals.
void gotoxy(int row, int col) {
    printf("\033[%d;%dH", row, col);
    fflush(stdout); // Crucial for immediate cursor movement, especially on some systems
}

// Clears the console screen.
// Uses ANSI escape codes: \033[2J clears the entire screen, \033[H moves cursor to home (1,1).
void clearScreen() {
    printf("\033[2J\033[H");
    fflush(stdout); // Ensure the clear screen command is sent and processed immediately
}

// Clears a specific line (or part of it) on the console by printing spaces.
void clearLine(int row, int startCol, int length) {
    gotoxy(row, startCol); // Move cursor to the beginning of the area to clear
    for (int i = 0; i < length; i++) {
        printf(" "); // Print spaces to overwrite existing characters
    }
    fflush(stdout); // Ensure clearing is immediately visible
}

// Pauses execution and waits for the user to press Enter.
void pressEnterToContinue() {
    printf("\nPress Enter to continue...");
    clearInputBuffer(); // Consume the Enter key press
}

// Converts a given string to lowercase.
// Useful for case-insensitive comparisons.
void str_to_lower(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]); // Cast to unsigned char for safety with ctype functions
    }
}

// Displays the main menu and handles user navigation.
void mainMenu() {
    int choice;
    while (1) { // Loop indefinitely until user chooses to exit
        clearScreen();
        printf("=========================================\n");
        printf("         ADMISSION MANAGEMENT SYSTEM\n");
        printf("=========================================\n\n");
        
        printf("1. Student Registration\n");
        printf("2. Admin Login\n");
        printf("3. Exit\n\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) { // Check if input is a valid integer
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer(); // Clear the invalid input
             
            continue; // Go to the next iteration of the loop
        }
        clearInputBuffer(); // Clear the newline character left by scanf

        switch (choice) {
            case 1:
                studentRegistration();
                break;
            case 2:
                login();
                break;
            case 3:
                printf("Exiting system. Goodbye!\n");
                exit(0); // Terminate the program
            default:
                printf("Invalid choice. Please enter 1, 2, or 3.\n");
                 
        }
    }
}

// Handles the admin login process.
void login() {
    char user[NAME_LEN], pass[NAME_LEN];
    clearScreen();

    printf("==================\n");
    printf("   ADMIN LOGIN\n");
    printf("==================\n\n");

    printf("Username: ");
    fgets(user, sizeof(user), stdin);
    user[strcspn(user, "\n")] = 0; // Remove trailing newline from fgets

    printf("Password: ");
    fgets(pass, sizeof(pass), stdin);
    pass[strcspn(pass, "\n")] = 0; // Remove trailing newline

    // Compare entered credentials with predefined admin credentials
    if (strcmp(user, USERNAME) == 0 && strcmp(pass, PASSWORD) == 0) {
        printf("\nLogin successful!\n");
         
        adminMenu(); // Proceed to admin menu
    } else {
        printf("\nWrong username or password. Access denied.\n");
         
    }
}

// Displays the admin menu and handles admin actions.
void adminMenu() {
    int choice;
    do {
        clearScreen();
        printf("==================\n");
        printf("    ADMIN MENU\n");
        printf("==================\n\n");
        printf("1. Display All Students\n");
        printf("2. Modify Student Record\n");
        printf("3. Delete Student Record\n");
        printf("4. Search Student Record\n");
        printf("5. Logout\n\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
             
            continue;
        }
        clearInputBuffer(); // Clear newline

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: modifyStudent(); break;
            case 3: deleteStudent(); break;
            case 4: searchStudent(); break;
            case 5:
                printf("Logging out...\n");
                //   // Optional: allow user to see logout message
                return; // Return to the main menu
            default:
                printf("Invalid choice. Please enter a number between 1 and 5.\n");
                 
        }
    } while (1); // Loop until admin chooses to logout
}

// Calculates the total fee based on the course name.
// Course comparison is case-insensitive.
float getTotalFee(char course[]) {
    char lowerCourse[COURSE_LEN];
    strncpy(lowerCourse, course, sizeof(lowerCourse) - 1); // Copy course name
    lowerCourse[sizeof(lowerCourse) - 1] = '\0'; // Ensure null termination
    str_to_lower(lowerCourse); // Convert to lowercase for case-insensitive comparison

    if (strcmp(lowerCourse, "btech") == 0) return 1200000.0f;
    else if (strcmp(lowerCourse, "bca") == 0) return 800000.0f;
    else if (strcmp(lowerCourse, "bsc") == 0) return 400000.0f;
    return 0.0f; // Default fee for an unknown or invalid course
}


// Calculates percentage-based discount.
float getPercentDiscount(float percent) {
    if (percent > 95.0f) return 30.0f;  // 30% discount for >95%
    else if (percent > 85.0f) return 20.0f;  // 20% discount for >85%
    else if (percent > 75.0f) return 10.0f;  // 10% discount for >75%
    return 0.0f; // No discount otherwise
}

// Calculates domicile-based discount.
// Domicile comparison is case-insensitive.
float getDomicileDiscount(char dom[]) {
    char lowerDom[DOMICILE_LEN];
    strncpy(lowerDom, dom, sizeof(lowerDom) - 1); // Copy domicile string
    lowerDom[sizeof(lowerDom) - 1] = '\0'; // Ensure null termination
    str_to_lower(lowerDom); // Convert to lowercase

    if (strcmp(lowerDom, "uttarakhand") == 0) return 25.0f; // 25% discount for Uttarakhand domicile
    return 0.0f; // No discount for other domiciles
}

// Validates a mobile number string.
// Checks if it's exactly 10 digits and all characters are numeric.
int isValidMobile(const char* mobile) {
    if (strlen(mobile) != 10) {
        return 0; // Invalid if not 10 digits
    }
    for (int i = 0; i < 10; i++) {
        if (!isdigit((unsigned char)mobile[i])) {
            return 0; // Invalid if any character is not a digit
        }
    }
    return 1; // Valid
}

// Validates a percentage string.
// Checks if it can be converted to a float and is within the 0-100 range.
// Outputs the converted float value via the 'percentage' pointer.
int isValidPercentage(const char* percentStr, float* percentage) {
    char *endptr;
    *percentage = strtof(percentStr, &endptr); // Convert string to float

    // Check if conversion was successful and the entire string was consumed
    if (endptr == percentStr || *endptr != '\0') {
        return 0; // Invalid format (e.g., "abc", "75test")
    }
    // Check if the percentage is within the valid range
    if (*percentage < 0.0f || *percentage > 100.0f) {
        return 0; // Out of range
    }
    return 1; // Valid
}

// Parses a single line from the student data file into a StudentForm struct.
// The file is expected to be pipe-delimited (|).
// Format: Name|Mother|Father|Mobile|Percent|Domicile|Course|DOB|TotalFee|Discount|DomicileDiscount|FinalFee
// Returns 1 on successful parsing of all 12 fields, 0 otherwise.
int parseStudentLine(const char* line, StudentForm* s) {
    // %[^|] reads characters until a '|' is encountered.
    // The number before [^|] (e.g., %49[^|]) limits the number of characters read to prevent buffer overflow.
    return sscanf(line, "%49[^|]|%49[^|]|%49[^|]|%14[^|]|%9[^|]|%29[^|]|%49[^|]|%19[^|]|%f|%f|%f|%f",
                  s->name, s->mother, s->father, s->mobile, s->percent, s->domicile,
                  s->course, s->dob, &s->totalFee, &s->discount, &s->domicileDiscount, &s->finalFee) == 12;
}

// Handles the student registration process.
void studentRegistration() {
    StudentForm s; // Structure to hold the new student's data
    FILE *fp = fopen(FILENAME, "a"); // Open file in append mode
    if (!fp) {
        printf("Error: Could not open file '%s' for writing.\n", FILENAME);
        perror("Reason"); // Print system error message
         
        return;
    }
    clearScreen();
    printf("    ############## STUDENT REGISTRATION FORM #############\n\n");

    // --- Form Layout using gotoxy (for basic TUI) ---
    // This section defines the visual layout of the form on the console.
    int base_row = 3; // Starting row for the form fields
    int label_col = 5;
    int input_col = 30;
    int current_row = base_row;

    gotoxy(current_row, label_col);     printf("+---------------------+------------------------------+");
    gotoxy(++current_row, label_col);   printf("| Name                |                              |");
    gotoxy(++current_row, label_col);   printf("+---------------------+------------------------------+");
    gotoxy(++current_row, label_col);   printf("| Mother's Name       |                              |");
    gotoxy(++current_row, label_col);   printf("+---------------------+------------------------------+");
    gotoxy(++current_row, label_col);   printf("| Father's Name       |                              |");
    gotoxy(++current_row, label_col);   printf("+---------------------+------------------------------+");
    gotoxy(++current_row, label_col);   printf("| Mobile Number       |                              |"); // 10 digits
    gotoxy(++current_row, label_col);   printf("+---------------------+------------------------------+");
    gotoxy(++current_row, label_col);   printf("| 12th Percentage     |                              |"); // 0-100
    gotoxy(++current_row, label_col);   printf("+---------------------+------------------------------+");
    gotoxy(++current_row, label_col);   printf("| Domicile            |                              |");
    gotoxy(++current_row, label_col);   printf("+---------------------+------------------------------+");
    gotoxy(++current_row, label_col);   printf("| Course              |                              |"); // BTech, BCA, BSc
    gotoxy(++current_row, label_col);   printf("+---------------------+------------------------------+");
    gotoxy(++current_row, label_col);   printf("| DOB (DD/MM/YYYY)    |                              |");
    gotoxy(++current_row, label_col);   printf("+---------------------+------------------------------+");

    int input_field_row = base_row + 1; // Row for the first input field
    int error_message_row = current_row + 2; // Row for displaying error messages

    // --- Get Student Details ---

    // Name
    gotoxy(input_field_row, input_col); fgets(s.name, sizeof(s.name), stdin); s.name[strcspn(s.name, "\n")] = 0;
    input_field_row += 2;

    // Mother's Name
    gotoxy(input_field_row, input_col); fgets(s.mother, sizeof(s.mother), stdin); s.mother[strcspn(s.mother, "\n")] = 0;
    input_field_row += 2;

    // Father's Name
    gotoxy(input_field_row, input_col); fgets(s.father, sizeof(s.father), stdin); s.father[strcspn(s.father, "\n")] = 0;
    input_field_row += 2;

    // Mobile Number (with validation)
    do {
        clearLine(input_field_row, input_col, NAME_LEN -1); // Clear previous input area
        gotoxy(input_field_row, input_col);
        fgets(s.mobile, sizeof(s.mobile), stdin);
        s.mobile[strcspn(s.mobile, "\n")] = 0;
        if (!isValidMobile(s.mobile)) {
            clearLine(error_message_row, label_col, 70); // Clear previous error message
            gotoxy(error_message_row, label_col); printf("Invalid mobile number. Please enter 10 digits only.");
             
            clearLine(error_message_row, label_col, 70); // Clear the error message after acknowledgment
        } else {
            clearLine(error_message_row, label_col, 70); // Clear any lingering error message
            break; // Exit loop if valid
        }
    } while (1);
    input_field_row += 2;

    // 12th Percentage (with validation)
    float perc_val; // To store the validated float value of percentage
    do {
        clearLine(input_field_row, input_col, PERCENT_LEN -1);
        gotoxy(input_field_row, input_col);
        fgets(s.percent, sizeof(s.percent), stdin);
        s.percent[strcspn(s.percent, "\n")] = 0;
        if (!isValidPercentage(s.percent, &perc_val)) {
            clearLine(error_message_row, label_col, 70);
            gotoxy(error_message_row, label_col); printf("Invalid percentage. Please enter a number between 0 and 100.");
             
            clearLine(error_message_row, label_col, 70);
        } else {
            clearLine(error_message_row, label_col, 70);
            break; // Exit loop if valid
        }
    } while (1);
    input_field_row += 2;

    // Domicile
    gotoxy(input_field_row, input_col); fgets(s.domicile, sizeof(s.domicile), stdin); s.domicile[strcspn(s.domicile, "\n")] = 0;
    input_field_row += 2;

    // Course (with validation for fee calculation)
    do {
        clearLine(input_field_row, input_col, COURSE_LEN -1);
        gotoxy(input_field_row, input_col);
        fgets(s.course, sizeof(s.course), stdin);
        s.course[strcspn(s.course, "\n")] = 0;
        s.totalFee = getTotalFee(s.course); // Calculate fee based on course
        if (s.totalFee == 0.0f) { // Check if course was valid (fee would be non-zero)
            clearLine(error_message_row, label_col, 70);
            gotoxy(error_message_row, label_col); printf("Invalid course. Please enter BTech, BCA, or BSc.");
             
            clearLine(error_message_row, label_col, 70);
        } else {
            clearLine(error_message_row, label_col, 70);
            break; // Exit loop if valid
        }
    } while (1);
    input_field_row += 2;

    // Date of Birth (DOB)
    gotoxy(input_field_row, input_col); fgets(s.dob, sizeof(s.dob), stdin); s.dob[strcspn(s.dob, "\n")] = 0;

    // --- Calculate Fees ---
    s.discount = s.totalFee * (getPercentDiscount(perc_val) / 100.0f);
    s.domicileDiscount = (s.totalFee - s.discount) * (getDomicileDiscount(s.domicile) / 100.0f);
    s.finalFee = s.totalFee - s.discount - s.domicileDiscount;

    // --- Display Fee Details (Optional) ---
    char ch;
    gotoxy(error_message_row + 1, label_col); printf("Do you want to view fee details? (y/n): ");
    ch = getchar();
    clearInputBuffer(); // Consume newline after getchar

    if (tolower((unsigned char)ch) == 'y') {
        clearScreen();
        printf("========================\n");
        printf("      FEE DETAILS\n");
        printf("========================\n\n");
        printf("+--------------------------+------------------------+\n");
        printf("| %-24s | %-20.2f Rs  |\n", "Total Course Fee", s.totalFee);
        printf("+--------------------------+------------------------+\n");
        printf("| %-24s | %-20.2f Rs  |\n", "12th Percent. Discount", s.discount);
        printf("+--------------------------+------------------------+\n");
        printf("| %-24s | %-20.2f Rs  |\n", "Domicile Discount", s.domicileDiscount);
        printf("+--------------------------+------------------------+\n");
        printf("| %-24s | %-20.2f Rs  |\n", "Final Fee (Full Course)", s.finalFee);
        printf("+--------------------------+------------------------+\n");
        printf("| %-24s | %-20.2f Rs  |\n", "Fee per Year (Approx)", s.finalFee / 4.0f); // Assuming 4-year course
        printf("+--------------------------+------------------------+\n\n");
         
    }

    // --- Save Student Record to File ---
    fprintf(fp, "%s|%s|%s|%s|%s|%s|%s|%s|%.2f|%.2f|%.2f|%.2f\n",
            s.name, s.mother, s.father, s.mobile, s.percent, s.domicile, s.course, s.dob,
            s.totalFee, s.discount, s.domicileDiscount, s.finalFee);

    fclose(fp); // Close the file
    gotoxy(error_message_row + 3, label_col); printf("Student registered successfully!\n");
     
}

// Displays all student records from the file.
void displayStudents() {
    FILE *fp = fopen(FILENAME, "r"); // Open file in read mode
    if (!fp) {
        printf("No student records found. The file '%s' may not exist or is empty.\n", FILENAME);
         
        return;
    }

    clearScreen();
    printf("====================================================================================================================================\n");
    printf("| %-20s | %-15s | %-15s | %-12s | %-10s | %-15s | %-10s | %-15s |\n", "Name", "Mother's Name", "Father's Name", "Mobile", "12th %", "Domicile", "Course", "Final Fee");
    printf("====================================================================================================================================\n");

    StudentForm s;
    char line[MAX_LINE_LEN];
    int recordsFound = 0;

    while (fgets(line, sizeof(line), fp) != NULL) { // Read file line by line
        line[strcspn(line, "\n")] = 0; // Remove trailing newline for parsing
        if (parseStudentLine(line, &s)) { // Parse the line into StudentForm struct
            printf("| %-20s | %-15s | %-15s | %-12s | %-10s | %-15s | %-10s | Rs %-12.2f |\n",
                   s.name, s.mother, s.father, s.mobile, s.percent, s.domicile, s.course, s.finalFee);
            recordsFound = 1;
        }
    }

    if (!recordsFound) {
        // Adjust colspan for the "No records" message to fit the table width
        printf("| %-126s |\n", "No student records to display.");
    }

    printf("====================================================================================================================================\n");
    fclose(fp); // Close the file
     
}


// Searches for student records based on various criteria.
void searchStudent() {
    clearScreen();
    printf("========================\n");
    printf("  SEARCH STUDENT RECORD\n");
    printf("========================\n\n");

    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("No student records found to search. File '%s' is missing or empty.\n", FILENAME);
         
        return;
    }

    int choice;
    char searchTerm[NAME_LEN]; // Buffer for the search term

    printf("Search By:\n");
    printf("1. Name\n");
    printf("2. Course\n");
    printf("3. Mother's Name\n");
    printf("4. Father's Name\n");
    printf("5. Mobile Number\n");
    printf("6. Domicile\n");
    printf("Enter your choice (1-6): ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Please enter a number.\n");
        clearInputBuffer();
        fclose(fp);
         
        return;
    }
    clearInputBuffer(); // Clear newline

    if (choice < 1 || choice > 6) {
        printf("Invalid search option.\n");
        fclose(fp);
         
        return;
    }

    printf("Enter search term: ");
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0; // Remove trailing newline

    // For case-insensitive string searches, convert search term to lowercase
    char lowerSearchTerm[NAME_LEN];
    if (choice != 5) { // Mobile number search is exact match, not case-sensitive
        strncpy(lowerSearchTerm, searchTerm, sizeof(lowerSearchTerm) - 1);
        lowerSearchTerm[sizeof(lowerSearchTerm) - 1] = '\0';
        str_to_lower(lowerSearchTerm);
    }


    clearScreen();
    printf("SEARCH RESULTS FOR: '%s'\n", searchTerm);
    printf("====================================================================================================================================\n");
    printf("| %-20s | %-15s | %-15s | %-12s | %-10s | %-15s | %-10s | %-15s |\n", "Name", "Mother's Name", "Father's Name", "Mobile", "12th %", "Domicile", "Course", "Final Fee");
    printf("====================================================================================================================================\n");

    int found = 0;
    StudentForm s;
    char line[MAX_LINE_LEN];
    char compareStrLower[NAME_LEN]; // Buffer for lowercase version of student data field

    while (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\n")] = 0; // Remove newline for parsing

        if (parseStudentLine(line, &s)) {
            int match = 0; // Flag to indicate if current record matches search criteria

            switch (choice) {
                case 1: // Search by Name
                    strncpy(compareStrLower, s.name, sizeof(compareStrLower) - 1); compareStrLower[sizeof(compareStrLower)-1] = '\0'; str_to_lower(compareStrLower);
                    if (strstr(compareStrLower, lowerSearchTerm) != NULL) match = 1; // Use strstr for partial match
                    break;
                case 2: // Search by Course
                    strncpy(compareStrLower, s.course, sizeof(compareStrLower) - 1); compareStrLower[sizeof(compareStrLower)-1] = '\0'; str_to_lower(compareStrLower);
                    if (strstr(compareStrLower, lowerSearchTerm) != NULL) match = 1;
                    break;
                case 3: // Search by Mother's Name
                    strncpy(compareStrLower, s.mother, sizeof(compareStrLower) - 1); compareStrLower[sizeof(compareStrLower)-1] = '\0'; str_to_lower(compareStrLower);
                    if (strstr(compareStrLower, lowerSearchTerm) != NULL) match = 1;
                    break;
                case 4: // Search by Father's Name
                    strncpy(compareStrLower, s.father, sizeof(compareStrLower) - 1); compareStrLower[sizeof(compareStrLower)-1] = '\0'; str_to_lower(compareStrLower);
                    if (strstr(compareStrLower, lowerSearchTerm) != NULL) match = 1;
                    break;
                case 5: // Search by Mobile Number (exact match)
                    if (strcmp(searchTerm, s.mobile) == 0) match = 1;
                    break;
                case 6: // Search by Domicile
                    strncpy(compareStrLower, s.domicile, sizeof(compareStrLower) - 1); compareStrLower[sizeof(compareStrLower)-1] = '\0'; str_to_lower(compareStrLower);
                    if (strstr(compareStrLower, lowerSearchTerm) != NULL) match = 1;
                    break;
            }

            if (match) {
                 printf("| %-20s | %-15s | %-15s | %-12s | %-10s | %-15s | %-10s | Rs %-12.2f |\n",
                   s.name, s.mother, s.father, s.mobile, s.percent, s.domicile, s.course, s.finalFee);
                found = 1;
            }
        }
    }

    if (!found) {
        printf("| %-126s |\n", "No matching record found.");
    }

    printf("====================================================================================================================================\n");
    fclose(fp);
     
}

// Modifies an existing student record.
// Searches by student name (case-insensitive).
// Reads all records, writes non-matching and modified records to a temporary file,
// then replaces the original file with the temporary file.
void modifyStudent() {
    clearScreen();
    printf("========================\n");
    printf(" MODIFY STUDENT RECORD\n");
    printf("========================\n\n");

    FILE *fp_read = fopen(FILENAME, "r");
    if (!fp_read) {
        printf("No student records found to modify. File '%s' is missing or empty.\n", FILENAME);
         
        return;
    }

    FILE *fp_write = fopen(TEMP_FILENAME, "w"); // Open temporary file for writing
    if (!fp_write) {
        printf("Error: Could not create temporary file ('%s') for modification.\n", TEMP_FILENAME);
        perror("Reason");
        fclose(fp_read);
         
        return;
    }

    char searchName[NAME_LEN];
    printf("Enter the FULL NAME of the student to modify: ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = 0; // Remove newline

    // Convert search name to lowercase for case-insensitive comparison
    char lowerSearchName[NAME_LEN];
    strncpy(lowerSearchName, searchName, sizeof(lowerSearchName) - 1);
    lowerSearchName[sizeof(lowerSearchName) - 1] = '\0';
    str_to_lower(lowerSearchName);

    int found = 0;
    StudentForm s; // To hold current student data
    StudentForm original_s; // To hold original data of the student being modified for display
    char line[MAX_LINE_LEN];

    printf("\nProcessing records...\n");

    while (fgets(line, sizeof(line), fp_read) != NULL) {
        // It's important to work with a copy of 'line' for parsing if 'line' itself is needed later (e.g. for writing unparsed lines)
        // However, here we parse, and if it's the target, we modify 's', then write 's'. If not target, write original 'line'.
        char original_line_for_writing[MAX_LINE_LEN]; // Keep the original line with its newline for writing if not modified
        strcpy(original_line_for_writing, line);


        char parse_line[MAX_LINE_LEN]; // Copy of line for parsing (to strip newline safely)
        strcpy(parse_line, line);
        parse_line[strcspn(parse_line, "\n")] = 0; // Remove newline for parseStudentLine


        if (parseStudentLine(parse_line, &s)) {
            char lowerStudentName[NAME_LEN];
            strncpy(lowerStudentName, s.name, sizeof(lowerStudentName) - 1);
            lowerStudentName[sizeof(lowerStudentName) - 1] = '\0';
            str_to_lower(lowerStudentName);

            if (strcmp(lowerSearchName, lowerStudentName) == 0) { // Found the student to modify
                found = 1;
                original_s = s; // Store original data for display prompts

                printf("\n--- Student Found: %s ---\n", original_s.name);
                printf("Enter new details (leave blank and press Enter to keep current value):\n\n");

                char buffer[NAME_LEN]; // Temporary buffer for inputs

                // Helper macro for conditional update
                #define GET_MODIFIED_INPUT(prompt, current_value, target_field, max_len) \
                    printf(prompt, current_value); \
                    fgets(buffer, max_len, stdin); \
                    buffer[strcspn(buffer, "\n")] = 0; \
                    if (strlen(buffer) > 0) strcpy(target_field, buffer);

                GET_MODIFIED_INPUT("New Name (current: %s): ", original_s.name, s.name, sizeof(s.name));
                GET_MODIFIED_INPUT("New Mother's Name (current: %s): ", original_s.mother, s.mother, sizeof(s.mother));
                GET_MODIFIED_INPUT("New Father's Name (current: %s): ", original_s.father, s.father, sizeof(s.father));

                // Mobile Number (with validation)
                do {
                    printf("New Mobile Number (10-digits, current: %s): ", original_s.mobile);
                    fgets(buffer, sizeof(buffer), stdin); buffer[strcspn(buffer, "\n")] = 0;
                    if (strlen(buffer) == 0) { break; } // Keep current if blank
                    if (isValidMobile(buffer)) { strcpy(s.mobile, buffer); break; }
                    printf("Invalid mobile number. Please enter 10 digits only, or leave blank to keep current.\n");
                } while (1);

                // 12th Percentage (with validation)
                float perc_new_val;
                do {
                    printf("New 12th Percentage (0-100, current: %s): ", original_s.percent);
                    fgets(buffer, sizeof(buffer), stdin); buffer[strcspn(buffer, "\n")] = 0;
                    if (strlen(buffer) == 0) { perc_new_val = strtof(s.percent, NULL); break; } // Keep current
                    if (isValidPercentage(buffer, &perc_new_val)) { strcpy(s.percent, buffer); break; }
                    printf("Invalid percentage. Please enter a number between 0-100, or leave blank.\n");
                } while (1);


                GET_MODIFIED_INPUT("New Domicile (current: %s): ", original_s.domicile, s.domicile, sizeof(s.domicile));

                // Course (with validation for fee calculation)
                do {
                    printf("New Course (BTech, BCA, BSc, current: %s): ", original_s.course);
                    fgets(buffer, sizeof(buffer), stdin); buffer[strcspn(buffer, "\n")] = 0;
                    if (strlen(buffer) == 0) { s.totalFee = getTotalFee(s.course); break; } // Keep current
                    s.totalFee = getTotalFee(buffer);
                    if (s.totalFee != 0.0f) { strcpy(s.course, buffer); break;}
                    printf("Invalid course. Please enter BTech, BCA, or BSc, or leave blank.\n");
                } while (1);

                GET_MODIFIED_INPUT("New DOB (DD/MM/YYYY, current: %s): ", original_s.dob, s.dob, sizeof(s.dob));

                // Recalculate fees with potentially new data
                s.discount = s.totalFee * (getPercentDiscount(perc_new_val) / 100.0f);
                s.domicileDiscount = (s.totalFee - s.discount) * (getDomicileDiscount(s.domicile) / 100.0f);
                s.finalFee = s.totalFee - s.discount - s.domicileDiscount;

                // Write the modified record to the temporary file
                fprintf(fp_write, "%s|%s|%s|%s|%s|%s|%s|%s|%.2f|%.2f|%.2f|%.2f\n",
                        s.name, s.mother, s.father, s.mobile, s.percent, s.domicile, s.course, s.dob,
                        s.totalFee, s.discount, s.domicileDiscount, s.finalFee);
                printf("\nRecord updated.\n");

            } else {
                // If not the record to modify, write the original line (with its newline) to the temporary file
                fprintf(fp_write, "%s", original_line_for_writing);
            }
        } else {
            // If line cannot be parsed (e.g., corrupted data), write it as is to preserve file integrity as much as possible
            fprintf(fp_write, "%s", original_line_for_writing);
        }
    }

    fclose(fp_read);
    fclose(fp_write);

    if (found) {
        if (remove(FILENAME) != 0) { // Delete the original file
            printf("\nError: Could not delete the original file '%s'.\n", FILENAME);
            perror("Reason");
            // Attempt to remove temp file to clean up
            remove(TEMP_FILENAME);
        } else {
            if (rename(TEMP_FILENAME, FILENAME) == 0) { // Rename the temporary file to the original filename
                printf("\nStudent record modified successfully!\n");
            } else {
                printf("\nError: Could not rename temporary file '%s' to '%s'.\n", TEMP_FILENAME, FILENAME);
                perror("Reason");
                printf("The modified data might be in '%s'. Manual intervention may be needed.\n", TEMP_FILENAME);
            }
        }
    } else {
        remove(TEMP_FILENAME); // Delete the temporary file if no record was found/modified
        printf("\nNo student found with the name '%s' to modify.\n", searchName);
    }
     
}


// Deletes a student record from the file.
// Searches by student name (case-insensitive).
// Writes all records except the one to be deleted to a temporary file,
// then replaces the original file with the temporary file.
void deleteStudent() {
    clearScreen();
    printf("========================\n");
    printf(" DELETE STUDENT RECORD\n");
    printf("========================\n\n");

    FILE *fp_read = fopen(FILENAME, "r");
    if (!fp_read) {
        printf("No student records found to delete. File '%s' is missing or empty.\n", FILENAME);
         
        return;
    }

    FILE *fp_write = fopen(TEMP_FILENAME, "w"); // Open temporary file for writing
    if (!fp_write) {
        printf("Error: Could not create temporary file ('%s') for deletion.\n", TEMP_FILENAME);
        perror("Reason");
        fclose(fp_read);
         
        return;
    }

    char deleteName[NAME_LEN];
    printf("Enter the FULL NAME of the student to delete: ");
    fgets(deleteName, sizeof(deleteName), stdin);
    deleteName[strcspn(deleteName, "\n")] = 0; // Remove newline

    // Convert delete name to lowercase for case-insensitive comparison
    char lowerDeleteName[NAME_LEN];
    strncpy(lowerDeleteName, deleteName, sizeof(lowerDeleteName) - 1);
    lowerDeleteName[sizeof(lowerDeleteName) - 1] = '\0';
    str_to_lower(lowerDeleteName);

    int found = 0;
    StudentForm s;
    char line[MAX_LINE_LEN];

    printf("\nProcessing records...\n");

    while (fgets(line, sizeof(line), fp_read) != NULL) {
        char original_line_for_writing[MAX_LINE_LEN];
        strcpy(original_line_for_writing, line);

        char parse_line[MAX_LINE_LEN];
        strcpy(parse_line, line);
        parse_line[strcspn(parse_line, "\n")] = 0; // Remove newline for parsing

        if (parseStudentLine(parse_line, &s)) {
            char lowerStudentName[NAME_LEN];
            strncpy(lowerStudentName, s.name, sizeof(lowerStudentName) - 1);
            lowerStudentName[sizeof(lowerStudentName) - 1] = '\0';
            str_to_lower(lowerStudentName);

            if (strcmp(lowerDeleteName, lowerStudentName) == 0) { // Found the student to delete
                printf("Found student '%s'. Deleting record...\n", s.name);
                found = 1;
                // Do NOT write this record to the temporary file, effectively deleting it
            } else {
                // Write other records (those not being deleted) to the temporary file
                fprintf(fp_write, "%s", original_line_for_writing); // Write with original newline
            }
        } else {
            // If line cannot be parsed, write it as is to preserve file integrity
            fprintf(fp_write, "%s", original_line_for_writing);
        }
    }

    fclose(fp_read);
    fclose(fp_write);

    if (found) {
        if (remove(FILENAME) != 0) { // Delete the original file
             printf("\nError: Could not delete the original file '%s'.\n", FILENAME);
            perror("Reason");
            remove(TEMP_FILENAME); // Attempt to clean up
        } else {
             if (rename(TEMP_FILENAME, FILENAME) == 0) { // Rename the temporary file
                printf("\nStudent record deleted successfully!\n");
            } else {
                printf("\nError: Could not rename temporary file '%s' to '%s'.\n", TEMP_FILENAME, FILENAME);
                perror("Reason");
                printf("The data excluding the deleted record might be in '%s'. Manual intervention may be needed.\n", TEMP_FILENAME);
            }
        }
    } else {
        remove(TEMP_FILENAME); // Delete temp file if no record was found/deleted
        printf("\nNo student found with the name '%s' to delete.\n", deleteName);
    }
     
}
