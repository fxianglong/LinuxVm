all:tcpsock tcp_cli http_srv
tcpsock:tcpsock.hpp
	g++ $^ -o $@
tcp_cli:tcp_cli.cpp
	g++ $^ -o $@
http_srv:http_srv.cpp -lpthread
	g++ $^ -o $@
