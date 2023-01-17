/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradice <aradice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 22:35:53 by aradice           #+#    #+#             */
/*   Updated: 2023/01/17 01:02:33 by aradice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	ft_args_check(int argc, char **argv)
{
	int	i;
	int	y;

	i = 1;
	ft_args_check_2(argc);
	while (argv[i])
	{
		y = 0;
		if (ft_atoi(argv[i]) <= 0)
		{
			printf("Error: Arguments\n");
			exit(1);
		}
		while (argv[i][y])
		{
			if (argv[i][y] < '0' || argv[i][y] > '9')
			{
				printf("Error: Arguments\n");
				exit(1);
			}
			y++;
		}
		i++;
	}
}

void	ft_args_check_2(int argc)
{
	if (argc != 5 && argc != 6)
	{
		printf("Error: Arguments\n");
		exit(1);
	}
}

t_arguments	*ft_args_parsing(t_data *data, int argc, char **argv)
{
	t_arguments	*args;

	args = malloc(sizeof(t_arguments));
	if (!args)
		return (NULL);
	args->nb_philos = ft_atoi(argv[1]);
	args->time_die = ft_atoi(argv[2]);
	args->time_eat = ft_atoi(argv[3]);
	args->time_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		args->nb_philos_must_eat = ft_atoi(argv[5]);
	else
		args->nb_philos_must_eat = -1;
	data->bool_all_ate = 0;
	data->bool_philo_dead = 0;
	data->args = args;
	if (pthread_mutex_init(&data->display, NULL))
		return (NULL);
	if (pthread_mutex_init(&data->meal, NULL))
		return (NULL);
	return (args);
}
