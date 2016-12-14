//#define FMT_SHARED
#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
//#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "json.hpp"
#include "crow_all.h"

#include <sqlite3.h>
#include <string>
#include <memory>
#include <boost/optional.hpp>



using DBHandler = std::shared_ptr<sqlite3>;

auto make_logger(const std::string &name)
{
	try
	{
		return spdlog::stdout_logger_mt(name, true);
	}
	catch (spdlog::spdlog_ex &) {
		return spdlog::get(name);
	}
}

DBHandler connect_to_sqlite(const std::string &db_file, int flags)
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

template <typename Func = nullptr_t>
auto execute_query(const DBHandler &db, const std::string &query, Func &&func = nullptr)
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

auto insert_fibonacci_number(const DBHandler &db, int n, int64_t number) 
{
	const std::string query = "INSERT INTO `fibonacci_numbers` ('n', 'number') VALUES ({}, {})";

	return execute_query(db, fmt::format(query, n, number));
}

struct FibonacciNumber
{
	int n;
	int64_t number;
};

auto get_all_fibonacci_numbers(const DBHandler &db)
{
	std::string query = "SELECT * FROM fibonacci_numbers";


}

void do_stuff_db()
{
	auto logger = spdlog::stdout_logger_mt("sqlite3", true);
	const std::string db_file{ ":memory:" };
	//const std::string db_file{ "C:\\dev\\young_hawk.db" };


	char *zErrMsg = nullptr;
	int rc;


	auto db = connect_to_sqlite(db_file, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
	if (!db) { return; }


	std::string create_table_query{ "CREATE TABLE `fibonacci_numbers` ( `n` INTEGER UNIQUE, `number` INTEGER, PRIMARY KEY(`n`) )" };
	execute_query(db, create_table_query);

	insert_fibonacci_number(1, 1);
	insert_fibonacci_number(2, 2);
	insert_fibonacci_number(3, 3);


}

int main()
{

	fmt::print("{} \n", "Hello");

	auto logger = spdlog::stdout_logger_mt("abc");
	logger->set_level(spdlog::level::debug);
	//logger->info("abc");

	auto json2 = nlohmann::json::parse("{ \"happy\": true, \"pi\": 3.141 }");
	logger->debug("{}", json2.dump(2));


	//crow::SimpleApp app;

	//CROW_ROUTE(app, "/")([&json2]() {
	//	return json2.dump(2);
	//});

	//app.port(18080).multithreaded().run();

	do_stuff_db();

	return 0;
}