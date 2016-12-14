#pragma once
#include <sqlite3.h>
#include <memory>
#include <string>

#include "common.h"

using DBHandler = std::shared_ptr<sqlite3>;

class  FibonacciDatabase {
private:
	DBHandler db;
	DBHandler connect_to_sqlite(const std::string &db_file, int flags);

public:
	FibonacciDatabase(const std::string &db_file, int flags);

	template <typename Callback, typename Context>
	bool execute_query( const std::string &query, Callback &&callback, Context &ctx)
	{
		auto logger = make_logger("sqlite3");
		char *zErrMsg = nullptr;
		int rc;
		/*auto clbck = reinterpret_cast<int(*)(void*, int, char**, char**)>(callback);*/
		rc = sqlite3_exec(db.get(), query.data(), callback, &ctx, &zErrMsg);
		if (rc)
		{
			logger->error("Could not execute '{}' query. Error: {}", query, zErrMsg);
			sqlite3_free(zErrMsg);
			return false;
		}
		return true;
	}

	bool execute_query( const std::string &query);


	bool insert_fibonacci_number(int n, int64_t number);
	FibonacciNumbers get_all_fibonacci_numbers();

};