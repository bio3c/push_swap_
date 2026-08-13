/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sarahbanko <sarahbanko@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 14:02:58 by sbanko            #+#    #+#             */
/*   Updated: 2026/08/13 00:00:00 by adrperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "operations.h"

int	pa(t_opcount *c, t_stack *a, t_stack *b)
{
	int	count_pa;

	count_pa = 1;
	if (!a || !b || b->size == 0)
		return (0);
	stack_push_top(a, stack_pop_top(b));
	inc_opcount(c, OP_PA);
	write(1, "pa\n", 3);
	return (count_pa);
}

int	pb(t_opcount *c, t_stack *b, t_stack *a)
{
	int	count_pb;

	count_pb = 1;
	if (!a || !b || a->size == 0)
		return (0);
	stack_push_top(b, stack_pop_top(a));
	inc_opcount(c, OP_PB);
	write(1, "pb\n", 3);
	return (count_pb);
}
