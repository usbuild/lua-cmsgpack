LUAJIT_SRC=.
LUAJIT_LIB_DIR=.
LUAJIT_LIB_NAME=luajit-5.1

cmsgpack.so:
	gcc lua_cmsgpack.c lua_fast_impl.c -I$(LUAJIT_SRC) -L$(LUAJIT_LIB_DIR) -l$(LUAJIT_LIB_NAME) -ldl -lm --shared -fPIC -o $@
