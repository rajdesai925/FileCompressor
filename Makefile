OUTPUT=fileCompressor

%: %.c %.h
	gcc -o -l $@ $<

%: %.c
	gcc $(CFLAGS) -o $@ $<

all: $(OUTPUT)

clean:
	rm $(OUTPUT)
	rm HuffmanCodebook
