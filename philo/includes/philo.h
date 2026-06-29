/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:56:51 by doleksiu          #+#    #+#             */
/*   Updated: 2026/04/13 12:00:17 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_PHILO 200

typedef struct s_philo	t_philo;

typedef struct s_data
{
	long long		num_of_philos;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	long long		num_of_times_to_eat;
	int				stop_simulation;
	int				finished_eating_count;
	struct timeval	start_time;
	pthread_t		monitor_thread_id;
	pthread_mutex_t	mutex_print;
	pthread_mutex_t	mutex_stop;
	t_philo			*philo_array;
}				t_data;

struct s_philo
{
	int				philo_id;
	pthread_t		philo_thread_id;
	int				eat_count;
	long long		deathtime;
	pthread_mutex_t	mutex_fork;
	pthread_mutex_t	mutex_deathtime;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
};

int			init_program_data(int argc, char *argv[],
				t_data *data, t_philo **philo_array);
void		clean_exit(t_data *data, t_philo *philo_array);
int			run_simulation(t_data *data, t_philo *philo_array);
void		*philo_routine(void *arg);
void		*monitor_routine(void *arg);

//utils.c
int			str_to_int(char *str, long long *arg);
int			check_stop(t_data *data);
void		ft_sleep_ms(long long time);
long long	get_current_time(t_data *data);
int			print_state(t_data *data, int philo_id, char *str);
