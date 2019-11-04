#pragma once

#include <wchar.h>

/* auxiliary function for binary search in interval table */
bool bisearch(unsigned int ucs, const struct interval *table, int max);

int mk_wcwidth(unsigned int ucs);
int mk_wcswidth(const unsigned int *pwcs, size_t n);


int mk_wcwidth_cjk(unsigned int ucs);
int mk_wcswidth_cjk(const unsigned int *pwcs, size_t n);
