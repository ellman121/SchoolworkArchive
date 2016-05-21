# SchoolworkArchive - OPUS IN PROFECTUS

## This is a collection of all my schoolwork done at SDSMT

### Contents:

#### CSC 250 - Computer Science Two
This is a second course in CS, where students cover basic structures such as pointers, linked lists, and basic ADT's

| Program Name | Description |
|----------------------|----------------------------------------------------------------------------|
| MP3TagEditor | This program was made to edit ID3v1 Tags on MP3 files |
| Linked List Class | This program implements a linked list as a C++ Class |
| Reverse Typoglycemia | This program uses a dictionary to do reverse typoglycemia on an input file |

#### CSC 300 - Data Structures
This is a sophomore level course desigend to introduce basic data structures and algorithms, using C++ as the primary language.  Topics include trees, graphs, Big-Oh analysis, and introductory algorithms.

| Program Name | Description |
|-------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| LinkedList Struct | This program implements and bubble sorts a linked list using C-Structs |
| RedBlack Tree | This program implements a Red-Black Tree.  All updates to the tree are done on disk. |
| Set-Union Timing | This program uses three methods to find the union of two sets, and outputs the time difference between them.  The methods are bubble-sort style search, the merge algorithm from merge-sort, and hash table insert/collision.|

#### CSC 314 - Assembly Language
This is a sophomore level course where students implement programs in ARM assembly, and use a Raspberry Pi for debugging/testing.  Topics range include ARM architecture, and both fixed and floating point arithmetic.

| Program Name | Description |
|------------- | ----------- |
| Lab 1 | This lab was a demo program provided to students for them to figure out how to compile and run code on a Raspberry Pi |
| Lab 2 | This lab was for students to figure out how to use standard in and out in assembly language using printf and scanf calls |
| Lab 3 | This lab was the first "assignment" in Assembly Language.  Prompt the user for numbers, print the max and min, then print the count of negative and positive numbers. |
| Lab 4 | This lab provides students with a checksum program written in c and asks them to convert them into assembly. |
| Lab 5 | This program prompts the user for an integer and then prints out all prime numbers between 1 and n.  The algorithm to find primes is not fast. |
| Lab 6 | This lab reads in 4 sets of integers, where each integer is the number of coins (pennies, nickles, dimes, quarters) saved each week, and then prints out the yearly savings | 
| Lab 7 | This lab asks the user for two x values and a number of rows, then evaluates the function f(x) = (x^2 - 3x - 2) / (x^2 - 4) at a number of points, and then outputs a table of values for the function.
| Lab 8 | This lab is an introduction to GPIO, where the user has 3 buttons and 3 LED's.  The buttons increment, reset, and decrement the counter, and the LED's output the low 3 bits of the count.  

#### CSC 317 - Computer Organization and Architecture
This course is an introduction to computer hardware.  It provides students with a historical background in computing.  Topics include Error Correcting Codes, Computer Emulation, and hardware implementation of assembly instructions.

| Program Name | Description |
|----------------------|----------------------------------------------------------------------------|
| HammingCode | This program allows the user to input binary strings and run Hamming 7/11 or 11/15 error correction on the strings. |
| 24 Bit Computer Emulator | This program allows a user to input commands from a 24-bit assembly language, and run them.  More information can be found in the included pdf file |

#### CSC 372 - Analysis of Algorithms
This course is an intermediate course in algorithm analysis, where students study a wide range of algorithms and topics.  Specifcaly, Computational Geometry, Divide and Conquer Algorithms, Discrete and Fast Fourier Transforms, and Amortized Complexity Analysis are covered in detail.

| Program Name | Description |
|----------------------|----------------------------------------------------------------------------|
| Program 1 | This program reads in a text file and then for each case in the text file, outputs whether or not the string is composed of repeated strings or characters. |
| Program 2 | This program reads in a cases from an input file, where each case consists of a set of points defining a convex polygon.  For each case, the program outpus the x coordinate where a verticle line will cut the input shape in half by area.  |
| Program 3 | This program reads in a set of real values from an input case, and then outputs the nth FFT of that data's 5 highest peaks.  |

#### CSC 412 - Cryptography
This coures is an advanced course in cryptography from a mathematical standpoint.  Topics include classical cryptosystems (Caesar, Vigenere, Playfair, ENIGMA), symmetric-key cryptosystems (DES, AES), Public-Key Cryptography (RSA, Pohlig-Hellman, ElGamal), Secure Hashing, and Security Protocalls.  The coures also briefly covers attacking cryptosystems.

| Program Name | Description |
|----------------------|----------------------------------------------------------------------------|
| Affine Cihper | This program reads in command line arguments consisting of alpha and beta, and then does an affine encryption on the input text where each letter x is mapped such that x -> &#945; x + &#946; . |
| BabyStep GiantStep | This program reads in an alpha value, beta value, and prime number from an input file, and then solves for x where x = &#945;^x = &#946; (mod p).  In particular, this program solves the Discrete Logarithm problem using the Baby Step, Giant Step Algorithm.|
| Mini DES | This program is an implementation of the Mini DES algorithm found in the fourth chapter of *Introduction to Cryptography with Coding Theory* by Trapp and Washington.|
| Playfair Cipher | This program does playfair encryption on the provided input text in the command line arguments.|

#### CSC 461 - Programming Languages
This course is an introduction to the study of programming languages.  The first set of material is over programming language design.  Second, the course covers programming language paradigms.  Topics include formal languages, automata, parsing and compiling, data abstraction, variable binding, block-structured languages, OOP, functional programming, and parallelism.

| Program Name | Description |
|----------------------|----------------------------------------------------------------------------|
| Minimum Distance Classifier | This is a python program to read in a dataset (csv format) where the class is the 2nd column and then try to classify each object by the minimum distance to a centroid. |
| Paint in Java | This is a primitave version of MS Paint written in Java.|
| Family Tree | This program allows the user to input a .dat file consisting of family entries, and then query the data with functions found in genealogy.lsp |

#### SE2CO11 - Compilers
I took this course while studying abroad at the University of Reading.  This course focused on basic compiler techniques, including lexical analysis, parsing, code generation, TAC, Finite State Automata, and basic optimisation

| Program Name | Description |
|----------------------|----------------------------------------------------------------------------|
| Project | This project was to make a compiler that translates from any langauge to any other language. For the project, I wrote a compiler using flex and yacc which would translate from a language I craeted, called C+, to ANSI-C. Full specifications can be found in the project folder. |

#### SE2OS11 - Operating Systems
I took this course while studying abroad at the University of Reading.  This folder contains all of the practicals (labs) from my semeseter, as well as the .tex files and pdf of the final write up.  See below for descriptions of each practical.

| Program Name | Description |
|----------------------|----------------------------------------------------------------------------|
| Pratical 1 | This practical involved students writing C code which would use fork() to create new processes, and then pipe() to have the processes link input and output to each other.  The final result was a c program which would be the equivalent to "$ cat /etc/passwd \| cut -f1 -d: \| sort" |
| Pratical 2 | This practical required students to write c code both as multi-process (using fork()) and multi-threaded (using pthreads) in order to do a large computation.  The pthread version of the program required students to use semaphores to manage shared memory |
| Pratical 3 | This practical had students implement inter-process communication using message queues and signals on Linux.  The practical is dedicated to Leonard Nimoy - RIP. |
