/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_medium.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbanko <sbanko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 13:08:39 by sarahbanko        #+#    #+#             */
/*   Updated: 2026/08/13 00:00:00 by adrperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "algorithms.h"

static void	init_chunk(t_chunk *chunk, int size)
{
	chunk->count = square_root(size);
	chunk->base_size = size / chunk->count;
	chunk->remainder = size % chunk->count;
	chunk->elements = chunk->base_size;
	if (chunk->remainder > 0)
		chunk->elements++;
	chunk->start = 0;
	chunk->end = chunk->elements - 1;
	chunk->current = 0;
}

static int	move_b_to_a(t_stack *a, t_stack *b, t_opcount *c)
{
	int	current_index;

	if (!a || !b || b->size == 0)
		return (0);
	current_index = b->size - 1;
	while (b->size > 0)
	{
		push_stack_a(b, current_index, c);
		pa(c, a, b);
		current_index--;
	}
	return (0);
}

int	sort_medium(t_stack *a, t_stack *b, t_opcount *c)
{
	t_chunk	chunk;

	if (!a || !b || a->size < 2)
		return (0);
	init_chunk(&chunk, a->size);
	while (chunk.current < chunk.count)
	{
		process_chunk(a, b, &chunk, c);
		chunk.current++;
		chunk.start = chunk.end + 1;
		chunk.elements = chunk.base_size;
		if (chunk.current < chunk.remainder)
			chunk.elements++;
		chunk.end = chunk.start + chunk.elements - 1;
	}
	move_b_to_a(a, b, c);
	return (0);
}
