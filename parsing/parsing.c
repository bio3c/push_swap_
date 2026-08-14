/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbanko <sbanko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 12:20:35 by sarahbanko        #+#    #+#             */
/*   Updated: 2026/08/10 13:57:55 by sbanko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	flag_err(void)
{
	write(2, "Error\n", 6);
	return (0);
}

static int	parse_flags(int argc, char **argv, t_strategy *s, t_flags *f)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (argv[i][0] == '-' && argv[i][1] == '-')
		{
			if (match_flag(argv[i], "--simple"))
				*s = SIMPLE;
			else if (match_flag(argv[i], "--medium"))
				*s = MEDIUM;
			else if (match_flag(argv[i], "--complex"))
				*s = COMPLEX;
			else if (match_flag(argv[i], "--adaptive"))
				*s = ADAPTIVE;
			else if (match_flag(argv[i], "--bench"))
				f->bench = 1;
			else
				return (flag_err());
		}
		i++;
	}
	return (1);
}

static t_stack	*cleanup(t_stack *a)
{
	stack_free(a);
	write(2, "Error\n", 6);
	return (NULL);
}

static int	add_num(t_stack *a, char *arg)
{
	t_node	*n;
	int		v;

	if (!is_valid_int(arg, &v))
		return (0);
	if (has_duplicate(a, v))
		return (0);
	n = new_node(v);
	if (!n)
		return (-1);
	stack_push_top(a, n);
	return (1);
}

t_stack	*parsing(int argc, char **argv, t_strategy *strat, t_flags *flags)
{
	t_stack	*a;
	int		i;

	*strat = ADAPTIVE;
	flags->bench = 0;
	flags->mode = 0;
	if (!parse_flags(argc, argv, strat, flags))
		return (NULL);
	a = stack_init();
	if (!a)
		return (NULL);
	i = argc;
	while (--i >= 1)
		if (argv[i][0] != '-' || argv[i][1] != '-')
			if (!add_num(a, argv[i]))
				return (cleanup(a));
	return (a);
}
