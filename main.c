#include <bson.h>
#include <mongoc.h>
#include <stdio.h>
#include "funcdefs.h"

int main (int argc, char *argv[]) {
	mongoc_client_t *client;
	mongoc_collection_t *collection;
	bson_t *doc;
	bson_error_t error;

	mongoc_init();

	client = mongoc_client_new("mongodb://localhost:27017/?appname=insert-example");
	collection = mongoc_client_get_collection(client, "thisdb", "mc");

	doc = bson_new();
	doc = BCON_NEW(
	"foo", "{",
		"jeff", BCON_UTF8("BSON"),
	"}"
	);

	bool ok = insert(collection, MONGOC_INSERT_NONE, doc, &error);
	if (!ok) {
		fprintf(stderr, "%s\n",error.message);
	}
	bson_t *q = bson_new();
	bool hi = sel(collection, q);
	printBool(hi);


	bson_destroy(doc);
	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
	mongoc_cleanup();

	return 0;
}

void printBool(bool vr) {
	printf("%s\n",vr ? "true":"false");
}
bool insert(mongoc_collection_t *col, mongoc_insert_flags_t flag, bson_t *doc, bson_error_t *error) {
	bool ret = mongoc_collection_insert(col,flag,doc,NULL,error);
	return ret;
}

bool sel(mongoc_collection_t *col, bson_t *query) {
	const bson_t *doc;
	char *str;
	mongoc_cursor_t *cursor;
	cursor = mongoc_collection_find_with_opts(col,query,NULL,NULL);
	while (mongoc_cursor_next (cursor, &doc)) {
		str = bson_as_json(doc, NULL);
		printf ("%s\n", str);
		bson_free(str);
	}
	return true;
}
