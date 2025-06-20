#include "TestTask.h"
#include <fstream>
#include <thread>
#include <cassert>
#include <unordered_set>
#include <iostream>

std::pair<std::string, std::string> normalizeFlightString(const std::string& input) {
    std::string code, number;
    code = normalizeAirlineCode(input);

    if (code == "invalid")
        return { code, "invalid" };
    std::string remaining = input.substr(code.size());
    remaining.erase(std::remove(remaining.begin(), remaining.end(), ' '), remaining.end());
    number = processFlightNumber(remaining);

    return { code, number };
}


void processFile(const std::string& inputFile, const std::string& outputFile) {
    unsigned int strMaxSize = 7;
    // открываем входящий файл
    std::ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        std::cerr << "ERROR: can't open input file " << inputFile << std::endl;
        return;
    }

    // выходной
    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        std::cerr << "ERROR: can't create file " << outputFile << std::endl;
        inFile.close();  // Закрываем входной файл
        return;
    }

    // проверка, что файлы не пустые
    inFile.seekg(0, std::ios::end);
    if (inFile.tellg() == 0) {
        std::cerr << "This file is empty " << inputFile << std::endl;
    }
    inFile.seekg(0, std::ios::beg);  // Возвращаем указатель в начало

    // использую unordered_set для хранения уникальных комбинаций кода авиакомпании и номера рейса,
    // так как сможем гарантировать, что каждая комбинация будет уникальна 
    // + быстрый поиск для проверки
    std::unordered_set<std::string> uniqueEntries;
    std::string line;

    // чтение файла
    while (getline(inFile, line)) {
        // проверка на длинные строки
        if (line.length() > strMaxSize) {
            std::cerr << "The line is too long " << inputFile << std::endl;
            continue;
        }
        if (count(line.begin(), line.end(), ' ') > 1)
            continue;
        // получаем пару в виде код авиакомпании и номер рейса
        auto [code, number] = normalizeFlightString(line);
        if (number == "invalid" || code == "invalid")
            continue;
        std::string normalized = code + "|" + number;

        if (uniqueEntries.find(normalized) == uniqueEntries.end()) {
            // проверка записи в файл
            if (!(outFile << code + number << std::endl)) {
                std::cerr << "ERORR: can't write" << outputFile << std::endl;
                break;
            }
            uniqueEntries.insert(normalized);
        }
    }

    if (!inFile.eof() && inFile.fail()) {
        std::cerr << "ERORR: can't read " << inputFile << std::endl;
    }

}



int main(int argc, char* argv[]) 
{

    if (argc == 5) {
        try {
            // Запуск обработки в двух потоках
            std::thread t1(processFile, argv[1], argv[2]);
            std::thread t2(processFile, argv[3], argv[4]);

            t1.join();
            t2.join();
        }
        catch (const std::exception& e) {
            std::cerr << "ERROR: " << e.what() << "\n";
            return 2;
        }

    }
    else {
        std::thread t1(processFile, "1_in.txt", "1_out.txt");
        std::thread t2(processFile, "2_in.txt", "2_out.txt");
        // Ожидаем завершения потоков
        t1.join();
        t2.join();
    }
    return 0;
}
