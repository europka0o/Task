#include <iostream>
#include <sstream>

//CAT - система исчисления котиков
//BIN - двоичная
//OCT - восьмеричная
//HEX - шестнадцатиричная
std::string itoBase(uint32_t nb, std::string base);
std::string itoBase(std::string nb, std::string baseSrc, std::string baseDst);


std::string intToStr(int input) { //Парсер инта к строке
    std::ostringstream output;
    output << input;
    return output.str();
}

uint32_t strToInt(std::string input) { //Парсер строки к целочисленному беззначному
    uint32_t output;
    std::istringstream ist(input);
    ist >> output;
    return output;
}

int main()
{
    system("chcp 1251");


    std::string _baseSrc;//Система исчисления исходного числа
    std::string _baseDst;//Система исчисления, к которой преображают исходную 
    std::string _value; //Исходное число
    bool loop = true;
    
    while (loop) {

        std::cout << "Введите значени или exit если хотите выйти: ";
        std::cin >> _value;
        if (_value == "exit") {
            loop = false;
            break;
        }
        std::cout << "Введите исходную систему исчисления(BIN, OCT, DEC, HEX): ";
        std::cin >> _baseSrc;
        std::cout << "Введите систему исчисления к которой хотите преобразовать(BIN, OCT, DEC, HEX): ";
        std::cin >> _baseDst;

        std::cout << "Результат: " << itoBase(_value, _baseSrc, _baseDst) << std::endl; 
        std::cout << std::endl;
    }
    return 0;
}

std::string itoBase(uint32_t nb, std::string base) {

    std::string ret_string; //Выходная строка

    if (base == "CAT") { //Преобразование к котам
        if (nb > 0) {
            return intToStr(nb) + " котикиков";
        } else {
            return "4294967295 котикиков, потому что 0 котиков не бывает";
        }

    } else if (base == "BIN") { //Преобразование к двоичной системе

        char mass[32];
        for (int i = 0; i < 32; i++, nb <<= 1) {
            mass[i] = (48 + (nb >> 31));
        }
        return std::string{ mass, sizeof(mass) };

    } else if (base == "OCT") { //Преобразование к восьмеричной системе

        for (; nb; nb /= 8) {
            ret_string = char('0' + nb % 8) + ret_string;
        }
        return ret_string;

    } else if (base == "HEX") { //Преобразование к шестнадцатеричной системе

        int rem = 0;
        while (nb > 0) {
            rem = nb % 16; 
            if (rem > 9) {
                switch (rem) {
                    case 10: ret_string = "A" + ret_string; break;
                    case 11: ret_string = "B" + ret_string; break;
                    case 12: ret_string = "C" + ret_string; break;
                    case 13: ret_string = "D" + ret_string; break;
                    case 14: ret_string = "E" + ret_string; break;
                    case 15: ret_string = "F" + ret_string; break;
                }
            } else {
                ret_string = char(rem + 48) + ret_string;    
            }
            nb = nb / 16;
        }
        return ret_string;

    } else if (base == "DEC") { //Если приобразование к самому себе
        return intToStr(nb);
    } else {
        return "usage"; //Если что-то введено не правильно
    } 
}

std::string itoBase(std::string nb, std::string baseSrc, std::string baseDst) {
    
    uint32_t value_i = 0; //Целочисленное значение, нужно для преобразования к остальным системам счисления

    /* 
        Сначала мы узнаем, какой системы счисления исходное значение, узнав, преобразуем его десятичному.
        Дальше мы функцией itoBase(uint32_t nb, std::string base) преобразуем полученное число в ту систему,
        которая указана в параметре baseDst.
    */

    if (baseSrc == "BIN") { //Если исходная система двоичная

        for (int i = 0; i < nb.length(); i++) {
            if ((nb[i] < '0') || (nb[i] > '1')) break;
            value_i <<= 1;
            value_i += nb[i] - '0';
        };

    } else if (baseSrc == "OCT") { //Если исходная система восьмеричная

        std::istringstream(nb) >> std::oct >> value_i;

    } else if (baseSrc == "DEC") { //Если исходная система десятичная

        value_i = strToInt(nb);

    } else if (baseSrc == "HEX") { //Если исходная система шестнадцатиричная
    
        std::istringstream(nb) >> std::hex >> value_i;

    }
    
    if (baseDst == "BIN" && baseSrc == "BIN") { //Если преобразовать к самому себе, то просто выведем исходное значение
        return nb;
    } else if (baseDst == "BIN") { //Преобразование к двоичной системе
        return itoBase(value_i, "BIN");
    }

    if (baseDst == "OCT" && baseSrc == "OCT") { //Если преобразовать к самому себе, то просто выведем исходное значение
        return nb;
    } else if (baseDst == "OCT") { //Преобразование к восьмеричной системе
        return itoBase(value_i, "OCT");
    }

    if (baseDst == "DEC") { //Так как исходное число у нас уже преобразованно в int, то просто парсим его в строку и возвращаем 
        return intToStr(value_i);
    }

    if (baseDst == "HEX" && baseSrc == "HEX") { //Если преобразовать к самому себе, то просто выведем исходное значение
        return nb;
    } else if (baseDst == "HEX") { //Преобразование к шестнадцатеричной системе
        return itoBase(value_i, "HEX");
    }

    return "usage"; //Если что-то введено не правильно
}