/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 20:01:46 by doleksiu          #+#    #+#             */
/*   Updated: 2026/06/29 17:05:03 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	str_to_int(char *str, long long *arg)
{
	int	i;

	i = 0;
	*arg = 0;
	while (str[i])
	{
		if (*arg > (UINT_MAX - (str[i] - '0')) / 10)
			return (1);
		*arg *= 10;
		*arg += (str[i] - '0');
		i++;
	}
	if (*arg > UINT_MAX / 1000)
		return (1);
	return (0);
}

int	check_stop(t_data *data)
{
	pthread_mutex_lock(&data->mutex_stop_sim);
	if (data->stop_simulation)
	{
		pthread_mutex_unlock(&data->mutex_stop_sim);
		return (1);
	}
	pthread_mutex_unlock(&data->mutex_stop_sim);
	return (0);
}

void	ft_sleep_ms(long long time)
{
	struct timeval	start;
	struct timeval	end;
	long long		time_elapsed;

	gettimeofday(&start, NULL);
	usleep(time * 1000 * 0.7);
	while (1)
	{
		gettimeofday(&end, NULL);
		time_elapsed = (end.tv_sec - start.tv_sec) * 1000;
		time_elapsed += (end.tv_usec - start.tv_usec) / 1000;
		if (time_elapsed >= time)
			break ;
		usleep(1000);
	}
}

long long	get_current_time(t_data *data)
{
	struct timeval	current_time;
	long long		time;

	gettimeofday(&current_time, NULL);
	time = (current_time.tv_sec - data->start_time.tv_sec) * 1000;
	time += (current_time.tv_usec - data->start_time.tv_usec) / 1000;
	return (time);
}

int	print_state(t_data *data, int philo_id, char *str)
{
	long long		time;

	pthread_mutex_lock(&data->mutex_print);
	time = get_current_time(data);
	printf("%lld %d %s\n", time, philo_id, str);
	pthread_mutex_unlock(&data->mutex_print);
	return (0);
}
