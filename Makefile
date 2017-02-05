main.exe: main.cpp
	g++ main.cpp -o main.exe

excec: clean main.exe
	./main.exe

clean:
	rm main.exe
