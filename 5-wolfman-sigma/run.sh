## 32.txt
echo "==== 32.txt ===" &&
python3 main.py tcmatmul/32.txt &&
sleep 10 &&
javac Main.java && java Main tcmatmul/32.txt &&
go main.go tcmatmul/32.txt &&

## 64.txt
echo "==== 64.txt ===" &&
python3 main.py tcmatmul/64.txt &&

## 128.txt
echo "==== 128.txt ===" &&
python3 main.py tcmatmul/128.txt &&

## 256.txt
echo "==== 256.txt ===" &&
python3 main.py tcmatmul/256.txt &&

## 512.txt
echo "==== 512.txt ===" &&
python3 main.py tcmatmul/512.txt &&

## 1024.txt
echo "==== 1024.txt ===" &&
python3 main.py tcmatmul/1024.txt &&

## 2048.txt
echo "==== 2048.txt ===" &&
python3 main.py tcmatmul/2048.txt