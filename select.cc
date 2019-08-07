#include<cstdio>
#include<cstdlib>
#include<mysql/mysql.h>

int main(){
  //创建一个数据库的操作句柄
  MYSQL* mysql = mysql_init(NULL);
  //2.建立句柄和数据库服务器之间的联系(连接服务器)
  if(mysql_real_connect(mysql,"127.0.0.1","root","199805","dish_system",3306,NULL,0)==NULL){
    printf("Connect failed! %s\n",mysql_error(mysql));
    return 1; 
  }
  //3.设置编码方式
  mysql_set_character_set(mysql,"utf8");
  //4.拼装SQL语句
  char sql[1024] = {0};
  sprintf(sql,"select * from dish_table");
  //5.执行 SQL 
  int ret = mysql_query(mysql,sql);
  if(ret!=0){
   printf("mysql_query 执行失败 %s\n",mysql_error(mysql));
   mysql_close(mysql);
     return 1;
  }
  //6.获取并遍历结果集合
  //获取结果集合
  MYSQL_RES* result = mysql_store_result(mysql);
  //获取结果中有几行
  int rows = mysql_num_rows(result);
  int cols = mysql_num_fields(result);
  //根据循环遍历结果
  for(int row = 0;row<rows;++row){
    //得到一行数据;
    MYSQL_ROW mysql_row = mysql_fetch_row(result);
    //使用[]取下标操作获取每一列
    for(int col =0;col <cols;++col)
      printf("%s\t",mysql_row[col]);
    printf("\n");
  }
  //7.结果集合手动释放
  mysql_free_result(result);
  //8.断开连接
  mysql_close(mysql);
  return 0;
}
