.PHONY: it

it:
	gcc css\\Test.c -o css\\Test.exe -mconsole -mwindows
	css\\Test.exe