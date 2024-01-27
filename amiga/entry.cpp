// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/asl.h>

#include <dos/dos.h>
#include <workbench/workbench.h>
#include <inline/exec.h>
#include <inline/dos.h>

#define MIN_STACK_SIZE 65536U

int __nocommandline = 1;
int __initlibraries = 0;
extern char  *__commandline;
struct DosLibrary* DOSBase = NULL;
struct IntuitionBase* IntuitionBase = NULL;
struct GfxBase* GfxBase = NULL;
struct Library * AslBase = NULL;
extern struct WBStartup* _WBenchMsg;

extern int apk_main();

int main(void) {
    int rv;

	if ((DOSBase = (struct DosLibrary*)OpenLibrary("dos.library", 37)) == NULL) {
		return RETURN_FAIL;
	}

	struct Task*thisTask = FindTask(NULL);
	ULONG currentStack=(ULONG) thisTask->tc_SPUpper-(ULONG)thisTask->tc_SPLower;

	if (currentStack < MIN_STACK_SIZE) {

		if (_WBenchMsg) {
			if ((IntuitionBase = (struct IntuitionBase*)OpenLibrary("intuition.library", 33)) != NULL) {
				EasyStruct str;
				str.es_StructSize = sizeof(EasyStruct);
				str.es_Flags = 0;
				str.es_GadgetFormat = (CONST_STRPTR)"OK";
				str.es_TextFormat = (CONST_STRPTR)"Not enough stack space!\n\n%ld bytes given.\n\nPlease increase it to at least %ld bytes\nin the Workbench Information Window.";
				str.es_Title = (CONST_STRPTR)"Cruise";

				EasyRequest(NULL, &str, NULL, currentStack, MIN_STACK_SIZE);
				CloseLibrary((struct Library*)IntuitionBase);
			}
		}
		else {
			Printf("Not enough stack space!\n\n%ld bytes given.\n\nPlease run the command \"stack %lu\" before running this program from the CLI.\n", currentStack, MIN_STACK_SIZE);
		}

		CloseLibrary((struct Library*)DOSBase);

		return RETURN_FAIL;
	}

	if ((IntuitionBase = (struct IntuitionBase*)OpenLibrary("intuition.library", 37)) == NULL) {
		CloseLibrary((struct Library*)DOSBase);
		return RETURN_FAIL;
	}

	if ((GfxBase = (struct GfxBase*)OpenLibrary("graphics.library", 37)) == NULL) {
		CloseLibrary((struct Library*)IntuitionBase);
		CloseLibrary((struct Library*)DOSBase);
		return RETURN_FAIL;
	}

	if ((AslBase = OpenLibrary("asl.library", 37)) == NULL) {
		CloseLibrary((struct Library*)GfxBase);
		CloseLibrary((struct Library*)IntuitionBase);
		CloseLibrary((struct Library*)DOSBase);
		return RETURN_FAIL;
	}

	rv = apk_main();

#if 0
	if (_WBenchMsg) {
		EasyStruct str;
		str.es_StructSize = sizeof(EasyStruct);
		str.es_Flags = 0;
		str.es_GadgetFormat = (CONST_STRPTR)"OK";
		str.es_TextFormat = (CONST_STRPTR)"Thanks for playing!";
		str.es_Title = (CONST_STRPTR)"Cruise";

		EasyRequest(NULL, &str, NULL);
	}
	else {
		PutStr("Thanks for playing!\n");
	}
#endif

	CloseLibrary((struct Library*)AslBase);
	CloseLibrary((struct Library*)GfxBase);
	CloseLibrary((struct Library*)IntuitionBase);
	CloseLibrary((struct Library*)DOSBase);

	return rv;
}