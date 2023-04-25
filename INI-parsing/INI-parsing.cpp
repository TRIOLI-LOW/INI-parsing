// INI-parsing.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <Windows.h>

class ini_parser {
private:

    std::map<std::string, std::map<std::string, std::string>> sections_;

    // создаем метод для пропуска символов пробела, табуляции или переноса строки 
    std::string trim(const std::string& str) {
        std::string result = str;
        auto first = result.find_first_not_of(" \t\n");
        if (first == std::string::npos) {
            return "";
        }
        auto last = result.find_last_not_of(" \t\n");
        result = result.substr(first, last - first + 1);
        return result;
    }


public:
    ini_parser(const std::string& ini) {
        std::ifstream file(ini);
        //проверяем на открытие файла или выбрасываем исключение
    
        if (!file.is_open()) {
            throw std::runtime_error("File is not open");
        }
        std::string line = "";
        std::string current_section = "";
        line = trim(line);
        // создаем цикл построчного чтения
        while (std::getline(file, line)) {
            try {
                if (line.empty() || line[0] == ';') {
                    // пропускаем строку
                    continue;
                }
                else {
                    // проверям на наличие коментария и записываем в строку всё, до знака ';'
                    std::string line1 = "";
                    for (const char c : line) {
                        if (c == ';') {
                            break;
                        }
                        line1 += c;
                    }
                    line = line1;
                }
                //проверяем на имя сектора и записываем в строку current_section
                if (line[0] == '[' && line.back() == ']') {
                    current_section = line.substr(1, line.size() - 2);
                }
                else {
                    // иначе создаем пара-ключ значение
                    std::string key = " ", value = " ";
                    auto pos = line.find('=');
                    if (pos == std::string::npos) {
                        throw std::runtime_error("Invalid key-value pair in: " + current_section);
                    }
                    key = line.substr(0, pos);
                    value = line.substr(pos + 1);
                    if (key.empty() || value.empty()) {
                        throw std::runtime_error("Invalid key-value pair in: " + line);
                    }
                    sections_[current_section][key] = value;
                }
            }
            catch (const std::exception& e ){
                std::cerr << "Error: " << e.what() << std::endl;
            }

        }



    }
    template<typename T>
    T get_value(const std::string& section_key) {
        // делим строку на пара-ключ значение
        auto pos = section_key.find('.');
        if(pos == std::string::npos) {
            throw std::runtime_error("Invalid section_key:" + section_key);
        }
        else {
            auto section = section_key.substr(0, pos);
            auto key = section_key.substr(pos + 1);
            if (sections_.count(section) == 0 || sections_.at(section).count(key) == 0) {
                throw std::runtime_error("Key not found " + section_key);
            }
            std::stringstream ss(sections_.at(section).at(key));
            T value;
            ss >> value;
            return value;
        }
    }


};



    int main()
    {
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
        try {  
            ini_parser parser("input.ini");
            auto value = parser.get_value<std::string>("Section1.var1");// проблемы только с Section1 , не могу понять почему другие значения
            std::cout << value;
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
        


        return 0;
    }
