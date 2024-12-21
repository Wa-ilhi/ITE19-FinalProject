// Renyl A. Banua

#include <stdio.h>  // input-output library for file and console operations
#include <stdlib.h> // library for memory management and process control
#include <string.h> // string manipulation functions
#include <ctype.h>  // character classification and conversion functions

// Function prototypes
int romanToDecimal(const char *roman);                           // converts a Roman numeral to a decimal integer
void numberToWords(long long num, char *words);                  // converts a number to its word representation
int isValidRoman(const char *roman);                             // validates whether a string is a valid Roman numeral
int processExpression(const char *expression, char *result);     // processes arithmetic expressions involving Roman numerals
void processFile(const char *inputFile, const char *outputFile); // processes input file and generates output file

// Convert Roman numeral to decimal
int romanToDecimal(const char *roman)
{
    int values[256] = {0}; // array to store Roman numeral values
    values['I'] = 1;       // value 1 to 'I'
    values['V'] = 5;       // value 5 to 'V'
    values['X'] = 10;      // value 10 to 'X'
    values['L'] = 50;      // value 50 to 'L'
    values['C'] = 100;     // value 100 to 'C'
    values['D'] = 500;     // value 500 to 'D'
    values['M'] = 1000;    // value 1000 to 'M'

    int total = 0, prevValue = 0; // initialize total sum and previous Roman numeral value
    for (int i = 0; roman[i] != '\0'; i++)
    {                                                                           // loop through each character of the Roman numeral string
        int currValue = values[toupper(roman[i])];                              // get the value of the current Roman numeral
        total += currValue > prevValue ? currValue - 2 * prevValue : currValue; // adjust based on subtraction rule
        prevValue = currValue;                                                  // update previous value for next iteration
    }
    return total; // return the total decimal value
}

// Validate if a string is a valid Roman numeral
int isValidRoman(const char *roman)
{
    const char *validChars = "IVXLCDM"; // accepted Roman numeral characters
    while (*roman)
    {                                               // iterate through the string
        if (!strchr(validChars, toupper(*roman++))) // check if the character is not in validChars
            return 0;                               // if invalid character is found, return 0
    }
    return 1; //  if all characters are valid, return 1
}

// Convert a number to words
void numberToWords(long long num, char *words)
{
    const char *units[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    const char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    const char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    const char *thousands[] = {"", "Thousand", "Million", "Billion"};

    char temp[500] = ""; // temp. buffer to store words
    for (int i = 0; num > 0; i++)
    {                           // process the number in chunks of 3 digits
        int chunk = num % 1000; // extract the last 3 digits
        if (chunk)
        {                                                         // if the chunk is non-zero, convert it to words
            char part[100] = "";                                  // temporary buffer for the current chunk
            if (chunk / 100)                                      // if there are hundreds
                sprintf(part, "%s Hundred ", units[chunk / 100]); // add the hundreds place
            if (chunk % 100 >= 20)                                // if there are tens
                sprintf(part + strlen(part), "%s ", tens[(chunk % 100) / 10]);
            if (chunk % 100 >= 10 && chunk % 100 < 20) // if the number is a teen
                sprintf(part + strlen(part), "%s ", teens[chunk % 100 - 10]);
            if (chunk % 10 && chunk % 100 < 10) // if there are units
                sprintf(part + strlen(part), "%s ", units[chunk % 10]);
            if (i) // add the appropriate thousand-level suffix
                sprintf(part + strlen(part), "%s ", thousands[i]);
            strcat(part, temp); // append the current chunk to the result
            strcpy(temp, part); // update the temporary buffer
        }
        num /= 1000; // move to the next chunk
    }
    strcpy(words, temp);             // copy the final result to the output
    words[strlen(words) - 1] = '\0'; // remove trailing space
}

// Process a Roman arithmetic expression
int processExpression(const char *expression, char *result)
{
    char roman1[50], roman2[50], operand; // buffers to hold the operands and operator
    if (sscanf(expression, "%s %c %s", roman1, &operand, roman2) != 3 || !isValidRoman(roman1) || !isValidRoman(roman2))
        return 0; // validate the operands and expression

    long long num1 = romanToDecimal(roman1), num2 = romanToDecimal(roman2), res = 0;

    if (operand == '+') // addition
        res = num1 + num2;
    else if (operand == '-' && num1 >= num2) //  subtraction
        res = num1 - num2;
    else if (operand == '*') //  multiplication
        res = num1 * num2;
    else if (operand == '^') //  multiplication
        res = num1 * num2;
    else if (operand == '/' && num2 != 0) // division
        res = num1 / num2;
    else
        return 0; //  for invalid operations, return 0

    numberToWords(res, result); // convert the result to words
    return 1;                   // return success
}

// Process input file and output results
void processFile(const char *inputFile, const char *outputFile)
{
    FILE *in = fopen(inputFile, "r"), *out = fopen(outputFile, "w");
    if (!in || !out)
    {                                // check if the files are accessible
        printf("File corrupted.\n"); // print error if file cannot be opened
        exit(1);                     // exit
    }

    char line[100], result[500]; // buffers for input line and result
    while (fgets(line, sizeof(line), in))
    {                                     // read each line from the input file
        line[strcspn(line, "\n")] = '\0'; // remove trailing newline
        if (isValidRoman(line))
        {                                                // if line is a valid Roman numeral
            numberToWords(romanToDecimal(line), result); // convert it to words
            fprintf(out, "%s\n", result);                // write the result to the output file
        }
        else if (processExpression(line, result))
        {                                 // if line is a valid expression
            fprintf(out, "%s\n", result); // write the result to the output file
        }
        else
        {                                                       // if line is invalid
            fprintf(out, "Invalid Roman Numerals: %s\n", line); // write error message to output
        }
    }

    fclose(in);  // close input file
    fclose(out); // close output file
}

int main()
{
    processFile("input.txt", "output.txt"); // process the input and output files
    return 0;                               // end of the program
}
