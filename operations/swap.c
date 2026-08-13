/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sarahbanko <sarahbanko@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 14:01:59 by sbanko            #+#    #+#             */
/*   Updated: 2026/08/13 00:00:00 by adrperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "operations.h"

void	swap(t_stack *s)
{
	t_node	*temp;
	t_node	*temp_2;

	if (!s || s->size < 2)
		return ;
	temp = stack_pop_top(s);
	temp_2 = stack_pop_top(s);
	stack_push_top(s, temp);
	stack_push_top(s, temp_2);
}

int	sa(t_opcount *c, t_stack *a)
{
	int	count_sa;

	count_sa = 1;
	if (!a || a->size < 2)
		return (0);
	swap(a);
	inc_opcount(c, OP_SA);
	write(1, "sa\n", 3);
	return (count_sa);
}

int	sb(t_opcount *c, t_stack *b)
{
	int	count_sb;

	count_sb = 1;
	if (!b || b->size < 2)
		return (0);
	swap(b);
	inc_opcount(c, OP_SB);
	write(1, "sb\n", 3);
	return (count_sb);
}

int	ss(t_opcount *c, t_stack *a, t_stack *b)
{
	int	count_ss;

	count_ss = 1;
	if (!a || !b)
		return (0);
	if (a->size < 2 && b->size < 2)
		return (0);
	swap(a);
	swap(b);
	inc_opcount(c, OP_SS);
	write(1, "ss\n", 3);
	return (count_ss);
}
