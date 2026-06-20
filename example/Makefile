# Build dark-theme test app
WDT := ..

C := clang -c -target x86_64-w64-mingw32
LINK := clang -target x86_64-w64-mingw32 -fuse-ld=lld
WINDRES := llvm-windres
CFLAGS := -I$(WDT)
LINKFLAGS := -mwindows -lcomctl32 -lgdi32 -luxtheme

default: test-dark-theme.exe

test-dark-theme.exe: test-dark-theme.o dark-theme.o exe.coff
	$(LINK) $+ $(LINKFLAGS) -o $@

test-dark-theme.o: $(WDT)/example/test-dark-theme.c $(WDT)/dark-theme.h
	$(C) $(CFLAGS) $< -o $@

exe.coff: $(WDT)/example/exe.rc 
	$(WINDRES) $< $@

dark-theme.o: $(WDT)/dark-theme.c $(WDT)/dark-theme.h
	$(C) $(CFLAGS) $< -o $@

clean:
	rm -f test-dark-theme.exe exe.coff *.o
