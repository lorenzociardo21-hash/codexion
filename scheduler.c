/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lciardo <lciardo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 17:05:08 by lciardo           #+#    #+#             */
/*   Updated: 2026/06/15 11:49:03 by lciardo          ###   ########.fr       */
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

static int	register_req(t_coder *coder, t_dongles *dongle, size_t time)
{
	int	my_idx;

	pthread_mutex_lock(&dongle->lock);
	my_idx = 0;
	if (dongle->queue[0].coder_id != 0)
		my_idx = 1;
	dongle->queue[my_idx].coder_id = coder->id;
	dongle->queue[my_idx].priority = time;
	pthread_mutex_unlock(&dongle->lock);
	return (my_idx);
}

static int	can_take(t_coder *coder, t_dongles *dongle, int my_idx)
{
	int	riv_idx;

	riv_idx = 1 - my_idx;
	if (dongle->flag == 0)
	{
		if (dongle->queue[my_idx].priority < dongle->queue[riv_idx].priority
			|| (dongle->queue[my_idx].priority == dongle->queue[riv_idx].priority
				&& coder->id < dongle->queue[riv_idx].coder_id))
		{
			return (1);
		}
	}
	return (0);
}

static void	deregister_req(t_dongles *dongle, int my_idx)
{
	dongle->queue[my_idx].coder_id = 0;
	dongle->queue[my_idx].priority = ULONG_MAX;
}

static void	while_take(t_coder *coder, size_t time, int	 idx_l, int	 idx_r)
{
		while (1)
	{
		if (stop_simu(coder) == 1)
			return ;
		idx_l = register_req(coder, coder->left_dongle, time);
		idx_r = register_req(coder, coder->right_dongle, time);
		pthread_mutex_lock(&coder->left_dongle->lock);
		pthread_mutex_lock(&coder->right_dongle->lock);

		if (can_take(coder, coder->left_dongle, idx_l) 
			&& can_take(coder, coder->right_dongle, idx_r))
		{
			coder->left_dongle->flag = 1;
			coder->right_dongle->flag = 1;
			pthread_mutex_unlock(&coder->left_dongle->lock);
			pthread_mutex_unlock(&coder->right_dongle->lock);
			return ;
		}
		deregister_req(coder->left_dongle, idx_l);
		deregister_req(coder->right_dongle, idx_r);
		pthread_mutex_unlock(&coder->left_dongle->lock);
		pthread_mutex_unlock(&coder->right_dongle->lock);
		usleep(100);
	}
}
void	take_both_dongles(t_coder *coder)
{
	size_t	time;
	int		idx_l;
	int		idx_r;

	idx_l = 0;
	idx_r = 0;
	if (coder->config->scheduler == 0)
		time = get_time();
	else
		time = coder->last_compile_start + coder->config->time_to_burnout;
	while_take(coder, time, idx_l, idx_r);
}

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
