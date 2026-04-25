## Take input form user and save it in a file named log.txt
- fopen, -fprintf, -fclose
## Create a utility that reads any text file and outputs the total number of lines, words, and characters.
- The Goal: Understand fgetc() and the EOF (End Of File) constant.

## Read a file where data is stored as Name,Age,RollNo and print it as a clean, aligned table in the console.

- The Goal: Use fgets() and strtok() or sscanf().

- The Technical Truth: fscanf() with %s is dangerous because it doesn't handle spaces in names and can cause buffer overflows. fgets() is the "truth" for reading lines safely.

- Concepts: Buffer management and string parsing.

## Write a program that copies a file (could be an image or a PDF) to a new location.
- The Goal: Switch from "Text Mode" to "Binary Mode" using rb and wb.

- The Technical Truth: You shouldn't read one byte at a time for large files; it’s painfully slow. You must use a buffer (an array of bytes, e.g., 4KB) and use fread() and fwrite().

- Concepts: fread, fwrite, and heap-allocated buffers.

## Create a system where you can "Jump" to the 10th student in a file without reading the first 9.

- The Goal: Use structs and fseek().

- The Technical Truth: To jump accurately, every record must be the exact same size in bytes. This is how real file systems and early databases work.

- Concepts: sizeof(struct), fseek(fp, offset, SEEK_SET), and ftell().

## This project moves beyond simple fprintf. You will build a CLI tool that saves notes to a file but encrypts them using a simple XOR or Caesar cipher before writing to disk.

- Key Concepts: fopen (modes "ab" and "rb"), fgetc, fputc, and basic bitwise manipulation.

- The "Truth" Check: Beginners often use w mode, which overwrites data. Use a (append) or r+ for actual file management. Use binary modes ("rb", "wb") to avoid issues with newline translations on different OS platforms.

## A Disk-Based Key-Value Store (Intermediate)
- Build a simple database where keys and values are stored in a single binary file. You should be able to "Put" a value at a key and "Get" it back.

- Key Concepts: Fixed-length records, calculating offsets (e.g., offset = index * record_size), fseek, and fwrite.

- The Challenge: Implement a simple "deletion" mechanism by marking a byte in the record as "inactive" rather than shifting the whole file (which is inefficient).