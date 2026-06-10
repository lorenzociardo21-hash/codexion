/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lciardo <lciardo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 17:05:08 by lciardo           #+#    #+#             */
/*   Updated: 2026/06/10 13:19:32 by lciardo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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


static void	priority_queue(t_coder *coder, t_dongles *dongle, int my_idx)
{
	int	riv_idx;

	riv_idx = 1 - my_idx;
	while (1)
	{
		if (stop_simu(coder) == 1)
			return ;
		pthread_mutex_lock(&dongle->lock);
		if (dongle->flag == 0)
		{
			if (dongle->queue[my_idx].priority < dongle->queue[riv_idx].priority
				|| (dongle->queue[my_idx].priority == dongle->queue[riv_idx].priority
					&& coder->id < dongle->queue[riv_idx].coder_id))
			{
				dongle->flag = 1;
				pthread_mutex_unlock(&dongle->lock);
				return ;
			}
		}
		pthread_mutex_unlock(&dongle->lock);
		usleep(100);
	}
}


void	take_dongle(t_coder *coder, t_dongles *dongle)
{
	size_t	time;
	int		my_idx;

	pthread_mutex_lock(&dongle->lock);
	if (coder->config->scheduler == 0)
		time = get_time();
	else
		time = coder->last_compile_start + coder->config->time_to_burnout;
	my_idx = 0;
	if (dongle->queue[0].coder_id != 0)
		my_idx = 1;
	dongle->queue[my_idx].coder_id = coder->id;
	dongle->queue[my_idx].priority = time;
	pthread_mutex_unlock(&dongle->lock);
	priority_queue(coder, dongle, my_idx);
}

void	release_dongle(t_coder	*coder, t_dongles	*dongle)
{
	int	my_idx;

	pthread_mutex_lock(&dongle->lock);
	dongle->flag = 0;
	my_idx = 0;
	if (dongle->queue[1].coder_id == coder->id)
		my_idx = 1;
	dongle->queue[my_idx].coder_id = 0;
	dongle->queue[my_idx].priority = ULONG_MAX;
	pthread_mutex_unlock(&dongle->lock);
}
