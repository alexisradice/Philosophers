/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradice <aradice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 02:15:46 by alexis            #+#    #+#             */
/*   Updated: 2023/01/17 01:17:33 by aradice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo {
	struct s_data	*data;
	int				id;
}	t_philo;

typedef enum e_type {
	PHILO,
	FORK,
}	t_type;

typedef struct s_arguments {
	int	nb_philos;
	int	time_die;
	int	time_eat;
	int	time_sleep;
	int	nb_philos_must_eat;
}	t_arguments;

typedef struct s_table {
	struct s_data	*data;
	t_arguments		args;
	t_type			type;
	int				id;
	time_t			time_last_meal;
	int				nb_finished_meal;
	struct s_table	*next;
	struct s_table	*prev;
	pthread_mutex_t	mutex;
	pthread_t		thread;
}	t_table;

typedef struct s_data {
	pthread_mutex_t	display;
	pthread_mutex_t	meal;
	t_arguments		*args;
	time_t			start_time;
	int				bool_all_ate;
	int				bool_philo_dead;
}	t_data;

void		ft_args_check(int argc, char **argv);
void		ft_args_check_2(int argc);
t_arguments	*ft_args_parsing(t_data *data, int argc, char **argv);
t_table		*ft_table_init(t_data *data, t_arguments *args, int i);
t_table		*ft_philo_or_fork_init(t_type type, t_data *data,
				t_arguments args, int nb);
void		ft_process(t_data *data, t_table *table);
void		ft_process_2(t_data *data, t_table *table, t_table *temp);
void		ft_philo_eat(t_table *table);
void		*ft_sleep_and_think(void *table);
void		ft_one_philo_eat(t_table *table);
void		ft_philo_die(t_data *data, t_table	*table);
int			ft_philo_die_2(t_data *data, t_table *temp, int i);
void		ft_philo_die_3(t_data *data, t_table *table, t_table *temp, int i);
void		ft_exit_program(t_data *data, t_table *table);
time_t		ft_convert_time(void);
void		ft_wait_time(t_data *data, time_t time);
void		ft_write_text(t_data *data, int id, char *str);
int			ft_atoi(const char *str);

#endif