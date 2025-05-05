#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 20
#define MAX_CHUNKS 10
#define MAX_FRAMES 200  // maximum memory frames we'll support

typedef struct {
  int pid;
  int arrival_time;
  int lifetime;
  int num_chunks;
  int chunks[MAX_CHUNKS];
  int total_memory;
  int start_time;
  int end_time;
  int in_memory;
} Process;

typedef struct {
  int pid;      // -1 if free
  int page_num; // which page of the process this is
} Frame;

int memory_size, page_size;
Frame memory_map[MAX_FRAMES];
int num_frames;

void PrintInputQueue(Process processes[], int queue[], int size) {
    printf("       Input Queue:[");
    for (int i = 0; i < size; i++) {
        printf("%d", processes[queue[i]].pid);
        if (i < size - 1) printf(" ");
    }
    printf("]\n");
}

void PrintMemoryMap() {
  for (int i = 0; i < num_frames; i++) {
    int start = i * page_size;
    int end = start + page_size - 1;
    if (memory_map[i].pid == -1) {
      printf("              %d-%d: Free frame(s)\n", start, end);
    } else {
      printf("              %d-%d: Process %d, Page %d\n", start, end,
             memory_map[i].pid, memory_map[i].page_num + 1);
    }
  }
}

int main() {
  int memorySize;
  int pageOption;
  char filename[100];

  // Get memory size
  printf("Memory size> ");
  scanf("%d", &memorySize);

  // Get page size
  printf("Page Size (1: 100, 2: 200, 3: 400)> ");
  scanf("%d", &pageOption);

  switch (pageOption) {
    case 1: page_size = 100; break;
    case 2: page_size = 200; break;
    case 3: page_size = 400; break;
    default:
      printf("Invalid page size option. Exiting.\n");
      return 1;
  }

  // set global memory vars
  memory_size = memorySize;
  num_frames = memory_size / page_size;

  // init memory map
  for (int i = 0; i < num_frames; i++) {
    memory_map[i].pid = -1;
    memory_map[i].page_num = -1;
  }

  // get input file
  printf("Input filename> ");
  scanf("%s", filename);

  FILE* file = fopen(filename, "r");
  if (!file) {
    printf("Could not open file: %s\n", filename);
    return 1;
  }

  printf("\n--- Simulation Config ---\n");
  printf("Memory Size: %d\n", memory_size);
  printf("Page Size: %d\n", page_size);
  printf("Input File: %s\n", filename);

  int numProcesses;
  fscanf(file, "%d", &numProcesses);

  Process processes[MAX_PROCESSES];
  for (int i = 0; i < numProcesses; i++) {
    fscanf(file, "%d", &processes[i].pid);
    fscanf(file, "%d %d", &processes[i].arrival_time, &processes[i].lifetime);
    fscanf(file, "%d", &processes[i].num_chunks);
    processes[i].total_memory = 0;
    for (int j = 0; j < processes[i].num_chunks; j++) {
      fscanf(file, "%d", &processes[i].chunks[j]);
      processes[i].total_memory += processes[i].chunks[j];
    }
    processes[i].in_memory = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF && ch != '\n');
  }
  fclose(file);

  // simulation starts
  int clockTime = 0;
  int freeMemory = memory_size;
  int inQueue[MAX_PROCESSES], queueSize = 0;
  int active[MAX_PROCESSES] = {0}, completed = 0;

  printf("\n--- Starting Simulation ---\n");

  while (completed < numProcesses && clockTime <= 100000) {
    for (int i = 0; i < numProcesses; i++) {
      if (processes[i].arrival_time == clockTime) {
        inQueue[queueSize++] = i;
        printf("t = %d: Process %d arrives\n", clockTime, processes[i].pid);
        PrintInputQueue(processes, inQueue, queueSize);
      }
    }

    for (int i = 0; i < numProcesses; i++) {
      if (active[i] && clockTime == processes[i].end_time) {
        printf("         Process %d completes\n", processes[i].pid);
        for (int f = 0; f < num_frames; f++) {
          if (memory_map[f].pid == processes[i].pid) {
            memory_map[f].pid = -1;
            memory_map[f].page_num = -1;
          }
        }
        printf("         Memory Map after freeing:\n");
        PrintMemoryMap();
        freeMemory += processes[i].total_memory;
        active[i] = 0;
        completed++;
      }
    }

    int admitted = 1;
    while (admitted) {
      admitted = 0;
      for (int i = 0; i < queueSize; i++) {
        int idx = inQueue[i];
        if (processes[idx].total_memory <= freeMemory) {
          processes[idx].start_time = clockTime;
          processes[idx].end_time = clockTime + processes[idx].lifetime;
          freeMemory -= processes[idx].total_memory;
          active[idx] = 1;

          int pages_needed = 0;
          for (int k = 0; k < processes[idx].num_chunks; k++) {
            pages_needed += (processes[idx].chunks[k] + page_size - 1) / page_size;
          }

          int assigned = 0;
          for (int f = 0; f < num_frames && assigned < pages_needed; f++) {
            if (memory_map[f].pid == -1) {
              memory_map[f].pid = processes[idx].pid;
              memory_map[f].page_num = assigned;
              assigned++;
            }
          }

          printf("       MM moves Process %d to memory\n", processes[idx].pid);
          PrintInputQueue(processes, inQueue, queueSize - 1);
          PrintMemoryMap();

          for (int j = i; j < queueSize - 1; j++) {
            inQueue[j] = inQueue[j + 1];
          }
          queueSize--;
          admitted = 1;
          break;
        }
      }
    }

    clockTime++;
  }

  double totalTurnaround = 0;
  for (int i = 0; i < numProcesses; i++) {
    totalTurnaround += (processes[i].end_time - processes[i].arrival_time);
  }
  printf("\nAverage Turnaround Time: %.2f\n", totalTurnaround / numProcesses);

  printf("All memory freed. Exiting simulation.\n");

  return 0;
}
