#include <iostream>
#include <mysql.h>

//int main() {
//    MYSQL *connection;
//    MYSQL_RES *result;
//    MYSQL_ROW row;
//
//    // Initialize the MySQL connection
//    connection = mysql_init(NULL);
//
//    // Connect to the database
//    if (mysql_real_connect(connection, "localhost", "root", "", "cpp", 0, NULL, 0) == NULL) {
//        std::cerr << "Failed to connect to database: " << mysql_error(connection) << std::endl;
//        return 1;
//    }
//
//    // Execute the SQL query
//    if (mysql_query(connection, "SELECT * FROM student")) {
//        std::cerr << "Failed to execute query: " << mysql_error(connection) << std::endl;
//        return 1;
//    }
//
//    // Get the query results
//    result = mysql_store_result(connection);
//
//    // Retrieve and process the rows
//    while ((row = mysql_fetch_row(result)) != NULL) {
//        std::cout << "Column1: " << row[0] << ", Column2: " << row[1] << std::endl;
//    }
//
//    // Clean up resources and close the connection
//    mysql_free_result(result);
//    mysql_close(connection);
//
//    return 0;
//}

