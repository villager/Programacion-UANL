create database users;

create table users(id SERIAL,
 email text,
 name varchar(100),
 lastname varchar(100),
 password varchar(250),
 rank varchar(10),
 date varchar(100),
 courses int[]);

create table items (id SERIAL,
name varchar(250),
price int,
owner int,
des text);

insert into users values(0,'aldair.beltran@outlook.com','Aldair', 'Beltran', '$2b$10$ag2vNaFFAXqPpHrkoCPuiu2fdSDNiYuySeKJUUALIl76iEo5xDe/u', 'admin', 'NA', '[0]');


insert into items values(0, 'Curso de prueba', 0, 0,'Este es un curso de prueba');