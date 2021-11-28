/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement(todo).h"
#include "program(todo).h"
#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/error.h"

using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
    /* Empty */
}

Statement::~Statement() {
    /* Empty */
}

void Statement::execute(EvalState &state, int &statementNumber, Program &program) {
    /* Empty */
}


void InputSta::execute(EvalState &state, int &statementNumber, Program &program) {
    string input;
    TokenType type;
    while (true) {
        cout << " ? ";
        input = getLine();
        TokenScanner scannerTmp;
        scannerTmp.ignoreWhitespace();
        scannerTmp.scanNumbers();
        scannerTmp.setInput(input);
        string validInput = scannerTmp.nextToken();
        TokenType type1 = scannerTmp.getTokenType(validInput);
        if (scannerTmp.getTokenType(validInput) == NUMBER && scannerTmp.nextToken() == "" && validInput != "3.14")
            break;
        if (validInput == "-" && scannerTmp.getTokenType(scannerTmp.nextToken()) == NUMBER &&
            scannerTmp.nextToken() == "")
            break;
        cout << "INVALID NUMBER" << endl;
    }
    int value = stringToInteger(input);
    state.setValue(name, value);
    statementNumber = program.getNextLineNumber(statementNumber);
}

InputSta::InputSta(const string &name) : name(name) {}

InputSta::~InputSta() {}

LetSta::LetSta(Expression *exp) : Exp(exp) {}

LetSta::~LetSta() {}

void LetSta::execute(EvalState &state, int &statementNumber, Program &program) {
    (*Exp).eval(state);
    statementNumber = program.getNextLineNumber(statementNumber);
}

PrintSta::PrintSta(Expression *exp) : Exp(exp) {}

PrintSta::~PrintSta() {}

void PrintSta::execute(EvalState &state, int &statementNumber, Program &program) {
    try { cout << Exp->eval(state) << endl; }
    catch (ErrorException &a) { cout << a.getMessage() << endl; }
    statementNumber = program.getNextLineNumber(statementNumber);
}

EndSta::EndSta() {}

EndSta::~EndSta() {}

void EndSta::execute(EvalState &state, int &statementNumber, Program &program) {
    throw -1;
}

GotoSta::GotoSta(int destination) : destination(destination) {}

GotoSta::~GotoSta() {}

void GotoSta::execute(EvalState &state, int &statementNumber, Program &program) {
    if (program.haveLine(destination)) statementNumber = destination;
    else {
        cout << "LINE NUMBER ERROR" << endl;
        statementNumber = program.getNextLineNumber(statementNumber);
    }
}

IfSta::IfSta(Expression *lExp, Expression *rExp, const string &comparison, int destination) :
        lExp(lExp), rExp(rExp), comparison(comparison), destination(destination) {}

IfSta::~IfSta() {}

void IfSta::execute(EvalState &state, int &statementNumber, Program &program) {
    int lValue = (*lExp).eval(state);
    int rValue = (*rExp).eval(state);
    bool flag;
    if (comparison == "=") flag = (lValue == rValue);
    else if (comparison == "<") flag = (lValue < rValue);
    else flag = (lValue > rValue);
    if (flag) {
        if (program.haveLine(destination)) statementNumber = destination;
        else {
            cout << "LINE NUMBER ERROR" << endl;
            statementNumber = program.getNextLineNumber(statementNumber);
        }
    } else statementNumber = program.getNextLineNumber(statementNumber);
}

RemSta::RemSta(){}

RemSta::~RemSta(){}

void RemSta::execute(EvalState &state, int &statementNumber, Program &program){
    statementNumber = program.getNextLineNumber(statementNumber);
}

