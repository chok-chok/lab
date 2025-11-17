#include <iostream>
#include "substring.h"

void print_result(const std::vector<int>& result) {
    for (int index : result) {
        std::cout << index << " ";
    }
    std::cout << "\n";
}

int main() {
    std::string text = "ababcabcababc";
    std::string pattern = "abc";

    std::cout << "Boyer-Moore: ";
    print_result(boyer_moore_search(text, pattern));

    std::cout << "Rabin-Karp: ";
    print_result(rabin_karp_search(text, pattern));

    std::cout << "KMP: ";
    print_result(kmp_search(text, pattern));

    std::cout << "Finite Automaton: ";
    print_result(finite_automaton_search(text, pattern));

    return 0;
}
