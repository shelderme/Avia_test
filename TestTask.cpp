// TestTask.cpp: определяет точку входа для приложения.
//

#include "TestTask.h"
#include <algorithm>
#include <cctype>


// функция получает строку, выводит код авиакомпании, извлеченный из полученной строки
std::string normalizeAirlineCode(const std::string& input) {
    // в случае пустой строки возвращаем ее
    if (input.empty()) return "";

    std::string code;
    bool hasLetter = false;
    size_t i = 0;

    // Обрабатываем до 3 символов (макс. длина кода)
    while (i < input.size() && code.size() < 3) {
        char c = input[i];
        // случай [A-Z][0-9].., 3 символ уже не должен относиться к коду авиакомпании
        if (code.size() == 2 && isdigit(code[1]) && !isdigit(code[0])) {
            return code;
        }

        if (isalnum(c)) {
            // случай [0-9][0-9], должна быть минимум одна буква
            if (i == 1 && isdigit(code[0]) && isdigit(c))
                return "";

            if (isalpha(c)) {
                if (isupper(c)) {  // Проверяем, что буква заглавная
                    hasLetter = true;
                    code += c;
                }
                else {
                    return "invalid";  // Строчная буква -> невалидно
                }
            }
            else {
                if (i == 2 && isdigit(c))
                    return code;
                code += c;  // Цифра
            }
            i++;
        }
        else if (c == ' ') {
            break;  // Пробел - конец кода
        }
        else {
            return "invalid";  // Недопустимый символ
        }
    }

    // Код должен содержать минимум 1 букву и длину >= 2
    return (hasLetter && code.size() >= 2) ? code : "";
}

// функция для извлечения номера рейса
std::string processFlightNumber(const std::string& input) {
    // Проверяем, что строка состоит только из цифр (1-5 символов)
    if (input.empty() || input.size() > 5 ||
        !std::all_of(input.begin(), input.end(), ::isdigit)) {
        return "invalid";
    }

    // Удаляем ведущие нули
    size_t firstNonZero = input.find_first_not_of('0');
    if (firstNonZero == std::string::npos) {
        return "invalid";  // Все цифры — нули
    }

    std::string result = input.substr(firstNonZero);
    return result;
}

// сравнение двух строк
bool compareFlightStrings(const std::string& a, const std::string& b) {
    unsigned int strMaxSize = 7; // максимальная длина строки(по тз)
    // в строке не должно быть более 1 пробела
    if (count(a.begin(), a.end(), ' ') > 1 || count(b.begin(), b.end(), ' ') > 1)
        return false;

    // случай невалидной длины строки
    if (a.size() > strMaxSize || b.size() > strMaxSize)
        return false;

    std::string codeA = normalizeAirlineCode(a);
    std::string codeB = normalizeAirlineCode(b);
    if (codeA == "invalid" || codeB == "invalid") {
        return false;
    }
    // Если коды не равны (и не оба пустые)
    if (codeA != codeB && !(codeA.empty() && codeB.empty())) {
        return false;
    }

    // Извлекаем номер рейса, удаляя пробел
    std::string remainingA = a.substr(codeA.size());
    remainingA.erase(std::remove(remainingA.begin(), remainingA.end(), ' '), remainingA.end());
    std::string numA = processFlightNumber(remainingA);

    std::string remainingB = b.substr(codeB.size());
    remainingB.erase(std::remove(remainingB.begin(), remainingB.end(), ' '), remainingB.end());
    std::string numB = processFlightNumber(remainingB);

    if (numA == "invalid" || numB == "invalid") return false;
    return numA == numB;
}

// вспомогательная функция, возвращет пару 



