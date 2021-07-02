## Power Supply
0. Change log_server path in 'server.c' to your log file
```
"{your-log-folder}/server_%Y-%m-%d_%H:%M:%S.txt"
```

1. Run makefile:
```
$ make
```

2. Open terminal & start server with a port:
```
$ ./server 9000
```

3. Open another terminal & run client with server ip and server port:
```
$ ./client 127.0.0.1 9000
```

More clients is allowed, maximum 10 clients at same time.
