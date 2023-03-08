#ifndef DUCKDB_BUILD_LOADABLE_EXTENSION
#define DUCKDB_BUILD_LOADABLE_EXTENSION
#endif
#include "duckdb.hpp"

#include "sqlite_scanner.hpp"
#include "sqlite_storage.hpp"

#include "duckdb/catalog/catalog.hpp"
#include "duckdb/parser/parsed_data/create_table_function_info.hpp"

using namespace duckdb;

extern "C" {

DUCKDB_EXTENSION_API void sqlite_scanner_init(duckdb::DatabaseInstance &db) {
	Connection con(db);
	con.BeginTransaction();
	auto &context = *con.context;
	auto &catalog = Catalog::GetSystemCatalog(context);

	SqliteScanFunction sqlite_fun;
	CreateTableFunctionInfo sqlite_info(sqlite_fun);
	catalog.CreateTableFunction(context, &sqlite_info);

	SqliteAttachFunction attach_func;

	CreateTableFunctionInfo attach_info(attach_func);
	catalog.CreateTableFunction(context, &attach_info);

	auto &config = DBConfig::GetConfig(db);
	config.AddExtensionOption("sqlite_all_varchar", "Load all SQLite columns as VARCHAR columns", LogicalType::BOOLEAN);

	//   # DO NOT INCLUDE THE STORAGE PART; NOT NEEDED FOR READONLY ACCESS
	// config.storage_extensions["sqlite_scanner"] = make_unique<SQLiteStorageExtension>();

	con.Commit();
}

DUCKDB_EXTENSION_API const char *sqlite_scanner_version() {
	return DuckDB::LibraryVersion();
}

//   # DO NOT INCLUDE THE STORAGE PART; NOT NEEDED FOR READONLY ACCESS
// DUCKDB_EXTENSION_API void sqlite_scanner_storage_init(DBConfig &config) {
// 	config.storage_extensions["sqlite_scanner"] = make_unique<SQLiteStorageExtension>();
// }
}
