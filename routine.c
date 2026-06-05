#include "codexion.h"

static void	onecoder(t_coder *coder)
{
	while (1)
	{
		pthread_mutex_lock(&coder->config->stop_mutex);
		if (coder->config->stop_sim == 1)
		{
			pthread_mutex_unlock(&coder->config->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&coder->config->stop_mutex);
		pthread_mutex_lock(coder->left_dongle);
		print_action(coder, "has taken a dongle");
		if (coder->config->number_of_coders == 1)
		{
			usleep(coder->config->time_to_burnout * 1000);
			pthread_mutex_unlock(coder->left_dongle);
			break ;
		}
	}
}

static void	routinee(t_coder *coder)
{
	while (1)
	{
		pthread_mutex_lock(&coder->config->stop_mutex);
		if (coder->config->stop_sim == 1)
		{
			pthread_mutex_unlock(&coder->config->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&coder->config->stop_mutex);
		pthread_mutex_lock(coder->left_dongle);
		print_action(coder, "has taken a dongle");
		pthread_mutex_lock(coder->right_dongle);
		print_action(coder, "has taken a dongle");
		pthread_mutex_lock(&coder->config->stop_mutex);
		coder->last_compile_start = get_time();
		pthread_mutex_unlock(&coder->config->stop_mutex);
		print_action(coder, "is compiling");
		usleep(coder->config->time_to_compile * 1000);
		pthread_mutex_unlock(coder->left_dongle);
		pthread_mutex_unlock(coder->right_dongle);
		print_action(coder, "is debugging");
		usleep(coder->config->time_to_debug * 1000);
		print_action(coder, "is refactoring");
		usleep(coder->config->time_to_refactor * 1000);
	}
}

void	*routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->config->number_of_coders == 1)
	{
		onecoder(coder);
		return (NULL);
	}
	routinee(coder);
	return (NULL);
}
