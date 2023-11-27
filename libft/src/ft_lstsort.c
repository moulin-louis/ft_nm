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
    // check if i->next has the same value as i
    if (i->next != NULL && cmp_fn(i->content, i->next->content) == 0) {
      // if so, we need to append i to the end of the node with the same value
      t_list* last_node = i;
      while (last_node != NULL && cmp_fn(i->content, last_node->content) == 0) {
        last_node = last_node->next;
      }
      //we append i to last_node
      i->next = last_node->next;
      last_node->next = i;
    }
  }
}