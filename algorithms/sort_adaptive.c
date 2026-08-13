/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_adaptive.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: PersonB <personb@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 00:00:00 by PersonB           #+#    #+#             */
/*   Updated: 2026/08/13 00:00:00 by adrperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "algorithms.h"

static void	sort_three(t_stack *a, t_opcount *c)
{
	int	top;
	int	mid;
	int	bot;

	top = a->top->index;
	mid = a->top->next->index;
	bot = a->top->prev->index;
	if (top > mid && mid < bot && top < bot)
		sa(c, a);
	else if (top > mid && mid > bot)
	{
		sa(c, a);
		rra(c, a);
	}
	else if (top > mid && mid < bot && top > bot)
		ra(c, a);
	else if (top < mid && mid > bot && top < bot)
	{
		sa(c, a);
		ra(c, a);
	}
	else if (top < mid && mid > bot && top > bot)
		rra(c, a);
}

static void	sort_five(t_stack *a, t_stack *b, t_opcount *c)
{
	int	min_pos;
	int	rot;

	while (a->size > 3)
	{
		min_pos = stack_min_pos(a);
		if (min_pos <= a->size / 2)
		{
			rot = min_pos;
			while (rot--)
				ra(c, a);
		}
		else
		{
			rot = a->size - min_pos;
			while (rot--)
				rra(c, a);
		}
		pb(c, b, a);
	}
	sort_three(a, c);
	while (b->size > 0)
		pa(c, a, b);
}

static void	sort_small(t_stack *a, t_stack *b, t_opcount *c)
{
	if (a->size == 2)
	{
		if (a->top->index > a->top->next->index)
			sa(c, a);
	}
	else if (a->size == 3)
		sort_three(a, c);
	else
		sort_five(a, b, c);
}

int	sort_adaptive(t_stack *a, t_stack *b, t_opcount *c)
{
	double	d;

	if (a->size <= 5)
	{
		sort_small(a, b, c);
		return (0);
	}
	d = compute_disorder(a);
	if (d < 0.2)
		sort_simple(a, b, c);
	else if (d < 0.5)
		sort_medium(a, b, c);
	else
		sort_complex(a, b, c);
	return (0);
}
