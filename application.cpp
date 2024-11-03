#include <iostream>
#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>

int main(){
    MYSQL conn;
    int res;
    mysql_init(&conn);
    //连接、ip、用户、密码、数据库、端口、socket指针、标记
    if(mysql_real_connect(&conn,"localhost","root","201214","user",3306,NULL,CLIENT_FOUND_ROWS)){
        std::cout << "connect success" << std::endl;
        res = mysql_query(&conn,"insert into user values('ShaoCHi','Aa123456789')");
        if(res){
            std::cout << "error" << std::endl;
        }
        else{
            std::cout << "OK" << std::endl;
        }
    }else{
        std::cout << "connect failed" << std::endl;
    }
    //提取数据
    MYSQL_RES* result = nullptr;
    MYSQL_ROW row;
    char sql[1024]{0};
    sprintf(sql,"select* from user");
    if(mysql_real_query(&conn,sql,(unsigned int)strlen(sql))){
        std::cout << "Query Failed!" << std::endl;
    }else{
        std::cout << "Query Success!" << std::endl;

        //load result
        result = mysql_store_result(&conn);
        if(result == nullptr){
            std::cout << "Load Data Failed: " << mysql_errno(&conn) << std::endl;
        }else{
            while(row=mysql_fetch_row(result)){
                std::cout << row[0] << "  " << row[1] << std::endl;
            }
        }
    }
    //释放结果集
    mysql_free_result(result);
    //关闭连接
    mysql_close(&conn);

    std::cout << "Exit\n";
    return 0;
}
