/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sarahbanko <sarahbanko@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 14:02:44 by sbanko            #+#    #+#             */
/*   Updated: 2026/08/12 11:30:00 by adrperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERATIONS_H
# define OPERATIONS_H

# include "../stack/stack.h"

typedef struct s_opcount
{
	int	sa;
	int	sb;
	int	ss;
	int	pa;
	int	pb;
	int	ra;
	int	rb;
	int	rr;
	int	rra;
	int	rrb;
	int	rrr;
	int	mode;
}	t_opcount;

enum e_op
{
	OP_SA,
	OP_SB,
	OP_SS,
	OP_PA,
	OP_PB,
	OP_RA,
	OP_RB,
	OP_RR,
	OP_RRA,
	OP_RRB,
	OP_RRR
};

void	inc_opcount(t_opcount *c, int type);
void	emit(t_opcount *c, int type, char *op, int len);
int		count_ops(t_opcount *c);
void	swap(t_stack *s);
int		sa(t_opcount *c, t_stack *a);
int		sb(t_opcount *c, t_stack *b);
int		ss(t_opcount *c, t_stack *a, t_stack *b);
int		pa(t_opcount *c, t_stack *a, t_stack *b);
int		pb(t_opcount *c, t_stack *b, t_stack *a);
void	rotate(t_stack *s);
int		ra(t_opcount *c, t_stack *a);
int		rb(t_opcount *c, t_stack *b);
int		rr(t_opcount *c, t_stack *a, t_stack *b);
void	rev_rotate(t_stack *s);
int		rra(t_opcount *c, t_stack *a);
int		rrb(t_opcount *c, t_stack *b);
int		rrr(t_opcount *c, t_stack *a, t_stack *b);

#endif
