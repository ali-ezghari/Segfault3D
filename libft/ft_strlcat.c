/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohalaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 13:26:28 by mohalaou          #+#    #+#             */
/*   Updated: 2024/11/07 18:39:36 by mohalaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	srs_len;
	size_t	i;

	if ((!dst || !src) && size == 0)
		return (ft_strlen(dst) + ft_strlen(src));
	dst_len = ft_strlen(dst);
	srs_len = ft_strlen(src);
	if (size <= dst_len)
		return (size + srs_len);
	i = 0;
	while (src[i] && dst_len + 1 + i < size)
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (dst_len + srs_len);
}
