// Header.txt - заголовочный файл с объявлениями классов
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

// Определения для режимов компиляции
//#define TEST       // Раскомментировать для тестового вывода
#define ONESTAR     // Использовать статический массив для LA

// Константы программы
const float dt = 1;               // Шаг времени симуляции
const int ConstTLACount = 9;      // Количество летательных аппаратов

// Базовый класс для всех объектов системы
class TAObject {
public:
    float x, y;                   // Координаты объекта
    bool enableToLanding = true;   // Флаг разрешения посадки

    // Конструктор и деструктор
    TAObject(float xi, float yi);
    ~TAObject();
};

// Базовый класс для летательных аппаратов (наследник TAObject)
class TLA : public TAObject {
public:
    float xc, yc;                 // Координаты центра движения
    float R, fi;                  // Радиус и угол в полярных координатах
    float V;                      // Скорость
    bool landed = false;          // Флаг завершения посадки
    bool isAirplane;              // Тип аппарата (самолет/вертолет)
    int a = 0;                    // Флаг активности движения
    int b = 0;                    // Вспомогательная переменная для расчета угла

    // Конструктор и методы
    TLA(float xi, float yi, float Vi, float xci, float yci);
    virtual void move(float t, int a);  // Виртуальный метод движения
    void RemakePhiNRadius();            // Пересчет полярных координат
};

// Класс самолета (наследник TLA)
class TAircraft : public TLA {
public:
    TAircraft(float xi, float yi, float Vi, float xci, float yci);
    void move(float t, int a) override; // Переопределение метода движения
};

// Класс вертолета (наследник TLA)
class THelicopter : public TLA {
public:
    THelicopter(float xi, float yi, float Vi, float xci, float yci);
    void move(float t, int a) override; // Переопределение метода движения
};

// Класс аэропорта (наследник TAObject)
class TAirport : public TAObject {
public:
    float l;                      // Длина взлетно-посадочной полосы

    // Массив указателей на летательные аппараты
    #ifdef ONESTAR
        TLA* LA[ConstTLACount];   // Статический массив
    #else
        TLA** LA;                 // Динамический массив
    #endif

    // Конструктор и метод симуляции
    TAirport(float xi, float yi, float li);
    void Do(float t0, float tk);  // Основной метод симуляции
};