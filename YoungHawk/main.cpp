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


#include "db.h"




void do_stuff_db()
{
	auto logger = spdlog::stdout_logger_mt("sqlite3", true);
	const std::string db_file{ ":memory:" };
	//const std::string db_file{ "C:\\dev\\young_hawk.db" };

	FibonacciDatabase fdb{ db_file, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE };


	fdb.insert_fibonacci_number(1, 1);
	fdb.insert_fibonacci_number( 2, 2);
	fdb.insert_fibonacci_number(3, 3);

	auto fibos = fdb.get_all_fibonacci_numbers();

	for (auto & fibo : fibos )
	{
		logger->info("{} {}", fibo.n, fibo.number);
	}
}

int main()
{

	fmt::print("{} \n", "Hello");

	auto logger = spdlog::stdout_logger_mt("abc");
	logger->set_level(spdlog::level::debug);
	//logger->info("abc");

	auto json2 = nlohmann::json::parse("{ \"happy\": true, \"pi\": 3.141 }");
	logger->debug("{}", json2.dump(2));




	do_stuff_db();

	crow::SimpleApp app;

	CROW_ROUTE(app, "/")([&json2]() {
		return json2.dump(2);
	});

	CROW_ROUTE(app, "/<int>")([](const crow::request& req) {
		req.url_params.get("aaa");
		return crow::response(200, "A");
	});

	app.port(18080).multithreaded().run();

	return 0;
}