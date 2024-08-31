
main: common dict list trees system http
	gcc -g -c test_server/test.c -o bin/test.o
	gcc bin/*.o -o test

common:
	gcc -g -c DataStructures/Common/Node.c -o bin/Node.o

dict:
	gcc -g -c DataStructures/Dictionary/Entry.c -o bin/Entry.o
	gcc -g -c DataStructures/Dictionary/Dictionary.c -o bin/Dictionary.o
	
list:
	gcc -g -c DataStructures/Lists/LinkedList.c -o bin/LinkedList.o
	gcc -g -c DataStructures/Lists/Queue.c -o bin/Queue.o

trees:
	gcc -g -c DataStructures/Trees/BinarySearchTree.c -o bin/BinarySearchTree.o

system:
	gcc -g -c Systems/ThreadPool.c -o bin/ThreadPool.o
	gcc -g -c Systems/Files.c -o bin/Files.o

http:
	gcc -g -c JsonParser.c -o bin/JsonParser.o

	gcc -g -c HTTPResponse.c -o bin/HTTPResponse.o
	gcc -g -c HTTPRequest.c -o bin/HTTPRequest.o
	gcc -g -c server.c -o bin/server.o
	gcc -g -c HTTPServer.c -o bin/HTTPServer.o

