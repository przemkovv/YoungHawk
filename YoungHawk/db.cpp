


#include "db.h"
FibonacciDatabase::FibonacciDatabase(const std::string &db_file, int flags) : db(connect_to_sqlite(db_file, flags))
{
	std::string create_table_query{ "CREATE TABLE `fibonacci_numbers` ( `n` INTEGER UNIQUE, `number` INTEGER, PRIMARY KEY(`n`) )" };
	execute_query( create_table_query);

}

DBHandler FibonacciDatabase::connect_to_sqlite(const std::string& db_file, int flags)
{
	
	auto logger = make_logger("sqlite3");
	sqlite3 *db;
	int rc;

	rc = sqlite3_open_v2(db_file.data(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
	if (rc)
	{
		logger->error("Error during connecting: {}", sqlite3_errmsg(db));
		sqlite3_close_v2(db);
		return{};
	}
	logger->info("Database connected");

	return{ db, [logger](sqlite3 *db)
	{
		logger->info("Closing db connection");
		sqlite3_close_v2(db);
	} };
}

bool FibonacciDatabase::execute_query( const std::string &query)
{
	auto logger = make_logger("sqlite3");
	char *zErrMsg = nullptr;
	int rc;

	rc = sqlite3_exec(db.get(), query.data(), nullptr, nullptr, &zErrMsg);
	if (rc)
	{
		logger->error("Could not execute '{}' query. Error: {}", query, zErrMsg);
		sqlite3_free(zErrMsg);
		return false;
	}
	return true;
}

bool FibonacciDatabase::insert_fibonacci_number( int n, int64_t number)
{
	const std::string query = "INSERT INTO `fibonacci_numbers` ('n', 'number') VALUES ({}, {})";

	return execute_query( fmt::format(query, n, number));
}

FibonacciNumbers FibonacciDatabase::get_all_fibonacci_numbers()
{
	std::string query = "SELECT * FROM fibonacci_numbers";


	FibonacciNumbers fibonacci_numbers;

	auto add_number_to_vector = [](void *ctx, int argc, char **argv, char **azColName)
	{
		auto &fibonacci_numbers = *static_cast<FibonacciNumbers*>(ctx);
		auto n = std::stoi(argv[0]);
		auto number = std::stoll(argv[1]);

		fibonacci_numbers.push_back({ n, number });
		return 0;
	};
	execute_query( query, add_number_to_vector, fibonacci_numbers);


	return fibonacci_numbers;


}
