/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   medium_a_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbanko <sbanko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 16:30:23 by sbanko            #+#    #+#             */
/*   Updated: 2026/08/13 00:00:00 by adrperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "algorithms.h"

int	square_root(int n)
{
	int	root;

	if (n <= 0)
		return (0);
	root = 1;
	while (root <= n / root)
		root++;
	return (root - 1);
}

static int	find_top_distance_a(t_stack *a, int start, int end)
{
	t_node	*current;
	int		distance;

	current = a->top;
	distance = 0;
	while (!(current->index >= start && current->index <= end))
	{
		distance++;
		current = current->next;
	}
	return (distance);
}

static int	find_bottom_distance_a(t_stack *a, int start, int end)
{
	t_node	*current;
	int		distance;

	current = a->top->prev;
	distance = 1;
	while (!(current->index >= start && current->index <= end))
	{
		distance++;
		current = current->prev;
	}
	return (distance);
}

static int	move_chunk_to_top_a(t_stack *a, int start, int end, t_opcount *c)
{
	if (find_top_distance_a(a, start, end)
		< find_bottom_distance_a(a, start, end))
	{
		while (!(a->top->index >= start
				&& a->top->index <= end))
			ra(c, a);
	}
	else
	{
		while (!(a->top->index >= start
				&& a->top->index <= end))
			rra(c, a);
	}
	return (0);
}

int	process_chunk(t_stack *a, t_stack *b, t_chunk *chunk, t_opcount *c)
{
	int	sent;

	sent = 0;
	while (sent < chunk->end - chunk->start + 1)
	{
		move_chunk_to_top_a(a, chunk->start, chunk->end, c);
		pb(c, b, a);
		if (b->top->index < (chunk->start + chunk->end) / 2)
			rb(c, b);
		sent++;
	}
	return (0);
}
