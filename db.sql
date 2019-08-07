create database if not exists dish_system charset=utf8;

use dish_system;

drop table if exists dish_system,dish_table;

create table if not exists dish_table(
  dish_id int not null primary key auto_increment,
  name varchar(50),
  price int
);

insert into dish_table values(null,'红烧肉',2200);
insert into dish_table values(null,'狗头',2600);
insert into dish_table values(null,'芋头',2300);
insert into dish_table values(null,'猪头',2400);
insert into dish_table values(null,'虾头',2500);

create table if not exists order_table(
  order_id int not null primary key auto_increment,
  table_id varchar(50),
  time varchar(50),
  dishes varchar(1024),
  status int 
)

