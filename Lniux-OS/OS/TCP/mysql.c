#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mysql/mysql.h>

MYSQL *con;			//mysql连接
MYSQL_RES *res = NULL;	//mysql记录集
MYSQL_ROW row;		//字符串数组，mysql记录行
char *user = "debian-sys-maint";
char *passwd = "NHqLsqHfc2mBjaKt";
char *host = "localhost";
char *db = "student_system";

int main()
{
	con = mysql_init(con);
	con = mysql_real_connect(con, host, user, passwd, db, 0, NULL, 0);
	char *sq = "INSERT INTO student(id, username) values('5', 'puuanu')";
	int len = strlen(sq);
	if(mysql_real_query(con, sq, len))
	{
		perror("mysql_real_query");
		exit(0);
	}
//	res = mysql_store_result(con);
//	row = mysql_fetch_row(res);      //
	char *sql = "SELECT id,username from student where id=5";
	if(mysql_real_query(con, sql, strlen(sql)))
	{
		perror("query error");
		exit(0);
	}
	res = mysql_store_result(con);
	while((row = mysql_fetch_row(res)))
	{
		printf("%s %s\n", row[0], row[1]);
	}
	mysql_free_result(res);                //
	mysql_close(con);
}
