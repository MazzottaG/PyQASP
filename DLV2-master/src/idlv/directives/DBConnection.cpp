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
 * DBManager.cpp
 *
 *  Created on: Jul 14, 2016
 *      Author: jessica
 */

#include "../../common/Assert.h"
#include "../../common/ErrorMessage.h"
#include "DBConnection.h"

namespace DLV2 {
namespace grounder {

#ifdef SQL
void DBConnection::connect() {
	if(connected) return;
    string sourceString;
    sourceString.append("DSN=").append(db).
    		append(";UID=").append(username).append(";PWD=").
			append(password).append(";");

    SQLRETURN retCode;
    try
    {
        // Allocate environment handle
        retCode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
        if( !SQL_SUCCEEDED(retCode) )
            throw SQLException(SQL_HANDLE_ENV,hEnv);
        // Set the ODBC version environment attribute
        retCode = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
        if( !SQL_SUCCEEDED(retCode) )
            throw SQLException(SQL_HANDLE_ENV,hEnv);
        // Allocate connection handle
        retCode = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDBc);
        if( !SQL_SUCCEEDED(retCode) )
            throw SQLException(SQL_HANDLE_DBC,hDBc);
        // Set login timeout to 5 seconds
        retCode = SQLSetConnectAttr(hDBc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
        if( !SQL_SUCCEEDED(retCode) )
            throw SQLException(SQL_HANDLE_DBC,hDBc);
        // Connect to the datasource
		retCode = SQLDriverConnect(
			hDBc,
			NULL,
			(SQLCHAR*)sourceString.c_str(),
			sourceString.size(),
			NULL,
			0,
			NULL,
			SQL_DRIVER_NOPROMPT);
        if( !SQL_SUCCEEDED(retCode) )
            throw SQLException(SQL_HANDLE_DBC,hDBc);
        else
        	connected = true;
    }
    catch( SQLException& exception )
    {
        SQLSMALLINT i = 1;
        SQLSMALLINT MsgLen;
        SQLINTEGER NativeError;
        SQLCHAR SqlState[6];
        SQLCHAR Msg[SQL_MAX_MESSAGE_LENGTH];
        retCode = SQLGetDiagRec(exception.first, exception.second, i, SqlState, &NativeError,Msg, sizeof(Msg), &MsgLen);
        if( retCode == SQL_SUCCESS ){
        	ErrorMessage::errorDBConnection("ODBC Error during connection.");
            ErrorMessage::errorDBConnection(Msg);
        }
        else
            ErrorMessage::errorDBConnection("ODBC Unknown Error during connection");
    }
}

void DBConnection::disconnect() {
   if(!connected) return;
    SQLRETURN retCode;
    try{
        // Free handles, and disconnect.
        if (hDBc) {
            retCode = SQLDisconnect(hDBc);
            if( !SQL_SUCCEEDED(retCode) )
                throw SQLException(SQL_HANDLE_DBC,hDBc);
            connected = false;

            retCode = SQLFreeHandle(SQL_HANDLE_DBC, hDBc);
            if( !SQL_SUCCEEDED(retCode) )
                throw SQLException(SQL_HANDLE_DBC,hDBc);
            hDBc = NULL;
        }
        if (hEnv) {
            retCode = SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
            if( !SQL_SUCCEEDED(retCode) )
                throw SQLException(SQL_HANDLE_DBC,hEnv);
            hEnv = NULL;
        }
    }
    catch( SQLException& exception )
    {
        SQLSMALLINT i = 1;
        SQLSMALLINT MsgLen;
        SQLINTEGER NativeError;
        SQLCHAR SqlState[6];
        SQLCHAR Msg[SQL_MAX_MESSAGE_LENGTH];
        retCode = SQLGetDiagRec(exception.first, exception.second, i, SqlState, &NativeError,Msg, sizeof(Msg), &MsgLen);
        if( retCode == SQL_SUCCESS ){
        	ErrorMessage::errorDBConnection("ODBC Error during disconnection.");
            ErrorMessage::errorDBConnection(Msg);
        }
        else
            ErrorMessage::errorDBConnection("ODBC Unknown Error during disconnection");
    }
}

void DBConnection::executeSQLStatement(const string& sql) const
{
    SQLRETURN retCode;
    SQLHSTMT hStmt = 0;
    try
    {
        // Allocate statement handle
        retCode = SQLAllocHandle(SQL_HANDLE_STMT, hDBc, &hStmt);
        if( !SQL_SUCCEEDED(retCode) )
            throw SQLException(SQL_HANDLE_STMT,hStmt);

        retCode = SQLExecDirect(hStmt,(SQLCHAR*)sql.c_str(),SQL_NTS);
        if( !SQL_SUCCEEDED(retCode) )
            throw SQLException(SQL_HANDLE_STMT,hStmt);

        retCode = SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        if( !SQL_SUCCEEDED(retCode) )
            throw SQLException(SQL_HANDLE_STMT,hStmt);
    }
    catch( SQLException& exception )
    {
        SQLSMALLINT i = 1;
        SQLSMALLINT MsgLen;
        SQLINTEGER NativeError;
        SQLCHAR SqlState[6];
        SQLCHAR Msg[SQL_MAX_MESSAGE_LENGTH];
        retCode = SQLGetDiagRec(exception.first, exception.second, i, SqlState, &NativeError,Msg, sizeof(Msg), &MsgLen);
        if( retCode == SQL_SUCCESS )
            ErrorMessage::errorDBConnection(Msg);
        else
            ErrorMessage::errorDBConnection("ODBC Unknown Error when executing statement: "+sql);
    }
}


bool DBConnection::getNumberOfColumns(SQLHSTMT *V_OD_hstmt, SQLSMALLINT &numcol){
	numcol=0;

	int res = SQLNumResultCols(*V_OD_hstmt, &numcol);
	if ( res != SQL_SUCCESS  &&  res != SQL_SUCCESS_WITH_INFO )
	{
		disconnect();
		return false;
	}

	return true;
}

void DBConnection::describeColumn(SQLSMALLINT *vectype, SQLSMALLINT numcol, SQLHSTMT *V_OD_hstmt){
    SQLSMALLINT namelength,dataType,decimaldigits,nullable;
    SQLULEN columnsize;

    for (SQLUSMALLINT j=0; j < numcol; j++){
		char columname[ODBC_MAX_STRING_LENGTH];

		SQLDescribeCol(*V_OD_hstmt, j+1,
					   (SQLCHAR*)columname, ODBC_MAX_STRING_LENGTH,
					   &namelength,
					   &dataType, &columnsize,
					   &decimaldigits, &nullable);
		vectype[j]=dataType;
	}
}

char* DBConnection::rtrim(char *st ){
    int l;
    l = strlen(st)-1;
    while(st[l] == ' ')
        st[l--] = '\0';
    return st;
}

char* DBConnection::ltrim(char *st ){
    while( *st == ' ' )
        st++;
    return st;
}

bool DBConnection::prepareConversion(const char *s, ValueConv v[], int &r)
{
    r=0;

    char *sp= new char [ strlen(s) +1];
    int t = 0;
    for (unsigned int i = 0;  i <= strlen(s); i++)
	{
        if( i == strlen(s)  || s[i] == ',' )
		{
            if (strncasecmp (ltrim(sp),"UT_INT",6) == 0)
                v[r] = UT_INT;
            else if (strncasecmp (ltrim(sp),"CONST",5) == 0)
                v[r] = CONST_;
            else if (strncasecmp (ltrim(sp),"Q_CONST",7) == 0)
                v[r] = Q_CONST;
            else if (strncasecmp (ltrim(sp),"UR_INT",6) == 0)
                v[r] = UR_INT;
            else if (strncasecmp (ltrim(sp),"U_INT",5) == 0)
                v[r] = U_INT;
            else if (strncasecmp (ltrim(sp),"T_INT",5) == 0)
                v[r] = T_INT;
            else if (strncasecmp (ltrim(sp),"R_INT",5) == 0)
                v[r] = R_INT;
            else
			{
        		ErrorMessage::errorDBConnection("ODBC Syntax Error during import: invalid conversion type(s) specified");
                disconnect();
                delete[] sp;
                return false;
			}
            r++;
            t = 0;
		}
        else
		{
            sp[t] = s[i];
            t++;
		}
	}
    delete[] sp;
    return true;
}

bool DBConnection::insertString(const char *s, SQLHSTMT *V_OD_hstmt ){
    int res = SQLAllocHandle(SQL_HANDLE_STMT, hDBc, V_OD_hstmt);
    if( res != SQL_SUCCESS  &&  res != SQL_SUCCESS_WITH_INFO )
	{
		string msg="ODBC Error during environment allocation: "+ to_string(res);
		ErrorMessage::errorDBConnection(msg);
        disconnect();
        return false;
	}

    // From MSDN:  When an ODBC 3.x application calls SQLExecDirect,
    // SQLExecute, or SQLParamData in an ODBC 2.x driver to execute
    // a searched update or delete statement that does not affect any
    // rows at the data source, the driver should return SQL_SUCCESS,
    // not SQL_NO_DATA.  When an ODBC 2.x or ODBC 3.x application working
    // with an ODBC 3.x driver calls SQLExecDirect, SQLExecute, or
    // SQLParamData with the same result, the ODBC 3.x driver should
    // return SQL_NO_DATA.

    res = SQLExecDirect(*V_OD_hstmt,(SQLCHAR*)s, SQL_NTS);
    if( res != SQL_SUCCESS &&  res != SQL_SUCCESS_WITH_INFO &&  res != SQL_NO_DATA )
	{
    	string msg="ODBC Error during execution of the statement "+string(s)+" ("+to_string(res)+")";
    	ErrorMessage::errorDBConnection(msg);
        disconnect();
        return false;
	}

    return true;
}

bool DBConnection::existsTable(const string table_name, const unsigned  arity, int &status, SQLSMALLINT *vectype)
{
    SQLHSTMT V_OD_hstmt;

    int res = SQLAllocHandle(SQL_HANDLE_STMT, hDBc, &V_OD_hstmt);
    if ( res != SQL_SUCCESS && res != SQL_SUCCESS_WITH_INFO )
	{
    	string msg="ODBC Error during environment allocation: "+ to_string(res);
    	ErrorMessage::errorDBConnection(msg);
        disconnect();
        return false;
	}

    // SQLBindCol truncates the retrieved information to length, so
    // we set a suitable length to distinguish tables which share the
    // same string prefix.
    unsigned length = table_name.size() + 2;
    char *t_name = new char[length];
    SQLLEN V_OD_err;
    // SQLTables returns the table name in the 3rd position.
    SQLBindCol( V_OD_hstmt, 3, SQL_C_CHAR, t_name, length, &V_OD_err );

    char sTABLE[strlen("TABLE")+1];
    strcpy(sTABLE,"TABLE");

    res = SQLTables( V_OD_hstmt, (SQLCHAR *) NULL /*db_n*/, SQL_NTS,
                     NULL, SQL_NTS, (SQLCHAR *) NULL /*table*/, SQL_NTS,
                     (SQLCHAR*)sTABLE, SQL_NTS );
    if( res != SQL_SUCCESS  &&  res != SQL_SUCCESS_WITH_INFO )
	{
    	string msg= "ODBC Error when retrieving the list of tables from the data source: "+ to_string(res);
    	ErrorMessage::errorDBConnection(msg);
        disconnect();
        delete[] t_name;
        return false;
	}

    // Verify if the table exists.
    status = 0;
    char *c_tname = new char[length];
    strcpy(c_tname,table_name.c_str());
    while ( status != 1  &&  SQLFetch(V_OD_hstmt) != SQL_NO_DATA )
        if ( ! strncasecmp(t_name,c_tname, length) )
            status = 1;
    // Deallocate our dynamic structures.
    SQLFreeHandle (SQL_HANDLE_STMT,V_OD_hstmt);
    delete[] t_name;
    delete[] c_tname;

    // Check arity.
    if ( status == 1 )
	{
        string tmp = "SELECT * FROM " + table_name;
        char *q = new char[tmp.size()+1];
        strcpy(q,tmp.c_str());
        bool insert=insertString(q, &V_OD_hstmt);
        delete[] q;
        if ( !insert)
            return false;

        SQLSMALLINT numberCol = 0;
        if ( !getNumberOfColumns(&V_OD_hstmt, numberCol) ){
        	string msg="ODBC Error when retrieving the number of columns of the statement: "+tmp;
			ErrorMessage::errorDBConnection(msg);
            return false;
        }

        if ( numberCol != (SQLSMALLINT)arity )
            status = -1;
        else
            describeColumn(vectype,arity,&V_OD_hstmt);

        // Free handle for SQL-statement.
        SQLFreeHandle (SQL_HANDLE_STMT,V_OD_hstmt);
	}

    return true;
}

char* DBConnection::insert_tuple (string ins, char **pa, unsigned arity, SQLSMALLINT *vectype )
{
    for (unsigned k = 0; k < arity; k++)
	{
        if ( vectype[k] == SQL_C_LONG
             || vectype[k] == SQL_C_NUMERIC
             || vectype[k] == SQL_C_SLONG
             || vectype[k] == SQL_C_ULONG
             || vectype[k] == SQL_C_DOUBLE
             || vectype[k] == SQL_C_FLOAT
             || vectype[k] == SQL_C_SHORT
             || vectype[k] == SQL_C_SSHORT
             || vectype[k] == SQL_C_USHORT
             || vectype[k] == SQL_BIGINT
             || vectype[k] == SQL_DECIMAL
             || vectype[k] == SQL_FLOAT
             || vectype[k] == SQL_TINYINT )
            ins += pa[k];

        if ( vectype[k] == SQL_C_CHAR
             || vectype[k] == SQL_C_BINARY
             || vectype[k] == SQL_C_TYPE_DATE
             || vectype[k] == SQL_C_TYPE_TIMESTAMP
             || vectype[k] == SQL_LONGVARCHAR
             || vectype[k] == SQL_VARCHAR
             || vectype[k] == SQL_C_BINARY )
            ins += '\'' + string(pa[k]) + '\'';

        if( k == arity-1 )
            ins += ")";
        else
            ins += ", ";
	}
    char *s = new char[ins.size()+1];
    strcpy( s, ins.c_str());
    return s;
}

bool DBConnection::delete_tuple(char *ntable, char *query ){
    char *lquery = ltrim(query);

    // Give up if the query does not start with "REPLACE" or if the
    // condition following "REPLACE" is not empty, but is not separated from
    // "REPLACE" by whitespace.
    if ( strncmp (lquery, "REPLACE", 7) ||
         ( strlen(lquery+7) == strlen(ltrim(lquery+7)) &&
         strlen(ltrim(lquery+7)) > 0))
	{
    	ErrorMessage::errorDBConnection("ODBC Syntax Error in the writing of REPLACE.");
        disconnect();
        return true;
	}

    string tmp("DELETE FROM ");
    tmp += ntable;
    tmp += lquery + 7;
    char *st = new char[tmp.size()+1];
    strcpy(st, tmp.c_str());

    SQLHSTMT V_OD_hstmt ;
    if (!insertString(st, &V_OD_hstmt)){
    	delete[] st;
        return false;
    }
    // to free a handle for a SQL-statement
    SQLFreeHandle (SQL_HANDLE_STMT,V_OD_hstmt);
    delete[] st;
    return true;
}

bool DBConnection::exportIntoDB(Predicate* predicate, const string& table_name, const string& query){
    if (!query.empty()){
        char * tn = new char[table_name.size()+1];
        strcpy(tn, table_name.c_str());
        char *qst = new char[query.size()+1];
        strcpy(qst, query.c_str());
        bool del = delete_tuple(tn, qst);
        delete [] tn;
        delete [] qst;
        if ( !del ){
        	ErrorMessage::errorDBConnection("ODBC Error during deletion.");
        	return false;
        }
    }

    SQLHSTMT V_OD_hstmt;
	unsigned parity = predicate->getArity();
//    unsigned count = 0;
//        if ( OptionPrintFacts ) FIXME
//            EDB.exportMatching (pname, atoms, count);
//        if ( A != 0 )
//            {
//            bool empty = A->isPositivePartEmpty();
//            if ( ! empty )
//                {
//                TERMS pterms;
//                for ( unsigned i = 0; i < parity; i++ )
//                    {
//                    TERM id(0,0);
//                    pterms.push_back(id);
//                    }
//                // The exact type (EDB/IDB) of the predicate is not known yet,
//                // but not important at this point, so we default to typeUndef.
//                ATOM ato1(pname,&pterms,PREDICATE_NAMES::typeUndef);
//                ATOMSET::FIND_RESULT res;
//
//                A->findInPositivePart(ato1,res);
//                for ( ; !res.atEnd(); res++ )
//                    {
//                    atoms.push_back(*res);
//                    count++;
//                    }
//                }
//            }
//        if ( count == 0 )
//            return false;

	PredicateExtension* predicateExtension=PredicateExtTable::getInstance()->getPredicateExt(predicate);
	if(predicateExtension->getTable(0)->size()==0 && predicateExtension->getTable(1)->size()==0){
		return true;
	}

	int exist = 0;
	string stm = "";
	SQLSMALLINT *vectype =  new SQLSMALLINT [parity];
	if ( !existsTable(string(table_name), parity, exist, vectype) ){
		delete[] vectype;
		return false;
	}
	if ( exist == 0 )
	{
		Atom* atom=(predicateExtension->getTable(0)->size()>0)
				?predicateExtension->getTable(0)->front()
				:predicateExtension->getTable(1)->front();

		unsigned  att_index = 1;
		stm = "CREATE TABLE " + string(table_name) + " (";
		for (unsigned j=0;j<atom->getTermsSize();j++,att_index++)
		{
			switch ( atom->getTerm(j)->getType() )
			{
				case TermType::NUMERIC_CONSTANT && predicateExtension->getPredicateInformation()->isNumeric(j):
				{
					// set default type for integer to INTEGER
					stringstream ss;
					ss << "a" << att_index << " INTEGER"
					   << (att_index == parity ? ")" : ",");
					stm += ss.str();
					vectype[att_index-1] = SQL_INTEGER;
					break;
				}
				case TermType::VARIABLE:
				case TermType::ANONYMOUS:
				case TermType::ARITH:
					break;
				default: //Symbolic, string and functional terms are represented as VARCHAR by default
				{
					// set default type for string to VARCHAR(255)
					stringstream ss;
					ss << "a" << att_index << " VARCHAR(255)"
					   << (att_index == parity ? ")" : ",");
					stm += ss.str();
					vectype[att_index-1] = SQL_VARCHAR;
					break;
				}
			} // switch
		} // FOR
		char *st = new char[stm.size()+1];
		strcpy(st, stm.c_str());
		SQLHSTMT V_OD_hstmt;
		if ( !insertString(st,&V_OD_hstmt) ){
			delete[] vectype;
			delete[] st;
			return false;
		}
		// to free a handle for a SQL-statement
		SQLFreeHandle(SQL_HANDLE_STMT,V_OD_hstmt);
		delete[] st;
	}
	else if ( exist == -1 )
	{
		string msg="ODBC Warning: table " +table_name+" exists but with different arity";
		ErrorMessage::errorDBConnection(msg);
		disconnect();
		delete[] vectype;
		return false;
	}
	for(unsigned i=0;i<=NOFACT;i++){
		AtomHistoryVector* atoms=predicateExtension->getTable(i);
		for(auto atom: *atoms)
		{
			if(!atom->isFact()) continue;  // TODO check
			stm = "INSERT INTO " + string(table_name) + " VALUES( ";
			unsigned t = 0;
			char **pa = new char* [parity];
			for (unsigned j=0;j<atom->getTermsSize();j++)
			{
				Term* k=atom->getTerm(j);
				switch( k->getType() )
				{
					case TermType::NUMERIC_CONSTANT && predicateExtension->getPredicateInformation()->isNumeric(j):
					{
						int *p = new int(k->getConstantValue());
						pa[t] = new char[50];
						sprintf(pa[t],"%d",*p);
						t++;
						delete p;
						break;
					}
					case TermType::VARIABLE:
					case TermType::ANONYMOUS:
					case TermType::ARITH:
						break;
					default:
					{
						string tmp = (*k).toString();
						// Replace all occurrences of single quotes ('\'') by
						// two occurrences of this character.
						string::size_type index = tmp.find("'",0);
						while ( index != string::npos )
						{
							tmp.replace(index,1,"''");
							index = tmp.find("'",index+2);
						}
						// If the string starts with a double quote ('\"'),
						// i.e. it is a double quoted string, strip the first
						// and last characters from the string.
						if ( tmp.find("\"",0) != string::npos )
						{
							tmp.erase(0,1);
							tmp.erase(tmp.size()-1,1);
						}
						pa[t] = new char[tmp.size()+1];
						strcpy(pa[t],tmp.c_str());
						t++;
						break;
					}
				} //  switch
			} // FOR
			char *tup = insert_tuple(stm, pa, parity, vectype);
			if ( !insertString(tup, &V_OD_hstmt)){
				delete[] vectype;
				delete[] tup;
				for ( unsigned i = 0; i < parity; i++ )
					delete[] pa[i];
				delete[] pa;
				return false;
			}
			// to free a handle for a SQL-statement
			SQLFreeHandle (SQL_HANDLE_STMT,V_OD_hstmt);
			delete[] tup;
			for ( unsigned i = 0; i < parity; i++ )
				delete[] pa[i];
			delete[] pa;
		}
	}

    disconnect();
    delete[] vectype;
    return true;
}

bool DBConnection::optimizeExportIntoDB(Predicate* predicate, const string& table_name, const string& query){
    if (!query.empty()){
        char * tn = new char[table_name.size()+1];
        strcpy(tn, table_name.c_str());
        char *qst = new char[query.size()+1];
        strcpy(qst, query.c_str());
        bool del = delete_tuple(tn, qst);
        delete [] tn;
        delete [] qst;
        if ( !del ){
        	ErrorMessage::errorDBConnection("ODBC Error during deletion");
        	return false;
        }
    }

	SQLHSTMT V_OD_hstmt;
    unsigned parity = predicate->getArity();

    PredicateExtension* predicateExtension=PredicateExtTable::getInstance()->getPredicateExt(predicate);
    if(predicateExtension->getTable(0)->size()==0 && predicateExtension->getTable(1)->size()==0){
    	return true;
    }

	int exist = 0;
	string stm = "";
	SQLSMALLINT *vectype =  new SQLSMALLINT [parity];
	if ( !existsTable(string(table_name), parity, exist, vectype) ){
		delete[] vectype;
		return false;
	}
	if ( exist == 0 )
	{
		Atom* atom=(predicateExtension->getTable(0)->size()>0)
				?predicateExtension->getTable(0)->front()
				:predicateExtension->getTable(1)->front();

		unsigned  att_index = 1;
		stm = "CREATE TABLE " + string(table_name) + " (";
		for (unsigned j=0;j<atom->getTermsSize();j++,att_index++)
		{
			switch ( atom->getTerm(j)->getType() )
			{
				case TermType::NUMERIC_CONSTANT && predicateExtension->getPredicateInformation()->isNumeric(j):
				{
					// set default type for integer to INTEGER
					stringstream ss;
					ss << "a" << att_index << " INTEGER"
					   << (att_index == parity ? ")" : ",");
					stm += ss.str();
					vectype[att_index-1] = SQL_INTEGER;
					break;
				}
				case TermType::VARIABLE:
				case TermType::ANONYMOUS:
				case TermType::ARITH:
					break;
				default: //Symbolic, string and functional terms are represented as VARCHAR by default
				{
					// set default type for string to VARCHAR(255)
					stringstream ss;
					ss << "a" << att_index << " VARCHAR(255)"
					   << (att_index == parity ? ")" : ",");
					stm += ss.str();
					vectype[att_index-1] = SQL_VARCHAR;
					break;
				}
			} // switch
		} // FOR
		char *st = new char[stm.size()+1];
		strcpy(st, stm.c_str());
		SQLHSTMT V_OD_hstmt;
		if ( !insertString(st,&V_OD_hstmt) ){
			delete[] vectype;
			delete[] st;
			return false;
		}
		// to free a handle for a SQL-statement
		SQLFreeHandle(SQL_HANDLE_STMT,V_OD_hstmt);
		delete[] st;
	}
	else if ( exist == -1 )
	{
		string msg="ODBC Warning: table " +table_name+" exists but with different arity";
		ErrorMessage::errorDBConnection(msg);
		disconnect();
		delete[] vectype;
		return false;
	}
	string insertSTM = "";
	for(unsigned i=0;i<=NOFACT;i++){
		AtomHistoryVector* atoms=predicateExtension->getTable(i);
		for(auto atom: *atoms)
		{
			if(!atom->isFact()) continue;  // TODO check
//			stm = "INSERT INTO " + string(table_name) + " VALUES( ";
			stm = " ( ";

			unsigned t = 0;
			char **pa = new char* [parity];
			for (unsigned j=0;j<atom->getTermsSize();j++)
			{
				Term* k=atom->getTerm(j);
				switch( k->getType() )
				{
					case TermType::NUMERIC_CONSTANT && predicateExtension->getPredicateInformation()->isNumeric(j):
					{
						int *p = new int(k->getConstantValue());
						pa[t] = new char[50];
						sprintf(pa[t],"%d",*p);
						t++;
						delete p;
						break;
					}
					case TermType::VARIABLE:
					case TermType::ANONYMOUS:
					case TermType::ARITH:
						break;
					default:
					{
						string tmp = (*k).toString();
						// Replace all occurrences of single quotes ('\'') by
						// two occurrences of this character.
						string::size_type index = tmp.find("'",0);
						while ( index != string::npos )
						{
							tmp.replace(index,1,"''");
							index = tmp.find("'",index+2);
						}
						// If the string starts with a double quote ('\"'),
						// i.e. it is a double quoted string, strip the first
						// and last characters from the string.
						if ( tmp.find("\"",0) != string::npos )
						{
							tmp.erase(0,1);
							tmp.erase(tmp.size()-1,1);
						}
						pa[t] = new char[tmp.size()+1];
						strcpy(pa[t],tmp.c_str());
						t++;
						break;
					}
				} //  switch
			} // FOR
			string s = insert_tuple(stm, pa, parity, vectype);

			if(insertSTM=="")
				insertSTM = "INSERT INTO " + string(table_name) +" VALUES "+ s;
			else
				insertSTM +=","+s;

			for ( unsigned i = 0; i < parity; i++ )
				delete[] pa[i];
			delete[] pa;
		}
	}
	insertSTM+=";";
	if ( !insertString(insertSTM.c_str(), &V_OD_hstmt)){
		return false;
	}
	// to free a handle for a SQL-statement
	SQLFreeHandle (SQL_HANDLE_STMT,V_OD_hstmt);

    disconnect();
    delete[] vectype;
    return true;
}

unsigned  DBConnection::determineNumberOfColumns(const string& queryStr, const string& pName){
	SQLHSTMT V_OD_hstmt;
	SQLSMALLINT numcol;

	char *query = new char[queryStr.length() + 1];
	strcpy(query, queryStr.c_str());

	if( strncasecmp(ltrim(query),"SELECT",6) != 0 )
	{
		ErrorMessage::errorDBConnection( "ODBC Syntax Error in the writing of SELECT");
		disconnect();
		delete [] query;
		return 0;
	}

	if ( !insertString(query,&V_OD_hstmt) || ! getNumberOfColumns(&V_OD_hstmt,numcol) ){
		delete [] query;
		return 0;
	}

	delete [] query;

	return numcol;
}

Predicate* DBConnection::determinePredicate(const string& query, const string& pName){
	unsigned numcol=determineNumberOfColumns(query,pName);
	string pN=pName;
	Predicate* p=new Predicate(pN,numcol);
	PredicateTable::getInstance()->insertPredicate(p);
	PredicateExtTable::getInstance()->addPredicateExt(p);
	return p;
}



bool DBConnection::importFromDB(const string& queryStr, const string& pName, const string& type)
{
    int NCol_type=0;
    SQLHSTMT V_OD_hstmt;
    SQLSMALLINT numcol;

    char *query = new char[queryStr.length() + 1];
    strcpy(query, queryStr.c_str());

    if( strncasecmp(ltrim(query),"SELECT",6) != 0 )
	{
		ErrorMessage::errorDBConnection( "ODBC Syntax Error in the writing of SELECT");
        disconnect();
        delete [] query;
        return false;
	}

    if ( !insertString(query,&V_OD_hstmt) || ! getNumberOfColumns(&V_OD_hstmt,numcol) ){
    	delete [] query;
        return false;
    }

    delete [] query;
    ////////////////////////////////////////

    ValueConv *typeConv = new ValueConv[numcol];
    if ( strcmp(type.c_str(), "") )
	{
        if( ! prepareConversion(type.c_str(),typeConv,NCol_type) )
		{
            disconnect();
        	delete [] typeConv;
            return false;
		}

        if (NCol_type != numcol)
		{
            disconnect();
            delete [] typeConv;
            return false;
		}
	}
    else
        for (int c=0; c < numcol; c++)
            typeConv[c] = DEFAULT_;

    ////////////////////////////////////////

    SQLSMALLINT *vectype = new SQLSMALLINT[numcol];
    describeColumn(vectype,numcol,&V_OD_hstmt);

    void  **values = new void *[numcol];
    SQLLEN *V_OD_err = new SQLLEN[numcol];
    int V_OD_erg = 0;
    for (SQLUSMALLINT k = 0; k < numcol; k++)
	{
        switch (vectype[k])
		{
            case SQL_C_TYPE_DATE:
            case SQL_LONGVARCHAR:
            case SQL_VARCHAR:
            case SQL_C_CHAR:
            case SQL_C_TYPE_TIMESTAMP:
            case SQL_C_BINARY:
            {
                values[k] = new char [ODBC_MAX_STRING_LENGTH+1];
                V_OD_erg = SQLBindCol( V_OD_hstmt,k+1,
                                       SQL_C_CHAR,
                                       values[k],
                                       ODBC_MAX_STRING_LENGTH,
                                       &V_OD_err[k] );
                if( typeConv[k] == DEFAULT_ )
                    typeConv[k]=Q_CONST;
                break;
            }
            case SQL_C_ULONG:
            case SQL_C_SHORT:
            case SQL_BIGINT:
            case SQL_C_LONG:
            case SQL_C_SLONG:
            case SQL_TINYINT:
            case SQL_C_SSHORT:
            case SQL_C_USHORT:
                switch (typeConv[k] )
				{
                    case UR_INT:
                    case UT_INT:
                    case U_INT:
                    case R_INT:
                    case T_INT:
                    case DEFAULT_:
                        values[k] = new (unsigned long)();
                        V_OD_erg = SQLBindCol( V_OD_hstmt,
                                               k+1,
                                               vectype[k],
                                               values[k],
                                               sizeof(vectype[k]),
                                               &V_OD_err[k] );
                        if( typeConv[k] == DEFAULT_ )
                            typeConv[k]=U_INT;
                        break;
                    case CONST_:
                    case Q_CONST:
                        values[k] = new char[ODBC_MAX_STRING_LENGTH+1];
                        V_OD_erg = SQLBindCol( V_OD_hstmt,
                                               k+1,
                                               SQL_C_CHAR,
                                               values[k],
                                               ODBC_MAX_STRING_LENGTH,
                                               &V_OD_err[k] );
                        break;
                    default:
                        assert(0);
				}
                break;
            case SQL_DECIMAL:
            case SQL_C_NUMERIC:
            case SQL_C_DOUBLE:
            case SQL_FLOAT:
            case SQL_C_FLOAT:
                switch (typeConv[k] )
				{
                    case UR_INT:
                    case UT_INT:
                    case U_INT:
                    case R_INT:
                    case T_INT:
                    case DEFAULT_:
                        values[k] = new (double) ();
                        V_OD_erg = SQLBindCol( V_OD_hstmt,
                                               k+1,
                                               SQL_C_DOUBLE,
                                               values[k],
                                               sizeof(SQL_C_DOUBLE),
                                               &V_OD_err[k] );
                        if( typeConv[k] == DEFAULT )
                            typeConv[k]=UR_INT;
                        break;
                    case CONST_:
                    case Q_CONST:
                        values[k] = new char[ODBC_MAX_STRING_LENGTH+1];
                        V_OD_erg = SQLBindCol( V_OD_hstmt,
                                               k+1,
                                               SQL_C_CHAR,
                                               values[k],
                                               ODBC_MAX_STRING_LENGTH,
                                               &V_OD_err[k] );
                        break;
                    default:
                        assert(0);
				}
                break;
            default:
            	ErrorMessage::errorDBConnection("ODBC Error Type not supported: "+to_string(vectype[k]));
                SQLFreeHandle(SQL_HANDLE_STMT,V_OD_hstmt);
                disconnect();
                for (SQLUSMALLINT y = 0; y < numcol; y++)
            	{
                    if ( V_OD_err[y] == SQL_NULL_DATA
                         || typeConv[y] == CONST_
                         || typeConv[y] == Q_CONST )
                        delete[] (char*)values[y];
                    else
                        delete (unsigned long*) values[y];
            	}
                delete [] V_OD_err;
                delete [] values;
                delete [] vectype;
                delete [] typeConv;
                return false;
		}
        if ( V_OD_erg != SQL_SUCCESS  &&  V_OD_erg != SQL_SUCCESS_WITH_INFO )
		{
        	ErrorMessage::errorDBConnection("ODBC Error during import: "+to_string(V_OD_erg));
            SQLFreeHandle(SQL_HANDLE_STMT,V_OD_hstmt);
            disconnect();
            for (SQLUSMALLINT y = 0; y < numcol; y++)
        	{
                if ( V_OD_err[y] == SQL_NULL_DATA
                     || typeConv[y] == CONST_
                     || typeConv[y] == Q_CONST )
                    delete[] (char*)values[y];
                else
                    delete (unsigned long*) values[y];
        	}
            delete [] V_OD_err;
            delete [] values;
            delete [] vectype;
            delete [] typeConv;
            return false;
		}
	} // for
    while( SQLFetch(V_OD_hstmt) != SQL_NO_DATA)
	{
        for (SQLUSMALLINT i = 0; i < numcol; i++)
		{
            if (V_OD_err[i] == SQL_NULL_DATA)
			{
                char *st = new char[10];
                strcpy(st, "null");
                rdbms_sendString (st, strlen(st),pName);
                delete[] st;
			}
            else
			{
                unsigned long val=0;
                switch ( typeConv[i] )
				{
                    case UR_INT:
					{
                        if( vectype[i] == SQL_C_SHORT
                            || vectype[i] == SQL_C_SSHORT
                            || vectype[i] == SQL_C_USHORT
                            || vectype[i] == SQL_C_LONG
                            || vectype[i] == SQL_C_SLONG
                            || vectype[i] == SQL_C_ULONG
                            || vectype[i] == SQL_BIGINT) {
                            val = (unsigned long)labs(*(long*)values[i]);
						}
                        else
                            if( vectype[i] == SQL_C_TYPE_TIMESTAMP
                                || vectype[i] == SQL_C_BINARY
                                || vectype[i] == SQL_C_TYPE_DATE ) {
                            	ErrorMessage::errorDBConnection("ODBC Error during import: Error in conversion type.");
                                disconnect();
                                for (SQLUSMALLINT y = 0; y < numcol; y++)
                            	{
                                    if ( V_OD_err[y] == SQL_NULL_DATA
                                         || typeConv[y] == CONST_
                                         || typeConv[y] == Q_CONST )
                                        delete[] (char*)values[y];
                                    else
                                        delete (unsigned long*) values[y];
                            	}
                                delete [] V_OD_err;
                                delete [] values;
                                delete [] vectype;
                                delete [] typeConv;
                                return false;
							}
                        else
                            if ( vectype[i] == SQL_C_CHAR
                                 || vectype[i] == SQL_VARCHAR
                                 || vectype[i] == SQL_LONGVARCHAR)
                                val = labs(atoi((char*)values[i]));
                        else
                            val = (unsigned long)(fabs(*(double*)values[i])+0.5);
                        rdbms_sendInt(val, pName);
                        break;
					}
                    case UT_INT:
                        if( vectype[i] == SQL_C_SHORT
                            || vectype[i] == SQL_C_SSHORT
                            || vectype[i] == SQL_C_USHORT
                            || vectype[i] == SQL_C_LONG
                            || vectype[i] == SQL_C_SLONG
                            || vectype[i] == SQL_C_ULONG
                            || vectype[i] == SQL_BIGINT) {
                            val = (unsigned long)labs(*(long*)values[i]);
						}
                        else
                            if( vectype[i] == SQL_C_TYPE_TIMESTAMP
                                || vectype[i] == SQL_C_BINARY
                                || vectype[i] == SQL_C_TYPE_DATE  ) {
                            	ErrorMessage::errorDBConnection("ODBC Error during import: Error in conversion type.");
                                disconnect();
                                for (SQLUSMALLINT y = 0; y < numcol; y++)
                            	{
                                    if ( V_OD_err[y] == SQL_NULL_DATA
                                         || typeConv[y] == CONST_
                                         || typeConv[y] == Q_CONST )
                                        delete[] (char*)values[y];
                                    else
                                        delete (unsigned long*) values[y];
                            	}
                                delete [] V_OD_err;
                                delete [] values;
                                delete [] vectype;
                                delete [] typeConv;
                                return false;
							}
                        else
                            if ( vectype[i] == SQL_C_CHAR
                                 || vectype[i] == SQL_VARCHAR
                                 || vectype[i] == SQL_LONGVARCHAR)
                                val = labs(atoi((char*)values[i]));
                        else
                            val = (unsigned long)fabs(floor(*(double*)values[i]));
                        rdbms_sendInt(val, pName);
                        break;
                    case R_INT:
					{
                        if( vectype[i] == SQL_C_SHORT
                            || vectype[i] == SQL_C_SSHORT
                            || vectype[i] == SQL_C_USHORT
                            || vectype[i] == SQL_C_LONG
                            || vectype[i] == SQL_C_SLONG
                            || vectype[i] == SQL_C_ULONG
                            || vectype[i] == SQL_BIGINT) {
                            val = (long)(*(long*)values[i]);
						}
                        else
                            if( vectype[i] == SQL_C_TYPE_TIMESTAMP
                                || vectype[i] == SQL_C_BINARY
                                || vectype[i] == SQL_C_TYPE_DATE ) {
                            	ErrorMessage::errorDBConnection("ODBC Error during import: Error in conversion type.");
                                disconnect();
                                for (SQLUSMALLINT y = 0; y < numcol; y++)
                            	{
                                    if ( V_OD_err[y] == SQL_NULL_DATA
                                         || typeConv[y] == CONST_
                                         || typeConv[y] == Q_CONST )
                                        delete[] (char*)values[y];
                                    else
                                        delete (long*) values[y];
                            	}
                                delete [] V_OD_err;
                                delete [] values;
                                delete [] vectype;
                                delete [] typeConv;
                                return false;
							}
                        else
                            if ( vectype[i] == SQL_C_CHAR
                                 || vectype[i] == SQL_VARCHAR
                                 || vectype[i] == SQL_LONGVARCHAR)
                                val = (atoi((char*)values[i]));
                        else
                            val = (long)(fabs(*(double*)values[i])+0.5);
                        rdbms_sendInt(val, pName);
                        break;
					}
                    case T_INT:
                        if( vectype[i] == SQL_C_SHORT
                            || vectype[i] == SQL_C_SSHORT
                            || vectype[i] == SQL_C_USHORT
                            || vectype[i] == SQL_C_LONG
                            || vectype[i] == SQL_C_SLONG
                            || vectype[i] == SQL_C_ULONG
                            || vectype[i] == SQL_BIGINT) {
                            val = (long)(*(long*)values[i]);
						}
                        else
                            if( vectype[i] == SQL_C_TYPE_TIMESTAMP
                                || vectype[i] == SQL_C_BINARY
                                || vectype[i] == SQL_C_TYPE_DATE  ) {
                            	ErrorMessage::errorDBConnection("ODBC Error during import: Error in conversion type.");
                                disconnect();
                                for (SQLUSMALLINT y = 0; y < numcol; y++)
                            	{
                                    if ( V_OD_err[y] == SQL_NULL_DATA
                                         || typeConv[y] == CONST_
                                         || typeConv[y] == Q_CONST )
                                        delete[] (char*)values[y];
                                    else
                                        delete (long*) values[y];
                            	}
                                delete [] V_OD_err;
                                delete [] values;
                                delete [] vectype;
                                delete [] typeConv;
                                return false;
							}
                        else
                            if ( vectype[i] == SQL_C_CHAR
                                 || vectype[i] == SQL_VARCHAR
                                 || vectype[i] == SQL_LONGVARCHAR)
                                val = (atoi((char*)values[i]));
                        else
                            val = (long)(floor(*(double*)values[i]));
                        rdbms_sendInt(val, pName);
                        break;
                    case U_INT:
                        if( vectype[i] == SQL_C_TYPE_TIMESTAMP
                            || vectype[i] == SQL_C_BINARY
                            || vectype[i] == SQL_C_TYPE_DATE ) {
                        	ErrorMessage::errorDBConnection("ODBC Error during import: Error in conversion type.");
                            disconnect();
                            for (SQLUSMALLINT y = 0; y < numcol; y++)
                        	{
                                if ( V_OD_err[y] == SQL_NULL_DATA
                                     || typeConv[y] == CONST_
                                     || typeConv[y] == Q_CONST )
                                    delete[] (char*)values[y];
                                else
                                    delete (unsigned long*) values[y];
                        	}
                            delete [] V_OD_err;
                            delete [] values;
                            delete [] vectype;
                            delete [] typeConv;
                            return false;
						}
                        else
                            if ( vectype[i] == SQL_C_CHAR
                                 || vectype[i] == SQL_VARCHAR
                                 || vectype[i] == SQL_LONGVARCHAR)
                                val = labs(atoi((char*)values[i]));
                        else
                            if ( vectype[i] == SQL_C_FLOAT
                                 || vectype[i] == SQL_FLOAT)
                                val = (unsigned long)fabs(floor(*(double*)values[i]));
                        else
                            val = labs(*(unsigned long*)values[i]);
                        rdbms_sendInt(val, pName);
                        break;
                    case CONST_:
                        rdbms_sendConst( (char*) values[i], pName);
                        break;
                    case Q_CONST:
                        rdbms_sendString( (char*) values[i], strlen((char*) values[i]), pName);
                        break;
                    default:
                        assert(0);
                    }
                }
            }
        rdbms_sendRowEnd(pName,numcol);
	}

    for (SQLUSMALLINT y = 0; y < numcol; y++)
	{
        if ( V_OD_err[y] == SQL_NULL_DATA
             || typeConv[y] == CONST_
             || typeConv[y] == Q_CONST )
            delete[] (char*)values[y];
        else
            delete (unsigned long*) values[y];
	}

    delete[] V_OD_err;
    delete[] values;
    delete [] vectype;
    delete [] typeConv;
    return true;

}

void DBConnection::rdbms_rowcheck(const string& pName){
	if (!row_open)  // new atom starts
    {
        row_open = true;
		terms.clear();
	}
}

void DBConnection::rdbms_sendInt(unsigned long i, const string& pName){
    rdbms_rowcheck(pName);

    Term* t=new NumericConstantTerm(false,i);
    TermTable::getInstance()->addTerm(t);
    terms.push_back(t);
}

void DBConnection::rdbms_sendConst(char *str, const string& pName){
    rdbms_rowcheck(pName);
    string s = rtrim(str);
//    if(!(str[0] >= 'a' && str[0] <='z')){
//    	unsigned len=strlen(str);
//        str[len] = '\0';
//        rtrim(str);
//
//        s = '\"' + string(str)  + '\"';
//
//    }


    Term* t=new StringConstantTerm(false,s);
    TermTable::getInstance()->addTerm(t);
    terms.push_back(t);
}

void DBConnection::rdbms_sendString(char *str, unsigned len, const string& pName){
    rdbms_rowcheck(pName);
    str[len] = '\0';
    rtrim(str);

    string s;
    s = '\"' + string(str)  + '\"';

    char *cstr = new char[s.length() + 1];
    strcpy(cstr, s.c_str());

    Term* t=new StringConstantTerm(false,rtrim(cstr));
    TermTable::getInstance()->addTerm(t);
    terms.push_back(t);
    delete [] cstr;
}

void DBConnection::rdbms_sendRowEnd(const string& pName, unsigned arity)
{
    if(row_open){
		row_open = false;
		string n=pName;
		Predicate* p=new Predicate(n,arity);
		PredicateTable::getInstance()->insertPredicate(p);
		PredicateExtTable::getInstance()->addPredicateExt(p);
		Atom* atom=new ClassicalLiteral(p,false,false,true);
		atom->setTerms(terms);
		terms.clear();

		IndexingStructure* atomSearcher=nullptr;
		PredicateExtTable::getInstance()->getPredicateExt(p)->getAtomSearcher(FACT);
		if(Options::globalOptions()->getCheckFactDuplicate())
			atomSearcher=PredicateExtTable::getInstance()->getPredicateExt(p)->addAtomSearcher(FACT,HASHSET,nullptr);

		if(atomSearcher==nullptr || atomSearcher->find(atom)==nullptr){
			PredicateExtTable::getInstance()->getPredicateExt(p)->addAtom(atom,FACT);
			if (!Options::globalOptions()->isNofacts()) {
				OutputBuilder::getInstance()->onFact(atom);
			}
		} else
			delete atom;
	}
}
#endif

} /* namespace grounder */
} /* namespace DLV2 */
