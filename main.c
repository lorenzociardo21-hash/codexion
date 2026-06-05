#include "codexion.h"

void	docodexion(t_coder *coders)
{
	int	numcode;
	int	i;

	numcode = coders[0].config->number_of_coders;
	i = 0;
	while (i < numcode)
	{
		coders[i].last_compile_start = get_time();
		if (pthread_create(&coders[i].thread_id, NULL, &routine, &coders[i]) != 0)
			ft_errorr();
		i++;
	}
	i = 0;
	controller(coders);
	while (i < numcode)
	{
		pthread_join(coders[i].thread_id, NULL);
		i++;
	}
}

int	main(int argc, char *argv[])
{
	t_config	*config;
	t_coder		*coders;

	if (argc != 9)
		ft_errorr();
	config = parsing(argv);
	coders = codercreate(config);
	docodexion(coders);
	return (0);
}
