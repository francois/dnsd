# dnsd

A very basic implementation of a DNS server. This was built to better understand the DNS protocol.

# Usage

    git clone https://github.com/francois/dnsd
    cd dnsd
    rake
    ./dnsd

# Sample run

    dnsd.c:6  Booting dnsd v0.0.1...
    boot.c:10 Received 1 arguments:
    boot.c:12   0: ./dnsd
    boot.c:33 Opening socket on port 2353
    boot.c:57 DNSD ready to accept requests on 0.0.0.0:2353
    run.c:68  Accepting request on port 2353


Sending a query:

    $ dig @127.0.0.1 -p 2353 seevibes.com ANY

Results in this:

    run.c:19  Received 30 bytes from 127.0.0.1:55305
    0000  85 8c 01 00 00 01 00 00   ........
    0008  00 00 00 00 08 73 65 65   .....see
    0010  76 69 62 65 73 03 63 6f   vibes.co
    0018  6d 00 00 ff 00 01         m.....
    run.c:50  id: 34188
    run.c:51  qr: question
    run.c:52  opcode: query
    run.c:53  aa: false
    run.c:54  tc: false
    run.c:55  rd: false
    run.c:56  ra: false
    run.c:57  z:  0
    run.c:58  rcode:  no error
    run.c:59  qdcount:  1
    run.c:60  ancount:  0
    run.c:61  nscount:  0
    run.c:62  arcount:  0
    run.c:111 Sent 12 bytes back to 127.0.0.1

And dig responds with:

    ;; Warning: query response not set

    ; <<>> DiG 9.6-ESV-R4-P3 <<>> @127.0.0.1 -p 2353 seevibes.com ANY
    ; (1 server found)
    ;; global options: +cmd
    ;; Got answer:
    ;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 34188
    ;; flags: rd cd; QUERY: 0, ANSWER: 0, AUTHORITY: 0, ADDITIONAL: 0
    ;; WARNING: recursion requested but not available

    ;; Query time: 2 msec
    ;; SERVER: 127.0.0.1#2353(127.0.0.1)
    ;; WHEN: Mon Apr 14 20:31:54 2014
    ;; MSG SIZE  rcvd: 12

# Implementation

I chose C because I wanted to get "back to basics". I coded in C a long time ago.

# License

I put this code in the public domain. I do not guarantee any parts of the code.
