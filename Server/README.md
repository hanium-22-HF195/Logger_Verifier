1. install mariaDB and create tables
   install mariaDB
   install default-libmysqlclient-dev
   create table unverified_frame(CID VARCHAR(24), Hash VARCHAR(64), Signed_Hash VARCHAR(350));
   create table verified_frame(CID VARCHAR(24), Hash VARCHAR(64), Signed_Hash VARCHAR(350));
   create table public_key(key_ID varchar(24), key_value varchar(450), key_status int);

2. install jsoncpp to read json file
   install jsoncpp 
   sudo apt-get install libjsoncpp-dev
