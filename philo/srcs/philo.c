/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:56:34 by doleksiu          #+#    #+#             */
/*   Updated: 2026/06/30 17:11:13 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

//valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes 
// ./philo 4 100 1 1 1
// valgrind --tool=helgrind ./philo 4 100 1 1 1
// valgrind --tool=drd ./philo 4 100 1 1 1

int	main(int argc, char *argv[])
{
	t_data	data;
	t_philo	*philo_array;

	philo_array = NULL;
	if (init_program_data(argc, argv, &data, &philo_array) != 0)
	{
		if (philo_array)
			clean_exit(&data, philo_array);
		return (1);
	}
	run_simulation(&data, philo_array);
	clean_exit(&data, philo_array);
	return (0);
}

/* creates threads for each of the philos and monitoring, 
 and joins them when finished */

int	run_simulation(t_data *data, t_philo *philo_array)
{
	int		i;
	t_philo	*p;

	i = 0;
	while (i < data->num_of_philos)
	{
		p = &philo_array[i];
		if (pthread_create(&p->philo_thread_id, NULL, &philo_routine, p) != 0)
			return (1);
		i++;
	}
	if (pthread_create(&data->monitor_thread_id, NULL,
			&monitor_routine, philo_array) != 0)
		return (1);
	i = 0;
	while (i < data->num_of_philos)
	{
		p = &philo_array[i];
		if (pthread_join(p->philo_thread_id, NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_join(data->monitor_thread_id, NULL) != 0)
		return (1);
	return (0);
}
