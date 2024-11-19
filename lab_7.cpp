#include <iostream>

using namespace std;

void swapColumns(int** arr, int n, int col1, int col2) {
    for (int i = 0; i < n; i++) {
        std::swap(arr[i][col1], arr[i][col2]);
    }
}

void sortColumnsBySum(int** arr, int n) {
    // Вычисляем суммы столбцов
    int* columnSums = new int[n];
    for (int j = 0; j < n; j++) {
        columnSums[j] = 0;
        for (int i = 0; i < n; i++) {
            columnSums[j] += arr[i][j];
        }
    }

    // Создаем массив индексов столбцов без них он ругается на типы данных
    int* indices = new int[n];
    for (int j = 0; j < n; j++) {
        indices[j] = j;
    }

    // Сортируем индексы по сумме столбцов
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (columnSums[indices[j]] > columnSums[indices[j + 1]]) {
                std::swap(indices[j], indices[j + 1]);
            }
        }
    }

    // Создаем новую матрицу для отсортированных столбцов
    int** sortedArr = new int*[n];
    for (int i = 0; i < n; i++) {
        sortedArr[i] = new int[n];
        for (int j = 0; j < n; j++) {
            sortedArr[i][j] = arr[i][indices[j]];
        }
    }

    // Копируем отсортированные столбцы обратно в оригинальную матрицу
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            arr[i][j] = sortedArr[i][j];
        }
    }

    // Освобождаем память
    delete[] columnSums;
    delete[] indices;
    for (int i = 0; i < n; i++) {
        delete[] sortedArr[i];
    }
    delete[] sortedArr;
}

int main() {
    int n;

    cout << "Введите размерность массива n (n x n): ";
    cin >> n;

    // Проверка корректности ввода
    if (n <= 0) {
        cerr << "Ошибка: размерность массива должна быть положительным целым числом." << endl;
        return 1; // Завершение программы с ошибкой
    }

    // Создаем двумерный массив
    int** arr = new int*[n];
    for (int i = 0; i < n; i++) {
        arr[i] = new int[n];
    }

    // Считываем массив с клавиатуры
    cout << "Введите элементы массива:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> arr[i][j];
        }
    }

    // Сортируем столбцы по сумме их элементов
    sortColumnsBySum(arr, n);

    // Выводим отсортированный массив
    cout << "Отсортированный массив по сумме столбцов:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }

    // Освобождаем память
    for (int i = 0; i < n; i++) {
        delete[] arr[i];
    }
    delete[] arr;

    return 0;
}
