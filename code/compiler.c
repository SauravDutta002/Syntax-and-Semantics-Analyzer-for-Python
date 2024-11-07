#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *file;
    char pythonCode[1024];
    char command[256];
    int result;

    printf("Enter your Python code (type 'END' on a new line to finish):\n");

    file = fopen("temp_code.py", "w");
    if (file == NULL)
    {
        perror("Error creating temporary file");
        return 1;
    }

    while (fgets(pythonCode, sizeof(pythonCode), stdin) != NULL)
    {
        if (strncmp(pythonCode, "END\n", 4) == 0)
        {
            break;
        }

        if (strncmp(pythonCode, "print", 5) == 0 && pythonCode[5] != '(')
        {
            memmove(pythonCode + 6, pythonCode + 5, strlen(pythonCode) + 1);
            pythonCode[5] = '(';
            pythonCode[strlen(pythonCode) - 1] = ')';
        }

        fprintf(file, "%s", pythonCode);
    }

    fclose(file);

    snprintf(command, sizeof(command), "python temp_code.py 2>&1");

    FILE *pipe = popen(command, "r");
    if (!pipe)
    {
        perror("Failed to run python command");
        return 1;
    }

    char buffer[256];
    int hasError = 0;
    while (fgets(buffer, sizeof(buffer), pipe) != NULL)
    {
        printf("%s", buffer);

        if (strstr(buffer, "SyntaxError") != NULL || strstr(buffer, "Error") != NULL)
        {
            hasError = 1;
        }
    }

    result = pclose(pipe);

    printf("Python command exit code: %d\n", result);

    if (result != 0 || hasError)
    {
        printf("Syntax error detected in the Python code.\n");
    }
    else
    {
        printf("No syntax errors detected.\n");
    }

    remove("temp_code.py");

    return 0;
}


// Keep coding, keep creating, Saurav! 🚀💻