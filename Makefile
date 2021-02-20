.PHONY: it

t:
	gcc css\\SimpleTest.c -o css\\SimpleTest.exe -mconsole -mwindows
	css\\SimpleTest.exe

it:
	gcc css\\Test.c -o css\\Test.exe -mconsole -mwindows
	css\\Test.exe