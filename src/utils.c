/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradice <aradice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 02:16:53 by aradice           #+#    #+#             */
/*   Updated: 2023/01/18 19:45:05 by aradice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	ft_atoi(const char *str)
{
	int		i;
	int		nb;
	int		sign;

	i = 0;
	nb = 0;
	sign = 1;
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		nb *= 10;
		nb += str[i] - '0';
		i++;
	}
	return (nb * sign);
}

time_t	ft_convert_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_write_text(t_data *data, int id, char *str)
{
	pthread_mutex_lock(&data->dead);
	if (!(data->bool_philo_dead))
		printf("[%ldms] - Philo %d %s\n", ft_convert_time()
			- data->start_time, id + 1, str);
	pthread_mutex_unlock(&data->dead);
}

void	ft_wait_time(t_data *data, time_t time)
{
	time_t	t;
	int		bool_philo_dead;

	pthread_mutex_lock(&data->dead);
	bool_philo_dead = data->bool_philo_dead;
	pthread_mutex_unlock(&data->dead);
	t = ft_convert_time();
	while (!(bool_philo_dead))
	{
		pthread_mutex_lock(&data->dead);
		bool_philo_dead = data->bool_philo_dead;
		pthread_mutex_unlock(&data->dead);
		if (ft_convert_time() - t >= time)
			break ;
		usleep(50);
	}
}

void	ft_exit_program(t_data *data, t_table *table)
{
	t_table	*temp;
	t_table	*temp2;
	int		i;

	i = 0;
	temp = table;
	while (temp)
	{
		if (temp->type == FORK)
			pthread_mutex_destroy(&temp->mutex);
		temp = temp->next;
		if (temp == table)
			break ;
	}
	temp = table;
	while (i < data->args->nb_philos * 2)
	{
		temp2 = temp;
		temp = temp->next;
		free(temp2);
		i++;
	}
	pthread_mutex_destroy(&data->display);
	pthread_mutex_destroy(&data->meal);
	pthread_mutex_destroy(&data->dead);
}
