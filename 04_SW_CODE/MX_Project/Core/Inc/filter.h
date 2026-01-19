/*
 * filter.h
 *
 *  Created on: Jan 5, 2026
 *      Author: lucas
 */

#ifndef INC_FILTER_H_
#define INC_FILTER_H_

//Structure utilisée pour faire une moyenne glissante dans un filtre numérique
typedef struct
{
    int buffer[8]; // Taille = 8 échantillons
    int index;
    int sum;
    int size;
} moving_avg_filter_t;

// Prototype des fonctions
void filter_init(moving_avg_filter_t *f, int size, int init_value);
int filter_update(moving_avg_filter_t *f, int new_value);



#endif /* INC_FILTER_H_ */
