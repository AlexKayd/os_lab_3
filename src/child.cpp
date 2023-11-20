#include <iostream>
#include <string>
#include "simple.h"

int main(int argc, char* argv[]){

    std::string data = argv[0];
    int size = atoi(argv[1]);
    
    for (int i = 0; i < size; i++){

        // Определение знака числа
        if (data[i] == '-' || data[i] == 0 || data[i] == 1){
            return 0;
        }

        // Запись числа в строку null
        std::string null = "";
        null += data[i];
        i++;

        while (i < size && data[i] >= '0' && data[i] <= '9'){
            null += data[i];
            i++;
        }

        int num = std::stoi(null);

        // Проверяем число (простое или нет)
        if (!Simple(num)){
            std::cout << num << "\n";
        }
        else{
            return 0;
        }
    }

    return 0;
}