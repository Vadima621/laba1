#include <stdio.h>
#include <stdlib.h>

int check() {
    int row;
    while (1) {
        scanf_s("%d", &row);
        if (getchar() != '\n') {
            puts("ошибка, попробуйте снова ");
            while (getchar() != '\n');
            continue;
        }
        else
            break;
    }
    return row;
}

void print_file(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file for reading");
        return;
    }

    int num;
    printf("Содержимое файла %s:\n", filename);
    while (fread(&num, sizeof(int), 1, file) == 1) {
        printf("%d ", num);
    }
    printf("\n");

    fclose(file);
}

int is_prime(int num) {
    if (num <= 1) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

void write_to_file(const char* filename) {
    FILE* file = fopen(filename, "wb");

    int num;
    printf("Введите целые числа (для завершения введите 0):\n");
    while (1) {
        printf("Число: ");
        num = check();

        if (num == 0) break;
        fwrite(&num, sizeof(int), 1, file);
    }
    printf("Данные успешно записаны в файл %s\n", filename);

    fclose(file);
}

void invert_numbers_in_file(const char* filename) {
    FILE* file = fopen(filename, "r+b");

    int num;
    while (fread(&num, sizeof(int), 1, file) == 1) {
        num *= -1;
        fseek(file, -1 * (sizeof(int)), SEEK_CUR);
        fwrite(&num, sizeof(int), 1, file);
        fflush(file);
    }
    printf("Знаки чисел в файле %s успешно изменены.\n", filename);
    fclose(file);
}

int count_prime_numbers_in_file(const char* filename) {
    FILE* file = fopen(filename, "rb");

    int count = 0;
    int num;

    while (fread(&num, sizeof(int), 1, file)) {
        if (is_prime(num)) {
            count++;
        }
    }

    fclose(file);
    return count;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bubble_sort(int* arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] < arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void sort_negative_numbers(const char* filename) {
    FILE* file = fopen(filename, "r+b");

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    int count = file_size / sizeof(int);

    int* numbers = (int*)malloc(file_size);
    fread(numbers, sizeof(int), count, file);

    int* negative_numbers = (int*)malloc(file_size);

    int negative_count = 0;
    for (int i = 0; i < count; i++) {
        if (numbers[i] < 0) {
            negative_numbers[negative_count++] = numbers[i];
        }
    }

    bubble_sort(negative_numbers, negative_count);

    int negative_index = 0;
    for (int i = 0; i < count; i++) {
        if (numbers[i] < 0) {
            numbers[i] = negative_numbers[negative_index++];
        }
    }

    fseek(file, 0, SEEK_SET);
    fwrite(numbers, sizeof(int), count, file);

    free(numbers);
    free(negative_numbers);
    fclose(file);

    printf("Отрицательные числа в файле %s отсортированы по убыванию.\n", filename);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Использование: %s <имя бинарного файла>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];

    write_to_file(filename);
    print_file(filename);

    int prime_count = count_prime_numbers_in_file(filename);
    printf("Количество простых чисел в файле: %d\n", prime_count);

    invert_numbers_in_file(filename);
    print_file(filename);

    sort_negative_numbers(filename);
    print_file(filename);
}
