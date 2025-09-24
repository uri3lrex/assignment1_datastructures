#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strcpy and strcmp
#include <ctype.h>  // for isalnum

#define MAX_STRING_SIZE 20 // max length of a string
#define ARRAY_SIZE 59  // best be prime
#define NAME_PROMPT "Enter term to get frequency or type \"quit\" to escape\n>>> " // user prompt for searching
#define NEW_LINE_PROMPT ">>> " // user prompt for input

typedef struct Element Element;
struct Element {
    int freq; // frequency of the surname
    char surname[MAX_STRING_SIZE]; // surname string
    int isEmpty; // a boolean flag to indicate if a spot is empty or not
};

Element* hashTable[ARRAY_SIZE]; // global hash table
int collisions = 0; // counting all collisions
int num_terms = 0; // total number of terms inserted

// Hash function to compute hash for a given string
int hash_function(const char* s) { // change to const char*
    int hash = 0;
    while (*s) {
        hash = (hash + *s - 'A') % ARRAY_SIZE; // hash calculation
        s++;
    }
    return hash;
}

int hash3(const char* s){
    int hash = 0;
    while(*s){
       hash = 1+ (hash + *s- 'A') % (ARRAY_SIZE-1);
       s++;
}
return hash;
}

// Create a new element for the hash table
Element* createNewElement(const char* name) { // change to const char*
    Element* newElement = (Element*)malloc(sizeof(Element));
    strcpy(newElement->surname, name);
    newElement->freq = 1; // start frequency at 1 for new insertions
    newElement->isEmpty = 0; // set the spot as occupied
    return newElement;
}

// Returns the element with name or NULL if the element is not present
Element* search(const char* name) { // change to const char*
    int index = hash_function(name);
    int original = index;
    int stepsize= hash3(name);

    while (hashTable[index] != NULL && hashTable[index]->isEmpty == 0) {
        if (strcmp(hashTable[index]->surname, name) == 0) { // correct strcmp usage
            return hashTable[index];
        }
        index = (index + stepsize) % ARRAY_SIZE; // linear probing
        if (index == original)
            break; // looped through entire table
    }

    return NULL;
}

// Insert a new name into the hash table
void insert(const char* name) { // change to const char*
    int index = hash_function(name);
    int stepsize= hash3(name);
    while (hashTable[index] != NULL && hashTable[index]->isEmpty == 0) {
      if (strcmp(hashTable[index]->surname, name) == 0) {
            hashTable[index]->freq++;  // If name is found, increment frequency
            return;
        }
        collisions++;  // Count the collision
        index = (index + stepsize) % ARRAY_SIZE;  // Move by the step size
    }

    // If an empty slot is found, insert the element
    hashTable[index] = createNewElement(name);
    num_terms++;
}

// Print the number of occurrences of a surname or not found
void printNumberOfOccurences(const char* name) { // keep as const char*
    Element* element = search(name);
    if (element != NULL) {
        printf("The surname %s was found %d times\n", name, element->freq);
    } else {
        printf("The surname %s was not found\n", name);
    }
}

// Increment the frequency of a name or insert if not present
void addOrIncrement(const char* name) { // change to const char*
    Element* countelement = search(name);
    if (countelement != NULL) {
        countelement->freq++; // increment frequency
    } else {
        insert(name); // insert new name
    }
}

// Read strings of alphanumeric characters from input file
void next_token(char *buf, FILE *f, int string_max) {
    buf[0] = fgetc(f);
    while (!isalnum(buf[0]) && !feof(f)) { 
        buf[0] = fgetc(f); 
    }
    int i = 1;
    for (;;) {
        buf[i] = fgetc(f);
        if (!isalnum(buf[i])) { 
            if (buf[i] != ' ') { // keep spaces
                break; // only load letters and numbers
            }
        } 
        if (feof(f)) { 
            break; // file ended?
        }
        if (i < (string_max - 1)) { 
            ++i; // truncate strings that are too long
        }
    }
    buf[i] = '\0'; // NULL terminate the string
}

// Read the contents of a file and adds them to the hash table
int load_file(const char *fname) { // change to const char*
    FILE *f;
    char buf[MAX_STRING_SIZE];

    f = fopen(fname, "r");
    if (!f) { 
        printf("Unable to open %s\n", fname);
        return 0; 
    }
    
    while (!feof(f)) {
        next_token(buf, f, MAX_STRING_SIZE);
        addOrIncrement(buf);
    }
    
    printf("File %s loaded\n", fname);
    float load = (float)num_terms / ARRAY_SIZE; // calculate load factor
    printf("Capacity: %i\nNum Terms: %i\nCollisions: %i\nLoad: %.2f\n", ARRAY_SIZE, num_terms, collisions, load);
    fclose(f); // always remember to close your file stream

    return 1;
}

int main() {
    // Initialize hash table
    for (int i = 0; i < ARRAY_SIZE; i++) {
        hashTable[i] = NULL; // set all elements to NULL initially
    }

    if (load_file("names.csv") == 0) {
        printf("Cannot load file.\n");
    } else {
        // Interactive prompt to allow user to search for name frequencies
        char input[MAX_STRING_SIZE];
        printf(NAME_PROMPT);
        while (scanf("%s", input) != EOF) {
            if (strcmp(input, "quit") == 0) {
                break;
            }
            printNumberOfOccurences(input);
            printf(NEW_LINE_PROMPT);
        }
    }
    
    return 0;
}
