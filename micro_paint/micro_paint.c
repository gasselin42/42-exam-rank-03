/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasselin <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 10:34:35 by gasselin          #+#    #+#             */
/*   Updated: 2021/09/17 10:55:28 by gasselin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int		H;
int		W;
char	BG;
char	**TAB;

typedef struct draw {
	char	r;
	float	x;
	float	y;
	float	x2;
	float	y2;
	float	w;
	float	h;
	char	c;
}Draw;

int	is_in_rect(int x, int y, Draw el)
{
	if ((x - el.x < 1.0f) || (el.x2 - x < 1.0f) ||
		(y - el.y < 1.0f) || (el.y2 - y < 1.0f))
		return (1);
	return (0);
}

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
		for (int i = 0; i < H; i++) {
			free(TAB[i]);
		}
		free(TAB);
	}
	return (err);
}

int	main(int ac, char **av)
{
	FILE	*fd;
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
					res = fscanf(fd, "\n%c %f %f %f %f %c", &el.r, &el.x, &el.y, &el.w, &el.h, &el.c);
					if (res == EOF)
						return (err_msg(fd, 0));
					else if (res != 6 || el.w <= 0 || el.h <= 0 || (el.r != 'r' && el.r != 'R'))
						break ;
					el.x2 = el.x + el.w;
					el.y2 = el.y + el.h;
					for (int line = 0; line < H; line++) {
						for (int col = 0; col < W; col++) {
							if (line >= el.y && el.y2 >= line && col >= el.x && el.x2 >= col)
							{
								if ((el.r == 'R') || (el.r == 'r' && is_in_rect(col, line, el)))
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
