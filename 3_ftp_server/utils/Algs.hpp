#ifndef ALGS_HPP
#define ALGS_HPP

#include <array>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

class Algs {
   public:
    template <class ContainerT>
    static void tokenize(const std::string& str, ContainerT& tokens,
                         const std::string& delimiters = " ", bool trimEmpty = false) {
        std::string::size_type pos, lastPos = 0, length = str.length();

        using value_type = typename ContainerT::value_type;
        using size_type = typename ContainerT::size_type;

        while (lastPos < length + 1) {
            pos = str.find_first_of(delimiters, lastPos);
            if (pos == std::string::npos) {
                pos = length;
            }

            if (pos != lastPos || !trimEmpty)
                tokens.push_back(value_type(str.data() + lastPos,
                                            (size_type)pos - lastPos));

            lastPos = pos + 1;
        }
    }

    static std::string readFile(std::string path) {
        std::ifstream ifs(path);
        if (!ifs.good()) {
            return std::string();
        }

        return std::string((std::istreambuf_iterator<char>(ifs)),
                           (std::istreambuf_iterator<char>()));
    }

    static void writeFile(std::string& filename, char* content, int size) {
        exec("touch " + filename);
        std::ofstream binFile(filename, std::ios::out | std::ios::binary);
        binFile.write(content, size);
        binFile.close();
    }

    static bool exists(const std::string& name) {
        std::ifstream f(name.c_str());
        return f.good();
    }

    static std::string exec(std::string command) {
        const char* cmd = command.c_str();
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

    static int fastExp(int n, int e, int m) {
        if (e == 1) {
            return n % m;
        }
        bool even = e % 2 == 0;
        int num = fastExp(n, even ? e / 2 : e - 1, m);
        if (even) {
            return num * num % m;
        } else {
            return n * num % m;
        }
    }
};

#endif  // AGS_HPP