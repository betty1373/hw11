#ifndef Database_H
#define Database_H
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <mutex>

using table_t = std::map<int, std::string>;
/// @file
/// @brief Class for database.
/// @author btv<example@example.com>
class Database 
{
public:
	Database() {}
/// @brief add table
  	void AddTable(const std::string& tab_name);
  	enum db_error 
	{
		OK,
		NO_TABLE,
        DUPLICATED,
        TABLE_IS_EMPTY
	};
/// @brief db error to string
	std::string Db_Err(db_error err, std::string str);
/// @brief Insert row in table
	db_error Insert(const std::string& tab_name, int id, std::string& value);
/// @brief delete all rows from table
	db_error Truncate(const std::string& tab_name);
/// @brief Intersection two tables
	std::string Intersection();
/// @brief Symmetric_difference two tables	
	std::string Symmetric_difference();
private:
  	std::mutex m_mutex;
  	std::map<std::string,table_t> m_tables;
};
#endif