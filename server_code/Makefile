FLAG=-L/usr/lib64/mysql -lmysqlclient -ljsoncpp -lpthread -std=c++11

.PHONY:all 
all:insert select test server

server:order_server.cc
	g++ $^ -o $@ $(FLAG)
insert:insert.cc
	g++ $^ -o $@ $(FLAG)
select:select.cc
	g++ $^ -o $@ $(FLAG)
test:db_test.cc
	g++ $^ -o $@ $(FLAG)

.PHONY:clean
clean:
	rm insert select test server
