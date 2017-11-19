bool insert(mongoc_collection_t *col, mongoc_insert_flags_t flag, bson_t *doc, bson_error_t *error);
bool sel(mongoc_collection_t *col, bson_t *query);
void printBool(bool vr);
bool update(mongoc_collection_t *col, bson_t *query, bson_t *update, bson_error_t *error);
