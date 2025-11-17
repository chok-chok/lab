#include "substring.h"
#include <vector>
#include <string>
#include <unordered_map>

// алгоритм Бойера-Мура
std::vector<int> boyer_moore_search(const std::string& text, const std::string& pattern) {
    int n = text.size(); //длина текста
    int m = pattern.size(); //длина образца
    std::vector<int> result;

    if (m == 0 || n < m) return result;

    // Инициализируем все значения -1 (символ не встречается в образце)
    std::vector<int> bad_char(256, -1);
    
    // Для каждого символа в образце сохраняем его последнюю позицию
    for (int i = 0; i < m; ++i) {
        bad_char[(unsigned char)pattern[i]] = i;
    }

    int shift = 0;

    while (shift <= n - m) {
        // Сравниваем образец с текстом справа налево
        int j = m - 1;  // Начинаем с последнего символа образца
        
        // Пока символы совпадают, двигаемся влево
        while (j >= 0 && pattern[j] == text[shift + j]) {
            j--;
        }
        
        // Если дошли до начала образца - найдено совпадение
        if (j < 0) {
            // Добавляем позицию в результат
            result.push_back(shift);
            // Вычисляем следующее смещение:
            // Если после текущего образца есть символ в тексте,
            // сдвигаем на длину образца минус позиция этого символа в образце
            // Иначе сдвигаем на 1
            shift += (shift + m < n) ? m - bad_char[(unsigned char)text[shift + m]] : 1;
        } else {
            // Если не совпало, используем правило плохого символа:
            // Сдвигаем образец так, чтобы несовпавший символ в тексте
            // совпал с последним его вхождением в образце
            // Минимальный сдвиг - 1 (гарантия продвижения)
            shift += std::max(1, j - bad_char[(unsigned char)text[shift + j]]);
        }
    }
    return result;
}

// Рабина-Карпа
std::vector<int> rabin_karp_search(const std::string& text, const std::string& pattern) {
    const int d = 256; //размер алфавита
    const int q = 101; // простое число для хеширования
    int n = text.size(); //длина текста
    int m = pattern.size(); //длина подстроки для поиска
    std::vector<int> result;
    if (m == 0 || n < m) return result;

    // Вычисляем h = d^(m-1) mod q
    int h = 1;
    for (int i = 0; i < m-1; i++) {
        h = (h * d) % q;
    }

    int p = 0  // хеш образца
    int t = 0; // хеш текущего окна текста
    for (int i = 0; i < m; i++) {
        p = (d * p + pattern[i]) % q;  // Хеш образца: p = (d * p + pattern[i]) % q
        t = (d * t + text[i]) % q;     // Хеш первого окна текста: t = (d * t + text[i]) % q
    }

    for (int i = 0; i <= n - m; i++) {
        if (p == t) {
            if (text.substr(i, m) == pattern) { // При совпадении хешей делаем точное сравнение подстрок
                result.push_back(i);            // Если подстроки совпали, добавляем индекс в результат
            }
        }
        if (i < n - m) { // Пересчет хеша для следующего окна текста (если не достигли конца)
            // Формула пересчета хеша:
            // t = (d * (t - text[i] * h) + text[i + m]) % q
            t = (d * (t - text[i] * h) + text[i + m]) % q;
            if (t < 0) t += q;
        }
    }
    return result;
}

// Алгоритм Кнута-Морриса-Пратта
std::vector<int> kmp_search(const std::string& text, const std::string& pattern) {
    // Получаем длины текста и образца
    int n = text.size();
    int m = pattern.size();
    
    // Вектор для хранения результатов (индексов начала совпадений)
    std::vector<int> result;
    
    // Проверка крайних случаев:
    // - если образец пустой
    // - если текст короче образца
    if (m == 0 || n < m) return result;

    // Создаем массив lps (longest proper prefix which is also suffix)
    // lps[i] = длина наибольшего собственного префикса pattern[0..i], 
    // который также является суффиксом
    std::vector<int> lps(m, 0);

    // Препроцессинг образца - заполнение массива lps
    int len = 0;  // длина предыдущего наибольшего префикса-суффикса
    
    // Начинаем с i = 1, так как lps[0] всегда 0
    for (int i = 1; i < m; ) {
        // Если символы совпадают, увеличиваем длину префикса-суффикса
        if (pattern[i] == pattern[len]) {
            lps[i++] = ++len;
        }
        else {
            // Если не совпали и len != 0, используем предыдущее значение lps
            if (len != 0) {
                len = lps[len - 1];
            }
            else {
                // Если len == 0, префикса-суффикса нет
                lps[i++] = 0;
            }
        }
    }

    // Поиск образца в тексте
    int i = 0;  // индекс для текста
    int j = 0;  // индекс для образца
    
    while (i < n) {
        // Если символы совпадают, двигаемся дальше
        if (pattern[j] == text[i]) {
            j++;
            i++;
        }
        
        // Если весь образец совпал
        if (j == m) {
            // Добавляем позицию начала совпадения
            result.push_back(i - j);
            // Используем lps для продолжения поиска
            j = lps[j - 1];
        }
        // Если символы не совпали и не дошли до конца текста
        else if (i < n && pattern[j] != text[i]) {
            // Если j != 0, используем lps для сдвига образца
            if (j != 0) {
                j = lps[j - 1];
            }
            else {
                // Если j == 0, просто сдвигаемся по тексту
                i++;
            }
        }
    }
    return result;
}

// Поиск через конечный автомат
std::vector<int> finite_automaton_search(const std::string& text, const std::string& pattern) {
    int n = text.size(); //длина текста
    int m = pattern.size(); //длина образца
    
    std::vector<int> result;
    
    if (m == 0 || n < m) return result;

    const int NO_OF_CHARS = 256;
    
    std::vector<std::vector<int>> automaton(m + 1, std::vector<int>(NO_OF_CHARS, 0));

    // Построение таблицы переходов конечного автомата
    for (int state = 0; state <= m; ++state) {
        // Перебираем все возможные символы
        for (int x = 0; x < NO_OF_CHARS; ++x) {
            // Начальное предположение для следующего состояния
            int next_state = std::min(m, state + 1);
            
            // Ищем наибольший суффикс, который является префиксом образца
            while (next_state > 0) {
                // Формируем строку: текущий префикс + новый символ
                std::string current = pattern.substr(0, state) + (char)x;
                // Берем суффикс длины next_state
                std::string suffix = current.substr((state + 1) - next_state);
                // Сравниваем с префиксом образца
                if (pattern.substr(0, next_state) == suffix) {
                    break;
                }
                next_state--;
            }
            automaton[state][x] = next_state;
        }
    }

    // Поиск подстроки в тексте с использованием построенного автомата
    int state = 0;  
    for (int i = 0; i < n; i++) {
        // Переходим в следующее состояние на основе текущего символа
        state = automaton[state][(unsigned char)text[i]];
        if (state == m) {
            result.push_back(i - m + 1);
        }
    }
    return result;
}
