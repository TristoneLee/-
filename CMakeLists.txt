cmake_minimum_required(VERSION 3.16)
project(code)

set(CMAKE_CXX_STANDARD 17)

add_executable(code
        Basic/Basic.cpp
        "Basic/evalstate(todo).cpp"
        "Basic/exp(done).cpp"
        "Basic/parser(done).cpp"
        "Basic/program(todo).cpp"
        "Basic/statement(todo).cpp"
        StanfordCPPLib/tokenscanner.cpp
        StanfordCPPLib/error.cpp
        StanfordCPPLib/simpio.cpp
        StanfordCPPLib/strlib.cpp
        )
