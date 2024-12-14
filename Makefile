RSA: RSA.o key.o
	gcc RSA.o key.o -o RSA -lgmp

key_gen: key_generator.o key.o
	gcc key_generator.o key.o -o key_gen -lgmp

key_generator.o: key_generator.c key.h
	gcc -c key_generator.c -o key_generator.o

RSA.o: RSA.c key.h
	gcc -c RSA.c -o RSA.o

key.o: key.c key.h
	gcc -c key.c -o key.o

clean:
	rm key_gen RSA *.o