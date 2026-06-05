#include "codexion.h"

void	ft_errorr(void)
{
	write(2, "Error\n", 6);
	exit(1);
}

size_t	get_time(void)
{
	struct timeval	time;
	size_t			start;

	gettimeofday(&time, NULL);
	start = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (start);
}

void	print_action(t_coder *coder, char *action)
{
	size_t	time;

	time = get_time() - coder->config->start_time;
	pthread_mutex_lock(&coder->config->stop_mutex);
	if (coder->config->stop_sim == 1)
	{
		pthread_mutex_unlock(&coder->config->stop_mutex);
		return ;
	}
	pthread_mutex_unlock(&coder->config->stop_mutex);
	pthread_mutex_lock(&coder->config->log_mutex);
	printf("%zu %d %s\n", time, coder->id, action);
	pthread_mutex_unlock(&coder->config->log_mutex);
}
