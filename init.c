#include "codexion.h"

static void	codercreatee(int numcode, t_config *config, t_coder *coders)
{
	int	i;

	i = 0;
	while (i < numcode)
	{
		coders[i].config = config;
		coders[i].id = i + 1;
		if (i == 0)
		{
			coders[i].left_dongle = &config->dongles[i];
			coders[i].right_dongle = &config->dongles[numcode - 1];
		}
		else
		{
			coders[i].left_dongle = &config->dongles[i - 1];
			coders[i].right_dongle = &config->dongles[i];
		}
		i++;
	}
}

t_coder	*codercreate(t_config *config)
{
	int		numcode;
	t_coder	*coders;

	numcode = config->number_of_coders;
	coders = malloc(sizeof(t_coder) * numcode);
	if (!coders)
	{
		free(config->dongles);
		free(config);
		ft_errorr();
	}
	codercreatee(numcode, config, coders);
	return (coders);
}
