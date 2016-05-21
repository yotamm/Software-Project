#include "SPLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//File open mode
#define SP_LOGGER_OPEN_MODE "w"

// Global variable holding the logger
SPLogger logger = NULL;

//documentation later, with the func itself
SP_LOGGER_MSG printMyEWDI(const char* myStr);
SP_LOGGER_MSG printMyLineFileFunc(const char* file, const char* function,
		const int line);

struct sp_logger_t {
	FILE* outputChannel; //The logger file
	bool isStdOut; //Indicates if the logger is stdout
	SP_LOGGER_LEVEL level; //Indicates the level
};

SP_LOGGER_MSG spLoggerCreate(const char* filename, SP_LOGGER_LEVEL level) {
	if (logger != NULL) { //Already defined
		return SP_LOGGER_DEFINED;
	}
	logger = (SPLogger) malloc(sizeof(*logger));
	if (logger == NULL) { //Allocation failure
		return SP_LOGGER_OUT_OF_MEMORY;
	}
	logger->level = level; //Set the level of the logger
	if (filename == NULL) { //In case the filename is not set use stdout
		logger->outputChannel = stdout;
		logger->isStdOut = true;
	} else { //Otherwise open the file in write mode
		logger->outputChannel = fopen(filename, SP_LOGGER_OPEN_MODE);
		if (logger->outputChannel == NULL) { //Open failed
			free(logger);
			logger = NULL;
			return SP_LOGGER_CANNOT_OPEN_FILE;
		}
		logger->isStdOut = false;
	}
	return SP_LOGGER_SUCCESS;
}

void spLoggerDestroy() {
	if (!logger) {
		return;
	}
	if (!logger->isStdOut) { //Close file only if not stdout
		fclose(logger->outputChannel);
	}
	free(logger); //free allocation
	logger = NULL;
}

/**
 * 	Prints error message. The error message format is given below:
 * 	---ERROR---
 * 	- file: <file>
 *  - function: <function>
 *  - line: <line>
 *  - message: <msg>
 *
 * 	<file> 	   - is the string given by file, it represents the file in which
 * 		   		 the error print call occurred.
 * 	<function> - is the string given by function, it represents the function in which
 * 			   	 the error print call occurred.
 * 	<line> 	   - is the string given by line, it represents the line in which
 * 		   		 the error print call occurred
 * 	<msg> 	   - is the string given by msg, it contains the msg given by the user
 *
 * 	Error messages will be printed at levels:
 *
 * 	SP_LOGGER_ERROR_LEVEL,
 *	SP_LOGGER_WARNING_ERROR_LEVEL,
 *	SP_LOGGER_INFO_WARNING_ERROR_LEVEL,
 *	SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL
 *
 * 	A new line will be printed after the print call.
 *
 * @param msg     	- The message to printed
 * @param file    	- A string representing the filename in which spLoggerPrintError call occurred
 * @param function 	- A string representing the function name in which spLoggerPrintError call ocurred
 * @param line		- A string representing the line in which the function call occurred
 * @return
 * SP_LOGGER_UNDIFINED 			- If the logger is undefined
 * SP_LOGGER_INVAlID_ARGUMENT	- If any of msg or file or function are null or line is negative
 * SP_LOGGER_WRITE_FAIL			- If Write failure occurred
 * SP_LOGGER_SUCCESS			- otherwise
 */
SP_LOGGER_MSG spLoggerPrintError(const char* msg, const char* file,
		const char* function, const int line) {
	if (logger == NULL) //the logger is undefined
		return SP_LOGGER_UNDIFINED;

	if (msg == NULL || file == NULL || function == NULL || line < 0)
		//any of msg or file or function are null or line is negative
		return SP_LOGGER_INVAlID_ARGUMENT;

	//prints at all lvls

	/**
	 * 	Prints error message. The error message format is given below:
	 * 	---ERROR---
	 * 	- file: <file>
	 *  - function: <function>
	 *  - line: <line>
	 *  - message: <msg>
	 *
	 **/
	if (printMyEWDI("ERROR") == SP_LOGGER_WRITE_FAIL
			|| printMyLineFileFunc(file, function, line) == SP_LOGGER_WRITE_FAIL
			|| spLoggerPrintMsg(msg) == SP_LOGGER_WRITE_FAIL)
		return SP_LOGGER_WRITE_FAIL;

	return SP_LOGGER_SUCCESS; //otherwise
}

/**
 * 	Prints warning message. The warning message format is given below:
 * 	---WARNING---
 * 	- file: <file>
 *  - function: <function>
 *  - line: <line>
 *  - message: <msg>
 *
 * 	<file> 	   - is the string given by file, it represents the file in which
 * 		   		 the warning print call occurred.
 * 	<function> - is the string given by function, it represents the function in which
 * 			   	 the warning print call occurred.
 * 	<line> 	   - is the string given by line, it represents the line in which
 * 		   		 the warning print call occurred
 * 	<msg> 	   - is the string given by msg, it contains the msg given by the user
 *
 * 	Warning messages will be printed at levels:
 *
 *	SP_LOGGER_WARNING_ERROR_LEVEL,
 *	SP_LOGGER_INFO_WARNING_ERROR_LEVEL,
 *	SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL
 *
 *	A new line will be printed after the print call.
 *
 * @param msg     	- The message to printed
 * @param file    	- A string representing the filename in which spLoggerPrintWarning call occurred
 * @param function 	- A string representing the function name in which spLoggerPrintWarning call ocurred
 * @param line		- A string representing the line in which the spLoggerPrintWarning call occurred
 * @return
 * SP_LOGGER_UNDIFINED 			- If the logger is undefined
 * SP_LOGGER_INVAlID_ARGUMENT	- If any of msg or file or function are null or line is negative
 * SP_LOGGER_WRITE_FAIL			- If write failure occurred
 * SP_LOGGER_SUCCESS			- otherwise
 */
SP_LOGGER_MSG spLoggerPrintWarning(const char* msg, const char* file,
		const char* function, const int line) {
	if (logger == NULL)
		//the logger is undefined
		return SP_LOGGER_UNDIFINED;

	if (logger->level == SP_LOGGER_ERROR_LEVEL)
		//should not print warning
		return SP_LOGGER_SUCCESS;

	if (msg == NULL || file == NULL || function == NULL || line < 0)
		//any of msg or file or function are null or line is negative
		return SP_LOGGER_INVAlID_ARGUMENT;

	/**
	 * 	Prints warning message. The warning message format is given below:
	 * 	---WARNING---
	 * 	- file: <file>
	 *  - function: <function>
	 *  - line: <line>
	 *  - message: <msg>
	 **/
	if (printMyEWDI("WARNING") == SP_LOGGER_WRITE_FAIL
			|| printMyLineFileFunc(file, function, line) == SP_LOGGER_WRITE_FAIL
			|| spLoggerPrintMsg(msg) == SP_LOGGER_WRITE_FAIL)
		return SP_LOGGER_WRITE_FAIL;

	return SP_LOGGER_SUCCESS; //otherwise
}
/**
 * 	Prints Info message. The info message format is given below:
 * 	---INFO---
 *  - message: <msg>
 *
 * 	<msg> 	   - is the string given by msg, it contains the msg given by the user
 *
 * 	Info messages will be printed at levels:
 *
 *	SP_LOGGER_INFO_WARNING_ERROR_LEVEL,
 *	SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL
 *
 * 	A new line will be printed after the print call.
 *
 * @param msg     	- The message to printed
 * @return
 * SP_LOGGER_UNDIFINED 			- If the logger is undefined
 * SP_LOGGER_INVAlID_ARGUMENT	- If msg is null
 * SP_LOGGER_WRITE_FAIL			- If Write failure occurred
 * SP_LOGGER_SUCCESS			- otherwise
 */
SP_LOGGER_MSG spLoggerPrintInfo(const char* msg) {
	if (logger == NULL)
		//the logger is undefined
		return SP_LOGGER_UNDIFINED;

	if (msg == NULL)
		//msg is null
		return SP_LOGGER_INVAlID_ARGUMENT;

	if (logger->level == SP_LOGGER_ERROR_LEVEL
			|| logger->level == SP_LOGGER_WARNING_ERROR_LEVEL)
		//should not print info
		return SP_LOGGER_SUCCESS;

	/**
	 * 	Prints Info message. The info message format is given below:
	 * 	---INFO---
	 *  - message: <msg>
	 **/
	if (printMyEWDI("INFO") == SP_LOGGER_WRITE_FAIL
			|| spLoggerPrintMsg(msg) == SP_LOGGER_WRITE_FAIL)
		return SP_LOGGER_WRITE_FAIL;

	return SP_LOGGER_SUCCESS; //otherwise
}

/**
 * 	Prints the debug message. The debug message format is given below:
 * 	---DEBUG---
 * 	- file: <file>
 *  - function: <function>
 *  - line: <line>
 *  - message: <msg>
 *
 * 	<file> 	   - is the string given by file, it represents the file in which
 * 		   		 the debug print call occurred.
 * 	<function> - is the string given by function, it represents the function in which
 * 			   	 the debug print call occurred.
 * 	<line> 	   - is the string given by line, it represents the line in which
 * 		   		 the debug print call occurred
 * 	<msg> 	   - is the string given by msg, it contains the msg given by the user
 *
 * 	Debug messages will be printed at level:
 *
 *	SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL
 *
 * 	A new line will be printed after the print call.
 *
 * @param msg     	- The message to printed
 * @param file    	- A string representing the filename in which spLoggerPrintWarning call occurred
 * @param function 	- A string representing the function name in which spLoggerPrintWarning call ocurred
 * @param line		- A string representing the line in which the function call occurred
 * @return
 * SP_LOGGER_UNDIFINED 			- If the logger is undefined
 * SP_LOGGER_INVAlID_ARGUMENT	- If any of msg or file or function are null or line is negative
 * SP_LOGGER_WRITE_FAIL			- If Write failure occurred
 * SP_LOGGER_SUCCESS			- otherwise
 */
SP_LOGGER_MSG spLoggerPrintDebug(const char* msg, const char* file,
		const char* function, const int line) {
	if (logger == NULL)
		//the logger is undefined
		return SP_LOGGER_UNDIFINED;

	if (msg == NULL || file == NULL || function == NULL || line < 0)
		//any of msg or file or function are null or line is negative
		return SP_LOGGER_INVAlID_ARGUMENT;

	if (logger->level != SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL)
		//should not print debug
		return SP_LOGGER_SUCCESS;

	/**
	 * 	Prints the debug message. The debug message format is given below:
	 * 	---DEBUG---
	 * 	- file: <file>
	 *  - function: <function>
	 *  - line: <line>
	 *  - message: <msg>
	 **/
	if (printMyEWDI("DEBUG") == SP_LOGGER_WRITE_FAIL
			|| printMyLineFileFunc(file, function, line) == SP_LOGGER_WRITE_FAIL
			|| spLoggerPrintMsg(msg) == SP_LOGGER_WRITE_FAIL)
		return SP_LOGGER_WRITE_FAIL;

	return SP_LOGGER_SUCCESS; //otherwise
}

/**
 * The given message is printed. A new line is printed at the end of msg
 * The message will be printed in all levels.
 *
 * @param msg - The message to be printed
 * @return
 * SP_LOGGER_UNDIFINED 			- If the logger is undefined
 * SP_LOGGER_INVAlID_ARGUMENT	- If msg is null
 * SP_LOGGER_WRITE_FAIL			- If Write failure occurred
 * SP_LOGGER_SUCCESS			- otherwise
 */
SP_LOGGER_MSG spLoggerPrintMsg(const char* msg) {
	if (logger == NULL) //the logger is undefined
		return SP_LOGGER_UNDIFINED;

	if (msg == NULL)
		//msg is null
		return SP_LOGGER_INVAlID_ARGUMENT;

	if (fprintf(logger->outputChannel, "- message: %s\n", msg) < 0)
		return SP_LOGGER_WRITE_FAIL;

	return SP_LOGGER_SUCCESS; //otherwise
}

/*
 * @param - myStr = ERROR/WARNING/DEBUG/INFO
 * (E)rror(W)warning(D)ebug(I)nfo -> EWDI
 * we KNOW : myStr != null && logger is defined
 *
 * prints wanted line (as defined in header)
 * related to ERROR/WARNING/DEBUG/INFO into logger->outputChannel
 *
 * @return
 * SP_LOGGER_WRITE_FAIL			- If Write failure occurred
 * SP_LOGGER_SUCCESS			- otherwise
 */
SP_LOGGER_MSG printMyEWDI(const char* myStr) {
	if (fprintf(logger->outputChannel, "---%s---\n", myStr) < 0)
		return SP_LOGGER_WRITE_FAIL;
	return SP_LOGGER_SUCCESS;
}

/*
 * @param msg     	- The message to printed
 * @param file    	- A string representing the filename in which spLoggerPrintWarning call occurred
 * @param function 	- A string representing the function name in which spLoggerPrintWarning call ocurred
 * @param line		- A string representing the line in which the spLoggerPrintWarning call occurred
 *
 * we KNOW : file != null function != null
 * 			&& line >= 0 && logger is defined
 *
 * prints wanted lines (as defined in header)
 * related to file, function and line into logger->outputChannel
 *
 * @return
 * SP_LOGGER_WRITE_FAIL			- If Write failure occurred
 * SP_LOGGER_SUCCESS			- otherwise
 */
SP_LOGGER_MSG printMyLineFileFunc(const char* file, const char* function,
		const int line) {
	if (fprintf(logger->outputChannel, "- file: %s\n", file) < 0
			|| fprintf(logger->outputChannel, "- function: %s\n", function) < 0
			|| fprintf(logger->outputChannel, "- line: %d\n", line) < 0)
		return SP_LOGGER_WRITE_FAIL;
	return SP_LOGGER_SUCCESS;
}
