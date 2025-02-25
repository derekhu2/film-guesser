build:
	rm -f ./a.out
	g++ -std=c++17 -g -Wall main.cpp movie.cpp inforow.cpp curl_util.cpp -lcurl -I/opt/homebrew/include && ./a.out

valgrind:
	rm -f ./a.out
	g++ -std=c++17 -g -Wall main.cpp movie.cpp inforow.cpp curl_util.cpp -lcurl -I/opt/homebrew/include && ./a.out
	valgrind --tool=memcheck --leak-check=full ./a.out