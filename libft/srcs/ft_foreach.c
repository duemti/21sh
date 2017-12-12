/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_foreach.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetrov <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/07 12:48:41 by dpetrov           #+#    #+#             */
/*   Updated: 2017/12/07 12:49:24 by dpetrov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_foreach(char const *s, void (*f)(char))
{
	size_t	i;

	i = 0;
	if (!s || !f)
		return ;
	while (s[i])
		(*f)(s[i++]);
}