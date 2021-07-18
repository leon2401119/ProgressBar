progress_bar: progress_bar.c
	gcc -g -pthread progress_bar.c -o progress_bar

clean:
	rm progress_bar
