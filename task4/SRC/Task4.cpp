#include <iostream>
#include <vector>

std::string regx(std::string first, std::string second);

int main()
{
    system("chcp 1251");

    std::string _first;
    std::string _second;

    while (true) {

        std::cout << "Введите первую строку или exit если хотите выйти: ";
        std::cin >> _first;

        if (_first == "exit") {
            break;
        }

        std::cout << "Введите вторую строку: ";
        std::cin >> _second;

        std::cout << regx(_first, _second) << std::endl;
    }

    return 0;
}

std::string regx(std::string first, std::string second) {

    std::string vect_str;
    std::vector<std::string> buff;
    
    bool ret_value_b = false;

    if (first == second) { //Если в строке нет * и они равны, то задаем true и переходим в конец функции
        ret_value_b = true;
    } else {

        if (second[second.length() - 1]  != '*')  { //Если во второй строке последний символ не звёздочка и он не совпадает
            if (first[first.length() - 1] != second[second.length() - 1]) { //с последним символом в первой строке, то эти строки уже разные 
                goto exit;                                                 //и можно переходить в конец на метку exit 
            }
        }

        for (int i = 0; i < second.length();) { //Если в строке все символы *, то мы просто переходим на метку exit и возвращаем ОК
            if (second[i] != '*' ) { //Отделяем остальные симоволы строки от звездочек
                vect_str += second[i]; //Символы, которые не разделены звездочкой записываем в одну строку подряд
                i++;
            } else {
                buff.push_back(vect_str); //Пушим полученные строки в вектор 
                vect_str = "";
                while (second[i] == '*' && i < second.length()) { //Пропускаем звёздочки, если их идет много подряд 
                    i++;
                }
            }
        }

        if (vect_str != "") { //Если после выхода из предыдущего цикла строка не пуста, то пушим её в вектор
            buff.push_back(vect_str);
            vect_str = "";
        }

        int current_value = 0;

        int first_str_iter = 0, second_str_iter = 0;
        for (int i = 0; i < buff.size(); i++) {

            vect_str = buff[i]; //Кидаем i элемент вектора в строку, для более удобной работы сравнения символов 
            
            while (first_str_iter != first.length() && second_str_iter != vect_str.length()) { //Сравниваем один символ строки со всеми символами исходной первой строки,
                if (vect_str[second_str_iter] == first[first_str_iter]) {                     //если строка состоит всего из одного симола, то по нахождению такого же
                    first.erase(first_str_iter, 1);                                          //символа в первой строке, мы удаляем его из первой строки, если же вся исходная строка
                                                                                            //была перебрана, а символ не найден, то переходим на метку exit и выдаем значение КО
                    if (first == "") { 
                        ret_value_b = true;
                        goto exit;
                    }

                    if (buff[i].size() >= 2) { //Если в строке два или больше символа, то переходим на следующий символ
                        current_value++;
                        second_str_iter++;
                        continue;
                    } else {
                        break;
                    }
                    
                } else {
                    if (first == "" || current_value > 0) { //Если исходная строка уже пуста или следующий символ не был найден, то переходим на метку exit и выдаем значение КО
                        goto exit;
                    }
                    first.erase(first_str_iter, 1); //Удаляем найденный символ из строки
                    if (buff[i].size() >= 2) {
                        second_str_iter++;
                    }
                }
                if ((first == "" && second_str_iter < buff[i].size()) || (first == "" && i < buff.size())) { //Если исходная строка пуста и строка не была полностью перебрана или
                    goto exit;                                                                              //в векторе ещё остались элементы, а строка уже пуста, то переходим к метке exit
                }
            }
            first_str_iter = 0, second_str_iter = 0, current_value = 0;
        }
        ret_value_b = true; //Если все циклы и условия пройдены успешно, то строки равны
    }
    
    exit:
    
    if (ret_value_b) { //Определяем что вернуть
        return "ОК";
    } else {
        return "КО";
    }
}

