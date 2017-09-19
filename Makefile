all:	app

app:	app_mysql.c callbacks.c interface.c main.c cdapp_gnome.h app_mysql.h
	gcc -o app -I/usr/include/mysql app_mysql.c callbacks.c interface.c main.c -lmysqlclient `pkg-config --cflags --libs libgnome-2.0 libgnomeui-2.0`

clean:
	rm -f app
