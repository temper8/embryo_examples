/*
 * embryo_list_loader.c
 *
 *  Created on: Dec 11, 2016
 *      Author: Alex
 */


#include "embryo_list_loader.h"

#include <Elementary.h>
#include <app.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsmn.h"


void readJSONfile(const char *file_name){

	char file_path[PATH_MAX] = {0, };

	char *res_path = app_get_resource_path();
	if (res_path) {
		snprintf(file_path, (int)PATH_MAX, "%s%s", res_path, file_name);
		free(res_path);
	}

	DBG("file_path = %s", file_path);

	off_t file_size;
	char *buffer;
	struct stat stbuf;
	FILE *fp;

	fp = fopen(file_path,  "r");

	if (fp == NULL) {
		DBG("Cannot open file.");
	}

	int fd = fileno(fp);

	if ((fstat(fd, &stbuf) != 0) || (!S_ISREG(stbuf.st_mode))) {
		DBG("Handle error");
	}

	file_size = stbuf.st_size;

	DBG("file_size =%d ", file_size);

	buffer = (char*)malloc(file_size);
	if (buffer == NULL) {
		DBG("Handle error");
	}

}


/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

static const char *JSON_STRING =
	"{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\n  "
	"\"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int embryo_list_loader() {
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	readJSONfile("embyo_list.json");


	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		DBG("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		DBG("Object expected\n");
		return 1;
	}

	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "user") == 0) {
			/* We may use strndup() to fetch string value */
			DBG("- User: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "admin") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
			DBG("- Admin: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "uid") == 0) {
			/* We may want to do strtol() here to get numeric value */
			DBG("- UID: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "groups") == 0) {
			int j;
			DBG("- Groups:\n");
			if (t[i+1].type != JSMN_ARRAY) {
				continue; /* We expect groups to be an array of strings */
			}
			for (j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
				DBG("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
			}
			i += t[i+1].size + 1;
		} else {
			DBG("Unexpected key: %.*s\n", t[i].end-t[i].start,
					JSON_STRING + t[i].start);
		}
	}
	return EXIT_SUCCESS;
}




