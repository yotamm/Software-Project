#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "unit_test_util.h" //SUPPORTING MACROS ASSERT_TRUE/ASSERT_FALSE etc..
#include "../SPLogger.h"

#define LOOP_MAX_1 5
// This is a helper function which checks if two files are identical
static bool identicalFiles(const char* fname1, const char* fname2) {
	FILE *fp1, *fp2;
	fp1 = fopen(fname1, "r");
	fp2 = fopen(fname2, "r");
	char ch1 = EOF, ch2 = EOF;

	if (fp1 == NULL) {
		return false;
	} else if (fp2 == NULL) {
		fclose(fp1);
		return false;
	} else {
		ch1 = getc(fp1);
		ch2 = getc(fp2);

		while ((ch1 != EOF) && (ch2 != EOF) && (ch1 == ch2)) {
			ch1 = getc(fp1);
			ch2 = getc(fp2);
		}
		fclose(fp1);
		fclose(fp2);
	}
	if (ch1 == ch2) {
		return true;
	} else {
		return false;
	}
}

//Logger is not defined
static bool basicLoggerTest() {
	ASSERT_TRUE(spLoggerPrintError("A","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
	spLoggerDestroy();
	return true;
}

//Only Errors should be printed
static bool basicLoggerErrorTest() {
	const char* expectedFile = "basicLoggerErrorTestExp.log";
	const char* testFile = "basicLoggerErrorTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

//All messages should be printed in debug level
static bool basicLoggerDebugTest() {
	const char* expectedFile = "basicLoggerDebugTestExp.log";
	const char* testFile = "basicLoggerDebugTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

/*
 * testing conditions on SP_LOGGER_INVAlID_ARGUMENT
 * aka one of arguments is null or negative
 */
static bool myNullArgsLoggerTest() {
	//create Logger with debug level
	const char* testFile = "myNullLoggerTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);

	for(int i = 0; i <= LOOP_MAX_1; i++){
		//check all kind of print funcs with different null args or negative line
		//Error
		ASSERT_TRUE(spLoggerPrintError("Error??","sp_logger_unit_test.c",__func__,__LINE__) != SP_LOGGER_INVAlID_ARGUMENT);
		ASSERT_TRUE(spLoggerPrintError(NULL,"sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
		ASSERT_TRUE(spLoggerPrintError("A",NULL,__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
		ASSERT_TRUE(spLoggerPrintError("A","sp_logger_unit_test.c",NULL,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
		ASSERT_TRUE(spLoggerPrintError("A","sp_logger_unit_test.c",__func__,-1) == SP_LOGGER_INVAlID_ARGUMENT);

		//Warning
		ASSERT_TRUE(spLoggerPrintWarning("Warning!!","sp_logger_unit_test.c",__func__,__LINE__) != SP_LOGGER_INVAlID_ARGUMENT);
		ASSERT_TRUE(spLoggerPrintWarning(NULL,"sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
		ASSERT_TRUE(spLoggerPrintWarning("A",NULL,__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
		ASSERT_TRUE(spLoggerPrintWarning("A","sp_logger_unit_test.c",NULL,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
		ASSERT_TRUE(spLoggerPrintWarning("A","sp_logger_unit_test.c",__func__,-1) == SP_LOGGER_INVAlID_ARGUMENT);

		//Debug
		ASSERT_TRUE(spLoggerPrintDebug("Debug++","sp_logger_unit_test.c",__func__,__LINE__) != SP_LOGGER_INVAlID_ARGUMENT);
		ASSERT_TRUE(spLoggerPrintDebug(NULL,"sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
		ASSERT_TRUE(spLoggerPrintDebug("A",NULL,__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
		ASSERT_TRUE(spLoggerPrintDebug("A","sp_logger_unit_test.c",NULL,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
		ASSERT_TRUE(spLoggerPrintDebug("A","sp_logger_unit_test.c",__func__,-1) == SP_LOGGER_INVAlID_ARGUMENT);

		//Info
		ASSERT_TRUE(spLoggerPrintInfo("Info..") != SP_LOGGER_INVAlID_ARGUMENT);
		ASSERT_TRUE(spLoggerPrintInfo(NULL) == SP_LOGGER_INVAlID_ARGUMENT);
	}

	spLoggerDestroy();
	return true;
}

/*
 * testing conditions on SP_LOGGER_UNDIFINED
 * when all args are ok
 * when some args are not ok
 */
static bool myLoggerUndefinedTest() {
	//args are ok
	ASSERT_TRUE(spLoggerPrintWarning("A","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
	ASSERT_TRUE(spLoggerPrintInfo("A") == SP_LOGGER_UNDIFINED);
	ASSERT_TRUE(spLoggerPrintDebug("A","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
	ASSERT_TRUE(spLoggerPrintError("A","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);

	//args are not ok
	ASSERT_TRUE(spLoggerPrintWarning(NULL,"sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
	ASSERT_TRUE(spLoggerPrintInfo(NULL) == SP_LOGGER_UNDIFINED);
	ASSERT_TRUE(spLoggerPrintDebug(NULL,"sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
	ASSERT_TRUE(spLoggerPrintError(NULL,"sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);

	spLoggerDestroy();
	return true;
}

//Only Errors and warning should be printed
static bool basicLoggerWarningTest() {
	const char* expectedFile = "my_basicLoggerWarningTestExp.log";
	const char* testFile = "basicLoggerWarningTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

//All messages except debug should be printed in info level
static bool basicLoggerInfoTest() {
	const char* expectedFile = "my_basicLoggerInfoTestExp.log";
	const char* testFile = "basicLoggerInfoTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

int main() {
	RUN_TEST(basicLoggerTest);
	RUN_TEST(basicLoggerErrorTest);
	RUN_TEST(basicLoggerDebugTest);
	RUN_TEST(myNullArgsLoggerTest);
	RUN_TEST(myLoggerUndefinedTest);
	RUN_TEST(basicLoggerWarningTest);
	RUN_TEST(basicLoggerInfoTest);
	return 0;
}
