#include <stdio.h>
#include <stdlib.h>

/*
 * Use this macro if for single line rawdump if printf() is not an issue
 * you can replace the printf() with your own debug function
 */
#define RAWPRINT(data, size) do {char *s = rawdump(data, size); if (s) printf("%s\n", s); else break;} while(1)

#define BYTES_PER_LINE 16
#define ADDR_LEN 9
static char *rawdump(void *data, int size)
{
    /*
     * Due to use of static variables, this function is not thread safe!
     * A workaround is to past these variables in as parameters
     */
    static void *prev = NULL;
    static int offset = 0;
    static char sbuf[BYTES_PER_LINE * 5];
    int c, i, tmp, text_start;

    /* reset local statics if either params are 0 */
    if (!data || !size)
	goto reset;
    /* if pointer changed, start new */
    if (prev != data) {
	prev = data;
	offset = 0;
    }
    if (offset >= size)
	goto reset;
    if (size > offset + BYTES_PER_LINE)
	size = offset + BYTES_PER_LINE;
    sprintf(sbuf, "%08X:", (unsigned int)((unsigned long long)data + offset));
    for (i = ADDR_LEN, tmp = offset; offset < size; offset++) {
	sprintf(&sbuf[i], "%02X", ((unsigned char *)data)[offset]);
	i += 2;
	if (offset % 4 == 3)
	    sbuf[i++] = ' ';
    }
    for (text_start = ADDR_LEN + BYTES_PER_LINE / 4 * 9; i < text_start; i++)
	sbuf[i] = ' ';
    for (; tmp < size; tmp++) {
	c = ((char *)data)[tmp];
	if (c > 31 && c < 127)
	    sbuf[i++] = c;
	else
	    sbuf[i++] = '.';
    }
    sbuf[i] = 0;
    return sbuf;
reset:
    prev = NULL;
    offset = 0;
    return NULL;
}


int main()
{
    /* sample debug routine */
    void debug(char *s)
    {
        fprintf(stderr, "DBG: %s\n", s);
    }

    char abc[55]  = "hello, world. This is a test. It should be over 2 lines";
    char *s;

    /* using RAWPRINT 1-liner macro */
    RAWPRINT(abc, 10);
    abc[1] = 0x33;
    abc[4] = 0x30;
    /* using local code to print */
    while (s = rawdump(abc, sizeof(abc)))
	debug(s);
}
