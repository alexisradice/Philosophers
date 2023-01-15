/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradice <aradice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 02:16:53 by aradice           #+#    #+#             */
/*   Updated: 2023/01/16 00:43:48 by aradice          ###   ########.fr       */
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
	pthread_mutex_lock(&data->display);
	if (!(data->philo_dead))
		printf("[%ld] %d %s\n", ft_convert_time()
			- data->init_time, id + 1, str);
	pthread_mutex_unlock(&data->display);
}

void	ft_check_time(t_data *data, time_t time)
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
