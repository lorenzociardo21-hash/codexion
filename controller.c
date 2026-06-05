#include "codexion.h"

static void	*controlcoders(void *arg)
{
	int		i;
	int		f;
	size_t	diff;
	t_coder	*coder;

	coder = (t_coder *)arg;
	f = 0;
	while (1)
	{
		i = 0;
		while (i < coder->config->number_of_coders)
		{
			pthread_mutex_lock(&coder->config->stop_mutex);
			diff = get_time() - coder[i].last_compile_start;
			if (diff >= coder[i].config->time_to_burnout)
			{
				pthread_mutex_unlock(&coder->config->stop_mutex);
				print_action(&coder[i], "burned out");
				pthread_mutex_lock(&coder->config->stop_mutex);
				coder[i].config->stop_sim = 1;
				pthread_mutex_unlock(&coder->config->stop_mutex);
				f = 1;
				break ;
			}
			pthread_mutex_unlock(&coder->config->stop_mutex);
			i++;
		}
		if (f == 1)
			break ;
		usleep(500);
	}
	return (NULL);
}

void	controller(t_coder *coder)
{
	if (pthread_create(&coder[0].config->controller, NULL, &controlcoders, coder) != 0)
		ft_errorr();
	pthread_join(coder[0].config->controller, NULL);
}
