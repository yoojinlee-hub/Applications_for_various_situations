//*****************************************************************************
//	FindString
//
//	Can be used in EFI Shell & DOS
//*****************************************************************************

// Options =====================================================================
//#define _CRT_SECURE_NO_WARNINGS
#define EFI_APPLICATION 1


// Header Files ================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef EFI_APPLICATION
	#include <Uefi.h>
#else
	#include <dos.h>
	#include <conio.h>
#endif

void startPrint();
void howToUse();
void log_message(const char *message, const char *message2);

/*******************************************************************************
 *	Version
 *	23/03/09	V0.1.0		
 ******************************************************************************/
#define	VERSION_MAJOR					0			// Major version
#define	VERSION_MINOR					1			// Minor version 1
#define	VERSION_BUILD					0			// Minor version 2


 /*******************************************************************************
  * Main Function
  ******************************************************************************/

//main function
int main(int argc, char **argv) {
	//======== parameters ==========
	const char*filename = argv[1];
	const char*findString = argv[2];
	//==============================
	FILE *file;
	char *buffer = "";
	int found = 0;
	long file_size;
	int lineNumber = 1;

	//===== Start Routines =======
	startPrint(); 
	//============================

	if (argc != 3) {
		printf("Parameter Error!\n");
		howToUse();
		return -1;
	}

	// ================= open file
	file = fopen(filename, "rb");
	if (file == NULL) {
		printf("ERROR: Can not open file\n");
		return -1;
	}

	// ==============  find size of file
	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	rewind(file);

	// ============== allocate memory buffer
	buffer = (char*)malloc(file_size);
	if (!buffer) {
		printf("Memory allocation error.\n");
		return 1;
	}

	// ============== searching
	while (fgets(buffer, file_size, file) != NULL) {
		if (strstr(buffer, findString) != NULL) {
			found = 1;
			printf("'%s' was found in line %d\n", findString, lineNumber);
			break;
		}
		lineNumber++;
	}
	fclose(file);

	if (found) {
		printf("%s is founded, Pass\n", argv[2]);
		log_message(argv[2], argv[1]);
		free(buffer);
		return 0;
	}
	else {
		printf("Fail\n");
		free(buffer);
		return 1;
	}
}


/*******************************************************************************
 *	Subroutine
 ******************************************************************************/
 /*
 * when file started
 */
void startPrint() {
	printf("Copyright@ 2023-2025, YJ : FindString ver%d.%d.%d\n", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);
}

/*
* tell user how to use
*/
void howToUse() {
	char quotation = '"';

#ifdef EFI_APPLICATION
	printf("FindString.efi <fileName> %c<String which you want to find>%c\n", quotation, quotation);
	printf("[example] FindString.efi BIOSUpdateResult.log %cupdate factory data%c\n", quotation, quotation);
#else
	printf("FindString.exe <fileName> %c<String which you want to find>%c\n", quotation, quotation);
	printf("[example] FindString.exe BIOSUpdateResult.log %cupdate factory data%c\n", quotation, quotation);
#endif // EFI_APPLICATION

}

/*
* make log file
*/
void log_message(const char *message, const char *message2) {
	FILE *log_file = fopen("ResultPass.log", "a"); // open file
	char contents[] = " is in ";
	char *result = malloc((strlen(message) + strlen(contents) + strlen(message2) + 2) * sizeof(char));

	strcpy(result, message);
	strcat(result, contents);
	strcat(result, message2);
	strcat(result, "\n");

	fprintf(log_file, "%s", result);

	free(result);
	fclose(log_file); // close file
	printf("FILE made!\n");
}