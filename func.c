#include "func.h"

//find variable
struct Variable* findVariable(char name) {
    for (int i = 0; i < variableCount; i++) {
        if (variables[i].name == name) {
            return &variables[i];
        }
    }
    return NULL;
}

//precedence function
int precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

//operation function
int applyOperation(int a, char op, int b) {
    switch (op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            if (b != 0)
                return a / b;
            else {
                printf("Division by zero is not allowed.\n");
                exit(EXIT_FAILURE);
            }
        default:
            printf("Invalid operator: %c\n", op);
            exit(EXIT_FAILURE);
    }
}

//assign  integer to a variable
void assignIntegerVariable(char name, int value) {
    struct Variable* var = findVariable(name);
    if (var) {
        var->intValue = value;
    } else if (variableCount < MAX_VARIABLES) {
        variables[variableCount].name = name;
        variables[variableCount].intValue = value;
        variableCount++;
    } else {
        printf("Error: Maximum variable count exceeded.\n");
        exit(EXIT_FAILURE);
    }
}

//find address of the array
struct Variable* findarrVariable(char name) {
    for (int i = 0; i < variableCount; i++) {
        if (variables[i].name == name) {
            return &variables[i];
        }
    }
    return NULL;
}


//  assign  array value to a variable
void assignArrayVariable(char name, const int* values, int length) {
    struct Variable* var = findVariable(name);
    if (var) {
        var->isArray = 1;
        var->arrayLength = length;
        for (int i = 0; i < length; i++) {
            var->arrayValues[i] = values[i];
        }
    } else if (variableCount < MAX_VARIABLES) {
        variables[variableCount].name = name;
        variables[variableCount].isArray = 1;
        variables[variableCount].arrayLength = length;
        for (int i = 0; i < length; i++) {
            variables[variableCount].arrayValues[i] = values[i];
        }
        variableCount++;
    } else {
        printf("Error: Maximum variable count exceeded.\n");
        exit(EXIT_FAILURE);
    }
}

//  display the value of an array variable
void displayarrVariable(char name, int index) {
    struct Variable* var = findarrVariable(name);
    if (var && var->isArray && index == -1) {
        printf("%c = {", name);
        for (int i = 0; i < var->arrayLength; i++) {
            printf("%d", var->arrayValues[i]);
            if (i < var->arrayLength - 1) {
                printf(", ");
            }
        }
        printf("}\n");
    } else if (var && var->isArray && index >= 0 && index < var->arrayLength) {
        printf("%c[%d] = %d\n", name, index, var->arrayValues[index]);
    } else {
        printf("Invalid array access.\n");
    }
}

// assign  string value to  variable
void assignStringVariable(char name, const char* value) {
    struct Variable* var = findVariable(name);
    if (var) {
        free(var->stringValue); 
        var->stringValue = strdup(value); 
    } else if (variableCount < MAX_VARIABLES) {
        variables[variableCount].name = name;
        variables[variableCount].stringValue = strdup(value);
        variableCount++;
    } else {
        printf("Error: Maximum variable count exceeded.\n");
        exit(EXIT_FAILURE);
    }
}

// get user input
void getInput(char name, int isString) {
    char buffer[MAX_INPUT_LENGTH];
    printf(">> Enter a value for %c: ", name);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    if (isString) {
        assignStringVariable(name, buffer);
    } else {
        assignIntegerVariable(name, atoi(buffer));
    }
}

// display variable
void displayVariable(char name) {
    struct Variable* var = findVariable(name);
    if (var) {
        if (var->stringValue) {
            printf(">> %s\n", var->stringValue);
        } else {
            printf(">> %d\n", var->intValue);
        }
    } else {
        printf("Variable %c is not defined.\n", name);
    }
}

//check string type
int isStringVariable(char name) {
    struct Variable* var = findVariable(name);
    return (var && var->stringValue != NULL);
}

// concatenate two strings
void concatenateStrings(char resultVar, char var1, char var2) {
    struct Variable* varA = findVariable(var1);
    struct Variable* varB = findVariable(var2);
    
    if (varA && varA->stringValue && varB && varB->stringValue) {
        char* concatenatedString = malloc(strlen(varA->stringValue) + strlen(varB->stringValue) + 1);
        strcpy(concatenatedString, varA->stringValue);
        strcat(concatenatedString, varB->stringValue);
        assignStringVariable(resultVar, concatenatedString);
        free(concatenatedString);
    } else {
        printf("Error: Both variables must be defined and contain string values for concatenation.\n");
        exit(EXIT_FAILURE);
    }
}


// calculate the length of a string
int evaluateExpression(const char* expression, struct Variable* variables);
int strlenString(char var) {
    struct Variable* strVar = findVariable(var);
    if (strVar && strVar->stringValue) {
        return strlen(strVar->stringValue);
    } else {
        printf("Error: Variable %c is not defined or does not contain a string.\n", var);
        exit(EXIT_FAILURE);
    }
}

//check  condition
int evaluateCondition(const char* condition, struct Variable* variables) {
    char var1, var2;
    char operator[3];

    if (sscanf(condition, "%c%2[><==]%c", &var1, operator, &var2) == 3) {
        int value1, value2;
        struct Variable* v1 = findVariable(var1);
        struct Variable* v2 = findVariable(var2);

        if (v1 && v2) {
            value1 = v1->intValue;
            value2 = v2->intValue;
        } else {
            printf("Variables %c or %c are not defined.\n", var1, var2);
            exit(EXIT_FAILURE);
        }

        if (strcmp(operator, "==") == 0) {
            return (value1 == value2);
        } else if (strcmp(operator, ">") == 0) {
            return (value1 > value2);
        } else if (strcmp(operator, "<") == 0) {
            return (value1 < value2);
        } else {
            printf("Unsupported operator: %s\n", operator);
            exit(EXIT_FAILURE);
        }
    } else {
        printf("Invalid condition syntax: %s\n", condition);
        exit(EXIT_FAILURE);
    }
}

//if loop
void executeIfLoop(const char* input1, struct Variable* variables) {
    char condition[MAX_INPUT_LENGTH], input[MAX_INPUT_LENGTH];

    if (sscanf(input1, "if(%[^:]:%[^\n]", condition, input) == 2) {
        if (evaluateCondition(condition, variables)) {
            char* expression = strtok(input, ";");
            
            while (expression != NULL) {
               
                char varName, operand1, operand2;
                int value;
                
                if (sscanf(expression, "%c = concatenate(%c, %c)", &varName, &operand1, &operand2) == 3) {
                    concatenateStrings(varName, operand1, operand2);
                } else if (sscanf(expression, "%c : %[^\n]", &varName, input) == 2) {
                    if (isStringVariable(varName)) {
                        assignStringVariable(varName, input);
                    } else {
                        int result = evaluateExpression(input, variables);
                        assignIntegerVariable(varName, result);
                    }
                } else if (sscanf(expression, "%c = strlen(%c)", &varName, &operand1) == 2) {
                    int length = strlenString(operand1);
                    assignIntegerVariable(varName, length);
                } else if (sscanf(expression, "print : %[^\n]", input) == 1) {
                    int result = evaluateExpression(input, variables);
                    printf(">> %d\n", result);
                } else if (sscanf(expression, "print %c", &varName) == 1) {
                    displayVariable(varName);
                } else if (sscanf(expression, "%c = %[^\n]", &varName, input) == 2) {
                    int result = evaluateExpression(input, variables);
                    if (isStringVariable(varName)) {
                        assignStringVariable(varName, input);
                    } else {
                        assignIntegerVariable(varName, result);
                    }
                } else if (sscanf(expression, "input %c", &varName) == 1) {
                    getInput(varName, 0); 
                } else if (sscanf(expression, "string %c = %[^\n]", &varName, input) == 2) {
                    assignStringVariable(varName, input);
                } else if (sscanf(expression, "input string %c", &varName) == 1) {
                    getInput(varName, 1);
                } else {
                    printf("Invalid input in if block: %s\n", expression);
                    exit(EXIT_FAILURE);
                }

                
                expression = strtok(NULL, ";");
            }
        }
    } else {
        printf("Invalid if loop syntax: %s\n", input);
    }
}

//while loop
void executewhileLoop(const char* input1, struct Variable* variables) {
    char condition[MAX_INPUT_LENGTH], input[MAX_INPUT_LENGTH];

    if (sscanf(input1, "while(%[^:]:%[^\n]", condition, input) == 2) {
        while (evaluateCondition(condition, variables)) {
            char inputCopy[MAX_INPUT_LENGTH];
            strcpy(inputCopy, input);   
            char* tokenPointer = strtok(inputCopy, ";");
            while (tokenPointer != NULL) {             
                char expression[MAX_INPUT_LENGTH];
                strcpy(expression, tokenPointer);
                char varName, operand1, operand2;
                int value;    
                char expressionBuffer[MAX_INPUT_LENGTH];
                strcpy(expressionBuffer, expression);

                if (sscanf(expressionBuffer, "%c = concatenate(%c, %c)", &varName, &operand1, &operand2) == 3) {
                    concatenateStrings(varName, operand1, operand2);
                } else if (sscanf(expressionBuffer, "%c : %[^\n]", &varName, expressionBuffer) == 2) {
                    if (isStringVariable(varName)) {
                        assignStringVariable(varName, expressionBuffer);
                    } else {
                        int result = evaluateExpression(expressionBuffer, variables);
                        assignIntegerVariable(varName, result);
                    }
                } else if (sscanf(expressionBuffer, "%c = strlen(%c)", &varName, &operand1) == 2) {
                    int length = strlenString(operand1);
                    assignIntegerVariable(varName, length);
                } else if (sscanf(expressionBuffer, "print : %[^\n]", expressionBuffer) == 1) {
                    int result = evaluateExpression(expressionBuffer, variables);
                    printf(">> %d\n", result);
                } else if (sscanf(expressionBuffer, "print %c", &varName) == 1) {
                    displayVariable(varName);
                } else if (sscanf(expressionBuffer, "%c = %[^\n]", &varName, expressionBuffer) == 2) {
                    int result = evaluateExpression(expressionBuffer, variables);
                    if (isStringVariable(varName)) {
                        assignStringVariable(varName, expressionBuffer);
                    } else {
                        assignIntegerVariable(varName, result);
                    }
                } else if (sscanf(expressionBuffer, "input %c", &varName) == 1) {
                    getInput(varName, 0); 
                } else if (sscanf(expressionBuffer, "string %c = %[^\n]", &varName, expressionBuffer) == 2) {
                    assignStringVariable(varName, expressionBuffer);
                } else if (sscanf(expressionBuffer, "input string %c", &varName) == 1) {
                    getInput(varName, 1); 
                } else {
                    printf("Invalid input in while loop: %s\n", expression);
                    exit(EXIT_FAILURE);
                }

                
                tokenPointer = strtok(NULL, ";");
            }
        }
    } else {
        printf("Invalid while loop syntax: %s\n", input);
    }
}

//do while loop
void executedoWhileLoop(const char* input1, struct Variable* variables) {
    char condition[MAX_INPUT_LENGTH], input[MAX_INPUT_LENGTH];

    if (sscanf(input1, "do:%[^|]while(%[^)])", input, condition) == 2) {
        do {
            char inputCopy[MAX_INPUT_LENGTH];
            strcpy(inputCopy, input);

            char* tokenPointer = strtok(inputCopy, ";");

            while (tokenPointer != NULL) {
            
                char expression[MAX_INPUT_LENGTH];
                strcpy(expression, tokenPointer);

                char varName, operand1, operand2;
                int value;

              
                char expressionBuffer[MAX_INPUT_LENGTH];
                
                strcpy(expressionBuffer, expression);
                printf("%s",expressionBuffer);
                if (sscanf(expressionBuffer, "%c = concatenate(%c, %c)", &varName, &operand1, &operand2) == 3) {
                    concatenateStrings(varName, operand1, operand2);
                } else if (sscanf(expressionBuffer, "%c : %[^\n]", &varName, expressionBuffer) == 2) {
                    if (isStringVariable(varName)) {
                        assignStringVariable(varName, expressionBuffer);
                    } else {
                        int result = evaluateExpression(expressionBuffer, variables);
                        assignIntegerVariable(varName, result);
                    }
                } else if (sscanf(expressionBuffer, "%c = strlen(%c)", &varName, &operand1) == 2) {
                    int length = strlenString(operand1);
                    assignIntegerVariable(varName, length);
                } else if (sscanf(expressionBuffer, "print : %[^\n]", expressionBuffer) == 1) {
                    int result = evaluateExpression(expressionBuffer, variables);
                    printf(">> %d\n", result);
                } else if (sscanf(expressionBuffer, "print %c", &varName) == 1) {
                    displayVariable(varName);
                } else if (sscanf(expressionBuffer, "%c = %[^\n]", &varName, expressionBuffer) == 2) {
                    int result = evaluateExpression(expressionBuffer, variables);
                    if (isStringVariable(varName)) {
                        assignStringVariable(varName, expressionBuffer);
                    } else {
                        assignIntegerVariable(varName, result);
                    }
                } else if (sscanf(expressionBuffer, "input %c", &varName) == 1) {
                    getInput(varName, 0); 
                } else if (sscanf(expressionBuffer, "string %c = %[^\n]", &varName, expressionBuffer) == 2) {
                    assignStringVariable(varName, expressionBuffer);
                } else if (sscanf(expressionBuffer, "input string %c", &varName) == 1) {
                    getInput(varName, 1); 
                } else {
                    printf("Invalid input in do-while loop: %s\n", expression);
                    exit(EXIT_FAILURE);
                }

                tokenPointer = strtok(NULL, ";");
            }
        } while (evaluateCondition(condition, variables));
    } else {
        printf("Invalid do-while loop syntax: %s\n", input1);
    }
}


void executeStatement(const char* statement, struct Variable* variables) {
    char varName, operand1, operand2;
    int value;

    char statementBuffer[MAX_INPUT_LENGTH];
    strcpy(statementBuffer, statement);

    if (sscanf(statementBuffer, "%c = concatenate(%c, %c)", &varName, &operand1, &operand2) == 3) {
        concatenateStrings(varName, operand1, operand2);
    } else if (sscanf(statementBuffer, "%c : %[^\n]", &varName, statementBuffer) == 2) {
        if (isStringVariable(varName)) {
            assignStringVariable(varName, statementBuffer);
        } else {
            int result = evaluateExpression(statementBuffer, variables);
            assignIntegerVariable(varName, result);
        }
    } else if (sscanf(statementBuffer, "%c = strlen(%c)", &varName, &operand1) == 2) {
        int length = strlenString(operand1);
        assignIntegerVariable(varName, length);
    } else if (sscanf(statementBuffer, "print : %[^\n]", statementBuffer) == 1) {
        int result = evaluateExpression(statementBuffer, variables);
        printf(">> %d\n", result);
    } else if (sscanf(statementBuffer, "print %c", &varName) == 1) {
        displayVariable(varName);
    } else if (sscanf(statementBuffer, "%c = %[^\n]", &varName, statementBuffer) == 2) {
        int result = evaluateExpression(statementBuffer, variables);
        if (isStringVariable(varName)) {
            assignStringVariable(varName, statementBuffer);
        } else {
            assignIntegerVariable(varName, result);
        }
    } else if (sscanf(statementBuffer, "input %c", &varName) == 1) {
        getInput(varName, 0); 
    } else if (sscanf(statementBuffer, "string %c = %[^\n]", &varName, statementBuffer) == 2) {
        assignStringVariable(varName, statementBuffer);
    } else if (sscanf(statementBuffer, "input string %c", &varName) == 1) {
        getInput(varName, 1);
    } else {
        printf("Invalid statement in for loop: %s\n", statement);
        exit(EXIT_FAILURE);
    }
}

//for loop
void executeForLoop(const char* input1, struct Variable* variables) {
    char initialization[MAX_INPUT_LENGTH], condition[MAX_INPUT_LENGTH], update[MAX_INPUT_LENGTH], input[MAX_INPUT_LENGTH];
    if (sscanf(input1, "for(%[^;];%[^;];%[^)]):%[^\n]", initialization, condition, update, input) == 4) {      
        executeStatement(initialization, variables);
        while (evaluateCondition(condition, variables)) {          
            char inputCopy[MAX_INPUT_LENGTH];
            strcpy(inputCopy, input);   
            char* tokenPointer = strtok(inputCopy, ";");
            while (tokenPointer != NULL) {
              
                char expression[MAX_INPUT_LENGTH];
                strcpy(expression, tokenPointer);
                executeStatement(expression, variables);            
                tokenPointer = strtok(NULL, ";");
            }
            executeStatement(update, variables);
        }
    } else {
        printf("Invalid for loop syntax: %s\n", input1);
    }
}

int evaluateExpression(const char* expression, struct Variable* variables) {
    char trimmedExpression[MAX_INPUT_LENGTH];
    int numStack[MAX_INPUT_LENGTH];
    char opStack[MAX_INPUT_LENGTH];
    int numTop = -1;
    int opTop = -1;

    int i = 0;
    int j = 0;

    while (expression[i] == ' ') {
        i++;
    }

    while (expression[i]) {
        if (expression[i] != ' ') {
            trimmedExpression[j] = expression[i];
            j++;
        }
        i++;
    }

    trimmedExpression[j] = '\0';

    if (isalpha(trimmedExpression[j - 1])) {
        trimmedExpression[j] = '+';
        j++;
        trimmedExpression[j] = '0';
        j++;
        trimmedExpression[j] = '\0';
    }

    i = 0;

    while (trimmedExpression[i]) {
        if (isalpha(trimmedExpression[i])) {
            char variable = trimmedExpression[i];
            struct Variable* var = findVariable(variable);

            if (var) {
                if (var->stringValue) {
                    printf("Error: Cannot perform arithmetic operations on a string.\n");
                    exit(EXIT_FAILURE);
                } else {
                    numStack[++numTop] = var->intValue;
                }
            } else {
                printf("Variable %c is not defined.\n", variable);
                exit(EXIT_FAILURE);
            }
            i++;
        }

        if (isdigit(trimmedExpression[i]) || (trimmedExpression[i] == '-' && isdigit(trimmedExpression[i + 1]))) {
            int num = 0;
            int sign = 1;

            if (trimmedExpression[i] == '-') {
                sign = -1;
                i++;
            }

            while (isdigit(trimmedExpression[i])) {
                num = num * 10 + (trimmedExpression[i] - '0');
                i++;
            }
            numStack[++numTop] = num * sign;
        } else if (trimmedExpression[i] == '+' || trimmedExpression[i] == '-' ||
                   trimmedExpression[i] == '*' || trimmedExpression[i] == '/') {
            while (opTop >= 0 && precedence(opStack[opTop]) >= precedence(trimmedExpression[i])) {
                int b = numStack[numTop--];
                int a = numStack[numTop--];
                char op = opStack[opTop--];
                numStack[++numTop] = applyOperation(a, op, b);
            }
            opStack[++opTop] = trimmedExpression[i];
            i++;
        } else {
            printf("Invalid character in the expression: %c\n", trimmedExpression[i]);
            exit(EXIT_FAILURE);
        }
    }

    while (opTop >= 0) {
        int b = numStack[numTop--];
        int a = numStack[numTop--];
        char op = opStack[opTop--];
        numStack[++numTop] = applyOperation(a, op, b);
    }

    if (numTop == 0) {
        return numStack[0];
    } else {
        printf("Invalid expression: %s\n", trimmedExpression);
        exit(EXIT_FAILURE);
    }

    return 0;
}
