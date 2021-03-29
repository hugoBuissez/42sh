#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdbool.h>
#include <stddef.h>

char *str_replace(const char *str, const char *start, const char *old_w,
                  const char *new_w);
char *str_n_replace(const char *str, const char *start, size_t n,
                    const char *new_w);

void str_append(char **str, char *add);
void str_strip_trailing_newlines(char *str);

bool in_range(char c, const char start, const char end);
bool in_list(char c, const char *list);
bool match_seq(char *input, char *seq);

char *itao(int n);
int comp_str(const void *s1, const void *s2);

#endif
