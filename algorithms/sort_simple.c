/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: PersonB <personb@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 00:00:00 by PersonB           #+#    #+#             */
/*   Updated: 2026/08/13 00:00:00 by adrperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "algorithms.h"

int	sort_simple(t_stack *a, t_stack *b, t_opcount *c)
{
	int	min_idx;
	int	rot;

	if (a->size <= 1)
		return (0);
	while (a->size > 0)
	{
		min_idx = stack_min_pos(a);
		if (min_idx <= a->size / 2)
		{
			rot = min_idx;
			while (rot--)
				ra(c, a);
		}
		else
		{
			rot = a->size - min_idx;
			while (rot--)
				rra(c, a);
		}
		pb(c, b, a);
	}
	while (b->size > 0)
		pa(c, a, b);
	return (0);
}
