/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rev_rotate.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sarahbanko <sarahbanko@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 14:03:05 by sbanko            #+#    #+#             */
/*   Updated: 2026/08/13 00:00:00 by adrperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "operations.h"

void	rev_rotate(t_stack *s)
{
	t_node	*last_node;

	if (!s || s->size < 2)
		return ;
	last_node = stack_pop_bottom(s);
	stack_push_top(s, last_node);
}

int	rra(t_opcount *c, t_stack *a)
{
	int	count_rra;

	count_rra = 1;
	if (!a || a->size < 2)
		return (0);
	rev_rotate(a);
	emit(c, OP_RRA, "rra\n", 4);
	return (count_rra);
}

int	rrb(t_opcount *c, t_stack *b)
{
	int	count_rrb;

	count_rrb = 1;
	if (!b || b->size < 2)
		return (0);
	rev_rotate(b);
	emit(c, OP_RRB, "rrb\n", 4);
	return (count_rrb);
}

int	rrr(t_opcount *c, t_stack *a, t_stack *b)
{
	int	count_rrr;

	count_rrr = 1;
	if ((!a || a->size < 2) && (!b || b->size < 2))
		return (0);
	rev_rotate(a);
	rev_rotate(b);
	emit(c, OP_RRR, "rrr\n", 4);
	return (count_rrr);
}
