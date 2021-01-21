#include <iostream>
#include <fstream>
#include <sstream>

class barrel {
private:
    int volume_max; //Максимальное значение жидкости в бочке
    int volume_current; //Текущее значение жидкости в бочке
public:
    barrel(); //Куонструктор по умолчанию 
    barrel(int vol_max, int vol_cur); //Конструктор с параметрами 

    void setVolMax(int new_vol_max); //Функция, задающая максимальное значение жидкости в бочке
    void setVolCur(int new_vol_cur); //Функция, задающая текущее значение жидкости в бочке
    bool addWater(int count); //Добовляет указанное кол-во жидкости в бочку, если указанное кол-во не помещается в бочку возвращает false
    bool scoopWater(int count); //Вычитает указанное кол-во жидкости из бочки, если указанное кол-во жидкости больше, чем находится в бочке, то возвращает false
    int getCurrentVolume(); //Возвращает текущее значение жидксти в бочке
    int getMaxVolume(); //Возвращает максимально возможное кол-во жидкости в бочке
};

std::string SearchLog(std::string path, std::string dat_tim_start, std::string dat_tim_end);
unsigned __int64 strToInt(std::string input);

int main()
{
    system("chcp 1251");

    std::string _path;
    std::string _dat_tim_start;
    std::string _dat_tim_end;
    std::cout << "Введите путь к логу\n";
    std::cin >> _path;
    std::cout << "Введите начальную дату поиска, дата имет следующий формат \n";
    std::cout << "       2020-11-21-23-05-19\n";
    std::cout << "   ______|   |  |  |  | |\n";
    std::cout << "  |     _____|  |  |  | |____\n";
    std::cout << "  |    |     ___|  |  |      |\n";
    std::cout << "  |    |    |     _|  |      |\n";
    std::cout << "  |    |    |    |    |      |\n";
    std::cout << " год-месяц-день-час-минута-секунда\n";
    std::cin >> _dat_tim_start;
    std::cout << "Введите конечную дату поиска, дата имет такой же вид\n";
    std::cin >> _dat_tim_end;

    std::cout << SearchLog(_path, _dat_tim_start, _dat_tim_end) << std::endl;

    
    getchar();
    return 0;
}

std::string SearchLog(std::string path, std::string dat_tim_start, std::string dat_tim_end) {
    std::ifstream file; //Создаем поток, который будет считывать данные с файла лога 
    file.open(path, std::ios::in);

    if (!file.is_open()) { //Если файл не удается открыть, значит путь указан неверно, возвращаем usage
        return "usage";
    }

    std::ofstream file_out("return.csv"); //Создаем второй поток, который будет записывать результат в файл return.csv 

    barrel *bar = new barrel; //создаем класс, хранящий значение воды в бочке

    char mass[128]; //Массив символов для буферного хранения строки, получаемой из лога
    std::string st_log; //Строка принимает mass

    std::string date_time_start_str = dat_tim_start; //Начальные дата и время для поиска нужных записей по логу
    unsigned __int64 date_time_start_i = 0; 
    std::string date_time_current_str; //Текущая дата и время записи лога
    unsigned __int64 date_time_current_i = 0;
    std::string date_time_end_str = dat_tim_end; //Конечные дата и время для поиска нужных записей по логу
    unsigned __int64 date_time_end_i = 0;
    
    if (date_time_start_str.length() != 19 && date_time_end_str.length() != 19) { //Если входные строки даты и время меньше либо больше 20, то значит они введены не правильно 
        return "usage";
    }

    for (int i = 0; i < date_time_start_str.length(); i++) { //Удаляем из строк даты и времени символ -, тем самым делая её одним сплошным числом
        if (date_time_start_str[i] == '-') {
            date_time_start_str.erase(i, 1);
            date_time_end_str.erase(i, 1);
        } else { //Если в строках содержиться символ не равный цифре, то строка не задана не правильно, возвращаем usage
            if (date_time_start_str[i] != '0' && date_time_start_str[i] != '1' && date_time_start_str[i] != '2' && date_time_start_str[i] != '3' &&
                date_time_start_str[i] != '4' && date_time_start_str[i] != '5' && date_time_start_str[i] != '6' && date_time_start_str[i] != '7' &&
                date_time_start_str[i] != '8' && date_time_start_str[i] != '9') {
                return "usage";
            }
            if (date_time_end_str[i] != '0' && date_time_end_str[i] != '1' && date_time_end_str[i] != '2' && date_time_end_str[i] != '3' &&
                date_time_end_str[i] != '4' && date_time_end_str[i] != '5' && date_time_end_str[i] != '6' && date_time_end_str[i] != '7' &&
                date_time_end_str[i] != '8' && date_time_end_str[i] != '9') {
                return "usage";
            }
        }
        
    }

    date_time_start_i = strToInt(date_time_start_str); //Парсим строку начальной даты и времени в uint64, для более удобного и быстрого сравнения диапазона дат
    
    date_time_end_i = strToInt(date_time_end_str); //Парсим строку конечной даты и времени в uint64, для более удобного и быстрого сравнения диапазона дат

    int count_scoop_all = 0; //Счетчик кол-ва всех попыток вычерпываний
    int count_scoop_false = 0; //Счетчик кол-ва всех проваленных попыток вычерпываний
    int count_scoop_value_true = 0; //Счетчик кол-ва литров всех успешных попыток вычерпываний
    int count_scoop_value_false = 0; //Счетчик кол-ва литров всех проваленных попыток вычерпываний 
    int count_add_all = 0; //Счетчик кол-ва всех попыток добовления воды
    int count_add_false = 0; //Счетчик кол-ва всех проваленных попыток добовления воды
    int count_add_value_true = 0; //Счетчик кол-ва литров всех успешных попыток добовления воды
    int count_add_value_false = 0; //Счетчик кол-ва литров всех проваленных попыток добовления воды
    int start_bar_v_cur = 0; //Текущее значение воды в бочке

    std::string buff_str;
    
    int meta_loop = 0;

    while (!file.eof()) { //Пока файл считывается
        file.getline(mass, sizeof(mass)); //Получаем следующую линию файла
        st_log = mass;

        if (st_log.find("-meta") != std::string::npos) { //Если есть мета данные, то определяем их положение и присваиваем необходимым переменным

            meta_loop = st_log.find(":") + 1;

            while (st_log[meta_loop] == ' ' && st_log[meta_loop] != '\n') {
                meta_loop++;
            }

            while (st_log[meta_loop] != ' ' && st_log[meta_loop] != '\n' && st_log[meta_loop] != ')') {
                buff_str += st_log[meta_loop];
                meta_loop++;
            }

            meta_loop = strToInt(buff_str);
            buff_str = "";

            if (st_log.find("br_v_max") != std::string::npos) {
                bar->setVolMax(meta_loop);
            } else if (st_log.find("br_v_cur") != std::string::npos) {
                bar->setVolCur(meta_loop);
                start_bar_v_cur = bar->getCurrentVolume();
            }
        }
        if (st_log.find("[") != std::string::npos) { //Считывание текущей даты из полученной строки лога

            for (int i = st_log.find("[") + 2; i < st_log.length(); i++) { //Определяем положение даты и времени и заносим их в date_time_current_str
                if (st_log[i] != ' ') {
                    if (st_log[i] != '-') {
                        date_time_current_str += st_log[i];
                    }                  
                } else {
                    break;
                }
            }

            date_time_current_i = strToInt(date_time_current_str); //Парсим в uint64, для более удобного и быстрого сравнения диапазона дат
            date_time_current_str = ""; //Обнуляем строку с текущей датой и временем, так как будем сравнивать их в int

            if (!(date_time_current_i >= date_time_start_i && date_time_current_i <= date_time_end_i)) { //Если текущая дата и время не попадает в диапазон, то игнорируем строку лога и переходим на след итерацию цикла 
                break;
            }
        }
        if (st_log.find("добавить") != std::string::npos) { //Если нашли в строке добавить
            count_add_all++;
            if (st_log.find("успешно") != std::string::npos) { //Если добавить успешно

                if (st_log.find("литров") != std::string::npos) { //Определяем позицию цифр, котрые заносим в переменную count_add_value_true

                    for (int i = st_log.find("литров") - 2; i > 0; i--) {
                        if (st_log[i] != ' ') {
                            buff_str.insert(0, std::string(1, st_log[i]));
                        } else {
                            break;
                        }
                    }               
                    count_add_value_true += strToInt(buff_str);
                    bar->addWater(strToInt(buff_str));
                    buff_str = "";
                }
            } else { //Если добавить фейл
                count_add_false++;
                if (st_log.find("литров") != std::string::npos) {

                    for (int i = st_log.find("литров") - 2; i > 0; i--) { //Определяем позицию цифр, котрые заносим в переменную count_add_value_false
                        if (st_log[i] != ' ') {
                            buff_str.insert(0, std::string(1, st_log[i]));
                        } else {
                            break;
                        }
                    }
                    count_add_value_false += strToInt(buff_str);
                    buff_str = "";
                }   
            }
        }

        if (st_log.find("вычерпать") != std::string::npos) { //Если нашли в строке вычерпать
            count_scoop_all++;
            if (st_log.find("успешно") != std::string::npos) { //Если нашли в строке вычерпать успешно

                if (st_log.find("литров") != std::string::npos) { //Определяем позицию цифр, котрые заносим в переменную count_scoop_value_true

                    for (int i = st_log.find("литров") - 2; i > 0; i--) {
                        if (st_log[i] != ' ') {
                            buff_str.insert(0, std::string(1, st_log[i]));
                        } else {
                            break;
                        }
                    }
                    
                    count_scoop_value_true += strToInt(buff_str);
                    bar->scoopWater(strToInt(buff_str));
                    buff_str = "";
                }
            } else { //Если нашли в строке вычерпать файл
                count_scoop_false++;
                if (st_log.find("литров") != std::string::npos) { //Определяем позицию цифр, котрые заносим в переменную count_scoop_value_false

                    for (int i = st_log.find("литров") - 2; i > 0; i--) {
                        if (st_log[i] != ' ') {
                            buff_str.insert(0, std::string(1, st_log[i]));
                        } else {
                            break;
                        }
                    }
                    count_scoop_value_false += strToInt(buff_str);
                    buff_str = "";
                }   
            }
        }
        
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////---------Запись в файл
    float pr_vl = (float(count_add_false) / float(count_add_all)) * 100; //Вычисляем процент ошибок для добавления воды

    file_out << "Всего было попыток добавить воду в бочку;" << "\"" << count_add_all << "\"" << std::endl;
    file_out << "\"Процент ошибки, при попытке добавить воду\";" << "\"" << pr_vl << "\"" << std::endl;
    file_out << "Было налито литров воды в бочку;" << "\"" << count_add_value_true << "\"" << std::endl;
    file_out << "Не было налито литров воды в бочку;" << "\"" << count_add_value_false << "\"" << std::endl;
     
    pr_vl = (float(count_scoop_false) / float(count_scoop_all)) * 100; //Вычисляем процент ошибок для вычерпывания воды

    file_out << "Всего было попыток вычерпать воду из бочки;" << "\"" << count_scoop_all << "\"" << std::endl;
    file_out << "\"Процент ошибки, при попытке вычерпать воду\";" << "\"" << pr_vl << "\"" << std::endl;
    file_out << "Было вычерпано литров воды из бочки;" << "\"" << count_scoop_value_true << "\"" << std::endl;
    file_out << "Не было вычерпано литров воды из бочки;" << "\"" << count_scoop_value_false << "\"" << std::endl;

    file_out << "В начале в бочке было литров воды;" << "\"" << start_bar_v_cur << "\"" << std::endl;
    file_out << "В конце в бочке осталось литров воды;" << "\"" << bar->getCurrentVolume() << "\"" << std::endl;

    delete bar;
    
    file.close();
    file_out.close();

    return "В текущей директории создан файл \"return.csv\""; //Если всё прошло успешно возвращаем эту строку
}

unsigned __int64 strToInt(std::string input) { //Парсер строки к целочисленному беззначному
    unsigned __int64 output;
    std::istringstream ist(input);
    ist >> output;
    return output;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////class barrel
barrel::barrel() : volume_max(0), volume_current(0) {}
barrel::barrel(int vol_max, int vol_cur) : volume_max(vol_max), volume_current(vol_cur) {}

void barrel::setVolMax(int new_vol_max) {
    volume_max = new_vol_max;
}

void barrel::setVolCur(int new_vol_cur) {
    volume_current = new_vol_cur;
}

bool barrel::addWater(int count) {
    if (count + volume_current > volume_max) {
        return false;
    }
    else {
        volume_current += count;
        return true;
    }
}

bool barrel::scoopWater(int count) {
    if (volume_current - count < 0) {
        return false;
    }
    else {
        volume_current -= count;
        return true;
    }
}

int barrel::getCurrentVolume() {
    return volume_current;
}

int barrel::getMaxVolume() {
    return volume_max;
}
