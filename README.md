# 📖 Surname Frequency Counter (Hash Table)

A C program that uses **hash tables** to store and count the frequency of surnames from a file.  
The project explores different **hash functions**, **collision handling** techniques, and an extension using **linked lists**.  

---

## 🚀 Features

### Part 1 – Basic Hash Table
- Reads surnames from a file.  
- Uses a custom hash function to compute array indices.  
- Handles collisions with **linear probing**.  
- Increments frequency if surname already exists.  
- Interactive prompt to query surname frequencies.  
- Tracks number of collisions.  

---

### Part 2 – djb2 Hash Function
- Replaced the initial hash function with **[djb2](https://gist.github.com/MohamedTaha98/ccdf734f13299efb73ff0b12f7ce429f)** (Daniel J. Bernstein).  
- More efficient distribution and fewer collisions.  
- Prime table size (`ARRAY_SIZE = 59`) improves load factor.  
- **Result:** Collisions reduced from **39 → 25**.  

**djb2 Implementation Example:**
```c
unsigned long djb2(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}
```
---

### Part 3 - Secondary Hashing

- Added a secondary hash function (hash3) to compute probing step sizes.
- Reduces clustering during collisions.
- Collisions stayed around 25, but with better spread across the table.

--- 

### Part 4 - Linked List Extension

- Added linked lists at each index for collision resolution.
- Data stored in organized chains without overwriting.
- Proper memory management with dynamic allocation.
- Tested successfully on Truncated.csv.
