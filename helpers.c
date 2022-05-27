#include "minisynth.h"

void	print_list(t_node *head)
{
	t_node *list;

	list = head;
	while (list)
	{
		printf("'%f' - '%f'\t", list->frequency, list->len);
		list = list->next;
	}
	printf("\n");
}

int strings_match(char *str1, char *str2, int char_nb)
{
	if (strncmp(str1, str2, char_nb) == 0)
		return (1);
	return (0);
}

void	free_list(t_node **head)
{
	t_node *tmp;

	if (!head)
		return ;
	while ((*head))
	{
		tmp = (*head)->next;
		free(*head);
		*head = NULL;
		*head = tmp;
	}
}
