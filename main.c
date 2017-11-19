#include <bson.h>
#include <mongoc.h>
#include <stdio.h>
#include "funcdefs.h"

int main (int argc, char *argv[]) {
	//vars
	mongoc_client_t *client;
	mongoc_collection_t *collection;
	bson_t *doc;
	bson_error_t error;
	bson_oid_t oid;

	//init
	mongoc_init();

	//connect to db
	client = mongoc_client_new("mongodb://localhost:27017/?appname=insert-example");
	collection = mongoc_client_get_collection(client, "thisdb", "mc");

	//initialize oid for examples
	bson_oid_init (&oid, NULL);

	//create new document for insert
	doc = bson_new();
	doc = BCON_NEW(
	"_id", BCON_OID (&oid),
	"foo", "{",
		"jeff", BCON_UTF8("BSON"),
	"}"
	);

	//insert
	bool ok = insert(collection, MONGOC_INSERT_NONE, doc, &error);
	//check errors
	if (!ok) {
		fprintf(stderr, "%s\n",error.message);
	}
	//create new query
	bson_t *q = bson_new();
	//select from collection
	bool hi = sel(collection, q);
	//print if ok
	printBool(hi);

	//update query is by oid
	bson_t *query = BCON_NEW ("_id", BCON_OID (&oid));
	bson_t *u = BCON_NEW(
		"jeff",BCON_UTF8("murnamajaf"),
		"update",BCON_BOOL(true)
	);

	//update and check errors
	bool o = update(collection,query,u,&error);
	if (!o) {
		fprintf(stderr,"%s\n",error.message);
	}


	//select again, and print if ok
	hi = sel(collection, q);
	printBool(hi);

	//test updates when you are not updating anything
	bson_t *query2 = bson_new();
	bson_t *update2 = BCON_NEW(
		"test",BCON_UTF8("Success")
	);
	bool isok = update(collection,query2,update2,&error);
	if (!isok) {
		fprintf(stderr,"%s\n",error.message);
	}
	bson_destroy(query2);
	bson_destroy(update2);


	//cleanup time!
	bson_destroy(doc);
	bson_destroy(query);
	bson_destroy(u);
	bson_destroy(q);
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

bool update(mongoc_collection_t *col, bson_t *query, bson_t *update, bson_error_t *error ) {
	bool ok = mongoc_collection_update(col, MONGOC_UPDATE_NONE, query, update, NULL, error);
	return ok; 
}
