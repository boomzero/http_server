# http-server

<img width="1436" alt="image" src="https://github.com/boomzero/http_server/assets/85378277/d9b90225-269e-4b8b-92bd-1eb72b64dfb9">
<figcaption> a website being served with http-server </figcaption>


***

`http-server` is inspired by [http-server](https://www.npmjs.com/package/http-server). It serves everything in the current working directory.

It accepts one argument, the port you wnat to bind to. Nothing else.

It is loosely complies with HTTP/1.0, so it should work.


## Benchmarks

`http-server` is very fast. It is even faster than lightpd in my testing.
Each thread uses around 10% CPU during peak load, so CPU use would be around 80%.
You can reduce the number of threads, however it probably wouldn't work as well.

The following results are produced on a Raspberry Pi 4 with 2 GB of RAM:
```
This is ApacheBench, Version 2.3 <$Revision: 1903618 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 192.168.10.5 (be patient)
Completed 10000 requests
Completed 20000 requests
Completed 30000 requests
Completed 40000 requests
Completed 50000 requests
Completed 60000 requests
Completed 70000 requests
Completed 80000 requests
Completed 90000 requests
Completed 100000 requests
Finished 100000 requests


Server Software:        http-server
Server Hostname:        192.168.10.5
Server Port:            8086

Document Path:          /
Document Length:        145 bytes

Concurrency Level:      5
Time taken for tests:   29.350 seconds
Complete requests:      100000
Failed requests:        0
Non-2xx responses:      100000
Total transferred:      23300000 bytes
HTML transferred:       14500000 bytes
Requests per second:    3407.11 [#/sec] (mean)
Time per request:       1.468 [ms] (mean)
Time per request:       0.294 [ms] (mean, across all concurrent requests)
Transfer rate:          775.25 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.2      0      11
Processing:     0    1   0.3      1      18
Waiting:        0    1   0.2      1      16
Total:          0    1   0.3      1      18

Percentage of the requests served within a certain time (ms)
  50%      1
  66%      1
  75%      1
  80%      2
  90%      2
  95%      2
  98%      2
  99%      2
 100%     18 (longest request)
```
