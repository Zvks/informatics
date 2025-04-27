// Source.txt - реализация методов классов
#include "airport.h"

// Реализация методов TAObject
TAObject::TAObject(float xi, float yi) {
    this->x = xi, this->y = yi;
}
TAObject::~TAObject() = default;

// Реализация методов TLA
TLA::TLA(float xi, float yi, float Vi, float xci, float yci) :TAObject(xi, yi) {
    this->xc = xci;
    this->yc = yci;
    this->V = Vi;
    this->landed = false;
    this->a = 0;
}

// Метод движения базового ЛА
void TLA::move(float t, int a)
{
    // Управление флагом посадки
    if (a == 0) { enableToLanding = false; }
    if (a == 1) { enableToLanding = true; }
    
    // Пересчет полярных координат
    RemakePhiNRadius();
}

// Пересчет полярных координат (радиус и угол)
void TLA::RemakePhiNRadius()
{
    // Определение сектора для расчета угла
    if (x > xc) b = 0;
    if (x <= xc and y <= yc) { b = -1; }
    if (x <= xc and y >= yc) { b = 1; }
    
    // Расчет угла и радиуса
    fi = b * 3.14 + atan((y - yc) / (x - xc));
    R = sqrt(pow((x - xc), 2) + pow((y - yc), 2));
}

// Реализация методов TAircraft
TAircraft::TAircraft(float xi, float yi, float Vi, float xci, float yci) :TLA(xi, yi, Vi, xci, yci) {
    isAirplane = true;  // Установка типа аппарата
}

// Метод движения самолета
void TAircraft::move(float t, int a)
{
    // Вызов базового метода
    this->TLA::move(t, a);
    
    // Расчет угловой скорости
    double w = -V / R;
    
    // Обновление координат центра движения
    xc = -(x + R * cos(fi + w * t)) * (a - 1) + a * (xc - V * dt);
    yc = -(y + R * sin(fi + w * t)) * (a - 1) + yc * a;
}

// Реализация методов THelicopter
THelicopter::THelicopter(float xi, float yi, float Vi, float xci, float yci) :TLA(xi, yi, Vi, xci, yci) {
    isAirplane = false;  // Установка типа аппарата
}

// Метод движения вертолета
void THelicopter::move(float t, int a)
{
    // Вызов базового метода
    this->TLA::move(t, a);
    
    // Обновление координат центра движения
    xc = xc + (a * V * cos(fi) * dt);
    yc = yc + (a * V * sin(fi) * dt);
};

// Реализация методов TAirport
TAirport::TAirport(float xi, float yi, float li) :TAObject(xi, yi) {
    this->l = li;
    enableToLanding = true;
    
    // Инициализация массива ЛА
    #ifndef ONESTAR
        LA = new TLA * [n];  // Динамическое выделение памяти
    #endif

    // Генерация случайных параметров для ЛА
    float xc, yc, V;
    srand(time(0));
    for (int j = 0; j < ConstTLACount; j++) {
        xc = rand() % 10000 + 10000;
        yc = rand() % 1000 + 3000;
        V = rand() % 100 + 100;
        
        // Случайный выбор типа ЛА
        int type = rand() % 2;
        if (type) {
            LA[j] = new TAircraft(x, y, V, xc, yc);  // Создание самолета
        }
        else {
            LA[j] = new THelicopter(x, y, V, xc, yc); // Создание вертолета
        }
    }
}

// Основной метод симуляции
void TAirport::Do(float t0, float tk)
{
    // Открытие файла для записи результатов
    std::ofstream FileOutput("FileOutput.txt");
    int countOfLandings = 0;
    double bigX = x + 1.1 * l;  // Граница зоны посадки
    
    // Главный цикл симуляции по времени
    for (float i = t0; i <= tk; i += dt) {
        // Обработка каждого ЛА
        for (int j = 0; j < ConstTLACount; j++) {
            if (LA[j]->landed) continue;  // Пропуск уже приземлившихся
            
            // Обработка самолетов
            if (LA[j]->isAirplane) {
                if (LA[j]->a == 0) {
                    // Проверка условий для начала посадки
                    if ((!(this->enableToLanding && !(LA[j]->enableToLanding))) && 
                        (LA[j]->xc > bigX) && 
                        (abs(LA[j]->yc - this->y) < (this->l / 50))) {
                        LA[j]->a = 1;
                        this->enableToLanding = true;
                    }
                    else { 
                        LA[j]->a = 0; 
                    }
                }
                else {
                    // Проверка завершения посадки
                    if ((LA[j]->xc < (this->x + this->l)) && LA[j]->enableToLanding) {
                        LA[j]->landed = true;
                        this->enableToLanding = false;
                    }
                    else if (this->enableToLanding) { 
                        LA[j]->landed = false; 
                    }
                }
            }
            // Обработка вертолетов
            else {
                if (!LA[j]->a) {
                    // Проверка условий для начала посадки
                    if (!(this->enableToLanding && !(LA[j]->enableToLanding))) {
                        LA[j]->a = 1;
                        this->enableToLanding = true;
                    }
                    else { 
                        LA[j]->a = 0; 
                    }
                }
                else {
                    // Проверка завершения посадки
                    if (((pow((LA[j]->xc - this->x), 2) + pow((LA[j]->yc - this->y), 2)) < pow((this->l / 50), 2)) &&
                        LA[j]->enableToLanding) {
                        LA[j]->landed = true;
                        this->enableToLanding = false;
                    }
                    else { 
                        LA[j]->landed = false; 
                    }
                }
            }
            
            // Обновление положения ЛА
            LA[j]->move(i, LA[j]->a);

            // Обработка приземлившихся ЛА
            if (LA[j]->landed) {
                if (LA[j]->isAirplane) {
                    // Вывод информации о самолете
                    std::cout << "Aircraft number: " << j + 1 << " landed. landing time - " << i << std::endl;
                    FileOutput << "Aircraft number: " << j + 1 << " landed. landing time - " << i << std::endl;
                    countOfLandings++;
                }
                else {
                    // Вывод информации о вертолете
                    std::cout << "Helicopter number " << j + 1 << " landed. landing time - " << i << std::endl;
                    FileOutput << "Helicopter number " << j + 1 << " landed. landing time - " << i << std::endl;
                    countOfLandings++;
                }
            }
        }
        
        // Проверка завершения всех посадок
        if (countOfLandings == ConstTLACount) {
            std::cout << "all LA landed";
            break;
        }
        
        // Проверка истечения времени
        if (i == tk) {
            std::cout << "out of time; count of landed tla = " << countOfLandings << std::endl;
        }
    }

    FileOutput.close();  // Закрытие файла
}