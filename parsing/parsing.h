/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbanko <sbanko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 12:18:14 by sarahbanko        #+#    #+#             */
/*   Updated: 2026/08/10 14:06:36 by sbanko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "ft_split.h"
# include "../stack/stack.h"
# include <stdlib.h>
# include <unistd.h>

typedef enum s_strategy {
	SIMPLE,
	MEDIUM,
	COMPLEX,
	ADAPTIVE
}	t_strategy;

typedef struct s_flags {
	int	bench;
	int	mode;
}	t_flags;

int			match_flag(const char *user_flag, const char *real_flag);
int			is_valid_int(const char *str, int *value);
int			has_duplicate(t_stack *a, int value);
char		**build_args(int argc, char **argv, int *out_argc);
t_stack		*parsing(int argc, char **argv, t_strategy *strat, t_flags *flags);

#endif
