/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithms.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sarahbanko <sarahbanko@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 14:02:15 by sbanko            #+#    #+#             */
/*   Updated: 2026/08/11 16:37:34 by sarahbanko       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALGORITHMS_H
# define ALGORITHMS_H

# include "../operations/operations.h"
# include "../utils/ft_split.h"
# include "../parsing/parsing.h"

typedef struct s_chunk
{
	int	count;
	int	base_size;
	int	remainder;
	int	start;
	int	end;
	int	elements;
	int	current;
}	t_chunk;

double	compute_disorder(t_stack *s);
int		sort_simple(t_stack *a, t_stack *b, t_opcount *c);
int		sort_complex(t_stack *a, t_stack *b, t_opcount *c);
int		sort_medium(t_stack *a, t_stack *b, t_opcount *c);
int		square_root(int n);
int		process_chunk(t_stack *a, t_stack *b, t_chunk *chunk, t_opcount *c);
int		push_stack_a(t_stack *b, int current_index, t_opcount *c);
int		sort_adaptive(t_stack *a, t_stack *b, t_opcount *c);
void	bench_disorder(double disorder);
void	bench_strategy(t_strategy strat, double disorder, t_opcount *c);

#endif
