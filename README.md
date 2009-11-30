# nanodb - an ultra tiny db

nanodb is a small database engine based on:

* tokyo cabinet B+ tree for storage;
* libev for networking and
* yajl for parsing.

## Usage

    nanodb yourdb.ndb 5000
    
    echo '{"get":"hi"}' | nc localhost 5000

## Building

For now, you'll need to build all the stuff manually in vendor.
I'll automate this someday, but for now... good luck!

(c) macournoyer