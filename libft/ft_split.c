/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohalaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 15:23:50 by mohalaou          #+#    #+#             */
/*   Updated: 2024/11/08 16:05:22 by mohalaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**free_array(char **ptr, int i)
{
	while (i > 0)
	{
		i--;
		free(ptr[i]);
	}
	free(ptr);
	return (0);
}

static int	ft_count_words(char const *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			i++;
		else
		{
			count++;
			while (str[i] && str[i] != c)
				i++;
		}
	}
	return (count);
}

static void	ft_putword(char *word, char const *s, int word_len)
{
	int	i;

	i = 0;
	while (i < word_len)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
}

static char	**ft_split_words(char const *s, char c, char **s2, int num_words)
{
	int	i;
	int	word;
	int	word_len;

	i = 0;
	word = 0;
	while (word < num_words)
	{
		while (s[i] && s[i] == c)
			i++;
		word_len = 0;
		while (s[i + word_len] && s[i + word_len] != c)
			word_len++;
		s2[word] = (char *)malloc(sizeof(char) * (word_len + 1));
		if (!s2[word])
			return (free_array(s2, word));
		ft_putword(s2[word], s + i, word_len);
		i += word_len;
		word++;
	}
	s2[word] = 0;
	return (s2);
}

char	**ft_split(char const *s, char c)
{
	char			**s2;
	unsigned int	num_words;

	if (!s)
		return (0);
	num_words = ft_count_words(s, c);
	s2 = (char **)malloc(sizeof(char *) * (num_words + 1));
	if (!s2)
		return (NULL);
	s2 = ft_split_words(s, c, s2, num_words);
	return (s2);
}
