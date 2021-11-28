/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program(todo).h"

using namespace std;

Program::Program() {
    statementTable[1000000000]=new EndSta;
}

Program::~Program() {
    statementTable.clear();
    sourceTable.clear();
}

void Program::clear(EvalState &state) {
    statementTable.clear();
    sourceTable.clear();
    state.clearSymbolTable();
}

void Program::addSourceLine(int lineNumber, string line) {
    sourceTable[lineNumber]=line;
}

void Program::removeSourceLine(int lineNumber) {
    if(sourceTable.count(lineNumber)==0) return;
    sourceTable.erase(lineNumber);
}

string Program::getSourceLine(int lineNumber) {
    if(sourceTable.count(lineNumber)) return sourceTable[lineNumber];
    return "";
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    statementTable[lineNumber]=stmt;
}

int Program::getFirstLineNumber() {
    return statementTable.begin()->first;
}

int Program::getNextLineNumber(int lineNumber) {
    auto iter=statementTable.find(lineNumber);
    ++iter;
    return iter->first;
}

void Program::printLineList() {
    for (auto iter = sourceTable.begin();iter!=sourceTable.end();++iter) {
        cout << iter->second<<endl;
    }
}

void Program::executeStatement(EvalState &state,int &statementNumber,Program &program){
    statementTable[statementNumber]->execute(state,statementNumber,program);
}

void Program::removeParsedStatement(int lineNumber) {
    if(statementTable.count(lineNumber)==0) return;
    statementTable.erase(lineNumber);
}

int Program::getStatementNumber(){
    return statementTable.size();
}

bool Program::haveLine(int lineNumber) {
    return statementTable.count(lineNumber);
};
