#ifndef DATA_H
#define DATA_H

#include <stdbool.h>

#define STR_LEN 100
#define DEFAULT_FILENAME "tasks.dat"

typedef struct {
    char title[STR_LEN];
    char description[STR_LEN];
    bool is_completed;
    char deadline[STR_LEN];
} Task;

void clearBuffer(void);
int readInt(void);
void readFilename(char* buffer, int size);

void freeMemory(Task** tasks);
Task* resizeArray(Task* oldArray, int currentCount, int newCount);

void fillTask(Task* t);
void printTaskRow(int index, const Task* t);
void displayTasks(Task* tasks, int count);

Task* addToEnd(Task* tasks, int* count);
Task* insertAtPosition(Task* tasks, int* count);
Task* deleteAtIndex(Task* tasks, int* count);

void sortTasks(Task* tasks, int count);
void searchTasks(Task* tasks, int count);
void editTask(Task* tasks, int count);

int saveTasks(const Task* tasks, int count, const char* filename);
Task* loadTasks(int* count, const char* filename);

#endif
