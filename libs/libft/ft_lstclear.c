/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlchinen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 19:39:51 by vlchinen          #+#    #+#             */
/*   Updated: 2025/01/20 19:40:17 by vlchinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*node;
	t_list	*next_node;

	next_node = *lst;
	while (next_node)
	{
		node = next_node;
		del(node->content);
		next_node = next_node->next;
		free(node);
	}
	*lst = NULL;
}
