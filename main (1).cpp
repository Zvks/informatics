// Dispatch.txt - основной файл программы, точка входа
#include "airport.h"

int main()
{
    // Создание аэропорта с координатами (0,0) и длиной ВПП 3000
    TAirport airport = TAirport(0, 0, 3000);
    
    // Запуск симуляции от времени 0 до 2000
    airport.Do(0, 2000);
    
    return 0;
}