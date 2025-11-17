#ifndef SUBSTRING_H
#define SUBSTRING_H

#include <vector>
#include <string>

// Алгоритм Бойера-Мура
std::vector<int> boyer_moore_search(const std::string& text, const std::string& pattern);

// Алгоритм Рабина-Карпа
std::vector<int> rabin_karp_search(const std::string& text, const std::string& pattern);

// Алгоритм Кнута-Морриса-Пратта
std::vector<int> kmp_search(const std::string& text, const std::string& pattern);

// Поиск с помощью конечного автомата
std::vector<int> finite_automaton_search(const std::string& text, const std::string& pattern);

#endif // SUBSTRING_H
