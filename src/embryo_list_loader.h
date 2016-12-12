/*
 * embryo_list_loader.h
 *
 *  Created on: Dec 11, 2016
 *      Author: Alex
 */

#ifndef EMBRYO_LIST_LOADER_H_
#define EMBRYO_LIST_LOADER_H_

#include "logger.h"


typedef struct {
	char *title;
	char *description;
	char *edj_file;
} embryo_item_t;


embryo_item_t *embryo_list;

int embryo_list_loader();




#endif /* EMBRYO_LIST_LOADER_H_ */
