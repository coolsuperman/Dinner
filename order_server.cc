#include"httplib.h"
#include"db.hpp"


int main(){
  using namespace httplib;
  Server svr;
  svr.Get("/",[](const Request& req,Response& resp){
      (void) req;
      resp.set_content("hello world","text/html");
      });//λ表达式；
  return 0;
}
