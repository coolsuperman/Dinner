#include"db.hpp"
//单元测试，一个函数一个函数去测试；
//google test,boost::unittest,..
void TestDishTable(){
  MYSQL* mysql = order_system::MySQLInit();
  order_system::DishTable dishTable(mysql);
  Json::Value dish;
  //1.插入数据
  // dish["name"] = "宫保鸡丁";
  // dish["price"] = 1000;
  // bool ret=dishTable.Insert(dish);
  // printf("ret = %d\n",ret);
  //2.查找所有数据
  int ret = dishTable.SelectAll(&dish);
  printf("rer= %d\n",ret);
  Json::StyledWriter w;
  printf("%s/n",w.write(dish).c_str());
  //3.查找指定数据
  //int ret = dishTable.SelectOne(2,&dish);
  //printf("ret = %d\n",ret);
  //printf("%s\n",w.write(dish).c_str());
  //4.修改指定数据
  //dish["name"] = "泡菜肉丝";
  //dish["price"] = 1000;
  //dish["dish_id"] = 1;
  //int ret = dishTable.Update(dish);
  //printf("ret=%d\n",ret);
  //printf("%s\n",w.write(dish).c_str());
  //5.删除指定数据
  //int ret = dishTable.Delete(2);
  //printf("ret = %d\n",ret);
  order_system::MYSQLRelease(mysql);
}

void TestOrderTable(){
  MYSQL* mysql = order_system::MySQLInit();
  order_system::OrderTable orderTable(mysql);
  Json::Value order;
  //6.插入一个订单；
  //order["table_id"] = "少林派";
  //order["time"] = "2019/08/07 15:15";
  //order["dishes"] = "[1,3,5]";
  //order["status"] = 1;
  //int ret = orderTable.Insert(order);
  //printf("ret = %d\n",ret);
  //7.查看所有订单
  //int ret = orderTable.SelectAll(&order);
  Json::StyledWriter w;
  //printf("ret = %d\n",ret);
  //printf("%s",w.write(order).c_str());
  //8.修改订单状态
  order["order_id"] = 1;
  order["status"] = 0;
  int ret = orderTable.ChangeState(order);
  printf("ret = %d\n",ret);
  printf("%s",w.write(order).c_str());
  order_system::MYSQLRelease(mysql);
}

int main(){
  TestDishTable();
 //TestOrderTable();
  return 0;
}
