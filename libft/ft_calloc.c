/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohalaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:25:36 by mohalaou          #+#    #+#             */
/*   Updated: 2024/11/07 23:41:26 by mohalaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*res;

	if (size != 0 && nmemb > SIZE_MAX / size)
		return (NULL);
	res = (void *)malloc(nmemb * size);
	if (res == NULL)
		return (NULL);
	ft_bzero(res, (nmemb * size));
	return (res);
}
