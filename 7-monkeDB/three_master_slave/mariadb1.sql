CREATE DATABASE IF NOT EXISTS edbertdb;
USE edbertdb;

-- Buat pengguna untuk replikasi
CREATE USER 'master1'@'%' IDENTIFIED BY 'master1';
GRANT REPLICATION SLAVE ON *.* TO 'master1'@'%';
FLUSH PRIVILEGES;