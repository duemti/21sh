/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetrov <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 11:52:07 by dpetrov           #+#    #+#             */
/*   Updated: 2018/04/18 11:33:26 by dpetrov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"

/*
** 	Extract and concatenate all s_chain 'value' field.
*/
char	 	*ft_assemble_line(t_chain *line)
{
	t_chain	*tmp;
	char	*hold;
	char	*result;

	if ((tmp = line) == NULL)
		return (NULL);
	while (tmp->prev)
		tmp = tmp->prev;
	result = NULL;
	while (tmp)
	{
		hold = result;
		if (check_for_backslashend(tmp, NO) == MATCH)
			tmp->value[ft_strlen(tmp->value) - 1] = '\0';
		if ((result = ft_strjoin(hold, tmp->value)) == NULL && hold && tmp->value)
		{
			ft_log("ft_assemble_line: Insuficient memory.", 1);
			ft_strdel(&result);
			return (NULL);
		}
		ft_strdel(&hold);
		tmp = tmp->next;
	}
	return (result);
}

void	ft_recalibrate_cursor(t_cursor *cursor, int col)
{
	(*cursor).col_start = col;
	(*cursor).col = col;
	(*cursor).col_end = col;
}

int		ft_posinstr(t_cursor cursor)
{
	return (cursor.col - cursor.col_start);
}

/*	Erarse the line on which cursor is positioned.
*/
void		ft_erarse_line(t_cursor *cursor)
{
	if_msc_keypad(HOME_KEY, cursor);
	(*cursor).col = (*cursor).col_start;
	while ((*cursor).col_end --> (*cursor).col_start)
		tputs(DELETE_CHAR, 1, ft_puti);
}

/*	Print a string to STDIN and updates cursor respectively.
*/
void		ft_print_line(t_cursor *cursor, char *line)
{
	ft_putstr_fd(line, STDIN_FILENO);
	(*cursor).col_end += ft_strlen(line) + 1;
	(*cursor).col = (*cursor).col_end;
	tputs(tgoto(CH_CURSOR_COL, 0, (*cursor).col), 1, ft_puti);
}

/*	Replace line on which cursor is positoned .
*/
void		ft_replace_line(char *line, t_cursor *cursor)
{
	ft_erarse_line(cursor);
	ft_print_line(cursor, line);

}

int			ft_insert_char(char **line, t_cursor *cursor, char input)
{
	int		pos;
	int		restore;

	pos = (*cursor).col - (*cursor).col_start;
	if (ft_strichar(line, pos, input))
	{
		ft_log("Insuficient memory for inserting a character.", 1);
		return (RET_ERR);
	}
	restore = ++(*cursor).col;
	(*cursor).col_end++;
	ft_replace_line(*line, cursor);
	(*cursor).col = restore;
	tputs(tgoto(CH_CURSOR_COL, 0, (*cursor).col), 1, ft_puti);
	return (RET_OK);
}
static int	printable_input(char *input, t_chain **history, t_cursor *cursor, t_flag *flags);

char			*ft_readline(void)
{
	t_flag		flags;
	t_chain		*line;
	t_chain		*history;
	t_cursor	cursor;
	char		key[10];

	flags = 0;
	history = ft_init_history();
	if (ft_chainadd_front(&history) == ERR)
	{
		ft_log("ft_chainadd_front: Not enough memory.", 1);
		return (NULL);
	}
	line = ft_chainnew(NULL);

	ft_recalibrate_cursor(&cursor, ft_strlen(PROMPT));

//	find();
	init_terminal_data();
	tty_enable_raw();

	tputs(tgetstr("ks", 0), 1, ft_puti);
	ft_bzero(key, 10);
	ft_putstrstr("", PROMPT);
	while (read(STDIN_FILENO, &key, 9))
	{
		if (ft_isprint(key[0]) || key[0] == '\r')
		{
			if (printable_input(key, &line, &cursor, &flags))
				break;
		}
		else
		{
			if (arrows(key, &cursor, &line, &history) == DONE)
				;
			else if (detect_escape(key, &cursor, &line, &flags) == EXIT)
				break ;
		}
		ft_strclr(key);
	}

	char *result = ft_assemble_line(line);
	ft_update_history(result);
	ft_chainpurge(&history);
	ft_chainpurge(&line);
	tputs(tgetstr("ke", 0), 1, ft_puti);
	tty_disable_raw();

	if (flags & QUOTE_FLAG)
		ft_puterr(NAME, ": unexpected EOF while looking for matching `''.");
	else if (flags & DQUOTE_FLAG)
		ft_puterr(NAME, ": unexpected EOF while looking for matching `\"'.");
	else if (flags & EXIT_FLAG)
	{
		ft_strdel(&result);
		result = ft_strdup("exit");
	}
	else if (flags & CANCEL_FLAG)
		ft_strdel(&result);
	return (result);
}

/*
**	Check if in the end of the string is an extra '\'.
**	'short' = 0 is current t_chain struct, else it shearch on last struct.
*/
int	check_for_backslashend(t_chain *line, short last)
{
	t_chain	*clast;
	int		len;
	int		odd;

	if (last)
	{
		if ((clast = ft_chain_gettail(line)) == NULL)
			return (NOTHING_DONE);
	}
	else
		clast = line;
	if (clast->value == NULL)
		return (NOTHING_DONE);
	odd = 0;
	len = ft_strlen(clast->value) - 1;
	while (len >= 0 && clast->value[len] == '\\')
	{
		odd++;
		len--;
	}
	return (odd % 2 ? MATCH : NO_MATCH);
}

static int	move_cursor_toend(t_chain **line, t_cursor *cursor)
{
	while (shift_plus_arrows(SHIFT_DOWN, cursor, line) == DONE)
		;
	if_msc_keypad(END_KEY, cursor);
	return (DONE);
}

static int	add_new_line(t_chain **line, t_cursor *cursor, char *prompt)
{
	if (ft_chainadd_back(line) == ERR)
	{
		delete_char(&((*line)->value), cursor);
		ft_log("printable_input: Insuficient memory for a new line.", 1);
		return (ERR);
	}
	ft_recalibrate_cursor(cursor, ft_strlen(prompt));
	ft_putstrstr(NEWLINE, prompt);
	return (DONE);
}

static int	printable_input(char *input, t_chain **line, t_cursor *cursor, t_flag *flags)
{
	int		i;

	i = 0;
	*flags = 0;
	while (input[i])
	{
		if (input[i] == '\r')
		{
			proccess_line_for_quotes(*line, flags);
			if (*flags & QUOTE_FLAG)
			{
				if (add_new_line(line, cursor, QUOTE_PROMPT) == ERR)
					return (1);
			}
			else if (*flags & DQUOTE_FLAG)
			{
				if (add_new_line(line, cursor, DQUOTE_PROMPT) == ERR)
					return (1);
			}
			else
			{
				move_cursor_toend(line, cursor);
				if (check_for_backslashend(*line, YES) == MATCH)
				{
					if (add_new_line(line, cursor, BACKSLASH_PROMPT) == ERR)
						return (1);
				}
				else
					return (-1);
			}
		}
		else if (ft_isprint(input[i]))
			if (ft_insert_char(&((*line)->value), cursor, input[i]))
				return (1);
		++i;
	}
	return (0);
}
