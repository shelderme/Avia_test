// TestTask.h : включаемый файл для стандартных системных включаемых файлов
// или включаемые файлы для конкретного проекта.

#pragma once

#ifndef TESTTASK_H
#define TESTTASK_H

#include <string>
#include <utility> 

// Нормализация кода авиакомпании
std::string normalizeAirlineCode(const std::string& input);

// Нормализация номера рейса
std::string processFlightNumber(const std::string& input);

// Сравнение двух строк рейсов
bool compareFlightStrings(const std::string& a, const std::string& b);

#endif 