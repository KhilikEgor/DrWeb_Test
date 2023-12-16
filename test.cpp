#include <cstring>

void TrimRight(char *s) {
    if (s == nullptr) {
        return;
    }

    int length = strlen(s);

    if (length == 0) {
        return;
    }

    for (int i = length - 1; i >= 0; --i) {
        if (s[i] == ' ') {
            s[i] = '\0';
        } else {
            break;
        }
    }
}

// Пример использования:
#include <iostream>

int main() {
    const int maxLength = 1000000; 
    char* inputString = new char[maxLength];

    strcpy(inputString, "string string          ");

    TrimRight(inputString);

    std::cout << "Результат: '" << inputString << "'" << std::endl;

    delete[] inputString;

    return 0;
}
