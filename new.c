#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void fileChecker(char str[]);
void detectTokens(char *str);
bool isValidDelimiter(char ch);
bool isValidOperator(char ch);
bool isvalidIdentifier(char *str);
bool isValidKeyword(char *mainStr);
bool isValidInteger(char *str);
bool isRealNumber(char *str);
char *getSubString(char *str, int left, int right);

bool isValidReservedword(char *mainStr);
bool isValidNoiseword(char *mainStr);
int identifyOperator(char *str, int right);
void identifyDelimiter(char ch);
void identifyKeyword(char *mainStr);
bool isaDigit(char ch);
bool hasAlphabet(char *str);
bool isIdentifierElement(char ch);

void identifyCharacterLit(char *subStr, int right); // to delete

char *token;

FILE *outputptr;
FILE *inputptr;

int main()
{
    char filepath[100];
    printf("Input filepath: ");
    scanf("%s", filepath);
    fileChecker(filepath);

    outputptr = fopen("SymbolTable.txt", "w");
    char str[500] = "#";
    printf("The Program is : '%s' \n", str);
    fprintf(outputptr, "The Program is : '%s' \n", str);
    printf("All Tokens are : \n");
    fprintf(outputptr, "All Tokens are : \n");
    detectTokens(str);
    fclose(outputptr);
    return (0);
}
void fileChecker(char str[])
{
    printf("%s\n", str);
}

void detectTokens(char *str)
{
    int left = 0, right = 0;
    int length = strlen(str);

    while (right <= length && left <= right)
    {

        // COMMENT
        if (str[right] == '/' && str[right + 1] == '*')
        {
            right += 2;

            char subs[500];
            int i = 0;

            // string inside the /* */ will be stored in subs variable one by one
            while (str[right] != '*' && str[right + 1] != '/')
            {
                subs[i] = str[right];
                right++;
                i++;
            }
            subs[i] = '\0';
            token = "comment";

            printf("Comment : %s\t%s\n", subs, token);
            fprintf(outputptr, "Comment : %s\t%s\n", subs, token);

            right += 2;
            left = right;
            // extracted a lexeme, then update the left value
        }

        // CHARACTER LITERAL
        if (str[right] == '\'')
        {
            char c[2] = "";
            char subs[300] = ""; // stores the char literal

            // if ch='' or empty character literal
            if (str[right + 1] == '\'')
            {
                right++;
                token = "invalid char_literal";
            }
            // if the third character is not '
            else if (str[right + 2] != '\'')
            {
                right++;

                // gets values between ' '
                while (str[right] != '\'')
                {
                    c[0] = str[right];
                    strcat(subs, c);
                    right++;
                }
                token = "invalid char_literal";
            }
            // normal character
            else
            {
                right++;
                c[0] = str[right];
                strcat(subs, c);
                token = "char_literal";
                right++;
            }

            printf("Character literal: %s\t%s\n", subs, token);
            fprintf(outputptr, "Character literal: %s\t%s\n", subs, token);
            right++;
            left = right;
        }

        // STRING LITERAL
        if (str[right] == '\"')
        {
            right++;
            char c[2] = "";
            char subs[300] = ""; // stores the char literal

            // gets values between " "
            while (str[right] != '\"')
            {
                c[0] = str[right];
                strcat(subs, c);
                right++;
            }
            token = "str_literal";
            printf("String literal: %s\t%s\n", subs, token);
            fprintf(outputptr, "String literal: %s\t%s\n", subs, token);

            right++;
            left = right;
        }

        // checks if character is NOT a delimiter
        // if not a delimiter then proceed to evaluating the next character
        if (isValidDelimiter(str[right]) == false)
        { // (!isValidDelimiter(str[right]))
            right++;
        }

        // if char is a delimiter AND left == right
        if (isValidDelimiter(str[right]) == true && left == right)
        {
            // OPERATOR
            if (isValidOperator(str[right]) == true)
            {
                // gives new right value, index after the operator is evaluated
                int newRight = identifyOperator(str, right);
                right = newRight;
            }
            // IDENTIFY DELIMITER
            else
            {
                if (str[right] != ' ')
                {
                    identifyDelimiter(str[right]);
                }
                right++;
            }

            left = right;
        }

        // if char is delimiter AND left != right
        // or if right == length AND left != right
        else if ((isValidDelimiter(str[right]) == true && left != right) || (right == length && left != right))
        {
            char *subStr = getSubString(str, left, right - 1);

            if (isValidKeyword(subStr) == true)
            {
                printf("Valid Keyword : %s\t%s\n", subStr, token);
                fprintf(outputptr, "Valid Keyword : %s\t%s\n", subStr, token);
            }
            else if (isValidReservedword(subStr) == true)
            {
                printf("Valid Reserved Word : %s\t%s\n", subStr, token);
                fprintf(outputptr, "Valid Reserved Word : %s\t%s\n", subStr, token);
            }
            else if (isValidNoiseword(subStr) == true)
            {
                printf("Valid Noiseword : %s\t%s\n", subStr, token);
                fprintf(outputptr, "Valid Noiseword : %s\t%s\n", subStr, token);
            }
            else if (isValidInteger(subStr) == true)
            {
                printf("Valid Integer : %s\t%s\n", subStr, token);
                fprintf(outputptr, "Valid Integer : %s\t%s\n", subStr, token);
            }
            else if (isRealNumber(subStr) == true)
            {
                printf("Real Number : %s\t%s\n", subStr, token);
                fprintf(outputptr, "Real Number : %s\t%s\n", subStr, token);
            }
            else if (isvalidIdentifier(subStr) == true)
            {
                printf("Valid Identifier : %s\t%s\n", subStr, token);
                fprintf(outputptr, "Valid Identifier : %s\t%s\n", subStr, token);
            }
            else if (isvalidIdentifier(subStr) == false)
            {
                printf("Unknown input : %s\n", subStr);
                fprintf(outputptr, "Unknown input : %s\n", subStr);
            }
            /*
            else{
                printf("! unknown : %s\n", subStr);
            }
            */

            left = right;
        }
    }
    return;
}

bool isValidDelimiter(char ch)
{
    if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
        ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
        ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == ':' ||
        ch == '%' || ch == '!' || ch == '\n')
        return (true);
    return (false);
}

void identifyDelimiter(char ch)
{
    if (ch == '(')
    {
        token = "(";
    }
    else if (ch == ')')
    {
        token = ")";
    }
    else if (ch == '[')
    {
        token = "[";
    }
    else if (ch == ']')
    {
        token = "]";
    }
    else if (ch == '{')
    {
        token = "{";
    }
    else if (ch == '}')
    {
        token = "}";
    }
    else if (ch == ';')
    {
        token = ";";
    }
    else if (ch == ',')
    {
        token = ",";
    }
    else if (ch == ':')
    {
        token = ":";
    }

    printf("Delimiter : %c\t%s\n", ch, token);
    fprintf(outputptr, "Delimiter : %c\t%s\n", ch, token);
}

bool isValidOperator(char ch)
{
    char operatorList[] = {'=', '+', '-', '*', '/', '%', '!', '>', '<', '\0'};
    int i = 0;
    while (i < strlen(operatorList))
    {
        if (ch == operatorList[i])
        {
            return (true);
        }
        i++;
    }
    return (false);
}

int identifyOperator(char *str, int right)
{
    char c[2] = "";
    char mainStr[100] = "";

    while (isValidOperator(str[right]))
    {
        c[0] = str[right];
        strcat(mainStr, c);
        right++;
    }

    // arithmetic operator

    if (mainStr[0] == '+' && mainStr[1] == '\0')
    {
        token = "+";
    }
    else if (mainStr[0] == '-' && mainStr[1] == '\0')
    {
        token = "-";
    }
    else if (mainStr[0] == '/' && mainStr[1] == '\0')
    {
        token = "/";
    }
    else if (mainStr[0] == '*' && mainStr[1] == '\0')
    {
        token = "*";
    }
    else if (mainStr[0] == '%' && mainStr[1] == '\0')
    {
        token = "%";
    }
    else if (mainStr[0] == '*' && mainStr[1] == '*' && mainStr[2] == '\0')
    {
        token = "**";
    }
    else if (mainStr[0] == '/' && mainStr[1] == '/' && mainStr[2] == '\0')
    {
        token = "//";
    }

    // assignment operator

    else if (mainStr[0] == '=' && mainStr[1] == '\0')
    {
        token = "=";
    }
    else if (mainStr[0] == '+' && mainStr[1] == '=' && mainStr[2] == '\0')
    {
        token = "+=";
    }
    else if (mainStr[0] == '-' && mainStr[1] == '=' && mainStr[2] == '\0')
    {
        token = "-=";
    }
    else if (mainStr[0] == '*' && mainStr[1] == '=' && mainStr[2] == '\0')
    {
        token = "*=";
    }
    else if (mainStr[0] == '%' && mainStr[1] == '=' && mainStr[2] == '\0')
    {
        token = "%=";
    }
    else if (mainStr[0] == '/' && mainStr[1] == '=' && mainStr[2] == '\0')
    {
        token = "/=";
    }
    else if (mainStr[0] == '/' && mainStr[1] == '/' && mainStr[2] == '=' && mainStr[3] == '\0')
    {
        token = "//=";
    }

    // increment & decrement

    else if (mainStr[0] == '+' && mainStr[1] == '+' && mainStr[2] == '\0')
    {
        token = "++";
    }
    else if (mainStr[0] == '-' && mainStr[1] == '-' && mainStr[2] == '\0')
    {
        token = "--";
    }

    // boolean relational

    else if (mainStr[0] == '=' && mainStr[1] == '=' && mainStr[2] == '\0')
    {
        token = "==";
    }
    else if (mainStr[0] == '!' && mainStr[1] == '=' && mainStr[2] == '\0')
    {
        token = "!=";
    }
    else if (mainStr[0] == '>' && mainStr[1] == '=' && mainStr[2] == '\0')
    {
        token = ">=";
    }
    else if (mainStr[0] == '<' && mainStr[1] == '=' && mainStr[2] == '\0')
    {
        token = "<=";
    }
    else if (mainStr[0] == '>' && mainStr[1] == '\0')
    {
        token = ">";
    }
    else if (mainStr[0] == '<' && mainStr[1] == '\0')
    {
        token = "<";
    }
    else
    {
        token = "invalid operator";
    }
    printf("Operator : %s\t%s\n", mainStr, token);
    fprintf(outputptr, "Operator : %s\t%s\n", mainStr, token);

    return right;
}

// to delete
void identifyCharacterLit(char *str, int right)
{
    char c[2] = "";
    char subs[300] = ""; // stores the char literal

    // if ch ='' or empty character constant
    if (str[right + 1] == '\'')
    {
        printf("! invalid empty character literal: %s\n", subs);
        fprintf(outputptr, "! invalid empty character literal: %s\n", subs);
    }
    // if the third character is not '
    else if (str[right + 2] != '\'')
    {
        int i = right + 1;

        // gets values between ' '
        while (str[i] != '\'')
        {
            c[0] = str[i];
            strcat(subs, c);
            i++;
        }
        printf("! invalid character literal: %s\n", subs);
        fprintf(outputptr, "! invalid character literal: %s\n", subs);
    }
    // normal character
    else
    {
        printf("Character Literal : %c\n", str[1]);
        fprintf(outputptr, "Character Literal : %c\n", str[1]);
    }
}

// Returns 'true' if the string is a VALID IDENTIFIER.
bool isvalidIdentifier(char *str)
{

    // first checkpoint
    // no alphabet = not qualified to be an identifier
    if (hasAlphabet(str) == false)
        return (false);

    // second checkpoint
    // checks if each character in the str is an identifier element (a-zA-z,0-9,_)
    // if even 1 element is not an identifier element, then return true and tag it as an invalid id
    for (int i = 0; i < strlen(str); i++)
    {
        if (isIdentifierElement(str[i]) == false)
        {
            token = "invalid identifier";
            return (true);
        }
    }

    // third checkpoint
    // checks if the first character is an alphabet
    // if not, then tag the str as an invalid identifier
    char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                       'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                       'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                       'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    for (int i = 0; i < strlen(alphabet); i++)
    {
        if (alphabet[i] == str[0])
        {
            token = "identifier";
            return (true);
        }
    }

    token = "invalid identifier";
    return (true);
}

bool hasAlphabet(char *str)
{
    char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                       'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                       'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                       'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    for (int i = 0; i < strlen(str); i++)
    {
        for (int j = 0; j < strlen(alphabet); j++)
        {
            if (str[i] == alphabet[j])
            {
                return (true);
            }
        }
    }
    return (false);
    /*
        if the string has as an alphabet, return true
        if the loop ends, then str has no alphabet, return false
    */
}

bool isIdentifierElement(char ch)
{

    char identifierElements[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                                 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                                 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                                 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                                 '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '_'};

    for (int i = 0; i < strlen(identifierElements); i++)
    {
        if (identifierElements[i] == ch)
        {
            return (true);
        }
    }

    return (false);
}

bool isValidKeyword(char *mainStr)
{

    if (mainStr[0] == 'b' && mainStr[1] == 'o' && mainStr[2] == 'o' && mainStr[3] == 'l' && mainStr[4] == '\0')
    {
        token = "boolean_keyword";
    }
    else if (mainStr[0] == 'c' && mainStr[1] == 'u' && mainStr[2] == 'e' && mainStr[3] == 'r' && mainStr[4] == '\0')
    {
        token = "string_keyword";
    }
    else if (mainStr[0] == 'd' && mainStr[1] == 'e' && mainStr[2] == 'c' && mainStr[3] == '\0')
    {
        token = "elif_keyword";
    }
    else if (mainStr[0] == 'd' && mainStr[1] == 'e' && mainStr[2] == 's' && mainStr[3] == 'c' &&
             mainStr[4] == 'a' && mainStr[5] == 'n' && mainStr[6] == 's' && mainStr[7] == 'o' && mainStr[8] == '\0')
    {
        token = "break_keyword";
    }
    else if (mainStr[0] == 'd' && mainStr[1] == 'o' && mainStr[2] == 'b' && mainStr[3] == 'l' &&
             mainStr[4] == 'e' && mainStr[5] == '\0')
    { // doble
        token = "double_keyword";
    }
    else if (mainStr[0] == 'e' && mainStr[1] == 'n' && mainStr[2] == 't' && mainStr[3] == '\0')
    {
        token = "integer_keyword";
    }
    else if (mainStr[0] == 'e' && mainStr[1] == 's' && mainStr[2] == 'c' && mainStr[3] == 'a' && mainStr[4] == 'n' &&
             mainStr[5] == '\0')
    {
        token = "scanf_keyword";
    }
    else if (mainStr[0] == 'f' && mainStr[1] == 'l' && mainStr[2] == 'o' && mainStr[3] == 't' && mainStr[4] == '\0')
    {
        token = "float_keyword";
    }
    else if (mainStr[0] == 'h' && mainStr[1] == 'a' && mainStr[2] == 'c' && mainStr[3] == 'e' &&
             mainStr[4] == 'r' && mainStr[5] == '\0')
    {
        token = "do_keyword";
    }
    else if (mainStr[0] == 'i' && mainStr[1] == 'm' && mainStr[2] == 'p' && mainStr[3] == 'r' && mainStr[4] == 'i' &&
             mainStr[5] == '\0')
    {
        token = "printf_keyword";
    }
    else if (mainStr[0] == 'm' && mainStr[1] == 'a' && mainStr[2] == 's' && mainStr[3] == '\0')
    {
        token = "else_keyword";
    }
    else if (mainStr[0] == 'm' && mainStr[1] == 'i' && mainStr[2] == 'e' && mainStr[3] == 'n' &&
             mainStr[4] == 't' && mainStr[5] == 'r' && mainStr[6] == 'a' && mainStr[7] == 's' && mainStr[8] == '\0')
    {
        token = "while_keyword";
    }
    else if (mainStr[0] == 'p' && mainStr[1] == 'e' && mainStr[2] == 'r' && mainStr[3] == 's' && mainStr[4] == 'o' &&
             mainStr[5] == '\0')
    {
        token = "char_keyword";
    }
    else if (mainStr[0] == 'p' && mainStr[1] == 'o' && mainStr[2] == 'r' && mainStr[3] == '\0')
    {
        token = "for_keyword";
    }
    else if (mainStr[0] == 'r' && mainStr[1] == 'e' && mainStr[2] == 't' && mainStr[3] == 'o' &&
             mainStr[4] == 'r' && mainStr[5] == 'n' && mainStr[6] == 'o' && mainStr[7] == '\0')
    {
        token = "return_keyword";
    }
    else if (mainStr[0] == 's' && mainStr[1] == 'e' && mainStr[2] == 'q' && mainStr[3] == 'u' && mainStr[4] == 'i' &&
             mainStr[5] == 'r' && mainStr[6] == '\0')
    {
        token = "continue_keyword";
    }
    else if (mainStr[0] == 's' && mainStr[1] == 'i' && mainStr[2] == '\0')
    {
        token = "if_keyword";
    }
    else if (mainStr[0] == 'v' && mainStr[1] == 'a' && mainStr[2] == 'c' && mainStr[3] == 'i' && mainStr[4] == 'o' &&
             mainStr[5] == '\0')
    {
        token = "void_keyword";
    }
    else if (mainStr[0] == 'v' && mainStr[1] == 'a' && mainStr[2] == 'm' && mainStr[3] == 'o' && mainStr[4] == 's' &&
             mainStr[5] == '\0')
    {
        token = "goto_keyword";
    }

    // boolean

    else if (mainStr[0] == 'y' && mainStr[1] == '\0')
    {
        token = "and_op";
    }
    else if (mainStr[0] == 'n' && mainStr[1] == 'i' && mainStr[2] == '\0')
    {
        token = "or_op";
    }
    else if (mainStr[0] == 'n' && mainStr[1] == 'o' && mainStr[2] == '\0')
    {
        token = "not_op";
    }
    else
        return (false);

    return (true);
}

bool isValidReservedword(char *mainStr)
{
    if (mainStr[0] == 'f' && mainStr[1] == 'a' && mainStr[2] == 'l' && mainStr[3] == 's' && mainStr[4] == 'o' &&
        mainStr[5] == '\0')
    {
        token = "false_reserved";
    }
    else if (mainStr[0] == 'v' && mainStr[1] == 'e' && mainStr[2] == 'r' && mainStr[3] == 'd' && mainStr[4] == 'a' &&
             mainStr[5] == 'd' && mainStr[6] == '\0')
    {
        token = "true_reserved";
    }
    else
        return (false);

    return (true);
}

bool isValidNoiseword(char *mainStr)
{
    if (mainStr[0] == 'b' && mainStr[1] == 'o' && mainStr[2] == 'o' && mainStr[3] == 'l' && mainStr[4] == 'e' &&
        mainStr[5] == 'a' && mainStr[6] == 'n' && mainStr[7] == 'o' && mainStr[8] == '\0')
    {
        token = "boolean_noiseword";
    }

    else if (mainStr[0] == 'c' && mainStr[1] == 'u' && mainStr[2] == 'e' && mainStr[3] == 'r' && mainStr[4] == 'd' &&
             mainStr[5] == 'a' && mainStr[6] == '\0')
    {
        token = "string_noiseword";
    }
    else if (mainStr[0] == 'd' && mainStr[1] == 'e' && mainStr[2] == 'c' && mainStr[3] == 'o' && mainStr[4] == 'n' &&
             mainStr[5] == 't' && mainStr[6] == 'r' && mainStr[7] == 'a' && mainStr[8] == '\0')
    {
        token = "elif_noiseword";
    }
    else if (mainStr[0] == 'e' && mainStr[1] == 'n' && mainStr[2] == 't' && mainStr[3] == 'e' &&
             mainStr[4] == 'r' && mainStr[5] == 'o' && mainStr[6] == '\0')
    {
        token = "integer_noiseword";
    }
    else if (mainStr[0] == 'e' && mainStr[1] == 's' && mainStr[2] == 'c' && mainStr[3] == 'a' &&
             mainStr[4] == 'n' && mainStr[5] == 'e' && mainStr[6] == 'a' && mainStr[7] == 'r' && mainStr[8] == '\0')
    {
        token = "scanf_noiseword";
    }
    else if (mainStr[0] == 'f' && mainStr[1] == 'l' && mainStr[2] == 'o' && mainStr[3] == 't' && mainStr[4] == 'a' &&
             mainStr[5] == 'r' && mainStr[6] == '\0')
    {
        token = "float_noiseword";
    }
    else if (mainStr[0] == 'i' && mainStr[1] == 'm' && mainStr[2] == 'p' && mainStr[3] == 'r' &&
             mainStr[4] == 'i' && mainStr[5] == 'm' && mainStr[6] == 'i' && mainStr[7] == 'r' && mainStr[8] == '\0')
    {
        token = "printf_noiseword";
    }
    else if (mainStr[0] == 'p' && mainStr[1] == 'e' && mainStr[2] == 'r' &&
             mainStr[3] == 's' && mainStr[4] == 'o' && mainStr[5] == 'n' && mainStr[6] == 'a' &&
             mainStr[7] == 'j' && mainStr[8] == 'e' && mainStr[9] == '\0')
    {
        token = "char_noiseword";
    }
    else
        return (false);

    return (true);
}

bool isValidInteger(char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (isaDigit(str[i]) == false)
            return (false);
    }

    token = "int_literal";
    return (true);
}

bool isaDigit(char ch)
{
    char digitList[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    for (int i = 0; i < strlen(digitList); i++)
    {
        if (ch == digitList[i])
        {
            return (true);
        }
    }
    return (false);
    /*
        gets every element in digitList, compares it to the digit stored in ch variable
        if a similar element/digit is found, then return true
        if the loop ends but no element is similar, then return false
    */
}

bool isRealNumber(char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (isaDigit(str[i]) == false && str[i] != '.')
            return (false);
    }

    int count = 0;

    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == '.')
            count++;
    }

    if (count > 1)
    {
        token = "invalid float_literal";
    }
    else
    {
        token = "float_literal";
    }

    return (true);
    /*
        if a character in the substring is neither a digit nor a '.', then return false
        if the first loop ends w/o returning, then the str contains only numeric values and decimal
    */
}

char *getSubString(char *str, int left, int right)
{
    char subs[300];
    int i = 0;

    while (left <= right)
    {
        subs[i] = str[left];
        left++;
        i++;
    }
    subs[i] = '\0';
    char *subStr = subs;

    return (subStr);
}

// left - lowerbound
// right- upperbound
