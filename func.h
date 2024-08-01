#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_VARIABLES 100
#define MAX_INPUT_LENGTH 100
#define MAX_ARRAY_LENGTH 100

struct Variable {
    char name;
    int intValue;             
    char* stringValue;      
    int arrayValues[MAX_ARRAY_LENGTH];
    int isArray;             
    int arrayLength;    
};
struct Variable variables[MAX_VARIABLES];
int variableCount;
void assignIntegerVariable(char name, int value);
void assignStringVariable(char name, const char* value);
struct Variable* findVariable(char name);
struct Variable* findarrVariable(char name);
int precedence(char op);
int applyOperation(int a, char op, int b);
void assignArrayVariable(char name, const int* values, int length);
void displayarrVariable(char name, int index);
void assignStringVariable(char name, const char* value);
void getInput(char name, int isString);
void displayVariable(char name);
int isStringVariable(char name);
void concatenateStrings(char resultVar, char var1, char var2);
int evaluateExpression(const char* expression, struct Variable* variables);
int strlenString(char var);
int evaluateCondition(const char* condition, struct Variable* variables);
void executeIfLoop(const char* input1, struct Variable* variables);
void executewhileLoop(const char* input1, struct Variable* variables);
void executedoWhileLoop(const char* input1, struct Variable* variables);
void executeStatement(const char* statement, struct Variable* variables);
void executeForLoop(const char* input1, struct Variable* variables);
