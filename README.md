# nanodb - an ultra tiny db

nanodb is a small database engine based on:

* tokyo cabinet B+ tree, for storage;
* libev, for networking and
* ragel, for parsing.

It's mostly will be a key-value store for now, but I hope to make it look
more like a real db someday.

## Try it (not yet working...)

    nanodb yourdb.ndb 5000
    
    $ nc localhost 5000 << EOF
    PUT mykey
    this is the value stored in the key.
    EOF
    
    $ echo 'GET mykey' | nc localhost 5000
    this is the value stored in the key.

## Building

For now, you'll need to build tokyo cabinet in vendor and install ragel.
I'll automate this someday, but for now... good luck!

(c) macournoyer