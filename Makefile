.PHONY: win

win:
	gcc low\\WinShot\\WinTest.c -o low\\WinShot\\WinTest.exe -mconsole -mwindows
	low\\WinShot\\WinTest.exe