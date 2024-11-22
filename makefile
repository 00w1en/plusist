all:
	cl.exe /EHsc /I include src\main.cpp src\midi.cpp src\util.cpp User32.lib

clean:
	del main.obj midi.obj util.obj main.exe
