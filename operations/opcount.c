/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opcount.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrperei <adrperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 11:30:00 by adrperei          #+#    #+#             */
/*   Updated: 2026/08/13 00:00:00 by adrperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "operations.h"

void	inc_opcount(t_opcount *c, int type)
{
	if (!c)
		return ;
	if (type == OP_SA)
		c->sa++;
	else if (type == OP_SB)
		c->sb++;
	else if (type == OP_SS)
		c->ss++;
	else if (type == OP_PA)
		c->pa++;
	else if (type == OP_PB)
		c->pb++;
	else if (type == OP_RA)
		c->ra++;
	else if (type == OP_RB)
		c->rb++;
	else if (type == OP_RR)
		c->rr++;
	else if (type == OP_RRA)
		c->rra++;
	else if (type == OP_RRB)
		c->rrb++;
	else if (type == OP_RRR)
		c->rrr++;
}
