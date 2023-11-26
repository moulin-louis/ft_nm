//
// Created by loumouli on 11/26/23.
//

#include "libft.h"

void ft_lstsort(t_list* head, int (*cmp_fn)(const void*, const void*)) {
  if (cmp_fn == NULL)
    return;
  for (t_list* i = head; i != NULL; i = i->next) {
    for (t_list* j = i->next; j != NULL; j = j->next) {
      if (cmp_fn(i->content, j->content) > 0) {
        void* tmp = i->content;
        i->content = j->content;
        j->content = tmp;
      }
    }
  }
}