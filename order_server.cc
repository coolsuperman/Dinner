#include"httplib.h"
#include"db.hpp"
#include<jsoncpp/json/json.h>
#include<signal.h>

const char* CONTENT_TYPE = "application/json";
MYSQL* mysql = NULL;


int main(){
  using namespace httplib;
  Server svr;
  mysql = order_system::MySQLInit();
  order_system::OrderTable order_table(mysql);
  order_system::DishTable  dish_table(mysql);
  signal(SIGINT,[](int){order_system::MYSQLRelease(mysql);exit(0);});//ctrl c 释放资源；
  svr.Post("/dish",[&dish_table](const Request& req,Response& resp){//按照引用捕捉dish_table;
      Json::Value req_json,res_json;
      Json::Reader reader;
      Json::FastWriter writer;
      printf("新增菜品\n");
      //获取数据解析Json；
      bool ret=reader.parse(req.body,req_json);
      if(!ret){
        printf("parse body failed!\n");
        res_json["ok"] = false;
        res_json["reason"]="pare body failed!";
        resp.status = 400;
        resp.set_content(writer.write(res_json),CONTENT_TYPE);
        return;
      }
      //校验Json信息；
      if(req_json["name"].empty()||req_json["price"].empty()){
        printf("Request Json error format\n");
        res_json["ok"] = false;
        res_json["reason"]="Request Json error format!";
        resp.status = 400;
        resp.set_content(writer.write(res_json),CONTENT_TYPE);
        return;
      }
      //调用数据库插入数据;
      ret = dish_table.Insert(req_json);
      if(!ret){
        printf("dish_table insert failed!\n");
        res_json["ok"] = false;
        res_json["reason"]="dish_table insert failed!";
        resp.status = 500;
        resp.set_content(writer.write(res_json),CONTENT_TYPE);
        return;
      }
      //构造正确响应
      res_json["ok"] = true;
      resp.status = 200;
      resp.set_content(writer.write(res_json),CONTENT_TYPE);
      });//新增一道菜

  svr.Get("/dish",[&dish_table](const Request& req,Response& resp){
      (void)req;
      Json::Value res_json;
      Json::FastWriter writer;
      printf("查看所有菜品\n"); 
      bool ret = dish_table.SelectAll(&res_json);
      if(!ret){
        printf("select all dish failed!\n");
        res_json["ok"] = false;
        res_json["reason"]="select all dish failed!";
        resp.status = 500;
        resp.set_content(writer.write(res_json),CONTENT_TYPE);
        return;
      }
      resp.status = 200;
      //res_json["ok"] = true;//这里相应的json格式是[{},{},{}],不可以在[ok=true,{},{},{}],会发生逻辑错误；
      resp.set_content(writer.write(res_json),CONTENT_TYPE);
      });//查看所有菜；
  svr.Get(R"(/dish/(\d+))",[&dish_table](const Request& req,Response& resp){
      //std::to_string
      int32_t dish_id = std::stoi(req.matches[1]);
      printf("获取到编号为%d的菜品\n",dish_id);
      Json::Value res_json;
      Json::FastWriter writer;
      bool ret = dish_table.SelectOne(dish_id,&res_json);
      if(!ret){
        printf("selectone  dish failed!\n");
        res_json["ok"] = false;
        res_json["reason"]="selectOne dish failed!";
        resp.status = 500;
        resp.set_content(writer.write(res_json),CONTENT_TYPE);
        return;
      }
      res_json["ok"] = true;
      resp.status = 200;
      resp.set_content(writer.write(res_json),CONTENT_TYPE);
      });//查看一道菜；

  svr.Put(R"(/dish/(\d+))",[&dish_table](const Request& req,Response& resp){
      Json::Value req_json,res_json;
      Json::Reader reader;
      Json::FastWriter writer;
      int32_t dish_id = std::stoi(req.matches[1]);
      printf("修改编号为%d菜品\n",dish_id);
      //获取数据解析Json；
      bool ret=reader.parse(req.body,req_json);
      if(!ret){
        printf("parse body failed!\n");
        res_json["ok"] = false;
        res_json["reason"]="pare body failed!";
        resp.status = 400;
        resp.set_content(writer.write(res_json),CONTENT_TYPE);
        return;
      }
      //校验Json信息；
      if(req_json["name"].empty()||req_json["price"].empty()){
        printf("Request Json error format\n");
        res_json["ok"] = false;
        res_json["reason"]="Request Json error format!";
        resp.status = 400;
        resp.set_content(writer.write(res_json),CONTENT_TYPE);
        return;
      }
      //调用数据库插入数据;
      req_json["dish_id"]=dish_id;
      ret = dish_table.Update(req_json);
      if(!ret){
        printf("dish_table Update failed!\n");
        res_json["ok"] = false;
        res_json["reason"]="dish_table Updtae failed!";
        resp.status = 500;
        resp.set_content(writer.write(res_json),CONTENT_TYPE);
        return;
      }
      //构造正确响应
      res_json["ok"] = true;
      resp.status = 200;
      resp.set_content(writer.write(res_json),CONTENT_TYPE);
      });//修改一道菜
  svr.Delete(R"(/dish/(\d+))",[&dish_table](const Request& req,Response& resp){
      int32_t dish_id = std::stoi(req.matches[1]);
      printf("获取到编号为%d的菜品\n",dish_id);
      Json::Value res_json;
      Json::FastWriter writer;
      bool ret = dish_table.Delete(dish_id);
      if(!ret){
        printf("Delete  dish failed!\n");
        res_json["ok"] = false;
        res_json["reason"]="Delete dish failed!";
        resp.status = 500;
        resp.set_content(writer.write(res_json),CONTENT_TYPE);
        return;
      }
      res_json["ok"] = true;
      resp.status = 200;
      resp.set_content(writer.write(res_json),CONTENT_TYPE);
      });//删除一道菜;

  svr.Post("/order",[&order_table](const Request& req,Response& resp){
      Json::Value req_json,resp_json;
      Json::Reader reader;
      Json::FastWriter writer;
      printf("新增订单！\n");
      int ret = reader.parse(req.body,req_json);
      if(!ret){
        printf("Parse Json failed!\n");
        resp_json["ok"] = false;
        resp_json["reason"] = "Parse json failed!";
        resp.status = 400;
        resp.set_content(writer.write(resp_json).c_str(),CONTENT_TYPE);
        return ;
      }
      if(req_json["table_id"].empty()||req_json["time"].empty()||req_json["dishes"].empty()){
        printf("Require Json error format!\n");
        resp_json["ok"] = false;
        resp_json["reason"] = "Require Json error format!";
        resp.status = 400;
        resp.set_content(writer.write(resp_json).c_str(),CONTENT_TYPE);
        return ;
      }
      req_json["status"] = 1;
      ret = order_table.Insert(req_json);
      if(!ret){
        printf("Insert Order failed!\n");
        resp_json["ok"] = false;
        resp_json["reason"]="Insert Order failed!";
        resp.status = 500;
        resp.set_content(writer.write(resp_json),CONTENT_TYPE);
        return;
      }
      resp_json["ok"] = true;
      resp.status = 200;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      });//新增订单；
  svr.Get("/order",[&order_table](const Request& req,Response& resp){
      (void)req;
      Json::Value res_json;
      Json::FastWriter writer;
      printf("查看所有订单\n"); 
      bool ret = order_table.SelectAll(&res_json);
      if(!ret){
        printf("select all order failed!\n");
        res_json["ok"] = false;
        res_json["reason"]="select all order failed!";
        resp.status = 500;
        resp.set_content(writer.write(res_json),CONTENT_TYPE);
        return;
      }
      resp.status = 200;
      //res_json["ok"] = true;//这里相应的json格式是[{},{},{}],不可以在[ok=true,{},{},{}],会发生逻辑错误；
      resp.set_content(writer.write(res_json),CONTENT_TYPE);
      });//查看订单；
  svr.Put(R"(/order/(\d+))",[&order_table](const Request& req,Response& resp){ 
      Json::Value req_json,res_json;
      Json::Reader reader;
      Json::FastWriter writer;
      int32_t order_id = std::stoi(req.matches[1]);
      printf("修改编号为%d订单\n",order_id);
      //获取数据解析Json；
      bool ret=reader.parse(req.body,req_json);
      if(!ret){
        printf("parse body failed!\n");
        res_json["ok"] = false;
        res_json["reason"]="pare body failed!";
        resp.status = 400;
        resp.set_content(writer.write(res_json),CONTENT_TYPE);
        return;
      }
      //校验Json信息；
      if(req_json["status"].empty()){
        printf("Request Json error format\n");
        res_json["ok"] = false;
        res_json["reason"]="Request Json error format!";
        resp.status = 400;
        resp.set_content(writer.write(res_json),CONTENT_TYPE);
        return;
      }
      //调用数据库插入数据;
      req_json["order_id"] = order_id;
      ret = order_table.ChangeState(req_json);
      if(!ret){
        printf("order_table ChangeState failed!\n");
        res_json["ok"] = false;
        res_json["reason"]="order_table ChangeState failed!";
        resp.status = 500;
        resp.set_content(writer.write(res_json),CONTENT_TYPE);
        return;
      }
      //构造正确响应
      res_json["ok"] = true;
      resp.status = 200;
      resp.set_content(writer.write(res_json),CONTENT_TYPE);
      });//修改订单状态
  svr.set_base_dir("./wwwroot");
  svr.listen("0.0.0.0",1998);
  return 0;
}
