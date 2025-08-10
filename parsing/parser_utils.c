#include "parser.h"

void free_str_array(char **arr)
{
    char **tmp = arr;
    while (*tmp)
        free(*tmp++);
    free(arr);
}

int ft_isspace(unsigned char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || 
            c == '\v' || c == '\f' || c == '\r');
}

int ft_is_all_spaces(const char *s)
{
    if (!s || !*s)
        return (0);
    while (*s)
        if (!ft_isspace((unsigned char)*s++))
            return (0);
    return (1);   
}

int ft_is_all_digits(const char *s)
{
    char *tmp;
    char *iter;

    if (!s || !*s)
        return (0);

    tmp = ft_strtrim(s, " ");
    if (!tmp)
        return (0);

    iter = tmp;
    while (*iter)
    {
        if (!ft_isdigit((unsigned char)*iter))
        {
            free(tmp);
            return (0);
        }
        iter++;
    }
    free(tmp);
    return (1);
}

int total_lines(char **arr)
{
    int i = 0;
    int count = 0;

    while (arr[i++])
        count++;
    return (count);
}
