/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lciardo <lciardo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 16:01:40 by lciardo           #+#    #+#             */
/*   Updated: 2026/06/05 18:00:16 by lciardo          ###   ########.fr       */
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

typedef struct s_node
{
	int				number_of_coders;
	size_t			time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	int				scheduler;
	pthread_mutex_t	*dongles;
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
	pthread_mutex_t	*left_dongle;
	pthread_mutex_t	*right_dongle;
	size_t			last_compile_start;
}	t_coder;

/* utils.c */
size_t		get_time(void);
void		ft_errorr(void);
void		print_action(t_coder *coder, char *action);

/* parsing.c */
t_config	*parsing(char **av);

/* init.c */
t_coder		*codercreate(t_config *config);

/* routine.c */
void		*routine(void *arg);

/* controller.c */
void		controller(t_coder *coder);

/* main.c */
void		docodexion(t_coder *coders);

#endif
