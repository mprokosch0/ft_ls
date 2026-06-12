#include "ft_ls.h"

void swap_addr(t_entry_info **addr1, t_entry_info **addr2)
{
	t_entry_info *tmp = *addr1;
	*addr1 = *addr2;
	*addr2 = tmp;
}

void	free_struct(t_entry_info **st, size_t len)
{
	if (!st)
		return ;
	for (size_t i = 0; i < len; i++)
	{
		if (st[i])
		{
			if (st[i]->entry_name)
				free(st[i]->entry_name);
			free(st[i]);
		}
	}
	free(st);
}

t_id_cache	*id_cache_create(unsigned int id, char *name)
{
	t_id_cache *new = ft_calloc(1, sizeof(t_id_cache));
	if (!new)
		return NULL;
	new->id = id;
	new->id_str = ft_strdup(name);
	new->next = NULL;
	return new;
}

void	id_cache_add(t_id_cache **head, t_id_cache *new)
{
	t_id_cache *tmp = *head;

	if (!*head)
    {
        *head = new;
        return ;
    }

	while (tmp && tmp->next)
		tmp = tmp->next;

	tmp->next = new;
}

char	*id_cache_get_str_by_id(t_id_cache *head, unsigned int id)
{
	t_id_cache	*tmp = head;
	while (tmp)
	{
		if (tmp->id == id)
			return tmp->id_str;
		tmp = tmp->next;
	}
	return NULL;
}

void	id_cache_free(t_id_cache *head)
{
	t_id_cache	*tmp = head;

	while (head)
	{
		head = head->next;
		free(tmp->id_str);
		free(tmp);
		tmp = head;
	}
}

static unsigned int hash_ext(const char *ext)
{
    unsigned int hash = 5381;
    while (*ext)
        hash = ((hash << 5) + hash) ^ (unsigned char)*ext++;
    return hash & (HASH_SIZE - 1);
}

void	free_map(t_color_map *map)
{
	for (size_t i = 0; i < HASH_SIZE; i++)
	{
		if (map->list[i])
		{
			t_color_entry *tmp = map->list[i];
			while (map->list[i])
			{
				map->list[i] = map->list[i]->next;
				free(tmp->key);
				free(tmp->value);
				free(tmp);	
				tmp = map->list[i];
			}
		}
	}
}

void	color_map_insert(t_color_map *map, char *key, char *value)
{
	unsigned int	h = hash_ext(key);
	t_color_entry	*entry = ft_calloc(1, sizeof(t_color_map));

	if (!entry)
		return ;

	entry->hash = h;
	entry->key = key;
	entry->value = value;
	entry->next = map->list[h];
	map->list[h] = entry;
}


char	*color_map_get(t_color_map *map, char *key)
{
	unsigned int h = hash_ext(key);
	t_color_entry *e = map->list[h];

	while (e)
	{
		if (e->hash == h && !ft_strcmp(key, e->key))
			return e->value;
		e = e->next;
	}
	return NULL;
}