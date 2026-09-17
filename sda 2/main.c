#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>

#define STR_LEN 100

typedef struct {
    char title[STR_LEN];
    char description[STR_LEN];
    bool is_completed;
    char deadline[STR_LEN];
} Task;

// Вспомогательная функция очистки буфера
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 1. Ручной механизм освобождения памяти
void freeMemory(Task** tasks) {
    if (*tasks != NULL) {
        free(*tasks);
        *tasks = NULL;
        printf("Память успешно освобождена.\n");
    }
}

// 2. Функция расширения массива (БЕЗ realloc)
// Создает новую область памяти, копирует данные и удаляет старую
Task* resizeArray(Task* oldArray, int currentCount, int newCount) {
    // Выделяем новую область памяти
    Task* newArray = (Task*)malloc(newCount * sizeof(Task));
    if (newArray == NULL) {
        printf("Ошибка выделения памяти!\n");
        return oldArray;
    }

    // Если в старом массиве были данные, копируем их
    if (oldArray != NULL && currentCount > 0) {
        // Копируем столько элементов, сколько влезет в новый размер
        int elementsToCopy = (currentCount < newCount) ? currentCount : newCount;
        memcpy(newArray, oldArray, elementsToCopy * sizeof(Task));
        
        // Освобождаем старую память
        free(oldArray);
    }

    return newArray;
}

void fillTask(Task* t) {
    printf("Введите название задачи: ");
    fgets(t->title, STR_LEN, stdin);
    t->title[strcspn(t->title, "\n")] = 0;

    printf("Введите описание: ");
    fgets(t->description, STR_LEN, stdin);
    t->description[strcspn(t->description, "\n")] = 0;

    printf("Статус (1 - выполнена, 0 - нет): ");
    int status;
    scanf("%d", &status);
    t->is_completed = (status == 1);
    clearBuffer();

    printf("Введите срок: ");
    fgets(t->deadline, STR_LEN, stdin);
    t->deadline[strcspn(t->deadline, "\n")] = 0;
}

void displayTasks(Task* tasks, int count) {
    if (count == 0 || tasks == NULL) {
        printf("\n--- Список задач пуст (память не выделена) ---\n");
        return;
    }
    printf("\n%-3s | %-15s | %-20s | %-12s | %s\n", "ID", "Название", "Описание", "Статус", "Срок");
    printf("--------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-3d | %-15.15s | %-20.20s | %-12s | %s\n",
            i, tasks[i].title, tasks[i].description,
            tasks[i].is_completed ? "Выполнена" : "В процессе", tasks[i].deadline);
    }
}

int main() {
    setlocale(LC_ALL, "Rus");
    
    Task* taskList = NULL;
    int currentCount = 0;
    int choice;

    do {
        printf("\n--- Динамическое управление задачами ---");
        printf("\nТекущее кол-во: %d", currentCount);
        printf("\n1. Добавить задачу (расширить массив)");
        printf("\n2. Удалить последнюю задачу (уменьшить массив)");
        printf("\n3. Показать все задачи");
        printf("\n4. Освободить всю память");
        printf("\n0. Выход");
        printf("\nВыбор: ");
        
        if (scanf("%d", &choice) != 1) break;
        clearBuffer();

        switch (choice) {
            case 1:
                // Увеличиваем размер на 1
                taskList = resizeArray(taskList, currentCount, currentCount + 1);
                fillTask(&taskList[currentCount]);
                currentCount++;
                break;

            case 2:
                if (currentCount > 0) {
                    // Уменьшаем размер на 1
                    currentCount--;
                    if (currentCount == 0) {
                        freeMemory(&taskList);
                    } else {
                        taskList = resizeArray(taskList, currentCount + 1, currentCount);
                    }
                    printf("Последний элемент удален.\n");
                } else {
                    printf("Массив уже пуст.\n");
                }
                break;

            case 3:
                displayTasks(taskList, currentCount);
                break;

            case 4:
                freeMemory(&taskList);
                currentCount = 0;
                break;

            case 0:
                freeMemory(&taskList);
                break;

            default:
                printf("Неверный ввод.\n");
        }
    } while (choice != 0);

    return 0;
}
