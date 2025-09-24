#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ARRAY_SIZE 59     // Prime number for array size (hash table)
#define MAX_STRING_SIZE 200

// Structure for a Person
typedef struct Person {
    int personID;
    char* depositionID;
    char* surname;
    char* forename;
    int age;
    char* personType;
    char* gender;
    char* nationality;
    char* religion;
    char* occupation;
    struct Person* next;  // For handling collisions via linked lists
} Person;

// Hash table to store pointers to Person objects
Person* hashTable[ARRAY_SIZE];  // Pointers to Person objects
int collisions = 0;
int num_terms = 0;

// Hash function for surnames
int hash_function(const char* s) {
    int hash = 0;
    while (*s) {
        hash = (hash + *s - 'A') % ARRAY_SIZE;
        s++;
    }
    return hash;
}

// Helper function to allocate and copy a string
char* allocate_and_copy_string(const char* str) {
    char* new_str = (char*) malloc(strlen(str) + 1);  // Allocate memory
    if (new_str) {
        strcpy(new_str, str);  // Copy string content
    }
    return new_str;
}

// Function to create a new Person object
Person* createNewPerson(int personID, const char* depositionID, const char* surname, const char* forename,
                        int age, const char* personType, const char* gender, const char* nationality, const char* religion,
                        const char* occupation) {
    Person* person = (Person*) malloc(sizeof(Person));  // Allocate memory for the person
    if (!person) {
        return NULL;  // Memory allocation failed
    }

    person->personID = personID;
    person->depositionID = allocate_and_copy_string(depositionID);
    person->surname = allocate_and_copy_string(surname);
    person->forename = allocate_and_copy_string(forename);
    person->age = age;
    person->personType = allocate_and_copy_string(personType);
    person->gender = allocate_and_copy_string(gender);
    person->nationality = allocate_and_copy_string(nationality);
    person->religion = allocate_and_copy_string(religion);
    person->occupation = allocate_and_copy_string(occupation);
    person->next = NULL;  // No next person initially (no collision yet)

    return person;
}

// Insert function to handle collisions (same surname)
/*void handleCollision(Person* current, Person* newPerson) {
    // Traverse the linked list until we find the end
    while (current->next != NULL) {
        current = current->next;
    }
    // Insert the new person at the end of the list
    current->next = newPerson;
    collisions++;
}

// Separate insert function
void insertPerson(int personID, const char* depositionID, const char* surname, const char* forename,
                  int age, const char* personType, const char* gender, const char* nationality,
                  const char* religion, const char* occupation) {
    int index = hash_function(surname);
    
    Person* newPerson = createNewPerson(personID, depositionID, surname, forename, age, personType, gender, nationality, religion, occupation);

    // If no collision, insert directly
    if (hashTable[index] == NULL) {
        hashTable[index] = newPerson;
        num_terms++;
    } else {
        // If collision, handle it in the linked list at the index
        handleCollision(hashTable[index], newPerson);
    }
}*/
// Insert function to handle collisions (same surname)
void handleCollision(Person* current, Person* newPerson) {
    // Traverse the linked list until we find the end
    while (current->next != NULL) {
        current = current->next;
    }
    // Insert the new person at the end of the list
    current->next = newPerson;
    //num_terms++;
    collisions++;
}

// Separate insert function
/*void insertPerson(int personID, const char* depositionID, const char* surname, const char* forename,
                  int age, const char* personType, const char* gender, const char* nationality,
                  const char* religion, const char* occupation) {
    int index = hash_function(surname);
    
    Person* newPerson = createNewPerson(personID, depositionID, surname, forename, age, personType, gender, nationality, religion, occupation);

    // If no collision, insert directly
    if (hashTable[index] == NULL) {
        hashTable[index] = newPerson;
        //num_terms++;
    } else {
        // If collision, handle it in the linked list at the index
        handleCollision(hashTable[index], newPerson);
    }
    
    // Increment num_terms regardless of whether it's a collision or not
    num_terms++;
}*/
void insertPerson(int personID, const char* depositionID, const char* surname, const char* forename,
                  int age, const char* personType, const char* gender, const char* nationality,
                  const char* religion, const char* occupation) {
    int index = hash_function(surname);
    
    Person* newPerson = createNewPerson(personID, depositionID, surname, forename, age, personType, gender, nationality, religion, occupation);

    // If no collision, insert directly and increment num_terms since it's a unique surname
    if (hashTable[index] == NULL) {
        hashTable[index] = newPerson;
        num_terms++;  // Only increment when it's a new surname
    } else {
        // If there's a collision, search the linked list to see if this surname already exists
        Person* current = hashTable[index];
        bool surnameExists = false;

        // Traverse the linked list to check if the surname already exists
        while (current != NULL) {
            if (strcmp(current->surname, surname) == 0) {
                surnameExists = true;
                break;
            }
            current = current->next;
        }

        // If surname doesn't exist, handle the collision and add a new person
        if (!surnameExists) {
            handleCollision(hashTable[index], newPerson);
            num_terms++;  // Increment only if it's a new surname
        } else {
            // If the surname exists, just add the new person to the linked list (don't increment num_terms)
            handleCollision(hashTable[index], newPerson);
        }
    }
}



// Add or handle duplicates using this function
/*void addOrIncrement(int personID, const char* depositionID, const char* surname, const char* forename,
                    int age, const char* personType, const char* gender, const char* nationality, 
                    const char* religion, const char* occupation) {
    // Use the insertPerson function to handle the actual insertion or collision
    insertPerson(personID, depositionID, surname, forename, age, personType, gender, nationality, religion, occupation);
}*/

// Search for a person by surname in the hash table
Person* searchBySurname(const char* surname) {
    int index = hash_function(surname);
    Person* current = hashTable[index];

    // Traverse the linked list at this index to find a match
    while (current != NULL) {
        if (strcmp(current->surname, surname) == 0) {
            return current;  // Found a matching surname
        }
        current = current->next;  // Check the next person in case of collisions
    }
    return NULL;  // Surname not found
}


// Print all people with the given surname
void printPeopleWithSurname(const char* surname) {
    Person* person = searchBySurname(surname);
    if (person == NULL) {
        printf("No people with surname %s found.\n", surname);
        return;
    }

    // Iterate through the linked list and print each person
    while (person != NULL) {
        if (strcmp(person->surname, surname) == 0) {
            printf(" %d \t %s \t %s \t %s \t %d \t %s \t %s \t %s \t %s \t %s\n",
                   person->personID, person->depositionID, person->surname, person->forename, person->age, person->personType,
                   person->gender, person->nationality, person->religion, person->occupation);
        }
        person = person->next;  // Traverse to the next person in case of collision
    }
}

// Function to read fields from CSV file
int next_field(FILE *csv, char *buffer, int max_len) {
    int i = 0, inside_quotes = 0;
    char c;

    while ((c = fgetc(csv)) != EOF && i < max_len - 1) {
        if (c == '"') {
            inside_quotes = !inside_quotes;
            continue;
        }
        if (!inside_quotes && c == ',') {
            break;
        }
        if (c == '\n' || c == EOF) {
            break;
        }
        buffer[i++] = c;
    }
    buffer[i] = '\0';
    return (c == EOF) ? 1 : 0;
}

// Load data from CSV file
int load_file(const char* fname) {
    FILE* f = fopen(fname, "r");
    if (!f) {
        printf("Unable to open %s\n", fname);
        return 0;
    }

    char buffer[100];
    fgets(buffer, sizeof(buffer), f);  // Skip header

    while (!feof(f)) {
        int personID;
        char depositionID[MAX_STRING_SIZE];
        char surname[MAX_STRING_SIZE];
        char forename[MAX_STRING_SIZE];
        int age;
        char personType[MAX_STRING_SIZE];
        char gender[MAX_STRING_SIZE];
        char nationality[MAX_STRING_SIZE];
        char religion[MAX_STRING_SIZE];
        char occupation[MAX_STRING_SIZE];

        next_field(f, buffer, MAX_STRING_SIZE);
        personID = atoi(buffer);

        next_field(f, depositionID, MAX_STRING_SIZE);
        next_field(f, surname, MAX_STRING_SIZE);
        next_field(f, forename, MAX_STRING_SIZE);
        next_field(f, buffer, MAX_STRING_SIZE);
        age = atoi(buffer);
        next_field(f, personType, MAX_STRING_SIZE);
        next_field(f, gender, MAX_STRING_SIZE);
        next_field(f, nationality, MAX_STRING_SIZE);
        next_field(f, religion, MAX_STRING_SIZE);
        next_field(f, occupation, MAX_STRING_SIZE);

        // Use addOrIncrement to handle insertion and collision handling
        insertPerson(personID, depositionID, surname, forename, age, personType, gender, nationality, religion, occupation);
    }

    fclose(f);
    return 1;
}

int main() {
    // Initialize hash table to NULL
    for (int i = 0; i < ARRAY_SIZE; i++) {
        hashTable[i] = NULL;
    }

    if (load_file("truncated.csv")) {
        printf("File loaded!\nCapacity: %i\nNum Terms: %i\nCollisions: %i\nLoad: %.2f\n", 
                ARRAY_SIZE, num_terms, collisions, (float)num_terms / ARRAY_SIZE);
        
        char input[100];
        printf("Enter surname to search or type \"quit\" to escape\n>>> ");
        while (scanf("%s", input) && strcmp(input, "quit") != 0) {
            printf("Person ID      Deposition ID \t Surname \t Forename  Age  Person Type \t Gender\t Nationality \t Religion \tOccupation \n");
            printPeopleWithSurname(input);
            printf(">>> ");
        }
    }

    return 0;
}