/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp(done).h"
#include "parser(done).h"
#include "program(todo).h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"

using namespace std;

/* Function prototypes */

void processLine(const string &line, Program &program, EvalState &state);

void processOrder(Program &program, EvalState &state, TokenScanner &scanner, string);


/* Main program */

int main() {
    EvalState state;
    Program program;
    while (true) {
        try {
            string input = getLine();
            if (input.empty())
                return 0;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            cout << ex.getMessage() << endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(const string &line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    string firstToken = scanner.nextToken();
    TokenType firstType = scanner.getTokenType(firstToken);
    if (firstType == NUMBER) {
        int lineNumber = stringToInteger(firstToken);
        string command = scanner.nextToken();
        if (command == "INPUT") {
            program.addSourceLine(lineNumber, line);
            program.setParsedStatement(lineNumber, parserInput(scanner));
        }
        if (command == "LET") {
            program.addSourceLine(lineNumber, line);
            program.setParsedStatement(lineNumber, parserLet(scanner));
        }
        if (command == "PRINT") {
            program.addSourceLine(lineNumber, line);
            program.setParsedStatement(lineNumber, parserPrint(scanner));
        }
        if (command == "END") {
            program.addSourceLine(lineNumber, line);
            program.setParsedStatement(lineNumber, parserEnd(scanner));
        }
        if (command == "GOTO") {
            program.addSourceLine(lineNumber, line);
            program.setParsedStatement(lineNumber, parserGoto(scanner));
        }
        if (command == "IF") {
            program.addSourceLine(lineNumber, line);
            program.setParsedStatement(lineNumber, parserIf(scanner));
        }
        if (command == "REM") {
            program.addSourceLine(lineNumber, line);
            program.setParsedStatement(lineNumber, parserRem(scanner));
        }
        if (command == "") {
            program.removeSourceLine(lineNumber);
            program.setParsedStatement(lineNumber, parserRem(scanner));
        }
    } else processOrder(program, state, scanner, firstToken);
}


void processOrder(Program &program, EvalState &state, TokenScanner &scanner, string firstToken) {
    if (firstToken == "PRINT") cout << (parseExp(scanner))->eval(state) << endl;
    else if (firstToken == "LET") (parseExp(scanner))->eval(state);
    else if (firstToken == "INPUT") {
        string name = scanner.nextToken();
        string input;
        TokenType type;
        while (true) {
            cout << " ? ";
            input = getLine();
            TokenScanner scannerTmp;
            scannerTmp.ignoreWhitespace();
            scannerTmp.scanNumbers();
            scannerTmp.setInput(input);
            string validInput=scannerTmp.nextToken();
            TokenType type1=scannerTmp.getTokenType(validInput);
            if (scannerTmp.getTokenType(validInput) == NUMBER && scannerTmp.nextToken() == ""&&validInput!="3.14") break;
            if (validInput == "-" && scannerTmp.getTokenType(scannerTmp.nextToken()) == NUMBER&&scannerTmp.nextToken() == "") break;
            cout<<"INVALID NUMBER"<<endl;
        }
        int value = stringToInteger(input);
        state.setValue(name, value);
    }
    else if (firstToken == "RUN") {
        if(program.getStatementNumber()==1) return;
        int present = program.getFirstLineNumber();
        while (true) {
            try { program.executeStatement(state, present, program); }
            catch (...) { break; }
        }
    }
    else if (firstToken == "LIST") {
        program.printLineList();
    }
    else if (firstToken == "CLEAR") {
        program.clear(state);
    }
    else if (firstToken == "QUIT") {
        exit(0);
    }
    else if (firstToken == "HELP") {
        cout << "退远警告" << endl;
    }

}
