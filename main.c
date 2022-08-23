/*
        GetPsyQSyms
        Grabs symbol data from a generated Psy-Q symbol file

        This is free and unencumbered software released into the public domain.

        Anyone is free to copy, modify, publish, use, compile, sell, or
        distribute this software, either in source code form or as a compiled
        binary, for any purpose, commercial or non-commercial, and by any
        means.

        In jurisdictions that recognize copyright laws, the author or authors
        of this software dedicate any and all copyright interest in the
        software to the public domain. We make this dedication for the benefit
        of the public at large and to the detriment of our heirs and
        successors. We intend this dedication to be an overt act of
        relinquishment in perpetuity of all present and future rights to this
        software under copyright law.

        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
        EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
        MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
        IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
        OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
        ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
        OTHER DEALINGS IN THE SOFTWARE.

        For more information, please refer to <https://unlicense.org>
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
        char sig[4];
        char numStr[16];
        FILE* inFile;
        FILE* outFile;
        uint32_t value;
        char nameLen;
        char* name;
        char* equ = " EQU $";

        if (argc < 3) {
                printf("Usage: GetPsyQSyms [input] [output]");
                return 1;
        }

        inFile = fopen(argv[1], "rb");
        if (!inFile) {
                printf("Error: Cannot open \"%s\" for reading.", argv[1]);
                return 1;
        }

        outFile = fopen(argv[2], "wb");
        if (!outFile) {
                fclose(inFile);
                printf("Error: Cannot open \"%s\" for writing.", argv[2]);
                return 1;
        }

        /* Check signature */
        fread(sig, 1, 3, inFile);
        sig[3] = 0;
        if (strcmp(sig, "MND")) {
                printf("Error: Not a valid symbols file.");
                return 1;
        }
        fseek(inFile, 8, SEEK_SET);

        while (1) {
                if (fread(&value, 4, 1, inFile) != 1) {
                        break;
                }
#ifdef _MSC_VER
                _itoa(value, numStr, 16);
#else
                itoa(value, numStr, 16);
#endif

                fgetc(inFile);
                nameLen = fgetc(inFile);
                name = malloc(nameLen + 1);
                name[nameLen] = 0;
                fread(name, 1, nameLen, inFile);

                fputs(name, outFile);
                fputs(equ, outFile);
                fputs(numStr, outFile);
                fputc('\n', outFile);

                free(name);
        }

        fclose(inFile);
        fclose(outFile);

        return 0;
}