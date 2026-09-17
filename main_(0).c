#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include "data.h"

void printMenu(int currentCount) {
    printf("\r\n--- Динамическое управление задачами ---");
    printf("\r\nТекущее кол-во: %d", currentCount);
    printf("\r\n1. Добавить задачу в конец");
    printf("\r\n2. Вставить задачу на позицию");
    printf("\r\n3. Удалить задачу по индексу");
    printf("\r\n4. Показать все задачи");
    printf("\r\n5. Найти задачу");
    printf("\r\n6. Сортировать список");
    printf("\r\n7. Редактировать задачу");
    printf("\r\n8. Освободить всю память");
    printf("\r\n9. Сохранить список в файл");
    printf("\r\n10. Загрузить список из файла");
    printf("\r\n0. Выход (с сохранением)");
    printf("\r\nВыбор: ");
}

void setupConsole(void) {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF-8");
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (hOut != INVALID_HANDLE_VALUE && GetConsoleMode(hOut, &mode)) {
        SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#else
    setlocale(LC_ALL, "ru_RU.UTF-8");
#endif
}

int main(void) {
    setupConsole();

    Task* taskList = NULL;
    int currentCount = 0;
    int choice;
    char filename[STR_LEN];

    do {
        printMenu(currentCount);
        choice = readInt();

        switch (choice) {
            case 1:
                taskList = addToEnd(taskList, &currentCount);
                break;

            case 2:
                taskList = insertAtPosition(taskList, &currentCount);
                break;

            case 3:
                taskList = deleteAtIndex(taskList, &currentCount);
                break;

            case 4:
                displayTasks(taskList, currentCount);
                break;

            case 5:
                searchTasks(taskList, currentCount);
                break;

            case 6:
                sortTasks(taskList, currentCount);
                break;

            case 7:
                editTask(taskList, currentCount);
                break;

            case 8:
                freeMemory(&taskList);
                currentCount = 0;
                break;

            case 9:
                readFilename(filename, STR_LEN);
                saveTasks(taskList, currentCount, filename);
                break;

            case 10:
                readFilename(filename, STR_LEN);
                if (taskList != NULL) {
                    free(taskList);
                    taskList = NULL;
                    currentCount = 0;
                }
                taskList = loadTasks(&currentCount, filename);
                displayTasks(taskList, currentCount);
                break;

            case 0:
                saveTasks(taskList, currentCount, DEFAULT_FILENAME);
                freeMemory(&taskList);
                currentCount = 0;
                break;

            default:
                printf("Неверный ввод.\n");
        }
    } while (choice != 0);

    return 0;
}
