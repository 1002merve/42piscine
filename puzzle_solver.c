#include <unistd.h>
#include <stdlib.h>

/*
** clues layout:
**   [0..3]  = top view of columns 0..3
**   [4..7]  = bottom view of columns 0..3
**   [8..11] = left view of rows 0..3
**   [12..15]= right view of rows 0..3
**
** A clue of 0 means no constraint.
*/

static int	clues[16];
static int	grid[4][4];

static int	count_visible(int *line, int len)
{
	int	max;
	int	count;
	int	i;

	max = 0;
	count = 0;
	i = 0;
	while (i < len)
	{
		if (line[i] > max)
		{
			max = line[i];
			count++;
		}
		i++;
	}
	return (count);
}

/* Call only when the row is fully filled (col == 3 just placed) */
static int	check_row(int row)
{
	int	line[4];
	int	rev[4];
	int	i;

	i = 0;
	while (i < 4)
	{
		line[i] = grid[row][i];
		rev[i] = grid[row][3 - i];
		i++;
	}
	if (clues[8 + row] && count_visible(line, 4) != clues[8 + row])
		return (0);
	if (clues[12 + row] && count_visible(rev, 4) != clues[12 + row])
		return (0);
	return (1);
}

/* Call only when the column is fully filled (row == 3 just placed) */
static int	check_col(int col)
{
	int	line[4];
	int	rev[4];
	int	i;

	i = 0;
	while (i < 4)
	{
		line[i] = grid[i][col];
		rev[i] = grid[3 - i][col];
		i++;
	}
	if (clues[col] && count_visible(line, 4) != clues[col])
		return (0);
	if (clues[4 + col] && count_visible(rev, 4) != clues[4 + col])
		return (0);
	return (1);
}

static int	is_used_in_row(int row, int col, int val)
{
	int	i;

	i = 0;
	while (i < col)
	{
		if (grid[row][i] == val)
			return (1);
		i++;
	}
	return (0);
}

static int	is_used_in_col(int row, int col, int val)
{
	int	i;

	i = 0;
	while (i < row)
	{
		if (grid[i][col] == val)
			return (1);
		i++;
	}
	return (0);
}

static int	solve(int pos)
{
	int	row;
	int	col;
	int	val;
	int	ok;

	if (pos == 16)
		return (1);
	row = pos / 4;
	col = pos % 4;
	val = 1;
	while (val <= 4)
	{
		if (!is_used_in_row(row, col, val) && !is_used_in_col(row, col, val))
		{
			grid[row][col] = val;
			ok = 1;
			if (col == 3 && !check_row(row))
				ok = 0;
			if (ok && row == 3 && !check_col(col))
				ok = 0;
			if (ok && solve(pos + 1))
				return (1);
			grid[row][col] = 0;
		}
		val++;
	}
	return (0);
}

static void	parse_clues(char *s)
{
	int		i;

	i = 0;
	while (i < 16)
	{
		while (*s && (*s < '0' || *s > '9'))
			s++;
		if (!*s)
			break ;
		clues[i++] = *s - '0';
		s++;
	}
	if (i != 16)
	{
		write(1, "Error\n", 6);
		exit(1);
	}
}

static void	print_solution(void)
{
	int		row;
	int		col;
	char	c;

	row = 0;
	while (row < 4)
	{
		col = 0;
		while (col < 4)
		{
			c = '0' + grid[row][col];
			write(1, &c, 1);
			if (col < 3)
				write(1, " ", 1);
			col++;
		}
		write(1, "\n", 1);
		row++;
	}
}

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		write(1, "Error\n", 6);
		return (1);
	}
	parse_clues(argv[1]);
	if (!solve(0))
	{
		write(1, "Error\n", 6);
		return (1);
	}
	print_solution();
	return (0);
}
