/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   medium_b_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbanko <sbanko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 12:09:35 by sbanko            #+#    #+#             */
/*   Updated: 2026/08/13 00:00:00 by adrperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "algorithms.h"

static int	find_top_distance_b(t_stack *b, int current_index)
{
	t_node	*current;
	int		distance;

	current = b->top;
	distance = 0;
	while (current->index != current_index)
	{
		distance++;
		current = current->next;
	}
	return (distance);
}

static int	find_bottom_distance_b(t_stack *b, int current_index)
{
	t_node	*current;
	int		distance;

	current = b->top->prev;
	distance = 1;
	while (current->index != current_index)
	{
		distance++;
		current = current->prev;
	}
	return (distance);
}

int	push_stack_a(t_stack *b, int current_index, t_opcount *c)
{
	if (find_top_distance_b(b, current_index)
		< find_bottom_distance_b(b, current_index))
	{
		while (b->top->index != current_index)
			rb(c, b);
	}
	else
	{
		while (b->top->index != current_index)
			rrb(c, b);
	}
	return (0);
}
