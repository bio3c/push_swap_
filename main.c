/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: PersonA <persona@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 00:00:00 by PersonA           #+#    #+#             */
/*   Updated: 2026/07/19 00:00:00 by PersonA          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"
#include <stdlib.h>
#include "operations.h"

static int	is_sorted(t_stack *a)
{
	t_node	*curr;
	int		i;

	if (a->size <= 1)
		return (1);
	curr = a->top;
	i = 0;
	while (i < a->size - 1)
	{
		if (curr->content > curr->next->content)
			return (0);
		curr = curr->next;
		i++;
	}
	return (1);
}

static void	free_args(char **args, int argc)
{
	int	i;

	i = 1;
	while (i < argc)
		free(args[i++]);
	free(args);
}

static void	run_strategy(t_stack *a, t_stack *b, t_strategy strat, t_opcount *c)
{
	index_stack(a);
	if (strat == SIMPLE)
		sort_simple(a, b, c);
	else if (strat == MEDIUM)
		sort_medium(a, b, c);
	else if (strat == COMPLEX)
		sort_complex(a, b, c);
	else
		sort_adaptive(a, b, c);
}

static void	sort_stack(t_stack *a, t_stack *b, t_strategy strat, int bench)
{
	double		disorder;
	t_opcount	opc;

	opc = (t_opcount){0};
	disorder = 0.0;
	if (bench)
		disorder = compute_disorder(a);
	if (is_sorted(a))
	{
		if (bench)
		{
			bench_disorder(disorder);
			bench_strategy(strat, disorder, &opc);
		}
		return ;
	}
	run_strategy(a, b, strat, &opc);
	if (bench)
	{
		bench_disorder(disorder);
		bench_strategy(strat, disorder, &opc);
	}
}

int	main(int argc, char **argv)
{
	t_strategy	strat;
	int			bench;
	t_stack		*a;
	t_stack		*b;
	char		**args;

	args = build_args(argc, argv, &argc);
	if (!args)
		return (1);
	a = parsing(argc, args, &strat, &bench);
	if (!a)
	{
		free_args(args, argc);
		return (1);
	}
	free_args(args, argc);
	if (a->size == 0)
		return (stack_free(a), 0);
	b = stack_init();
	if (!b)
		return (stack_free(a), 1);
	sort_stack(a, b, strat, bench);
	stack_free(a);
	stack_free(b);
	return (0);
}
