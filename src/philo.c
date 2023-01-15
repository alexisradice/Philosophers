/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradice <aradice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 02:16:50 by aradice           #+#    #+#             */
/*   Updated: 2023/01/15 03:05:03 by aradice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

time_t	ft_convert_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_write_text(t_data *data, int id, char *str)
{
	pthread_mutex_lock(&data->display);
	if (!(data->philo_dead))
		printf("[%ld] %d %s\n", ft_convert_time() - data->init_time, id + 1, str);
	pthread_mutex_unlock(&data->display);
}

void	ft_check_time(time_t time, t_data *data)
{
	time_t	t;

	t = ft_convert_time();
	while (!(data->philo_dead))
	{
		if (ft_convert_time() - t >= time)
			break ;
		usleep(50);
	}
}

int	main(int argc, char **argv)
{
	t_data		data;
	t_arguments	*args;
	t_table		*table;
	int			i;

	i = 0;
	ft_args_check(argc, argv);
	args = ft_args_parsing(&data, argc, argv);
	if (!args)
		return (1);
	table = ft_table_init(&data, args, i);
	if (!table)
	{
		free(args);
		return (1);
	}
	ft_process(&data, table);
	return (0);
}

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
		args->nb_times_philos_must_eat = ft_atoi(argv[5]);
	else
		args->nb_times_philos_must_eat = -1;
	data->nb_each_ate = 0;
	data->philo_dead = 0;
	data->args = args;
	if (pthread_mutex_init(&(data->display), NULL))
		return (NULL);
	if (pthread_mutex_init(&(data->meal), NULL))
		return (NULL);
	return (args);
}

t_table	*ft_table_init(t_data *data, t_arguments *args, int i)
{
	t_table	*first;
	t_table	*prev;
	t_table	*fork;
	t_table	*philosopher;

	first = ft_philo_or_fork_init(PHILO, *args, 0, data);
	fork = ft_philo_or_fork_init(FORK, *args, 0, data);
	first->next = fork;
	fork->prev = first;
	prev = fork;
	i = 1;
	while (i < args->nb_philos)
	{
		philosopher = ft_philo_or_fork_init(PHILO, *args, i, data);
		prev->next = philosopher;
		philosopher->prev = prev;
		fork = ft_philo_or_fork_init(FORK, *args, i, data);
		philosopher->next = fork;
		fork->prev = philosopher;
		prev = fork;
		i++;
	}
	prev->next = first;
	first->prev = prev;
	return (first);
}

t_table	*ft_philo_or_fork_init(t_type type,
	t_arguments args, int nb, t_data *data)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table)
		return (NULL);
	if (type == FORK)
		pthread_mutex_init(&table->mutex, NULL);
	table->args = args;
	table->type = type;
	table->id = nb;
	table->last_meal = 0;
	table->finished_meal = 0;
	table->data = data;
	table->next = NULL;
	table->prev = NULL;
	return (table);
}

void	ft_philo_die(t_data *data, t_table *table)
{
	t_table	*temp;
	int		i;

	temp = table;
	while (!(data->nb_each_ate))
	{
		i = 0;
		while (temp && !(data->philo_dead))
		{
			if (temp->type == PHILO)
			{
				pthread_mutex_lock(&(data->meal));
				if (ft_philo_die_2(data, temp, i))
					return ;
				pthread_mutex_unlock(&(data->meal));
				usleep(100);
				if (temp == table)
					break ;
			}
		}
		ft_philo_die_3(data, table, temp, i);
	}
}

int	ft_philo_die_2(t_data *data, t_table *temp, int i)
{
	if ((ft_convert_time() - temp->last_meal)
		> data->args->time_die)
	{
		ft_write_text(data, i, "died");
		data->philo_dead = 1;
		pthread_mutex_unlock(&data->meal);
		return (1);
	}
	return (0);
}

void	ft_philo_die_3(t_data *data, t_table *table, t_table *temp, int i)
{
	temp = table;
	i = 0;
	while (temp && data->args->nb_times_philos_must_eat != -1
		&& temp->finished_meal >= data->args->nb_times_philos_must_eat - 1)
	{
		temp = temp->next;
		i++;
	}
	if (i == data->args->nb_philos)
		data->nb_each_ate = 1;
}

void	*ft_sleep_and_think(void *philosopher)
{
	t_table	*philo;
	t_data	*data;
	int		i;

	i = 0;
	philo = (t_table *)philosopher;
	data = philo->data;
	if (philo->id % 2)
		usleep(15000);
	while (!(data->philo_dead))
	{
		ft_philo_eat(philo);
		if (data->nb_each_ate)
			break ;
		ft_write_text(data, philo->id, "is sleeping");
		ft_check_time(data->args->time_sleep, data);
		ft_write_text(data, philo->id, "is thinking");
		i++;
	}
	return (NULL);
}

void	ft_philo_eat(t_table *philo)
{
	t_data	*data;
	t_table	*left_fork;
	t_table	*right_fork;

	data = philo->data;
	left_fork = philo->prev;
	right_fork = philo->next;
	pthread_mutex_lock(&left_fork->mutex);
	pthread_mutex_lock(&right_fork->mutex);
	pthread_mutex_lock(&data->meal);
	ft_write_text(data, philo->id, "has taken a fork");
	ft_write_text(data, philo->id, "has taken a fork");
	ft_write_text(data, philo->id, "is eating");
	philo->last_meal = ft_convert_time();
	ft_check_time(data->args->time_eat, data);
	philo->finished_meal = philo->finished_meal + 1;
	pthread_mutex_unlock(&data->meal);
	pthread_mutex_unlock(&left_fork->mutex);
	pthread_mutex_unlock(&right_fork->mutex);
}

void	ft_exit_program(t_data *data, t_table *table)
{
	t_table	*temp;

	temp = table;
	while (temp)
	{
		if (temp->type == FORK)
		{
			pthread_mutex_destroy(&(temp->mutex));
			temp = temp->next;
		}
		if (temp == table)
			break ;
	}
	pthread_mutex_destroy(&(data->display));
	pthread_mutex_destroy(&(data->meal));
}

void	ft_process(t_data *data, t_table *table)
{
	t_table	*temp;
	int		i;

	i = 0;
	temp = table;
	data->init_time = ft_convert_time();
	while (temp)
	{
		if (temp->type == PHILO)
		{
			pthread_create(&temp->thread, NULL, ft_sleep_and_think, temp);
			temp->last_meal = ft_convert_time();
		}
		temp = temp->next;
		i++;
		if (temp == table)
			break ;
	}
	ft_process_2(data, table, temp);
}

void	ft_process_2(t_data *data, t_table *table, t_table *temp)
{
	ft_philo_die(data, table);
	temp = table;
	while (temp)
	{
		if (temp->type == PHILO)
			pthread_join(temp->thread, NULL);
		temp = temp->next;
		if (temp == table)
			break ;
	}
	if (data->args->nb_philos > 1)
		ft_exit_program(data, table);
}
