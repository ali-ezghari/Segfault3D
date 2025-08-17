/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohalaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 15:22:44 by mohalaou          #+#    #+#             */
/*   Updated: 2024/11/08 15:56:59 by mohalaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_check_set(char const c, char const *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	size;
	char	*new;

	if (!s1 || !set)
		return (NULL);
	while (s1 && ft_check_set(((char)*s1), set) == 1)
	{
		s1++;
	}
	size = ft_strlen(s1);
	while (size != 0 && ft_check_set(s1[size - 1], set) == 1)
	{
		size--;
	}
	new = (char *)malloc(size * sizeof(char) + 1);
	if (!new)
		return (NULL);
	ft_strlcpy(new, s1, size + 1);
	return (new);
}
