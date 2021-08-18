#include "../inc/Database.h"
#include <tuple>
#include <iostream>
#include <boost/algorithm/string.hpp>

void Database::AddTable(const std::string& tab_name) {
    m_tables.emplace(std::make_pair(tab_name,table_t()));
}

std::string Database::Db_Err(Database::db_error err, std::string str)
{
    std::string result;
    switch (err)
    {
    case db_error::OK:
        result = "OK";
        break;
    case db_error::NO_TABLE:
        result = "ERR no such table " + str;
        break;
    case db_error::DUPLICATED:
        result = "ERR duplicate " + str;
        break;
    case db_error::TABLE_IS_EMPTY:
        result = "ERR table is empty " + str;
        break;

    default:
        result = "unknown error code";
        break;
    }
    return result + "\n";
}
Database::db_error Database::Insert(const std::string& tab_name, int id, std::string& value)
{
    if (m_tables.find(tab_name)==m_tables.end())
        return db_error::NO_TABLE;
    
    table_t& table = m_tables[tab_name];
    if(table.find(id)==table.end()) {     
        std::unique_lock<std::mutex> lock(m_mutex);
        table.emplace(id,value);
        return db_error::OK;
    }
    else return db_error::DUPLICATED;
}
Database::db_error Database::Truncate(const std::string& tab_name){
    if (m_tables.find(tab_name)==m_tables.end())
        return db_error::NO_TABLE;
    
    table_t& table = m_tables[tab_name];
    if(!table.empty()) {     
        table.clear();
        return db_error::OK;
    }
    else return db_error::TABLE_IS_EMPTY;
}
std::string Database::Intersection(){
    std::string result;
    table_t& lh = m_tables["A"];
    table_t& rh = m_tables["B"];
    table_t map_out;
    
    table_t::iterator out_itr(map_out.begin());
   
    std::unique_lock<std::mutex> lock(m_mutex);
    std::set_intersection(lh.begin(),lh.end(),
                            rh.begin(),rh.end(),
                            std::inserter(map_out,out_itr),lh.value_comp());
    for (auto const& it : map_out ) {
        result+=std::to_string(it.first)+","+it.second+","+ rh[it.first]+"\n";
    }
    return result;
}
std::string Database::Symmetric_difference(){
    std::string result;
    table_t& lh = m_tables["A"];
    table_t& rh = m_tables["B"];
    table_t map_out;

    table_t::iterator out_itr(map_out.begin());
    
    std::unique_lock<std::mutex> lock(m_mutex);
    std::set_symmetric_difference(lh.begin(),lh.end(),
                            rh.begin(),rh.end(),
                            std::inserter(map_out,out_itr),lh.value_comp());
    for (auto const& it : map_out ) {
        result+=std::to_string(it.first)+","+(lh.find(it.first)==lh.end()? "," :"")+it.second+"\n";
    }
    
    return result;
}
