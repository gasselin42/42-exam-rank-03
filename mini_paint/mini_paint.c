/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_paint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasselin <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 10:07:01 by gasselin          #+#    #+#             */
/*   Updated: 2021/09/17 10:33:37 by gasselin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int		W;
int		H;
char	BG;
char	**TAB;

typedef struct draw {
	char	t;
	float	x;
	float	y;
	float	r;
	char	c;
}Draw;

int	err_msg(FILE *fd, int err)
{
	if (err == 2 && (err = 1))
		write(1, "Error: Operation file corrupted\n", 32);
	else if (err == 1)
		write(1, "Error: argument\n", 16);
	else
	{
		for (int i = 0; i < H; i++) {
			write(1, TAB[i], W);
			write(1, "\n", 1);
		}
	}
	if (fd)
		fclose(fd);
	if (TAB)
	{
		for (int j = 0; j < H; j++) {
			free(TAB[j]);
		}
		free(TAB);
	}
	return (err);
}

int	main(int ac, char **av)
{
	FILE	*fd;
	float	sqr;
	int		res;
	Draw	el;

	fd = NULL;
	if (ac != 2)
		return (err_msg(fd, 1));
	if ((fd = fopen(av[1], "r")))
	{
		if ((res = fscanf(fd, "%d %d %c", &W, &H, &BG)) == 3)
		{
			if (W > 0 && W <= 300 && H > 0 && H <= 300)
			{
				TAB = malloc(H * sizeof(char *));
				for (int i = 0; i < H; i++) {
					TAB[i] = malloc(W * sizeof(char));
					memset(TAB[i], BG, W);
				}
				while (1)
				{
					res = fscanf(fd, "\n%c %f %f %f %c", &el.t, &el.x, &el.y, &el.r, &el.c);
					if (res == EOF)
						return (err_msg(fd, 0));
					else if (res != 5 || el.r <= 0 || (el.t != 'C' && el.t != 'c'))
						break ;
					for (int line = 0; line < H; line++) {
						for (int col = 0; col < W; col++) {
							sqr = sqrtf(powf(line - el.y, 2.0) + powf(col - el.x, 2.0));
							if (sqr <= el.r)
							{
								if ((el.t == 'C') || (el.t == 'c' && el.r - sqr < 1.0f))
									TAB[line][col] = el.c;
							}
						}
					}
				}
			}
		}
	}
	return (err_msg(fd, 2));
}
