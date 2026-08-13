/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_complex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: PersonB <personb@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 00:00:00 by PersonB           #+#    #+#             */
/*   Updated: 2026/08/13 00:00:00 by adrperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "algorithms.h"

static void	process_bit(t_stack *a, t_stack *b, int bit, t_opcount *c)
{
	int	cnt;
	int	i;

	cnt = a->size;
	i = 0;
	while (i < cnt)
	{
		if (((a->top->index >> bit) & 1) == 0)
			pb(c, b, a);
		else
			ra(c, a);
		i++;
	}
	while (b->size > 0)
		pa(c, a, b);
}

int	sort_complex(t_stack *a, t_stack *b, t_opcount *c)
{
	int	max_bit;
	int	bit;

	max_bit = 0;
	while ((a->size - 1) >> max_bit)
		max_bit++;
	bit = 0;
	while (bit < max_bit)
	{
		process_bit(a, b, bit, c);
		bit++;
	}
	return (0);
}
