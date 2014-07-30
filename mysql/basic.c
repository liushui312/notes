
sudo apt-get install mysql-server

mysql -u root
-> create database bank;
-> GRANT ALL PRIVILEGES ON bank.* TO liushui@localhost IDENTIFIED BY "xyz";
-> quit;
mysql -u liushui -p bank
-> select now();
-> show charset;
-> create table person (person_id SMALLINT UNSIGNED, fname VARCHAR(20), iname VARCHAR(20), gender ENUM('M','F'), birth_date DATE, street VARCHAR(30), city VARCHAR(20), state VARCHAR(20), country VARCHAR(20), postal_code VARCHAR(20), CONSTRAINT pk_person PRIMARY KEY (person_id) );
-> desc person;

-> create table favorite_food  (person_id smallint unsigned, food varchar(20), constraint pk_favorite_food primary key (person_id, food), constraint fk_fav_food_person_id foreign key (person_id) references person (person_id) );
-> desc favorite_food;

-> alter table person modify person_id smallint unsigned auto_increment;

-> insert into person (person_id, fname, iname ,gender, birth_date) values(null, 'William', 'Turner', 'M', '1972-05-27');

-> select person_id, fname , iname, birth_date from person;
-> select person_id, fname , iname, birth_date from person2 where person_id = 1;
-> insert into favorite_food (person_id, food) values(1, 'pizza');
-> insert into favorite_food (person_id, food) values(1, 'cookies');
-> insert into favorite_food (person_id, food) values(1, 'nachos');
-> select food  from favorite_food where person_id = 1 order by food;
-> insert into person (person_id, fname, iname , gender, birth_date, street, city, state, country, postal_code) values(null, 'Susan', 'Smith', 'F', '1975-11-02', '23 Maple St.', 'Arlington', 'VA', 'USA', '20220');
-> select person_id, fname, iname, birth_date  from person;

mysql -u liushui -p --xml bank
-> select * from favorite_food;

-> update person  set street = '1225 Tremont St.', city = 'Boston', state = 'MA', country = 'USA', postal_code = '02138' where person_id =1;

-> delete from person where person_id = 2;


