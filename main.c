#include "func.h"
int main() {
    char input[MAX_INPUT_LENGTH];
    char condition[MAX_INPUT_LENGTH];
    int index,value;
    while (1) {
        printf(">>>");
        fgets(input, sizeof(input), stdin);

        char arrayValues[100],varName, operand1, operand2,var1,var2,input1[MAX_INPUT_LENGTH],initialization[MAX_INPUT_LENGTH],update[MAX_INPUT_LENGTH],condition[MAX_INPUT_LENGTH];
        int value;
        int vl;
    if (sscanf(input, "array %c = {%[^}]}", &varName, arrayValues) == 2) {
          
            int values[MAX_ARRAY_LENGTH];
            int length = 0;
            char* token = strtok(arrayValues, ",");
            while (token != NULL && length < MAX_ARRAY_LENGTH) {
                values[length++] = atoi(token);
                token = strtok(NULL, ",");
            }
            assignArrayVariable(varName, values, length);
        } else if (sscanf(input, "print:%c[%d]", &varName, &index) == 2) {
            displayarrVariable(varName, index);
        } else if (sscanf(input, "%c[%d]=%d", &varName, &index, &value) == 3) {
            struct Variable* var = findarrVariable(varName);
            if (var && var->isArray && index >= 0 && index < var->arrayLength) {
                var->arrayValues[index] = value;
            } else {
                printf("Invalid array assignment.\n");
            }
        }

        
     else if (sscanf(input, "for(%[^;];%[^;];%[^)]):%[^\n]", initialization, condition, update, input1) == 4){
        executeForLoop(input,variables);
        
    }
    else if (sscanf(input, "do:%[^|]while(%[^)])", input, condition) == 2) {
            executedoWhileLoop(input, variables);
        }
    
    else if (sscanf(input, "if(%[^:]:%[^\n]", condition, input1) == 2) {
    executeIfLoop(input, variables);
} 
     else if (sscanf(input, "while(%[^:]:%[^\n]", condition, input1) == 2) {
    executewhileLoop(input, variables);
}
 	 else if (sscanf(input, "%c = concatenate(%c, %c)", &varName, &operand1, &operand2) == 3) {
            concatenateStrings(varName, operand1, operand2);
        }
        else if (sscanf(input, "%c : %[^\n]", &varName, input) == 2) {
            if (isStringVariable(varName)) {
                assignStringVariable(varName, input);
            } else {
                int result = evaluateExpression(input, variables);
                assignIntegerVariable(varName, result);
            }
        }
	else if (sscanf(input, "%c = strlen(%c)", &varName, &operand1) == 2) {
            int length = strlenString(operand1);
            assignIntegerVariable(varName, length);
        }
	 else if (sscanf(input, "print : %[^\n]", input) == 1) {
            int result = evaluateExpression(input, variables);
            printf(">> %d\n", result);
        } else if (sscanf(input, "print %c", &varName) == 1) {
            displayVariable(varName);
        } else if (sscanf(input, "%c = %[^\n]", &varName, input) == 2) {
            int result = evaluateExpression(input, variables);
            if (isStringVariable(varName)) {
                assignStringVariable(varName, input);
            } else {
                assignIntegerVariable(varName, result);
            }
        } else if (sscanf(input, "input %c", &varName) == 1) {
            getInput(varName, 0); // Assume input is for integers by default
        } else if (sscanf(input, "string %c = %[^\n]", &varName, input) == 2) {
            assignStringVariable(varName, input);
        } else if (sscanf(input, "input string %c", &varName) == 1) {
            getInput(varName, 1);
        }  else {
            printf("Invalid input.\n");
        }
    }

    return 0;
}
