/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabrugge <mabrugge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 17:06:40 by mabrugge          #+#    #+#             */
/*   Updated: 2026/05/14 13:11:36 by mabrugge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#define BUFFER_SIZE 10

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

char *update_line(char *buffer, int number_bytes, char **rest, int position)
{
    int len_rest;
    char *line;
    int y;
    int j;

    y = 0;

    line = malloc(sizeof(char) * (position) + 1);
    if (*rest != NULL)
    {
        
        while(*rest[y])
        {
            line[y] = (*rest)[y];
            y++;
        }
    }
    j = 0;

    while (y < position)
    {
        line[y] = buffer[j++];
        y++;
    }
    line[y] = '\0';
    return (line);
}

char *construct_line(char *buffer, int *i, char **rest, int number_bytes)
{
    int position;
    char *line;
    int y;

    y = 0;
    position = *i;
    *i = 0;
    *rest = malloc(sizeof(char) * (number_bytes - (position + 1)));
    while (position < number_bytes - position)
    {
        (*rest)[y] = buffer[position];
        y++;
        position++;
    }
    (*rest)[y] = '\0'; 
    line = update_line(buffer, number_bytes, rest, position);
    free(*rest);
    return (line);
}

char *return_last_line(char **rest)
{
    char *line;
   // line = ft_strjoin(*tmp, *rest);
 //   free(*tmp);
    *rest = NULL;
    return (line);
}

char *return_buffer(int number_bytes, char *buffer)
{
    int i;
    char *str;

    i = 0;
    str = malloc(number_bytes + 1);
    while(i < number_bytes)
    {
      str[i] = buffer[i];
      i++;
    }
    str[i] = '\0';
    return (str);
}

char *get_next_line(int fd)
{
   char buffer[BUFFER_SIZE + 1];
    int i;
    int y;
    int number_bytes;
    char static *rest;

    while ((number_bytes = read(fd, buffer, BUFFER_SIZE)) > 0)
    {

        i = 0;
        y = 0;
        while(i < number_bytes)
        {
            if (buffer[i] == '\n' && i != 0)
            {
                return (construct_line(buffer, &i, &rest, number_bytes));
            }
            i++;
        }
        if(!rest)
        {
            rest = return_buffer(number_bytes, buffer);
        }
        else
        {
            rest = ft_strjoin(rest, return_buffer(number_bytes, buffer));
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
    if (fd < 0)
        return (1);
    line = get_next_line(fd);
    if (line)
        printf("addres %p: line: %s", line,line);
    line = get_next_line(fd);
    if (line)
        printf("addres %p: line: %s", line,line);
    // while ((line = get_next_line(fd)) != NULL)
    // {
    //     printf("%s", line);
    //     free(line);
    // }

    close(fd);
}
