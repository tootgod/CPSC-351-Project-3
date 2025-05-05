# CPSC 351 Programming Assignment – Memory Management Simulation

### Section Information  
**Section:** 09 14061  
**Name:** Kyler Geesink, Kenny Garcia  
**Email:** geesinkk@csu.fullerton.edu, kennygarcia15@csu.fullerton.edu  
**Contributions:** Kenny implemented the memory allocation logic and handled process input parsing.
Kyler managed the output formatting and helped debug allocation issues.

### Language Used  
This program is written in **C**

### Program Overview  
This project simulates a memory management system that uses paging and First Fit allocation.  The simulation will read a list of processes, which include their arrival and duration times, and as the simulation runs, it will move processes into and out of memory according to the scheduled events. Each process will allocate the same number of pages according to its size, and memory will be deallocated when the process completes its work. The simulation will produce a memory map that will change with use throughout the simulation. After all processes are complete, a turnaround time will also be calculated.

### Core Concepts  
- Simulates a memory management system - paging
- Uses First Fit memory allocation
- Tracks a process's arrival, memory allocation, completion, and deallocation
- Produces a visual memory map during simulation
- Calculates Average Turnaround Time based on duration of processes

### How to Compile and Run  
1. Ensure that you are using the Linux operating system and you have gcc installed. 
2. Place all project files (e.g., `Project3.c`, `out1.txt`) into the same folder.
3. Open a terminal and navigate to that folder:
   ```bash
   cd CPSC-351-Project-3
   ```
4. Compile the project:
    ```bash
    gcc Project3.c -o project3
    ```
5. Run the program:
    ```bash
    ./project3
    ```
6. When prompted by the program enter the following values: 
    ```c
    Memory size> 2000
    Page Size (1: 100, 2: 200, 3: 400)> 2
    Input filename> in1.txt
    ```
7. To save and compare the output against the reference file ```out1.txt```:
    ```bash
    ./project3 > out1_test.txt
    diff -y out1_test.txt out1.txt
    ```
### Testing Screenshot
![Test 1](https://media.discordapp.net/attachments/1299155448959598595/1368783627936075847/Screenshot_2025-05-04_195626.png?ex=68197a97&is=68182917&hm=c69fed304c7f60b3778842d296fd5ed8e485a80b845c8c99c26f603b502017c2&=&format=webp&quality=lossless&width=433&height=855)
![Test 2](https://media.discordapp.net/attachments/1299155448959598595/1368783628212895774/Screenshot_2025-05-04_195642.png?ex=68197a97&is=68182917&hm=910c98842e46324c146c61adc8794fc7b3dae0997711245ed8beea9d5528f68f&=&format=webp&quality=lossless&width=590&height=856)
![Test 3](https://media.discordapp.net/attachments/1299155448959598595/1368783628489850950/Screenshot_2025-05-04_195655.png?ex=68197a97&is=68182917&hm=c970f63cdadaa5abe437747ae1eb53ffc3ffbd9ab2578043dc27942aef4fa6b0&=&format=webp&quality=lossless&width=472&height=856)

You will see:
- When each process arrives
- When it moves into memory - and what frames it occupies
- A visual map of memory with free or used frames displayed
- When each process is complete - and memory is freed
- The average turnaround time at the end

### Special Notes
- Process sizes are rounded up to the nearest page
- You can easily change input data to test other scenarios.