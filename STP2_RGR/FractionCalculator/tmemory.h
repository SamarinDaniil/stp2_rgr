#ifndef TMEMORY_H
#define TMEMORY_H

#include <utility>

template <typename T>
class TMemory {
private:
    T number;
    bool state;

public:
    TMemory() : number(), state(false) {}

    // Геттеры
    const T& FNumber() const {
        return number;
    }
    bool FState() const {
        return state;
    }

    // Сеттер (если нужен прямой доступ)
    void SetFNumber(const T& value) {
        number = value;
        state = true;
    }

    // Методы управления памятью
    void Store(const T& num) { // MS - Memory Store
        number = num;
        state = true;
    }

    T Recall() const { // MR - Memory Recall
        return number; // Возвращаем копию
    }

    void Add(const T& num) { // M+ - Memory Add
        if (state) {
            number = number.Add(num);
        } else {
            // Если память пуста, M+ действует как MS
            Store(num);
        }
    }

    void Clear() { // MC - Memory Clear
        number = T(); // Сброс к значению по умолчанию
        state = false;
    }


    std::pair<T, bool> Edit(int command, const T& newNumber) {
        switch (command) {
            case 0: // MS
                Store(newNumber);
                break;
            case 1: // M+
                Add(newNumber);
                break;
            case 2: // MC
                Clear();
                break;
            default:
                break;
        }
        return {number, state};
    }
};

#endif // TMEMORY_H
