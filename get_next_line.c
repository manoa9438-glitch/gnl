/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabrugge <mabrugge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 17:06:40 by mabrugge          #+#    #+#             */
/*   Updated: 2026/05/12 16:41:43 by mabrugge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#define BUFFER_SIZE 10

int ft_strlen(const char *str)
{
    if(!str || str[0] == '\0')
        return (0);
    int i;

    i = 0;
    while (str[i])
    {
        i++;
    }
    return (i);
}

char *ft_strdup(char *src)
{
    char *dst;
    int i;
    int len;

    len = ft_strlen(src);
    i = 0;
    if (!src)
        return (NULL);
    dst = malloc(len + 1);
    if (!dst)
        return (NULL);
    while (src[i])
    {
        dst[i] = src[i];
        i++;
    }
    dst[len] = '\0';
    return (dst);
}

char *update_line(char *buffer, int *i, char *new_rest, int position)
{
    int len_rest;
    char *line;
    int y;

    y = 0;
    line = malloc(sizeof(char) * (position));
    len_rest = ft_strlen(new_rest);
    if (len_rest > 0)
    {
        while (*i < len_rest)
        {
            line[*i] = new_rest[*i];
            (*i)++;
        }
    }
    while (y <= position)
    {
        line[*i] = buffer[y];
        y++;
        (*i)++;
    }
    return (line);
}

char *construct_line(char *buffer, int *i, char **rest, int number_bytes)
{
    int position;
    char *new_rest;
    char *line;
    int y;

    y = 0;
    position = *i;
    *i = 0;
    new_rest = *rest;
    free(*rest);
    *rest = malloc(sizeof(char) * (number_bytes - (position + 1)));
    line = update_line(buffer, i, new_rest, position);
    while (position <= number_bytes)
    {
        (*rest)[y] = buffer[position];
        y++;
        position++;
    }
    (*rest[y]) = '\0'; 
    return (line);
}

char *return_last_line(char **rest, char **last_rest)
{
    char *line;
    line = ft_strjoin(*last_rest, *rest);
   //  free(rest);
    *rest = NULL;
    return (line);
}

char *get_next_line(int fd)
{
    char buffer[100];
    int i;
    int y;
    int number_bytes;
    char static *last_rest;
    char static *rest;

    while ((number_bytes = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        i = 0;
        y = 0;
        while(i < number_bytes)
        {
            if (buffer[i] == '\n' && i != 0)
                return (construct_line(buffer, &i, &rest, number_bytes));
            i++;
        }
        if(rest)
            {   
                last_rest = ft_strdup(rest);   
                free(rest);
            }
        rest = malloc(sizeof(char) * (number_bytes + 1));
        while(y < number_bytes)
        {
            rest[y] = buffer[y];
            y++;
        }
        rest[y] = '\0';
    }
    if (number_bytes == 0 && rest != NULL)
        return (return_last_line(&rest, &last_rest));
    return (NULL);
}

int main(void)
{
    int fd;
    char *line;

    fd = open("test.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }

    close(fd);
}