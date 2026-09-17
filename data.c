#define _CRT_SECURE_NO_WARNINGS
#include "data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clearBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int readInt(void) {
    int value;
    if (scanf("%d", &value) != 1) {
        clearBuffer();
        return -1;
    }
    clearBuffer();
    return value;
}

void readFilename(char* buffer, int size) {
    printf("Имя файла (пусто = %s): ", DEFAULT_FILENAME);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (buffer[0] == '\0') {
        strncpy(buffer, DEFAULT_FILENAME, size - 1);
        buffer[size - 1] = '\0';
    }
}

void freeMemory(Task** tasks) {
    if (*tasks != NULL) {
        free(*tasks);
        *tasks = NULL;
        printf("Память успешно освобождена.\n");
    }
}

Task* resizeArray(Task* oldArray, int currentCount, int newCount) {
    if (newCount <= 0) {
        if (oldArray != NULL) {
            free(oldArray);
        }
        return NULL;
    }

    Task* newArray = (Task*)malloc(newCount * sizeof(Task));
    if (newArray == NULL) {
        printf("Ошибка выделения памяти!\n");
        return oldArray;
    }

    if (oldArray != NULL && currentCount > 0) {
        int elementsToCopy = (currentCount < newCount) ? currentCount : newCount;
        memcpy(newArray, oldArray, elementsToCopy * sizeof(Task));
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
    int status = readInt();
    t->is_completed = (status == 1);

    printf("Введите срок: ");
    fgets(t->deadline, STR_LEN, stdin);
    t->deadline[strcspn(t->deadline, "\n")] = 0;
}

void printTaskRow(int index, const Task* t) {
    printf("%d) %s | %s | %s | %s\r\n",
        index, t->title, t->description,
        t->is_completed ? "выполнена" : "не выполнена", t->deadline);
}

void displayTasks(Task* tasks, int count) {
    if (count == 0 || tasks == NULL) {
        printf("\r\n--- Список задач пуст ---\r\n");
        return;
    }
    printf("\r\nID | Название | Описание | Статус | Срок\r\n");
    printf("--------------------------------------------\r\n");
    for (int i = 0; i < count; i++) {
        printTaskRow(i, &tasks[i]);
    }
}

static void swapTasks(Task* a, Task* b) {
    Task tmp = *a;
    *a = *b;
    *b = tmp;
}

static int compareTasks(const Task* a, const Task* b, int field) {
    switch (field) {
        case 1: return strcmp(a->title, b->title);
        case 2: return strcmp(a->description, b->description);
        case 3: return (int)a->is_completed - (int)b->is_completed;
        case 4: return strcmp(a->deadline, b->deadline);
        default: return 0;
    }
}

Task* addToEnd(Task* tasks, int* count) {
    Task* oldList = tasks;
    tasks = resizeArray(tasks, *count, *count + 1);
    if (tasks == NULL || (tasks == oldList && *count > 0)) {
        printf("Не удалось добавить задачу.\n");
        return oldList;
    }
    fillTask(&tasks[*count]);
    (*count)++;
    return tasks;
}

Task* insertAtPosition(Task* tasks, int* count) {
    printf("Введите позицию вставки (0 .. %d): ", *count);
    int position = readInt();

    if (position < 0 || position > *count) {
        printf("Некорректная позиция. Допустимо от 0 до %d.\n", *count);
        return tasks;
    }

    Task* newArray = resizeArray(tasks, *count, *count + 1);
    if (newArray == NULL || (newArray == tasks && *count > 0)) {
        printf("Не удалось вставить элемент.\n");
        return tasks;
    }

    for (int i = *count; i > position; i--) {
        newArray[i] = newArray[i - 1];
    }

    printf("\n--- Новая задача ---\n");
    fillTask(&newArray[position]);
    (*count)++;
    printf("Задача вставлена на позицию %d.\n", position);
    return newArray;
}

Task* deleteAtIndex(Task* tasks, int* count) {
    if (*count == 0 || tasks == NULL) {
        printf("Список пуст, удалять нечего.\n");
        return tasks;
    }

    displayTasks(tasks, *count);
    printf("Введите индекс удаляемой задачи (0 .. %d): ", *count - 1);
    int index = readInt();

    if (index < 0 || index >= *count) {
        printf("Некорректный индекс.\n");
        return tasks;
    }

    for (int i = index; i < *count - 1; i++) {
        tasks[i] = tasks[i + 1];
    }

    (*count)--;

    if (*count == 0) {
        free(tasks);
        printf("Задача удалена. Список стал пустым.\n");
        return NULL;
    }

    tasks = resizeArray(tasks, *count + 1, *count);
    printf("Задача с индексом %d удалена, элементы сдвинуты.\n", index);
    return tasks;
}

void sortTasks(Task* tasks, int count) {
    if (count < 2 || tasks == NULL) {
        printf("Для сортировки нужно хотя бы 2 задачи.\n");
        return;
    }

    printf("\nСортировать по полю:\n");
    printf("1. Название\n");
    printf("2. Описание\n");
    printf("3. Статус\n");
    printf("4. Срок\n");
    printf("Выбор: ");
    int field = readInt();
    if (field < 1 || field > 4) {
        printf("Некорректное поле.\n");
        return;
    }

    printf("Порядок (1 - по возрастанию, 0 - по убыванию): ");
    int ascending = readInt();

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int cmp = compareTasks(&tasks[j], &tasks[j + 1], field);
            int needSwap = (ascending == 1) ? (cmp > 0) : (cmp < 0);
            if (needSwap) {
                swapTasks(&tasks[j], &tasks[j + 1]);
            }
        }
    }

    printf("Список отсортирован.\n");
    displayTasks(tasks, count);
}

void searchTasks(Task* tasks, int count) {
    if (count == 0 || tasks == NULL) {
        printf("Список пуст, искать нечего.\n");
        return;
    }

    printf("\nИскать по полю:\n");
    printf("1. Название\n");
    printf("2. Описание\n");
    printf("3. Статус (1 - выполнена, 0 - нет)\n");
    printf("4. Срок\n");
    printf("Выбор: ");
    int field = readInt();

    char query[STR_LEN] = { 0 };
    int statusQuery = 0;

    if (field == 3) {
        printf("Введите статус (1/0): ");
        statusQuery = readInt();
    } else if (field >= 1 && field <= 4) {
        printf("Введите строку для поиска: ");
        fgets(query, STR_LEN, stdin);
        query[strcspn(query, "\n")] = 0;
    } else {
        printf("Некорректное поле.\n");
        return;
    }

    int found = 0;
    printf("\r\n--- Результаты поиска ---\r\n");
    printf("ID | Название | Описание | Статус | Срок\r\n");
    printf("--------------------------------------------\r\n");

    for (int i = 0; i < count; i++) {
        int match = 0;
        switch (field) {
            case 1: match = (strstr(tasks[i].title, query) != NULL); break;
            case 2: match = (strstr(tasks[i].description, query) != NULL); break;
            case 3: match = (tasks[i].is_completed == (statusQuery == 1)); break;
            case 4: match = (strstr(tasks[i].deadline, query) != NULL); break;
        }
        if (match) {
            printTaskRow(i, &tasks[i]);
            found++;
        }
    }

    if (found == 0) {
        printf("Ничего не найдено.\n");
    } else {
        printf("Найдено записей: %d\n", found);
    }
}

void editTask(Task* tasks, int count) {
    if (count == 0 || tasks == NULL) {
        printf("Список пуст, редактировать нечего.\n");
        return;
    }

    displayTasks(tasks, count);
    printf("Введите индекс задачи для редактирования (0 .. %d): ", count - 1);
    int index = readInt();
    if (index < 0 || index >= count) {
        printf("Некорректный индекс.\n");
        return;
    }

    printf("\nЧто изменить?\n");
    printf("1. Название\n");
    printf("2. Описание\n");
    printf("3. Статус\n");
    printf("4. Срок\n");
    printf("5. Все поля\n");
    printf("Выбор: ");
    int field = readInt();

    Task* t = &tasks[index];
    switch (field) {
        case 1:
            printf("Новое название: ");
            fgets(t->title, STR_LEN, stdin);
            t->title[strcspn(t->title, "\n")] = 0;
            break;
        case 2:
            printf("Новое описание: ");
            fgets(t->description, STR_LEN, stdin);
            t->description[strcspn(t->description, "\n")] = 0;
            break;
        case 3:
            printf("Новый статус (1 - выполнена, 0 - нет): ");
            t->is_completed = (readInt() == 1);
            break;
        case 4:
            printf("Новый срок: ");
            fgets(t->deadline, STR_LEN, stdin);
            t->deadline[strcspn(t->deadline, "\n")] = 0;
            break;
        case 5:
            fillTask(t);
            break;
        default:
            printf("Некорректный выбор.\n");
            return;
    }

    printf("Задача обновлена.\n");
}

int saveTasks(const Task* tasks, int count, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Не удалось открыть файл \"%s\" для записи.\n", filename);
        return 0;
    }

    if (fwrite(&count, sizeof(int), 1, file) != 1) {
        printf("Ошибка записи количества задач.\n");
        fclose(file);
        return 0;
    }

    if (count > 0 && tasks != NULL) {
        if (fwrite(tasks, sizeof(Task), count, file) != (size_t)count) {
            printf("Ошибка записи массива структур.\n");
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    printf("Сохранено задач: %d в файл \"%s\"\n", count, filename);
    return 1;
}

Task* loadTasks(int* count, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Не удалось открыть файл \"%s\" для чтения.\n", filename);
        *count = 0;
        return NULL;
    }

    int n = 0;
    if (fread(&n, sizeof(int), 1, file) != 1 || n < 0) {
        printf("Файл повреждён или имеет неверный формат.\n");
        fclose(file);
        *count = 0;
        return NULL;
    }

    if (n == 0) {
        fclose(file);
        *count = 0;
        printf("Файл пуст, загружено 0 задач.\n");
        return NULL;
    }

    Task* tasks = (Task*)malloc(n * sizeof(Task));
    if (tasks == NULL) {
        printf("Ошибка выделения памяти при загрузке.\n");
        fclose(file);
        *count = 0;
        return NULL;
    }

    if (fread(tasks, sizeof(Task), n, file) != (size_t)n) {
        printf("Ошибка чтения массива структур.\n");
        free(tasks);
        fclose(file);
        *count = 0;
        return NULL;
    }

    fclose(file);
    *count = n;
    printf("Загружено задач: %d из файла \"%s\"\n", n, filename);
    return tasks;
}
