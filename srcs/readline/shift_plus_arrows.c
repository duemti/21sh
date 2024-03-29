
#include "ft_readline.h"

static int	move_cursor_one_word_left(t_cursor *cursor, char *line)
{
	int		pos;
	int		by;

	by = 0;
	pos = (*cursor).col - (*cursor).col_start - 1;
	while (pos >= 0 && line[pos] && !ft_isalpha(line[pos]))
	{
		pos--;
		(*cursor).col--;
		by++;
	}
	while (pos >= 0 && line[pos] && ft_isalpha(line[pos]))
	{
		pos--;
		(*cursor).col--;
		by++;
	}
	if (by)
		tputs(tgoto(tgetstr("LE", 0), 0, by), 1, ft_puti);
	else
		return (NOTHING_DONE);
	return (DONE);
}

static int	move_cursor_one_word_right(t_cursor *cursor, char *line)
{
	int		pos;
	int		by;
	
	by = 0;
	pos = (*cursor).col - (*cursor).col_start;
	while (pos >= 0 && line[pos] && !ft_isalpha(line[pos]))
	{
		pos++;
		(*cursor).col++;
		by++;
	}
	while (pos >= 0 && line[pos] && ft_isalpha(line[pos]))
	{
		pos++;
		(*cursor).col++;
		by++;
	}
	if (by)
		tputs(tgoto(tgetstr("RI", 0), 0, by), 1, ft_puti);
	else
		return (NOTHING_DONE);
	return (DONE);
}

static int	move_cursor_one_line_up(t_cursor *cursor, t_chain **line)
{
	if ((*line)->prev)
	{
		*line = (*line)->prev;
		(*cursor).col_start = ft_strlen((*line)->prev ? BACKSLASH_PROMPT : PROMPT);
		(*cursor).col_end = (*cursor).col_start + ft_strlen((*line)->value);
		if ((*cursor).col < (*cursor).col_start)
			(*cursor).col = (*cursor).col_start;
		else if ((*cursor).col > (*cursor).col_end)
			(*cursor).col = (*cursor).col_end;
		tputs(CURSOR_UP, 1, ft_puti);
		tputs(tgoto(CH_CURSOR_COL, 0, (*cursor).col), 1, ft_puti);
	}
	else
		return (NOTHING_DONE);
	return (DONE);
}

static int	move_cursor_one_line_down(t_cursor *cursor, t_chain **line)
{
	if ((*line)->next)
	{
		*line = (*line)->next;
		(*cursor).col_start = ft_strlen(BACKSLASH_PROMPT);
		(*cursor).col_end = (*cursor).col_start + ft_strlen((*line)->value);
		if ((*cursor).col < (*cursor).col_start)
			(*cursor).col = (*cursor).col_start;
		else if ((*cursor).col > (*cursor).col_end)
			(*cursor).col = (*cursor).col_end;
		tputs(CURSOR_DO, 1, ft_puti);
		tputs(tgoto(CH_CURSOR_COL, 0, (*cursor).col), 1, ft_puti);
	}
	else
		return (NOTHING_DONE);
	return (DONE);
}

int			shift_plus_arrows(char *ctrl, t_cursor *cursor, t_chain **line)
{
	if (ft_strcmp(SHIFT_LEFT, ctrl) == 0)
		return (move_cursor_one_word_left(cursor, (*line)->value));
	else if (ft_strcmp(SHIFT_RIGHT, ctrl) == 0)
		return (move_cursor_one_word_right(cursor, (*line)->value));
	else if (ft_strcmp(SHIFT_UP, ctrl) == 0)
		return (move_cursor_one_line_up(cursor, line));
	else if (ft_strcmp(SHIFT_DOWN, ctrl) == 0)
		return (move_cursor_one_line_down(cursor, line));
	return (NO_MATCH);
}
