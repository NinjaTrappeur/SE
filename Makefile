all: menu sort

menu:
	cd menu; qmake; make

sort:
	cd sort; qmake; make

clean: cleanMenu cleanSort

cleanMenu:
	cd menu; make clean

cleanSort:
	cd sort; make clean

.PHONY : sort menu
