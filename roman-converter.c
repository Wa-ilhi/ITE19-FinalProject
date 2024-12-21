#include <stdio.h>  // input-output library for file operations and basic IO functions
#include <stdlib.h> // standard library for  memory allocation

// function prototypes
int romanToDecimal(const char *roman);
void numberToWords(long long num, char *words);
int isValidRoman(const char *roman);
int processExpression(const char *expression, char *result);
void processFile(const char *inputFile, const char *outputFile);

// function to convert a character to uppercase
int charToUpper(int ch)
{
    // if character is lowercase, convert to uppercase; otherwise, return as is
    return (ch >= 'a' && ch <= 'z') ? ch - 'a' + 'A' : ch;
}

// function to calculate the length of a string
size_t stringLength(const char *str)
{
    size_t len = 0;
    // count characters until null terminator is reached
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

// function to compare two strings lexicographically
int stringCompare(const char *str1, const char *str2)
{
    // iterate through both strings until characters differ or null terminator is reached
    while (*str1 && *str2 && *str1 == *str2)
    {
        str1++;
        str2++;
    }
    // return the difference between characters
    return *str1 - *str2;
}

// function to copy the content of one string into another
char *stringCopy(char *dest, const char *src)
{
    char *start = dest;
    // copy characters until null terminator is encountered
    while ((*dest++ = *src++))
        ;
    return start;
}

// function to concatenate two strings
char *stringConcat(char *dest, const char *src)
{
    char *start = dest;
    // move pointer to the end of the destination string
    while (*dest)
    {
        dest++;
    }
    // append source string to destination string
    while ((*dest++ = *src++))
        ;
    return start;
}

// function to validate roman numeral characters
int isValidRoman(const char *roman)
{
    const char validChars[] = "IVXLCDM"; // valid roman numeral characters
    while (*roman)
    {
        int isValid = 0;
        // check if each character matches any valid roman numeral character
        for (int i = 0; validChars[i] != '\0'; i++)
        {
            if (charToUpper(*roman) == validChars[i])
            {
                isValid = 1;
                break;
            }
        }
        // if an invalid character is found, return false
        if (!isValid)
        {
            return 0;
        }
        roman++;
    }
    return 1; // return true if all characters are valid
}

// function to convert a roman numeral to decimal value
int romanToDecimal(const char *roman)
{
    int values[256] = {0}; // initialize an array to map roman numeral characters to their values
    values['I'] = 1;
    values['V'] = 5;
    values['X'] = 10;
    values['L'] = 50;
    values['C'] = 100;
    values['D'] = 500;
    values['M'] = 1000;

    int total = 0, prevValue = 0; // initialize total and previous value
    while (*roman)
    {
        int currValue = values[charToUpper(*roman++)]; // get the value of the current character
        if (currValue > prevValue)
        {
            // subtract twice the previous value if a smaller numeral precedes a larger one
            total += currValue - 2 * prevValue;
        }
        else
        {
            // otherwise, simply add the value
            total += currValue;
        }
        prevValue = currValue; // update previous value
    }
    return total; // return the decimal equivalent
}

// funnction to convert a number into its word representation
void numberToWords(long long num, char *words)
{
    const char *units[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    const char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    const char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    const char *thousands[] = {"", "Thousand", "Million", "Billion"};

    char temp[500] = ""; // temporary buffer to words
    for (int i = 0; num > 0; i++)
    {
        int chunk = num % 1000; // process three digits at a time
        if (chunk)
        {
            char part[100] = "";
            // handle hundreds place
            if (chunk / 100)
            {
                stringConcat(part, units[chunk / 100]);
                stringConcat(part, " Hundred ");
            }
            // handle tens and units place
            if (chunk % 100 >= 20)
            {
                stringConcat(part, tens[(chunk % 100) / 10]);
                stringConcat(part, " ");
            }
            if (chunk % 100 >= 10 && chunk % 100 < 20)
            {
                stringConcat(part, teens[chunk % 100 - 10]);
                stringConcat(part, " ");
            }
            if (chunk % 10 && chunk % 100 < 10)
            {
                stringConcat(part, units[chunk % 10]);
                stringConcat(part, " ");
            }
            // append the appropriate scale (thousands, millions, etc.)
            if (i)
            {
                stringConcat(part, thousands[i]);
                stringConcat(part, " ");
            }
            // merge the current part with the result
            stringConcat(part, temp);
            stringCopy(temp, part);
        }
        num /= 1000; // move to the next three digits
    }
    stringCopy(words, temp); // copy final result to the output buffer
    if (stringLength(words) > 0)
    {
        words[stringLength(words) - 1] = '\0'; // remove trailing space
    }
}

// function to process a mathematical expression involving roman numerals
int processExpression(const char *expression, char *result)
{
    char roman1[50] = "", roman2[50] = ""; // buffers for roman numerals
    char operand = 0;
    int i = 0, j = 0;

    // extract the first roman numeral
    while (expression[i] != ' ' && expression[i] != '\0')
    {
        roman1[j++] = expression[i++];
    }
    roman1[j] = '\0';
    if (expression[i] == '\0') // invalid if no operator is found
    {
        return 0;
    }
    operand = expression[++i]; // extract the operator
    i += 2;                    // skip space and move to the second operand

    // extract the second roman numeral
    j = 0;
    while (expression[i] != '\0')
    {
        roman2[j++] = expression[i++];
    }
    roman2[j] = '\0';

    // validate both roman numerals
    if (!isValidRoman(roman1) || !isValidRoman(roman2))
    {
        return 0;
    }

    // convert roman numerals to decimal
    long long num1 = romanToDecimal(roman1);
    long long num2 = romanToDecimal(roman2);
    long long res = 0;

    // perform the operation based on the operator
    if (operand == '+')
    {
        res = num1 + num2;
    }
    else if (operand == '-' && num1 >= num2)
    {
        res = num1 - num2;
    }
    else if (operand == '*')
    {
        res = num1 * num2;
    }
    else if (operand == '/' && num2 != 0)
    {
        res = num1 / num2;
    }
    else
    {
        return 0; // invalid operation
    }

    // convert the result to words
    numberToWords(res, result);
    return 1;
}

// function to process a file containing roman numerals
void processFile(const char *inputFile, const char *outputFile)
{
    FILE *in = fopen(inputFile, "r");   // open the input file for reading
    FILE *out = fopen(outputFile, "w"); // open the output file for writing
    if (!in || !out)                    // exit if file cannot be opened
    {
        exit(1);
    }

    char line[100], result[500];
    while (fgets(line, sizeof(line), in)) // read each line from the input file
    {
        int len = stringLength(line);
        if (len > 0 && line[len - 1] == '\n') // remove trailing newline character
        {
            line[len - 1] = '\0';
        }

        if (isValidRoman(line)) // process if the line contains a roman numeral
        {
            numberToWords(romanToDecimal(line), result);
            fprintf(out, "%s\n", result);
        }
        else if (processExpression(line, result)) // process if the line contains an expression
        {
            fprintf(out, "%s\n", result);
        }
        else // output an error message for invalid input
        {
            fprintf(out, "Invalid Roman Numerals: %s\n", line);
        }
    }

    fclose(in);  // close the input file
    fclose(out); // close the output file
}

// main function
int main()
{
    processFile("input.txt", "output.txt"); // process input and output files
    return 0;                               // end of the program
}
