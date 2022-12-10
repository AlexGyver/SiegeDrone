/*
    Библиотека для удобного управления коллекторными моторами через драйвер
    Документация: https://alexgyver.ru/gyvermotor/
    GitHub: https://github.com/GyverLibs/GyverMotor
    Возможности:
    - Контроль скорости и направления вращения
    - Работа с ШИМ любого разрешения
    - Плавный пуск и изменение скорости
    - Активный тормоз
    - Порог минимального ШИМ
    - Deadtime
    - Поддержка 5 типов драйверов
    
    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License
    
    Версии:
    v1.1 - убраны дефайны
    v1.2 - возвращены дефайны
    v2.0:
        - Программный deadtime
        - Отрицательные скорости
        - Поддержка двух типов драйверов и реле
        - Плавный пуск и изменение скорости
    v2.1: небольшие фиксы и добавления
    v2.2: оптимизация
    v2.3: добавлена поддержка esp (исправлены ошибки)
    v2.4: совместимость с другими библами
    v2.5: добавлен тип DRIVER2WIRE_NO_INVERT
    v3.0: переделана логика minDuty, добавлен режим для ШИМ любой битности
    v3.1: мелкие исправления
    v3.2: улучшена стабильность плавного режима
    v3.2.1: вернул run() в public
    v4.0: исправлен баг в GyverMotor. Добавлен GyverMotor2
*/

#ifndef _GyverMotor_h
#define _GyverMotor_h
#include <Arduino.h>
#define _SMOOTH_PRD 50		// таймер smoothTick, мс

enum GM_driverType {
    DRIVER2WIRE_NO_INVERT,	// двухпроводной драйвер, в котором при смене направления не нужна инверсия ШИМ
    DRIVER2WIRE,			// двухпроводной драйвер (направление + ШИМ)
    DRIVER3WIRE,			// трёхпроводной драйвер (два пина направления + ШИМ)
    RELAY2WIRE,				// реле в качестве драйвера (два пина направления)
};

#define NORMAL 0
#define REVERSE 1

enum GM_workMode {
    FORWARD,
    BACKWARD,
    STOP,
    BRAKE,
    AUTO = 0,
};

static const int8_t _GM_NC = -1;	// not connected

class GMotor {
public:
    GMotor(GM_driverType type, int8_t param1 = _GM_NC, int8_t param2 = _GM_NC, int8_t param3 = _GM_NC, int8_t param4 = _GM_NC);
    // три варианта создания объекта в зависимости от драйвера:
    // GMotor motor(DRIVER2WIRE, dig_pin, PWM_pin, (LOW/HIGH) )
    // GMotor motor(DRIVER3WIRE, dig_pin_A, dig_pin_B, PWM_pin, (LOW/HIGH) )
    // GMotor motor(RELAY2WIRE, dig_pin_A, dig_pin_B, (LOW/HIGH) )
    
    // установка скорости -255..255 (8 бит) и -1023..1023 (10 бит)
    void setSpeed(int16_t duty);
    
    // сменить режим работы мотора:	
    // FORWARD - вперёд
    // BACKWARD - назад
    // STOP - остановить
    // BRAKE - активный тормоз
    // AUTO - подчиняется setSpeed (-255.. 255)
    void setMode(GM_workMode mode);
    
    // направление вращения	
    // NORM - обычное
    // REVERSE - обратное
    void setDirection(bool direction);
    
    // дать прямую команду мотору (без смены режима)
    void run(GM_workMode mode, int16_t duty = 0);
    
    // установить минимальную скважность (при которой мотор начинает крутиться)
    void setMinDuty(int duty);
    
    // установить разрешение ШИМ в битах
    void setResolution(byte bit);
    
    // установить deadtime (в микросекундах). По умолч 0
    void setDeadtime(uint16_t deadtime);	
    
    // установить уровень драйвера (по умолч. HIGH)
    void setLevel(int8_t level);			
    
    // плавное изменение к указанной скорости (к значению ШИМ)
    void smoothTick(int16_t duty);
    
    // скорость изменения скорости
    void setSmoothSpeed(uint8_t speed);	
    
    // возвращает -1 при вращении BACKWARD, 1 при FORWARD и 0 при остановке и торможении
    int getState();

    // внутренняя переменная скважности для отладки
    int16_t _duty = 0;
    
    // свовместимость со старыми версиями
    // установить выход в 8 бит
    void set8bitMode();		

    // установить выход в 10 бит
    void set10bitMode();
    
protected:
    void setPins(bool a, bool b, int c);
    int16_t _dutyS = 0;
    int _minDuty = 0, _state = 0;;
    int8_t _digA = _GM_NC, _digB = _GM_NC, _pwmC = _GM_NC;
    bool _direction = false;
    int8_t _level = LOW;	// логика инвертирована!
    int _maxDuty = 255;
    GM_workMode _mode = FORWARD, _lastMode = FORWARD;
    GM_driverType _type;
    uint16_t _deadtime = 0;
    uint8_t _speed = 20;
    uint32_t _tmr = 0;
    float _k;
};
#endif