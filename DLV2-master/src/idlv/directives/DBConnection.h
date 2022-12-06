/*******************************************************************************
 * Copyright 2016 Francesco Calimeri, Davide Fusca', Simona Perri and Jessica Zangari
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/
/*
 * DBManager.h
 *
 *  Created on: Jul 14, 2016
 *      Author: jessica
 */

#ifndef SRC_DIRECTIVES_DBCONNECTION_H_
#define SRC_DIRECTIVES_DBCONNECTION_H_

#ifdef MINGW
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#endif

#ifdef SQL
#include <sql.h>
#include <sqlext.h>
#endif

#include <cstring>
#include "../grounder/table/PredicateExtension.h"
using namespace std;

namespace DLV2 {
namespace grounder {

#define ODBC_MAX_STRING_LENGTH   1001
enum ValueConv { U_INT = 0, UT_INT, UR_INT, T_INT, R_INT, CONST_, Q_CONST, DEFAULT_};

#ifdef SQL
typedef pair<SQLSMALLINT,SQLHANDLE> SQLException;
class DBConnection {
public:
	DBConnection(const string& d, const string& u, const string& p):db(d),username(u),password(p),connected(false),hEnv(0),hDBc(0),row_open(false){}
	void connect();
    void disconnect();
    bool isConnected() const { return connected; }

    // Determine the number of resulting columns of the given query
    // which corresponds to the predicate arity
    unsigned determineNumberOfColumns(const string& query, const string& pName);

    Predicate* determinePredicate(const string& query, const string& pName);

    // Insert an atom with predicate name 'pname' in the table table_name.
    bool exportIntoDB(Predicate* predicate, const string& table_name, const string& query);

    // Insert an atom with predicate name 'pname' in the table table_name.
    bool optimizeExportIntoDB(Predicate* predicate, const string& table_name, const string& query);

    // Execute the query requested by the user and build an new predicate pName
    // whose atoms are the results of the query.
	bool importFromDB(const string& query, const string& pName, const string& type);

private:
	string db,username,password;
    bool connected;
    SQLHENV hEnv;
    SQLHDBC hDBc;

    bool row_open;
    vector<Term*> terms;

    // Start a new atom if necessary.
    void rdbms_rowcheck(const string& pName);

    // Set the n_th term to an integer.
    void rdbms_sendInt(unsigned long i, const string& pName);

    // Set n_th term to a string without quotes.
    void rdbms_sendConst(char *str, const string& pName);

    // Set the n_th term to a string with quotes.
    void rdbms_sendString(char *str, unsigned len, const string& pName);

    // Build the fact.
    void rdbms_sendRowEnd(const string& pName, unsigned arity);

    void executeSQLStatement(const string& sql) const;

    // Set numcol to number of columns in the table.
    bool getNumberOfColumns(SQLHSTMT *V_OD_hstmt, SQLSMALLINT &numcol);

    // Insert the type of every column in the selected table into vectype.
    void describeColumn(SQLSMALLINT *vectype, SQLSMALLINT numcol, SQLHSTMT *V_OD_hstmt);

    // Trim whitespace from the end of a string.
    char *rtrim(char *st );

    // Return the substring of a string that does not start with whitespace.
    char *ltrim(char *st );

    // Create a vector with conversion operators for every column of the table
    // and return the size of this vector in r.
    bool prepareConversion(const char *s, ValueConv v[], int &r);

    // Execute an SQL statement.
    bool insertString(const char *s, SQLHSTMT *V_OD_hstmt );

    // Returns false if an error happens, true otherwise.
    // Sets status to 0 if the table does not exist,
    //               -1 if the table exists with different arity, and
    //                1 if the table exists, and also fills vectype with
    //                  the types of the columns in this case.
    bool existsTable(const string table_name, const unsigned  arity, int &status, SQLSMALLINT *vectype);

    // This function creates a record to be inserted into a given table on the
    // currently connected database.
    char *insert_tuple (string ins, char **pa, unsigned arity, SQLSMALLINT *vectype );

    // This function deletes all or some records from a given table in the
    // currently connected database.
    //
    // @param ntable, the name of the output table.
    // @param query,  the WHERE condition of the SQL statement for the table tuple
    //                deletion; a query param is on the form:
    //                     REPLACE        WHERE_CONDITION
    //                    (keyword)       (SQL_STATEMENT)
    //                keyword must be removed in order to properly use this param.
    bool delete_tuple(char *ntable, char *query );
};
#else
class DBConnection {
public:
	DBConnection(const string& d, const string& u, const string& p){}
	void connect(){}
    void disconnect(){}
    bool isConnected() const { return false; }
    void executeSQLStatement(const string& sql) const{}
    bool delete_tuple(char *ntable, char *query ){
    	return false;
    }

    bool exportIntoDB(Predicate* predicate, const string& table_name, const string& query){
    	ErrorMessage::errorDBConnection("sql import/export not supported.");
    	return false;
    }

    bool optimizeExportIntoDB(Predicate* predicate, const string& table_name, const string& query){
    	ErrorMessage::errorDBConnection("sql import/export not supported.");
    	return false;
    }

	bool importFromDB(const string& query, const string& pName, const string& type){
    	ErrorMessage::errorDBConnection("sql import/export not supported.");
    	return false;
	}

    Predicate* determinePredicate(const string& query, const string& pName){
    	ErrorMessage::errorDBConnection("sql import/export not supported.");
    	return nullptr;
    }


private:

};
#endif

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_DIRECTIVES_DBCONNECTION_H_ */
