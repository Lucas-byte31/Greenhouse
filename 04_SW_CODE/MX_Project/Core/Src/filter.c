/*
 * filter.c
 *
 *  Created on: Jan 5, 2026
 *      Author: lucas
 */


#include "filter.h"


void filter_init(moving_avg_filter_t *f, int size, int init_value)
{
    f->size = size;
    f->index = 0;
    f->sum = init_value * size;

    for (int i = 0; i < size; i++)
        f->buffer[i] = init_value;
}


int filter_update(moving_avg_filter_t *f, int new_value)
{
    f->sum -= f->buffer[f->index];
    f->buffer[f->index] = new_value;
    f->sum += new_value;

    f->index++;
    if (f->index >= f->size)
        f->index = 0;

    return f->sum / f->size;
}
