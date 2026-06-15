/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lciardo <lciardo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 17:05:30 by lciardo           #+#    #+#             */
/*   Updated: 2026/06/15 12:15:45 by lciardo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	release_dongle(t_coder *coder, t_dongles *dongle)
{
	int	my_idx;

	pthread_mutex_lock(&dongle->lock);
	dongle->last_release_time = get_time();
	dongle->flag = 0;
	my_idx = 0;
	if (dongle->queue[1].coder_id == coder->id)
		my_idx = 1;
	dongle->queue[my_idx].coder_id = 0;
	dongle->queue[my_idx].priority = ULONG_MAX;
	pthread_mutex_unlock(&dongle->lock);
}

int	stop_simu(t_coder *coder)
{
	int	f;

	f = 0;
	pthread_mutex_lock(&coder->config->stop_mutex);
	if (coder->config->stop_sim == 1)
		f = 1;
	pthread_mutex_unlock(&coder->config->stop_mutex);
	return (f);
}

int	main(int argc, char *argv[])
{
	t_config	*config;
	t_coder		*coders;

	if (argc != 9)
		ft_errorr(NULL, NULL);
	config = parsing(argv);
	coders = codercreate(config);
	docodexion(coders);
	ft_cleanup(config, coders);
	return (0);
}
