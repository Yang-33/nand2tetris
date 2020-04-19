#include <assert.h>
#include <cctype>
#include <fstream>
#include <glob.h>
#include <iostream>
#include <string>

#include "CompilationEngine.h"
#include "JackTokenizer.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << __func__ << " needs arguments 2." << std::endl;
        return 1;
    }
    std::string path;
    {
        std::string infilepath = argv[1];
        auto last_slash_index = infilepath.find_last_of('/');
        assert(last_slash_index != std::string::npos);
        path = infilepath.substr(0, last_slash_index + 1);
        std::string outfilepath = infilepath.substr(last_slash_index + 1); // /NAME?
        auto dot_index = outfilepath.find_last_of('.');
        if (dot_index == std::string::npos) {
            path = infilepath + "/";
        }
    }

    glob_t glob_result;
    std::string target = path + "*.jack";
    glob(target.c_str(), GLOB_TILDE, NULL, &glob_result);
    for (unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
        std::string filepath = glob_result.gl_pathv[i];
        std::string xmlpath = filepath.substr(0, filepath.size() - 5) + ".vm";

        nand2tetris::CompilationEngine compilation_engine(filepath, xmlpath);
        compilation_engine.compile();
        std::cerr << "JackAnalyzer created" << filepath << " into " << xmlpath << std::endl;
    }

    return 0;
}
