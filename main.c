/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: logname <logname@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 11:31:45 by logname           #+#    #+#             */
/*   Updated: 2026/04/23 18:31:49 by logname          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
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

char *construct_line(char *buffer, int i, char **rest, int number_bytes)
{
    int position;
    char *new_rest;
    char *line;
    int y;

    y = 0;
    position = i;
    i = 0;
    new_rest = *rest;
    *rest = malloc(sizeof(char) * (number_bytes - (position + 1)));
    line = update_line(buffer, &i, new_rest, position);
    // a checker si il n y a pas de \r sur linux si oui le code est bon a refacto
    while (i < number_bytes)
    {
        (*rest)[y] = buffer[i];
        y++;
        i++;
    }
    return (line);
}

char *return_last_line(char **rest)
{
    char *line;

    line = ft_strdup(*rest);
   //  free(rest);
    *rest = NULL;
    return (line);
}

char *get_next_line(int fd)
{
    char buffer[100];
    int i;
    int number_bytes;
    char static *rest;

    while ((number_bytes = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        i = 0;
        while(i < number_bytes)
        {
            if (buffer[i] == '\n')
                return (construct_line(buffer, i, &rest, number_bytes));
            i++;
        }
    }

    if (number_bytes == 0 && rest != NULL)
        return (return_last_line(&rest));
    return (NULL);
}

int main(void)
{
    int fd;
    char *line;

    fd = open("test.txt", O_RDONLY);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }

    close(fd);
}