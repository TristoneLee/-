/*
 * File: parser.cpp
 * ----------------
 * Implements the parser.h interface.
 */

#include <iostream>
#include <string>

#include "exp(done).h"
#include "parser(done).h"

#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/strlib.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "statement(todo).h"

using namespace std;

/*
 * Implementation notes: parseExp
 * ------------------------------
 * This code just reads an expression and then checks for extra tokens.
 */

Expression *parseExp(TokenScanner &scanner) {
    Expression *exp = readE(scanner);
    if (scanner.hasMoreTokens()) {
        error("parseExp: Found extra token: " + scanner.nextToken());
    }
    return exp;
}

/*
 * Implementation notes: readE
 * Usage: exp = readE(scanner, prec);
 * ----------------------------------
 * This version of readE uses precedence to resolve the ambiguity in
 * the grammar.  At each recursive level, the parser reads operators and
 * subexpressions until it finds an operator whose precedence is greater
 * than the prevailing one.  When a higher-precedence operator is found,
 * readE calls itself recursively to read in that subexpression as a unit.
 */

Expression *readE(TokenScanner &scanner, int prec) {
    Expression *exp = readT(scanner);
    string token;
    while (true) {
        token = scanner.nextToken();
        int newPrec = precedence(token);
        if (newPrec <= prec) break;
        Expression *rhs = readE(scanner, newPrec);
        exp = new CompoundExp(token, exp, rhs);
    }
    scanner.saveToken(token);
    return exp;
}

/*
 * Implementation notes: readT
 * ---------------------------
 * This function scans a term, which is either an integer, an identifier,
 * or a parenthesized subexpression.
 */

Expression *readT(TokenScanner &scanner) {
    string token = scanner.nextToken();
    TokenType type = scanner.getTokenType(token);
    if (type == WORD) if(token=="LET"||token=="PRINT"||token=="RUN"||token=="LET")  error ("SYNTAX ERROR");
    else return new IdentifierExp(token);
    if (type == NUMBER) return new ConstantExp(stringToInteger(token));
    if (token != "(") error("Illegal term in expression");
    Expression *exp = readE(scanner);
    if (scanner.nextToken() != ")") {
        error("Unbalanced parentheses in expression");
    }
    return exp;
}

/*
 * Implementation notes: precedence
 * --------------------------------
 * This function checks the token against each of the defined operators
 * and returns the appropriate precedence value.
 */

int precedence(string token) {
    if (token == "=") return 1;
    if (token == "+" || token == "-") return 2;
    if (token == "*" || token == "/") return 3;
    return 0;
}


Statement *parserInput(TokenScanner &scanner) {
    string name;
    name = scanner.nextToken();
    auto thisStatement = new InputSta(name);
    return thisStatement;
}

Statement *parserLet(TokenScanner &scanner) {
    auto exp = parseExp(scanner);
    auto thisStatement = new LetSta(exp);
    return thisStatement;
}

Statement *parserPrint(TokenScanner &scanner) {
    auto exp = parseExp(scanner);
    auto thisStatement = new PrintSta(exp);
    return thisStatement;
}

Statement *parserEnd(TokenScanner &scanner) {
    auto thisStatement = new EndSta();
    return thisStatement;
}

Statement *parserGoto(TokenScanner &scanner) {
    int destination = stringToInteger(scanner.nextToken());
    auto thisStatement = new GotoSta(destination);
    return thisStatement;
}

Statement *parserIf(TokenScanner &scanner) {
    string a=scanner.nextToken();
    string b;
    while (a!=">"&&a!="="&&a!="<"){
        b+=a;
        a=scanner.nextToken();
    }
    string comparison=a;
    TokenScanner scannerTmp;
    scannerTmp.setInput(b);
    Expression *lExp = parseExp(scannerTmp);
    string c;
    a=scanner.nextToken();
    while (a!="THEN"){
        c+=a;
        a=scanner.nextToken();
    }
    scannerTmp.setInput(c);
    Expression *rExp = parseExp(scannerTmp);
    int destination = stringToInteger(scanner.nextToken());
    auto thisStatement = new IfSta(lExp,rExp,comparison,destination);
    return thisStatement;
}

Statement *parserRem(TokenScanner &scanner) {
    return new RemSta();
}
