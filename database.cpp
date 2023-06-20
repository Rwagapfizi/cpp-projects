#include <mysql_driver.h>
#include <mysql_connection.h>

int main()
{
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;

    // Create a MySQL driver instance
    driver = sql::mysql::get_mysql_driver_instance();

    // Establish a connection
    con = driver->connect("localhost:3306", "root", "0723723944");

    // Execute a query
    sql::Statement *stmt = con->createStatement();
    sql::ResultSet *res = stmt->executeQuery("SELECT * FROM mytable");

    // Process the query results
    while (res->next())
    {
        // Process each row of the result set
        std::string name = res->getString("name");
        int age = res->getInt("age");
        // Do something with the data...
    }

    // Clean up
    delete res;
    delete stmt;
    delete con;

    return 0;
}
