# Two Bidirectional Replication Database

in this project, i used docker container to simulate 2 databases.

there is `mariadbX.cnf` to make sure each database has different ID. As you can see, the server-id correspond to each db, the log-bin is for enabling binary log replication and specify base binary log file name. We want to ignore the mysql database since it contains user and other information. 

```
# mariadb1
[mysqld]
server-id=1
log-bin=mysql-bin
binlog-ignore-db=mysql
replicate-ignore-db=mysql
```

---
This is the default sql file. first it create `edbertdb` database. Then it create user with credential `master1:master1` (`masterX:masterX` corresponding to each db) and allow to connect from any host `@'%'`. Then it Grants the `REPLICATION SLAVE` privilege to the user `master1`, and flush to makesure the priv enabled. After that we Configures the current server to connect to another server (mariadb2) as a replication slave. and start the replication process.

```
CREATE DATABASE IF NOT EXISTS edbertdb;
USE edbertdb;

-- Buat pengguna untuk replikasi
CREATE USER 'master1'@'%' IDENTIFIED BY 'master1';
GRANT REPLICATION SLAVE ON *.* TO 'master1'@'%';
FLUSH PRIVILEGES;

CHANGE MASTER TO MASTER_HOST='mariadb2', MASTER_USER='master2', MASTER_PASSWORD='master2', MASTER_PORT=3306, MASTER_LOG_FILE='mysql-bin.000001', MASTER_LOG_POS=4;
START SLAVE;
```
## How To Run?
get into the folder `two_master_master` and run
```bash
docker compose up -d
```
if you want to remove then
```bash
docker compose down -v
```

---
# Additional Information

Binary logging di MariaDB dan MySQL mencatat perubahan data dalam bentuk log biner untuk mendukung replikasi dan pemulihan data. Replikasi menggunakan binary logs, bukan salinan data langsung, karena ini lebih efisien, mengurangi gangguan pada server master, dan memastikan konsistensi serta latensi rendah dalam penerapan perubahan. Dengan metode ini, data di server slave tetap sinkron dengan server master dan menyediakan jejak audit yang rinci.

Alasan Mengapa Tidak Menggunakan "Copy Data" Langsung:
- Minimalkan Gangguan: Mengurangi dampak pada performa server master.
- Konsistensi Data: Menjamin data di slave tetap konsisten dengan master.
- Latensi Rendah: Memastikan perubahan diterapkan dengan cepat.
- Fault Tolerance: Memudahkan pemulihan dari titik terakhir yang diketahui.
- Audit Trail: Menyediakan jejak audit perubahan untuk keamanan dan debugging.
- Selective Replication: Memungkinkan pemilihan database atau tabel yang akan direplikasi.