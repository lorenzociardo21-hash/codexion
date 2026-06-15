/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lciardo <lciardo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 16:01:40 by lciardo           #+#    #+#             */
/*   Updated: 2026/06/15 11:48:47 by lciardo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <unistd.h>
# include <stdio.h>
# include <ctype.h>
# include <string.h>
# include <stdlib.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_request
{
	int		coder_id;
	size_t	priority;
}	t_request;

typedef struct s_dongles
{
	pthread_mutex_t	lock;
	size_t			last_release_time;
	int				flag;
	t_request		queue[2];
}	t_dongles;

typedef struct s_node
{
	int				number_of_coders;
	size_t			time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	size_t			dongle_cooldown;
	int				scheduler;
	t_dongles		*dongles;
	size_t			start_time;
	pthread_mutex_t	log_mutex;
	int				stop_sim;
	pthread_mutex_t	stop_mutex;
	pthread_t		controller;
}	t_config;

typedef struct s_coder
{
	int				id;
	pthread_t		thread_id;
	t_config		*config;
	t_dongles		*left_dongle;
	t_dongles		*right_dongle;
	size_t			last_compile_start;
	int				n_compile;
}	t_coder;

int			stop_simu(t_coder *coder);
size_t		get_time(void);
void		ft_cleanup(t_config *config, t_coder *coders);
void		ft_errorr(t_config *config, t_coder *coders);
void		print_action(t_coder *coder, char *action);
void		ft_usleep(size_t milliseconds, t_coder *coder);
t_config	*parsing(char **av);
t_coder		*codercreate(t_config *config);
void		docodexion(t_coder *coders);
void		*routine(void *arg);
void		controller(t_coder *coder);
void		release_dongle(t_coder *coder, t_dongles *dongle);
void		take_both_dongles(t_coder *coder);

#endif
