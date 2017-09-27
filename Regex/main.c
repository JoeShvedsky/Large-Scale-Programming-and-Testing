//Joseph Shvedsky
//September 20, 2017

/*half-working regex pattern matcher
matches regular ascii strings, as well as 
wildcards ?+*, and special flags \w (alpha)
\s (space) and \d (digit)
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


/*This function would have been used in a more robust program, possibly

int compare(char c1, char c2, char mode)
{
    if(mode == ' ')
    {
        return c1 == c2;
    }
    
    if(mode == 'd')
    {
        return isdigit(c2);
    }
    if(mode == 'w')
    {
        return isalpha(c2);
    }
    if(mode == 's')
    {
        return isspace(c2);
    }
    return 0;
}*/


//splits the regex string so that the wildcards are in one char and
//the other characters are in another
int split_regex( char* regex, char* flags, char* new_string)
{

    int i; //regex
    int new_string_pos = 0; //only non-wildcards
    int flag_pos = 0; //wildcards
    for(i = 0; i < strlen(regex); ++i)
    {


        if(regex[i] == '\\' &&  (regex[i+1] == 'd' || regex[i+1] == 's' || regex[i+1] == 'w'))
        {
            //new_string will have the old regex chars
            new_string[new_string_pos] = regex[i];
            new_string[new_string_pos + 1] = regex[i+1];
            new_string_pos += 2;            
            
            //places wildcards
            if(regex[i+2] == '*' || regex[i+2] == '+' || regex[i+2] == '?')
            {
                flags[flag_pos] = regex[i+2];
                flag_pos++;
                i+=2;
                continue;
            }
            else
            {
                
                flags[flag_pos] = ' ';  //the flag/wildcard array is by default
                flag_pos++;
                i++;
            }

        }
        else
        {
            //new_string will have the old regex chars
            new_string[new_string_pos] = regex[i];
            new_string_pos++;
        
            //places wildcards
            if(regex[i+1] == '*' || regex[i+1] == '+' || regex[i+1] == '?')
            {
                flags[flag_pos] = regex[i+1];
                flag_pos++;
            }
            else
            {                
                flags[flag_pos] = ' ';
                flag_pos++;

            }
        }

    }
    new_string[new_string_pos] = '\0';
    flags[flag_pos] = '\0';
    return flag_pos;
}


//This function is for wildcards
//checks if pattern exists at start of given substring
//compares to flags (at first given position of flags, which corresponds to
//the regex string)
int has_pattern_at_start(const char* str, char* regex, char* flags)
{
    int j = 0; //string index
    if(flags[0] == '*')
    {
        if(regex[0] == '\\')
        {
            if(regex[1] == 'd')
            {
                //keeps counting digits
                while(isdigit(str[j]))
                {
                    ++j;
                }
            }
            else if(regex[1] == 'w')
            {
                //keeps counting letters
                while(isalpha(str[j]))
                {
                    ++j;
                }
                
            }
            else if(regex[1] == 's')
            {
                //keeps counting spaces
                while(isspace(str[j]))
                {
                    ++j;
                }
            }
            else
            {
                //counts backslashes
                while(str[j] == '\\')
                {
                    
                    ++j;
                }
            }
        
        }
        else
        {
            //keeps counting identical chars
            while(str[j] == regex[0])
            {
                ++j;
            }
        }

    }
    //same as asterisk, but only counts once
    else if(flags[0] == '?')
    {
        if(regex[0] == '\\')
        {
            if(regex[1] == 'd')
            {
                if(isdigit(str[j]))
                {
                    ++j;
                }
            }
            else if(regex[1] == 'w')
            {
                if(isalpha(str[j]))
                {
                    ++j;
                }
                
            }
            else if(regex[1] == 's')
            {
                if(isspace(str[j]))
                {
                    ++j;
                }
            }
        }
        else
        {
            if(str[j] == regex[0])
            {
                ++j;
            }
        }
    }
    //similar to asterisk, but if there isn't at least one,
    //function returns -1 
    else if(flags[0] == '+')
    {

        if(regex[0] == '\\')
        {
            if(regex[1] == 'd')
            {
                if(!isdigit(str[j]))
                    return -1;
                while(isdigit(str[j]))
                {
                    ++j;
                }
            }
            else if(regex[1] == 'w')
            {
                if(!isalpha(str[j]))
                    return -1;
                while(isalpha(str[j]))
                {
                    ++j;
                }
                
            }
            else if(regex[1] == 's')
            {
                if(!isspace(str[j]))
                    return -1;            
                while(isspace(str[j]))
                {
                    ++j;
                }
            }
        }
        else
        {
            if(str[j] != regex[0])
                return -1;
            while(str[j] == regex[0])
            {
                ++j;
            }
        }        
    }
    return j;
}

int has_pattern(const char *str,  char* regex, char* flags)
{
    
    if(strlen(regex) == 0)
    {
        return 1;
    }
    int i = 0; //regex index
    int j = 0; //string index
    int flag_pos = 0;
    while( i < strlen(regex) && j < strlen(str))
    {
        //for the symbols
        if(regex[i] == '\\' && (regex[i+1] == 'd' || regex[i+1] == 'w' || regex[i+1] == 's')) 
        {

            if(flags[flag_pos] == ' ')
            {
                if(regex[i + 1] == 'd')
                {
                    //general rule is the same:
                    //if found, increase string counter
                    //and increase regex counter appropriately
                    //+2 for \ since it's an extra char
                    if(isdigit(str[j]))
                    {
                        ++j;
                        flag_pos++;
                        i += 2;
                    }
                    //not found, only increment string if 
                    //regex hasn't made progress, otherwise 
                    //reset other counters
                    else
                    {
                        if(i == 0)
                            ++j;
                        i = 0;
                        flag_pos = 0;
                    }
                }
                else if(regex[i + 1] == 's')
                {
                    if(isspace(str[j]))
                    {
                        ++j;
                        i += 2;
                        flag_pos++;
                    }
                    else
                    {
                        if(i == 0)
                            ++j;
                        flag_pos = 0;
                        i = 0;
                    }
                }
                else if(regex[i+1] == 'w')
                {
                    if(isalpha(str[j]))
                    {
                        ++j;
                        flag_pos++;
                        i += 2;
                    }
                    else
                    {
                        if(i == 0)
                            ++j;
                        flag_pos = 0;
                        i = 0;
                    }
                }
                
            }
            
            else            
            {
                int new_pos = has_pattern_at_start(&str[j], &regex[i],&flags[flag_pos]);
                //nothing found 
                if(new_pos == -1)
                {
                    if(i == 0)
                        ++j;
                    i = 0;    
                    flag_pos = 0;
                    continue;
                }
                 if(new_pos == 0)
                {
                    //continue counting string if regex doesnt need to be reset
                    i+=2;
                    flag_pos++;
                    continue;
                }
                //found match(es), continue
                j+= new_pos;
                flag_pos++;
                i+=2;

            }
 
        }
        else
        {
            if(flags[flag_pos] != ' ')
            {
                int new_pos = has_pattern_at_start(&str[j], &regex[i],&flags[flag_pos]);
                //nothing found 
                if(new_pos == -1)
                {
                    if(i == 0)
                        ++j;
                    i = 0;    
                    flag_pos = 0;
                    continue;
                }
                 if(new_pos == 0)
                {
                    //continue counting string if regex doesnt need to be reset
                    i++;
                    flag_pos++;
                    continue;
                }
                //found match(es), continue
                j += new_pos;
                flag_pos++;
                i++;

            }


            else
            {
                //simple one-to-one match
                 if(regex[i] == '.' || regex[i] == str[j])
                {
                    
                        ++j;
                        ++i;
                        flag_pos++;
                }            
                else
                {
                    //keep going if not found
                    if(i == 0)
                        ++j;
                    flag_pos = 0;
                    i = 0;
                }
            }
                          
        }

    }
    //if regex made it to end
    if(i == strlen(regex))
        return 1;
 return 0;
    
    
}

int main(int argc, char* argv[])
{
    //open files and error check
    FILE* regex_file, *input_file;
    if((regex_file = fopen(argv[1], "r")) == NULL)
    {
        fprintf(stderr, "ERROR: couldn't open regex file to read.\n");
        return EXIT_FAILURE;
    }
    if((input_file = fopen(argv[2], "r")) == NULL)
    {
        fprintf(stderr, "ERROR: couldn't open input file to read.\n");
        return EXIT_FAILURE;        
    }   
    
    //read regex and remove newline
    char regex[256];
    fgets(regex, 256, regex_file);   
    regex[strcspn(regex, "\n")] = 0; 

    //read file
    char word[256];
    char flags[strlen(regex)];
    
    //strip the regex of wildcards into two strings
    //wildcards stores the wildcards
    char no_wildcards[256];    
    int has_flags = split_regex(regex, flags, no_wildcards);
    strcpy(regex, no_wildcards);
    
    //main loop
    while(fgets(word, 256, input_file) != NULL)
    {
        
        //simple substring check if no symbols or wildcards
        if(has_flags == 0)
        {
            if( strstr(word, regex) != NULL)
            {
    
                printf("%s", word);
                fflush(stdout);
                
            }  
    
        }
        //dirty work
        else if(has_pattern(word, regex, flags))
        {
            
            printf("%s", word);
            fflush(stdout);
        }
    }
    fclose(regex_file);
    fclose(input_file);
    return EXIT_SUCCESS;
}
