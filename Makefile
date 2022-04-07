all:
	cd src && bash test.sh

release:
	cd src && zip ../xkanto14.zip oems.c test.sh xkanto14.pdf