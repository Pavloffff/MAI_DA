#include <stdio.h>
#include <stdlib.h>

typedef struct list
{
	int key;
	struct list *prev;
	struct list *next;
} list;

list head;

void create(list *l)
{
    l->next = l;
    l->prev = l;
}

list *push(list *l, int x)
{
    list *tmp = (list *) malloc(sizeof(list));
    tmp->key = x;
    list *lnext = l->next;
    tmp->prev = l;
    tmp->next = lnext;
    l->next = tmp;
    lnext->prev = tmp;
    return tmp;
}

list *pop(list *l)
{
    list *x = head.prev;
    if (x == &head) return &head;
    list *prev = x->prev;
    prev->next = &head;
    head.prev = prev;
    free(x);
    return prev;
}

int back()
{
    return head.prev->key;
}

int front()
{
    return head.next->key;
}

void f(int k)
{
    list *l = &head;
    while (l->key != k) {
        l = l->next;
    }
    list *prev = l->next;
    list *x = prev->next;
    list *y = x->next;
    list *next = y->next;
    prev->next = next;
    next->prev = prev;
    if (x != &head) free(x);
    if (y != & head) free(y);
}

int main(int argc, char const *argv[])
{
    list *l = &head;
    create(l);
    l = push(l, 5);
    printf("%d\n", back());
    l = push(l, 6);
    // printf("%d\n", back());
    l = push(l, 7);
    printf("%d\n", l->key);
    l = pop(l);
    printf("%d\n", back());
    l = pop(l);
    l = pop(l);
    printf("%d\n", l->key);
    l = push(l, 1);
    l = push(l, 2);
    l = push(l, 3);
    l = push(l, 4);
    l = push(l, 5);
    l = &head;
    for (int i = 0; i < 6; i++) {
        printf("%d ", l->key);
        l = l->next;
    }
    printf("\n");
    f(2);
    l = &head;
    for (int i = 0; i < 6; i++) {
        printf("%d ", l->key);
        l = l->next;
    }
    printf("\n");
    return 0;
}
