README for lua-fast-cmsgpack.c
===
This project is fork of [lua-cmsgpack](https://github.com/antirez/lua-cmsgpack), but use [darc](https://github.com/hdiedrich/darc) instead of normal
lua stack iterate operation, which we can speed up by avoid using [lua\_next](http://pgl.yoyo.org/luai/i/lua_next).

Tested with test.lua, but be careful when you take it for formal usage.

This library is open source software licensed under the BSD two-clause license.
