/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohalaou <mohalaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:51:07 by mohalaou          #+#    #+#             */
/*   Updated: 2025/08/12 17:18:07 by mohalaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	free_str_array(char **arr)
{
	char	**tmp;

	tmp = arr;
	while (*tmp)
		free(*tmp++);
	free(arr);
}

int	ft_isspace(unsigned char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

int	ft_is_all_spaces(const char *s)
{
	if (!s || !*s)
		return (0);
	while (*s)
		if (!ft_isspace((unsigned char)*s++))
			return (0);
	return (1);
}

int	ft_is_all_digits(const char *s)
{
	char	*tmp;
	char	*iter;

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

int	total_lines(char **arr)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (arr[i++])
		count++;
	return (count);
}
