#include <criterion/criterion.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

Test(list, create_destroy)
{
    struct list *list = list_init();
    list_destroy(list);
}

Test(list, push_couple)
{
    struct list *list = list_init();

    cr_expect_eq(list_empty(list), true);

    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));
    int *c = malloc(sizeof(int));

    list_push(list, a);
    list_push(list, b);
    list_push(list, c);

    cr_expect_eq(list_size(list), 3);

    int *c_r = list_pop(list);
    int *b_r = list_pop(list);
    int *a_r = list_pop(list);

    cr_expect_eq(list_size(list), 0);
    cr_expect_eq(list_empty(list), true);

    cr_expect_eq(a, a_r);
    cr_expect_eq(b, b_r);
    cr_expect_eq(c, c_r);

    free(a);
    free(b);
    free(c);

    list_destroy(list);
}

Test(list, append_couple)
{
    struct list *list = list_init();

    cr_expect_eq(list_empty(list), true);

    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));
    int *c = malloc(sizeof(int));

    list_append(list, a);
    list_append(list, b);
    list_append(list, c);

    cr_expect_eq(list_size(list), 3);

    int *a_r = list_pop(list);
    int *b_r = list_pop(list);
    int *c_r = list_pop(list);

    cr_expect_eq(list_size(list), 0);
    cr_expect_eq(list_empty(list), true);

    cr_expect_eq(a, a_r);
    cr_expect_eq(b, b_r);
    cr_expect_eq(c, c_r);

    free(a);
    free(b);
    free(c);

    list_destroy(list);
}

int comp_int(void *_a, void *_b)
{
    int *a = _a;
    int *b = _b;
    return *a - *b;
}

Test(list, insert_sort)
{
    struct list *list = list_init();

    cr_expect_eq(list_empty(list), true);

    int a = 10;
    int b = 2;
    int c = 5;
    int d = 1;
    int e = 42;

    list_insert_sort(list, &a, comp_int);
    list_insert_sort(list, &b, comp_int);
    list_insert_sort(list, &c, comp_int);
    list_insert_sort(list, &d, comp_int);
    list_insert_sort(list, &e, comp_int);

    cr_expect_eq(list_size(list), 5);

    // Should give (1, 2, 5, 10, 42) => (d, b, c, a, e)

    int expected[] = { 1, 2, 5, 10, 42 };

    struct list_iterator *iter = iterator_init(list);
    int *element = NULL;
    int i = 0;

    while ((element = iterator_next(iter)))
        cr_expect_eq(*element, expected[i++]);

    iterator_destroy(iter);

    list_free_elements(list, NULL);
    list_destroy(list);
}

Test(list, iterator)
{
    struct list *list = list_init();

    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));
    int *c = malloc(sizeof(int));

    list_append(list, a);
    list_append(list, b);
    list_append(list, c);

    struct list_iterator *iterator = iterator_init(list);

    cr_expect_eq(iterator_get(iterator), a);
    cr_expect_eq(iterator_next(iterator), a);
    cr_expect_eq(iterator_next(iterator), b);
    cr_expect_eq(iterator_next(iterator), c);
    cr_expect_null(iterator_get(iterator));

    free(iterator);

    list_free_elements(list, free);
    list_destroy(list);
}
