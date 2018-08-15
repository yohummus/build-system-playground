/*
 * This file is part of the Yogi distribution https://github.com/yohummus/yogi.
 * Copyright (c) 2018 Johannes Bergmann.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef YOGI_CORE_H
#define YOGI_CORE_H

//! @defgroup VS Version Macros
//!
//! Various macros describing the version of the header file. Note that those
//! macros only denote the version of the header file which does not necessarily
//! have to match the version of the actual library that is loaded at runtime.
//!
//! @{

#define YOGI_HDR_VERSION "0.0.3" ///< Whole version number
#define YOGI_HDR_VERSION_MAJOR 0 ///< Major version number
#define YOGI_HDR_VERSION_MINOR 0 ///< Minor version number
#define YOGI_HDR_VERSION_PATCH 3 ///< Patch version number

//! @}
//!
//! @defgroup CV Constants
//!
//! Immutable values used in the library such as version numbers and default
//! values for function calls. The values of the constants can be obtained via
//! the YOGI_GetConstant() function.
//!
//! The type of each constant is denoted in brackets after its description.
//!
//! @{

//! Whole version number of the library (const char*)
#define YOGI_CONST_VERSION_NUMBER 1

//! Major version number of the library (int)
#define YOGI_CONST_VERSION_MAJOR 2

//! Major version number of the library (int)
#define YOGI_CONST_VERSION_MINOR 3

//! Major version number of the library (int)
#define YOGI_CONST_VERSION_PATCH 4

//! Default multicast addresses to use for advertising (const char*)
#define YOGI_CONST_DEFAULT_ADV_ADDRESS 5

//! Default port to use for advertising via UDP IPv6 multicasts (int)
#define YOGI_CONST_DEFAULT_ADV_PORT 6

//! Default advertising interval in nanoseconds (long long)
#define YOGI_CONST_DEFAULT_ADV_INTERVAL 7

//! Default connection timeout in nanoseconds (long long)
#define YOGI_CONST_DEFAULT_CONNECTION_TIMEOUT 8

//! Default logging verbosity (int)
#define YOGI_CONST_DEFAULT_LOGGER_VERBOSITY 9

//! Default textual format for timestamps in log entries (const char*)
#define YOGI_CONST_DEFAULT_LOG_TIME_FORMAT 10

//! Default textual format for log entries (const char*)
#define YOGI_CONST_DEFAULT_LOG_FORMAT 11

//! Maximum size of a message (int)
#define YOGI_CONST_MAX_MESSAGE_SIZE 12

//! Default textual format for timestamps
#define YOGI_CONST_DEFAULT_TIME_FORMAT 13

//! Default string to denote an infinite duration
#define YOGI_CONST_DEFAULT_INF_DURATION_STRING 14

//! Default textual format for duration strings
#define YOGI_CONST_DEFAULT_DURATION_FORMAT 15

//! Default string to denote an invalid object handle
#define YOGI_CONST_DEFAULT_INVALID_HANDLE_STRING 16

//! Default textual format for strings describing an object
#define YOGI_CONST_DEFAULT_OBJECT_FORMAT 17

//! @}
//!
//! @defgroup EC Error Codes
//!
//! Yogi error codes indicating failures are always < 0. A human-readable
//! description can be obtained by calling YOGI_GetErrorString().
//!
//! @{

//! Operation completed successfully
#define YOGI_OK 0

//! Unknown internal error occured
#define YOGI_ERR_UNKNOWN -1

//! The object is still being used by another object
#define YOGI_ERR_OBJECT_STILL_USED -2

//! Insufficient memory to complete the operation
#define YOGI_ERR_BAD_ALLOC -3

//! Invalid parameter
#define YOGI_ERR_INVALID_PARAM -4

//! Invalid Handle
#define YOGI_ERR_INVALID_HANDLE -5

//! Object is of the wrong type
#define YOGI_ERR_WRONG_OBJECT_TYPE -6

//! The operation has been canceled
#define YOGI_ERR_CANCELED -7

//! Operation failed because the object is busy
#define YOGI_ERR_BUSY -8

//! The operation timed out
#define YOGI_ERR_TIMEOUT -9

//! The timer has not been started or already expired
#define YOGI_ERR_TIMER_EXPIRED -10

//! The supplied buffer is too small
#define YOGI_ERR_BUFFER_TOO_SMALL -11

//! Could not open a socket
#define YOGI_ERR_OPEN_SOCKET_FAILED -12

//! Could not bind a socket
#define YOGI_ERR_BIND_SOCKET_FAILED -13

//! Could not listen on socket
#define YOGI_ERR_LISTEN_SOCKET_FAILED -14

//! Could not set a socket option
#define YOGI_ERR_SET_SOCKET_OPTION_FAILED -15

//! Invalid regular expression
#define YOGI_ERR_INVALID_REGEX -16

//! Could not open file
#define YOGI_ERR_OPEN_FILE_FAILED -17

//! Could not read from or write to socket
#define YOGI_ERR_RW_SOCKET_FAILED -18

//! Could not connect a socket
#define YOGI_ERR_CONNECT_SOCKET_FAILED -19

//! The magic prefix sent when establishing a connection is wrong
#define YOGI_ERR_INVALID_MAGIC_PREFIX -20

//! The local and remote branches use incompatible Yogi versions
#define YOGI_ERR_INCOMPATIBLE_VERSION -21

//! Could not deserialize a message
#define YOGI_ERR_DESERIALIZE_MSG_FAILED -22

//! Could not accept a socket
#define YOGI_ERR_ACCEPT_SOCKET_FAILED -23

//! Attempting to connect branch to itself
#define YOGI_ERR_LOOPBACK_CONNECTION -24

//! The passwords of the local and remote branch don't match
#define YOGI_ERR_PASSWORD_MISMATCH -25

//! The net names of the local and remote branch don't match
#define YOGI_ERR_NET_NAME_MISMATCH -26

//! A branch with the same name is already active
#define YOGI_ERR_DUPLICATE_BRANCH_NAME -27

//! A branch with the same path is already active
#define YOGI_ERR_DUPLICATE_BRANCH_PATH -28

//! Message is too large
#define YOGI_ERR_MESSAGE_TOO_LARGE -29

//! Parsing the command line failed
#define YOGI_ERR_PARSING_CMDLINE_FAILED -30

//! Parsing a JSON string failed
#define YOGI_ERR_PARSING_JSON_FAILED -31

//! Parsing a configuration file failed
#define YOGI_ERR_PARSING_FILE_FAILED -32

//! The configuration is not valid
#define YOGI_ERR_CONFIG_NOT_VALID -33

//! Help/usage text requested
#define YOGI_ERR_HELP_REQUESTED -34

//! Could not write to file
#define YOGI_ERR_WRITE_TO_FILE_FAILED -35

//! One or more configuration variables are undefined or could not be resolved
#define YOGI_ERR_UNDEFINED_VARIABLES -36

//! Support for configuration variables has been disabled
#define YOGI_ERR_NO_VARIABLE_SUPPORT -37

//! A configuration variable has been used in a key
#define YOGI_ERR_VARIABLE_USED_IN_KEY -38

//! Invalid time format
#define YOGI_ERR_INVALID_TIME_FORMAT -39

//! Could not parse time string
#define YOGI_ERR_PARSING_TIME_FAILED -40

//! @}
//!
//! @defgroup VB Log verbosity/severity
//!
//! Verbosity/severity levels for logging.
//!
//! @{

//! Used to disable logging
#define YOGI_VB_NONE -1

//! Fatal errors are error that requires a process restart
#define YOGI_VB_FATAL 0

//! Errors that the system can recover from
#define YOGI_VB_ERROR 1

//! Warnings
#define YOGI_VB_WARNING 2

//! Useful general information about the system state
#define YOGI_VB_INFO 3

//! Information for debugging
#define YOGI_VB_DEBUG 4

//! Detailed debugging
#define YOGI_VB_TRACE 5

//! @}
//!
//! @defgroup BC Boolean Constants
//!
//! Definitions for true and false.
//!
//! @{

//! Represents a logical false
#define YOGI_FALSE 0

//! Represents a logical true
#define YOGI_TRUE 1

//! @}
//!
//! @defgroup ST Stream Constants
//!
//! Constants for differentiating between stdout and stderr.
//!
//! @{

//! Standard output
#define YOGI_ST_STDOUT 0

//! Standard error output
#define YOGI_ST_STDERR 1

//! @}
//!
//! @defgroup SIG Signals
//!
//! Definitions of various signals.
//!
//! Signals in Yogi are intended to be used similar to POSIX signals. They have
//! to be raised explicitly by the user (e.g. when receiving an actual POSIX
//! signal like SIGINT) via YOGI_RaiseSignal(). A signal will be received by
//! all signal sets containing that signal.
//!
//! @{

//! No signal
#define YOGI_SIG_NONE 0

//! Interrupt (e.g. by receiving SIGINT or pressing STRG + C)
#define YOGI_SIG_INT (1<<0)

//! Termination request (e.g. by receiving SIGTERM)
#define YOGI_SIG_TERM (1<<1)

//! User-defined signal 1
#define YOGI_SIG_USR1 (1<<24)

//! User-defined signal 2
#define YOGI_SIG_USR2 (1<<25)

//! User-defined signal 3
#define YOGI_SIG_USR3 (1<<26)

//! User-defined signal 4
#define YOGI_SIG_USR4 (1<<27)

//! User-defined signal 5
#define YOGI_SIG_USR5 (1<<28)

//! User-defined signal 6
#define YOGI_SIG_USR6 (1<<29)

//! User-defined signal 7
#define YOGI_SIG_USR7 (1<<30)

//! User-defined signal 8
#define YOGI_SIG_USR8 (1<<31)

//! All signals
#define YOGI_SIG_ALL ( YOGI_SIG_INT  \
                     | YOGI_SIG_TERM \
                     | YOGI_SIG_USR1 \
                     | YOGI_SIG_USR2 \
                     | YOGI_SIG_USR3 \
                     | YOGI_SIG_USR4 \
                     | YOGI_SIG_USR5 \
                     | YOGI_SIG_USR6 \
                     | YOGI_SIG_USR7 \
                     | YOGI_SIG_USR8 )

//! @}
//!
//! @defgroup CFG Configuration Flags
//!
//! Flags used to change a configuration object's behaviour.
//!
//! @{

//! No flags
#define YOGI_CFG_NONE 0

//! Disables support for variables in the configuration
//!
//! Variables are used to define common values (or parts of values) in a
//! pre-defined  section. They can then be used in multiple other parts of the
//! configuration.
//!
//! Note: Variables can *not* be used in keys.
//!
//! Variables are defined in the *variables* section of the configuration:
//!
//!   {
//!     "variables": {
//!       "ROOT": "/usr/share/my-app",
//!       "DATA_DIR": "${ROOT}/data"
//!       "DURATION": 30,
//!       "MAX_DURATION": "${DURATION}",
//!   ...
//!
//! These variables can then be used anywhere in the configuration, even in the
//! *variables* section itself as shown above.
//!
//! Note: Even if the value of a variable is not a string, the placeholder
//!       *${name}* always has to be a string to conform to the JSON format.
//!       When resolving a placeholder for a non-string variable, the type of
//!       the target value will be changed accordingly if and only if the
//!       placeholder is surrounded by quotation marks as shown for the
//!       *MAX_DURATION* variable above. Othwise, the target value will remain
//!       a string and the placeholder will be replaced with the stringified
//!       value of the variable.
#define YOGI_CFG_DISABLE_VARIABLES (1<<0)

//! Makes configuration options given directly on the command line overridable
//!
//! By default, configuration options given direclty on the command line are
//! immutable, i.e. they will never be updated from another source.
#define YOGI_CFG_MUTABLE_CMD_LINE (1<<1)

//! @}
//!
//! @defgroup CLO Command Line Options
//!
//! Flags used to adjust how command line options are parsed.
//!
//! @{

//! No options
#define YOGI_CLO_NONE 0

//! Include logging configuration for file logging
//!
//! Adds the *--log-...*  switches.
#define YOGI_CLO_LOGGING (1<<0)

//! Include branch name configuration
//!
//! Adds the *--name* switch.
#define YOGI_CLO_BRANCH_NAME (1<<1)

//! Include branch description configuration
//!
//! Adds the *--description* switch.
#define YOGI_CLO_BRANCH_DESCRIPTION (1<<2)

//! Include network name configuration
//!
//! Adds the *--network* switch.
#define YOGI_CLO_BRANCH_NETWORK (1<<3)

//! Include network password configuration
//!
//! Adds the *--password* switch.
#define YOGI_CLO_BRANCH_PASSWORD (1<<4)

//! Include branch path configuration
//!
//! Adds the *--path* switch.
#define YOGI_CLO_BRANCH_PATH (1<<5)

//! Include branch advertising address configuration
//!
//! Adds the *--adv-addr* switch.
#define YOGI_CLO_BRANCH_ADV_ADDR (1<<6)

//! Include branch advertising port configuration
//!
//! Adds the *--adv-port* switch.
#define YOGI_CLO_BRANCH_ADV_PORT (1<<7)

//! Include branch advertising interval configuration
//!
//! Adds the *--adv-int* switch.
#define YOGI_CLO_BRANCH_ADV_INT (1<<8)

//! Include branch timeout configuration
//!
//! Adds the *--timeout* switch.
#define YOGI_CLO_BRANCH_TIMEOUT (1<<9)

//! Parse configuration files given on the command line
//!
//! The files will be parsed from left to right, i.e. if the same value is set
//! in two supplied configuration files, then the value from the rightmost file
//! will be used. However, values given directly on the command line, i.e. not
//! through files, have higher priority.
#define YOGI_CLO_FILES (1<<10)

//! Same as YOGI_CLO_FILES but at least one configuration must be given
#define YOGI_CLO_FILES_REQUIRED (1<<11)

//! Allow overriding arbitrary configuration sections
//!
//! Adds the *--override* switch.
//!
//! This is useful for supplying arbitrary parameters on the command line
//! without having to store them in a file.
//!
//! Note: Parameters supplied in this way override the same parameters in any
//!       given configuration file. If the same parameter is set directly on
//!       the command line multiple times, then the rightmost value is used.
#define YOGI_CLO_OVERRIDES (1<<12)

//! Allow setting variables via a dedicated switch
//!
//! Adds the *--var* switch.
#define YOGI_CLO_VARIABLES (1<<13)

//! Combination of all branch flags
#define YOGI_CLO_BRANCH_ALL ( YOGI_CLO_BRANCH_NAME        \
                            | YOGI_CLO_BRANCH_DESCRIPTION \
                            | YOGI_CLO_BRANCH_NETWORK     \
                            | YOGI_CLO_BRANCH_PASSWORD    \
                            | YOGI_CLO_BRANCH_PATH        \
                            | YOGI_CLO_BRANCH_ADV_ADDR    \
                            | YOGI_CLO_BRANCH_ADV_PORT    \
                            | YOGI_CLO_BRANCH_ADV_INT     \
                            | YOGI_CLO_BRANCH_TIMEOUT     \
                            )

//! Combination of all flags
//!
//! This is usually used when using the application object.
#define YOGI_CLO_ALL ( YOGI_CLO_LOGGING        \
                     | YOGI_CLO_BRANCH_ALL     \
                     | YOGI_CLO_FILES          \
                     | YOGI_CLO_FILES_REQUIRED \
                     | YOGI_CLO_OVERRIDES      \
                     | YOGI_CLO_VARIABLES      \
                     )

//! @}
//!
//! @defgroup BEV Branch Events
//!
//! Definitions of various events that can be observed on a branch.
//!
//! @{

//! No event (passed to event handler function if wait operation failed)
#define YOGI_BEV_NONE 0

//! A new branch has been discovered
//!
//! A remote branch is considered to be a _new branch_ if it is neither already
//! connected nor in the process of being connected to. This means if we
//! discover a branch and connect to it but connecting to it fails and the
//! remote branch itself has not connected to us, then the next time an
//! we receive an advertisement message we consider the branch new again.
//!
//! Associated event information:
//!
//!   {
//!     "uuid":               "123e4567-e89b-12d3-a456-426655440000",
//!     "tcp_server_address": "fe80::f086:b106:2c1b:c45",
//!     "tcp_server_port":    43384
//!   }
#define YOGI_BEV_BRANCH_DISCOVERED (1<<0)

//! Querying a new branch for information finished (successfully or not)
//!
//! Associated event information:
//!
//!   {
//!     "uuid":                 "123e4567-e89b-12d3-a456-426655440000",
//!     "name":                 "Pump Safety Logic",
//!     "description":          "Monitors the pump for safety",
//!     "net_name":             "Hardware Control",
//!     "path":                 "/Cooling System/Pump/Safety",
//!     "hostname":             "beaglebone",
//!     "pid":                  3321,
//!     "tcp_server_address":   "fe80::f086:b106:2c1b:c45",
//!     "tcp_server_port":      43384,
//!     "start_time":           "2018-04-23T18:25:43.511Z",
//!     "timeout":              3.0,
//!     "advertising_interval": 1.0
//!   }
#define YOGI_BEV_BRANCH_QUERIED (1<<1)

//! Connecting to a branch finished (successfully or not)
//!
//! Associated event information:
//!
//!   {
//!     "uuid": "123e4567-e89b-12d3-a456-426655440000"
//!   }
#define YOGI_BEV_CONNECT_FINISHED (1<<2)

//! The connection to a branch was lost
//!
//! Associated event information:
//!
//!   {
//!     "uuid": "123e4567-e89b-12d3-a456-426655440000"
//!   }
#define YOGI_BEV_CONNECTION_LOST (1<<3)

//! All branch events
#define YOGI_BEV_ALL ( YOGI_BEV_BRANCH_DISCOVERED \
                     | YOGI_BEV_BRANCH_QUERIED    \
                     | YOGI_BEV_CONNECT_FINISHED  \
                     | YOGI_BEV_CONNECTION_LOST   \
                     )

//! @}

#ifndef YOGI_API
# ifdef _MSC_VER
#   define YOGI_API __declspec(dllimport)
# else
#   define YOGI_API
# endif
#endif

//! @defgroup FN Library Functions
//!
//! Description of the various library functions
//!
//! @{

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * Get the version of the loaded library.
 *
 * The returned string is human-reable and in the same format as the
 * #YOGI_HDR_VERSION macro. Note that those two strings do not necessarily have
 * to match since the final executable can load a library with a version number
 * different from the version number in the used header file.
 *
 * \returns Library version
 ******************************************************************************/
YOGI_API const char* YOGI_GetVersion();

/***************************************************************************//**
 * Get the license that Yogi is published under.
 *
 * Note: The returned string contains the complete description of the license
 *       and is therefore very large.
 *
 * \returns License information
 ******************************************************************************/
YOGI_API const char* YOGI_GetLicense();

/***************************************************************************//**
 * Get the license information about the 3rd party libraries used in Yogi.
 *
 * Note: The returned string is very large.
 *
 * \returns License information
 ******************************************************************************/
YOGI_API const char* YOGI_Get3rdPartyLicenses();

/***************************************************************************//**
 * Get a description of an error code.
 *
 * Returns a human-readable string describing the given error code. The returned
 * string is always a valid, null-terminated and human-readable string, even if
 * the supplied error code is not valid.
 *
 * \param[in] err Error code (see \ref EC)
 *
 * \returns Description of the error code
 ******************************************************************************/
YOGI_API const char* YOGI_GetErrorString(int err);

/***************************************************************************//**
 * Get the value of a constant (see \ref CV).
 *
 * Depending on the type of constant, which can either be an integer number or a
 * null-terminated string (see \ref CV), the target of the \p dest parameter
 * will be set accordingly: For integer-type constants, \p dest will be treated
 * as a pointer to an int and its target will be set to the value of the
 * constant; for string-type constants, \p dest will be treated as a pointer to
 * a char* string and its target will be set to the address of the string
 * constant.
 *
 * \param[out] dest     Pointer to where the value will be written to
 * \param[in]  constant The constant to retrieve (see \ref CV)
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_GetConstant(void* dest, int constant);

/***************************************************************************//**
 * Get the current time.
 *
 * \param[out] timestamp Current time in nanoseconds since 01/01/1970 UTC.
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_GetCurrentTime(long long* timestamp);

/***************************************************************************//**
 * Converts a timestamp into a string.
 *
 * The \p timefmt parameter describes the format of the conversion. The
 * following placeholders are supported:
 *  - *%Y*: Four digit year
 *  - *%m*: Month name as a decimal 01 to 12
 *  - *%d*: Day of the month as decimal 01 to 31
 *  - *%F*: Equivalent to %Y-%m-%d (the ISO 8601 date format)
 *  - *%H*: The hour as a decimal number using a 24-hour clock (range 00 to 23)
 *  - *%M*: The minute as a decimal 00 to 59
 *  - *%S*: Seconds as a decimal 00 to 59
 *  - *%T*: Equivalent to %H:%M:%S (the ISO 8601 time format)
 *  - *%3*: Milliseconds as decimal number 000 to 999
 *  - *%6*: Microseconds as decimal number 000 to 999
 *  - *%9*: Nanoseconds as decimal number 000 to 999
 *
 * If \p timefmt is set to NULL, then the timestamp will be formatted in the
 * format "2009-02-11T12:53:09.123Z".
 *
 * \param[in]  timestamp Timestamp in nanoseconds since 01/01/1970 UTC
 * \param[out] str       Pointer to a string for storing the result
 * \param[in]  strsize   Maximum number of bytes to write to \p str
 * \param[in]  timefmt   Format of the time string (set to NULL for default)
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_FormatTime(long long timestamp, char* str, int strsize,
                             const char* timefmt);

/***************************************************************************//**
 * Converts a string into a timestamp.
 *
 * The \p timefmt parameter describes the format of the conversion. The
 * following placeholders are supported:
 *  - *%Y*: Four digit year
 *  - *%m*: Month name as a decimal 01 to 12
 *  - *%d*: Day of the month as decimal 01 to 31
 *  - *%F*: Equivalent to %Y-%m-%d (the ISO 8601 date format)
 *  - *%H*: The hour as a decimal number using a 24-hour clock (range 00 to 23)
 *  - *%M*: The minute as a decimal 00 to 59
 *  - *%S*: Seconds as a decimal 00 to 59
 *  - *%T*: Equivalent to %H:%M:%S (the ISO 8601 time format)
 *  - *%3*: Milliseconds as decimal number 000 to 999
 *  - *%6*: Microseconds as decimal number 000 to 999
 *  - *%9*: Nanoseconds as decimal number 000 to 999
 *
 * If \p timefmt is set to NULL, then the timestamp will be parsed in the
 * format "2009-02-11T12:53:09.123Z".
 *
 * \param[out] timestamp Resulting in nanoseconds since 01/01/1970 UTC
 * \param[in]  str       String to parse
 * \param[in]  timefmt   Format of the time string (set to NULL for default)
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_ParseTime(long long* timestamp, const char* str,
                            const char* timefmt);

/***************************************************************************//**
 * Converts a duration into a string.
 *
 * The \p durfmt parameter describes the format of the conversion. The
 * following placeholders are supported:
 *  - *%+*: Plus sign if duration is positive and minus sign if it is negative
 *  - *%-*: Minus sign (only) if duration is negative
 *  - *%d*: Total number of days
 *  - *%D*: Total number of days if days > 0
 *  - *%H*: Fractional hours (range 00 to 23)
 *  - *%M*: Fractional minutes (range 00 to 59)
 *  - *%S*: Fractional seconds (range 00 to 59)
 *  - *%T*: Equivalent to %H:%M:%S
 *  - *%3*: Fractional milliseconds (range 000 to 999)
 *  - *%6*: Fractional microseconds (range 000 to 999)
 *  - *%9*: Fractional nanoseconds (range 000 to 999)
 *
 * The \p inffmt parameter describes the format to use for infinite durations.
 * The following placeholders are supported:
 *  - *%+*: Plus sign if duration is positive and minus sign if it is negative
 *  - *%-*: Minus sign (only) if duration is negative
 *
 * If \p durfmt is set to NULL, then the duration will be formatted in the
 * format "-23d 04:19:33.123456789". If \p dur is -1 to indicate an infinite
 * duration, then \p infstr will be copied to \p str. If \p infstr is set to
 * NULL, then the format string "%-inf" will be used.
 *
 * \param[in]  dur     Duration in nanoseconds (-1 for infinity or >= 0)
 * \param[in]  neg     Duration is negative (#YOGI_TRUE or #YOGI_FALSE)
 * \param[out] str     Pointer to a string for storing the result
 * \param[in]  strsize Maximum number of bytes to write to \p str
 * \param[in]  durfmt  Format of the duration string (set to NULL for default)
 * \param[in]  inffmt  Format to use for infinity (set to NULL for default)
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_FormatDuration(long long dur, int neg, char* str, int strsize,
                                 const char* durfmt, const char* inffmt);

/***************************************************************************//**
 * Creates a string describing an object.
 *
 * The \p objfmt parameter describes the format of the string. The following
 * placeholders are supported:
 *  - *$T*: Type of the object (e.g. Branch)
 *  - *$x*: Handle of the object in lower-case hex notation
 *  - *$X*: Handle of the object in upper-case hex notation
 *
 * If \p objfmt is set to NULL, then the object will be formatted in the format
 * "Branch [44fdde]" with the hex value in brackets being the object's handle,
 * i.e. the address of the \p obj pointer. If \p obj is NULL then \p nullstr
 * will be copied to \p str. If \p nullstr is set to NULL, then the string
 * "INVALID HANDLE" will be used.
 *
 * \param[in]  obj     Handle of the object to print
 * \param[out] str     Pointer to a string for storing the result
 * \param[in]  strsize Maximum number of bytes to write to \p str
 * \param[in]  objfmt  Format of the string (set to NULL for default)
 * \param[in]  nullstr String to use if \p obj is NULL
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_FormatObject(void* obj, char* str, int strsize,
                               const char* objfmt, const char* nullstr);

/***************************************************************************//**
 * Allows the Yogi to write library-internal and user logging to stdout or
 * stderr.
 *
 * This function supports colourizing the output if the terminal that the
 * process is running in supports it. The color used for a log entry depends on
 * the entry's severity. For example, errors will be printed in red and warnings
 * in yellow.
 *
 * Writing to the console can be disabled by setting \p verbosity to
 * #YOGI_VB_NONE.
 *
 * Each log entry contains the *component* tag which describes which part of a
 * program issued the log entry. For entries created by the library itself, this
 * parameter is prefixed with the string "Yogi.", followed by the internal
 * component name. For example, the component tag for a branch would be
 * "Yogi.Branch".
 *
 * The \p timefmt parameter describes the textual format of a log entry's
 * timestamp. The following placeholders are supported:
 *  - *%Y*: Four digit year
 *  - *%m*: Month name as a decimal 01 to 12
 *  - *%d*: Day of the month as decimal 01 to 31
 *  - *%F*: Equivalent to %Y-%m-%d (the ISO 8601 date format)
 *  - *%H*: The hour as a decimal number using a 24-hour clock (range 00 to 23)
 *  - *%M*: The minute as a decimal 00 to 59
 *  - *%S*: Seconds as a decimal 00 to 59
 *  - *%T*: Equivalent to %H:%M:%S (the ISO 8601 time format)
 *  - *%3*: Milliseconds as decimal number 000 to 999
 *  - *%6*: Microseconds as decimal number 000 to 999
 *  - *%9*: Nanoseconds as decimal number 000 to 999
 *
 * The \p fmt parameter describes the textual format of the complete log entry
 * as it will appear on the console. The supported placeholders are:
 *  - *$t*: Timestamp, formatted according to \p timefmt
 *  - *$P*: Process ID (PID)
 *  - *$T*: Thread ID
 *  - *$s*: Severity as a 3 letter abbreviation (FAT, ERR, WRN, IFO, DBG or TRC)
 *  - *$m*: Log message
 *  - *$f*: Source filename
 *  - *$l*: Source line number
 *  - *$c*: Component tag
 *  - *$<*: Set console color corresponding to severity
 *  - *$>*: Reset the colours (also done after each log entry)
 *  - *$$*: A $ sign
 *
 * \param[in] verbosity Maximum verbosity of messages to log
 * \param[in] stream    The stream to use (#YOGI_ST_STDOUT or #YOGI_ST_STDERR)
 * \param[in] color     Use colours in output (#YOGI_TRUE or #YOGI_FALSE)
 * \param[in] timefmt   Format of the timestamp (set to NULL for default)
 * \param[in] fmt       Format of a log entry (set to NULL for default)
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_LogToConsole(int verbosity, int stream, int color,
                               const char* timefmt, const char* fmt);

/***************************************************************************//**
 * Installs a callback function for receiving log entries.
 *
 * This function can be used to get notified whenever the Yogi library itself or
 * the user produces log messages. These messages can then be processed further
 * in user code.
 *
 * Only one callback function can be registered. Calling LogToHook()
 * again will override the previous function. Setting \p fn to NULL or
 * \p verbosity to #YOGI_VB_NONE will disable the hook.
 *
 * Note: The library will call \p fn from only one thread at a time, i.e. \p fn
 *       does not have to be thread-safe.
 *
 * The parameters passed to \p fn are:
 *  -# *severity*: Severity (verbosity) of the entry (see \ref VB)
 *  -# *timestamp*: Timestamp of the entry in nanoseconds since 01/01/1970 UTC
 *  -# *tid*: ID of the thread that created the entry
 *  -# *file*: Source file name
 *  -# *line*: Source file line number
 *  -# *comp*: Component that created the entry
 *  -# *msg*: Log message
 *  -# *userarg*: Value of \p userarg
 *
 * Note: The two string arguments *comp* and *msg* of \p fn are valid only while
 *       \p fn is being executed. Do not access those variables at a later time!
 *
 * \param[in] verbosity Maximum verbosity of messages to call \p fn for
 * \param[in] fn        Callback function
 * \param[in] userarg   User-specified argument to be passed to \p fn
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_LogToHook(int verbosity,
                            void (*fn)(int severity, long long timestamp,
                                       int tid, const char* file, int line,
                                       const char* comp, const char* msg,
                                       void* userarg),
                            void* userarg);

/***************************************************************************//**
 * Creates log file.
 *
 * This function opens a file to write library-internal and user logging
 * information to. If the file with the given filename already exists then it
 * will be overwritten.
 *
 * Writing to a log file can be disabled by setting \p filename to NULL or
 * \p verbosity to #YOGI_VB_NONE.
 *
 * The \p timefmt and \p fmt parameters describe the textual format for a log
 * entry. The \p filename parameter supports all placeholders that are valid
 * for \p timefmt See the YOGI_LogToConsole() function for supported
 * placeholders.
 *
 * Note: The color-related placeholders are ignored when writing to log files.
 *
 * The \p genfn parameter can be used to obtain the filename generated by
 * replacing the placeholders in the \p filename parameter. If \p genfnsize
 * is too small, the function returns YOGI_ERR_BUFFER_TOO_SMALL, however, the
 * log file will still be created and \p genfn will be populated as much as
 * possible.
 *
 * \param[in]  verbosity Maximum verbosity of messages to log to stderr
 * \param[in]  filename  Path to the log file (see description for placeholders)
 * \param[out] genfn     Pointer to a string for storing the generated filename
 *                       (can be set to NULL)
 * \param[in]  genfnsize Maximum number of bytes to write to \p genfn
 * \param[in]  timefmt   Format of the timestamp (set to NULL for default)
 * \param[in]  fmt       Format of a log entry (set to NULL for default)
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_LogToFile(int verbosity, const char* filename, char* genfn,
                            int genfnsize, const char* timefmt,
                            const char* fmt);

/***************************************************************************//**
 * Creates a logger.
 *
 * A logger is an object used for generating log entries that are tagged with
 * the logger's component tag. A logger's component tag does not have to be
 * unique, i.e. multiple loggers can be created using identical \p component
 * strings.
 *
 * The verbosity of new loggers is #YOGI_VB_INFO by default.
 *
 * \param[out] logger    Pointer to the logger handle
 * \param[in]  component The component tag to use
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_LoggerCreate(void** logger, const char* component);

/***************************************************************************//**
 * Gets the verbosity of a particular logger.
 *
 * The verbosity of a logger acts as a filter. Only messages with a verbosity
 * less than or equal to the given value are being logged.
 *
 * Note: The verbosity of a logger affects only messages logged through that
 *       particular logger, i.e. if two loggers have identical component tags
 *       their verbosity settings are still independent from each other.
 *
 * \param[in]  logger    Logger handle (set to NULL for the App logger)
 * \param[out] verbosity Pointer to where the verbosity level shall be written
 *                       to (see \ref VB)
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_LoggerGetVerbosity(void* logger, int* verbosity);

/***************************************************************************//**
 * Sets the verbosity of a particular logger.
 *
 * The verbosity of a logger acts as a filter. Only messages with a verbosity
 * less than or equal to the given value are being logged.
 *
 * Note: The verbosity of a logger affects only messages logged through that
 *       particular logger, i.e. if two loggers have identical component tags
 *       their verbosity settings are still independent from each other.
 *
 * \param[in] logger    Logger handle (set to NULL for the App logger)
 * \param[in] verbosity Maximum verbosity entries to be logged (see \ref VB)
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_LoggerSetVerbosity(void* logger, int verbosity);

/***************************************************************************//**
 * Sets the verbosity of all loggers matching a given component tag.
 *
 * This function finds all loggers whose component tag matches the regular
 * expression given in the \p components parameter and sets their verbosity
 * to \p verbosity.
 *
 * \param[in]  components Regex (ECMAScript) for the component tags to match
 * \param[in]  verbosity  Maximum verbosity entries to be logged (see \ref VB)
 * \param[out] count      Number of matching loggers (can be set to NULL)
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_LoggerSetComponentsVerbosity(const char* components,
                                               int verbosity, int* count);

/***************************************************************************//**
 * Creates a log entry.
 *
 * The entry can be generated using a specific logger or, by setting \p logger
 * to NULL, the App logger will be used. The App logger is always present and
 * uses the string "App" as the component tag.
 *
 * \param[in] logger   The logger to use (set to NULL for the App logger)
 * \param[in] severity Severity (verbosity) of the entry (see \ref VB)
 * \param[in] file     Source file name (can be set to NULL)
 * \param[in] line     Source file line number (can be set to 0)
 * \param[in] msg      Log message
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_LoggerLog(void* logger, int severity, const char* file,
                            int line, const char* msg);

/***************************************************************************//**
 * Creates a configuration.
 *
 * A configuration represents a set of parameters that usually remain constant
 * throughout the runtime of a program. Parameters can come from different
 * sources such as the command line or a file. Configurations are used for
 * other parts of the library such as application objects, however, they are
 * also intended to store user-defined parameters.
 *
 * The \p flags parameter is used to change the behaviour of a configuration
 * object in certain key areas (see \ref CFG).
 *
 * \param[out] config Pointer to the configuration handle
 * \param[in]  flags  See \ref CFG for possible behaviour adjustments
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_ConfigurationCreate(void** config, int flags);

/***************************************************************************//**
 * Updates a configuration from command line options.
 *
 * The function populates \p err with:
 * - a description of the error if the function returns one of
 *   YOGI_ERR_PARSING_CMDLINE_FAILED, YOGI_ERR_PARSING_FILE_FAILED,
 *   YOGI_ERR_PARSING_JSON_FAILED or YOGI_ERR_CONFIG_NOT_VALID; and
 * - the help/usage text if the function returns YOGI_ERR_HELP_REQUESTED; and
 * - an empty, null-terminated string otherwise.
 *
 * If the string to be written to \p err is larger than \p errsize then \p err
 * will be populated as much as possible (and null-terminated). However, as
 * opposed to other API functions, the functions returns the original error.
 *
 * \param[in]  config  The configuration to update
 * \param[in]  argc    Number strings in \p argv
 * \param[in]  argv    Strings given on the command line
 * \param[in]  options Options to provide on the command line (see \ref CLO)
 * \param[out] err     Pointer to a char array for storing an error description
 *                     (can be set to NULL)
 * \param[in]  errsize Maximum number of bytes to write to \p err
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_ConfigurationUpdateFromCommandLine(void* config, int argc,
                                                     const char* const* argv,
                                                     int options, char* err,
                                                     int errsize);

/***************************************************************************//**
 * Updates a configuration from a JSON-formatted string.
 *
 * The function populates \p err with:
 * - a description of the error if the function returns one of
 *   YOGI_ERR_PARSING_JSON_FAILED or YOGI_ERR_CONFIG_NOT_VALID; and
 * - an empty, null-terminated string otherwise.
 *
 * If the string to be written to \p err is larger than \p errsize then \p err
 * will be populated as much as possible (and null-terminated). However, as
 * opposed to other API functions, the functions returns the original error.
 *
 * \param[in]  config  The configuration to update
 * \param[in]  json    Null-terminated, JSON-formatted string
 * \param[out] err     Pointer to a char array for storing an error description
 *                     (can be set to NULL)
 * \param[in]  errsize Maximum number of bytes to write to \p err
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_ConfigurationUpdateFromJson(void* config, const char* json,
                                              char* err, int errsize);

/***************************************************************************//**
 * Updates a configuration from a JSON file.
 *
 * The function populates \p err with:
 * - a description of the error if the function returns one of
 *   YOGI_ERR_PARSING_FILE_FAILED or YOGI_ERR_CONFIG_NOT_VALID; and
 * - an empty, null-terminated string otherwise.
 *
 * If the string to be written to \p err is larger than \p errsize then \p err
 * will be populated as much as possible (and null-terminated). However, as
 * opposed to other API functions, the functions returns the original error.
 *
 * \param[in]  config   The configuration to update
 * \param[in]  filename Path to the JSON file
 * \param[out] err      Pointer to a char array for storing an error description
 *                      (can be set to NULL)
 * \param[in]  errsize  Maximum number of bytes to write to \p err
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_ConfigurationUpdateFromFile(void* config,
                                              const char* filename, char* err,
                                              int errsize);

/***************************************************************************//**
 * Retrieves a configuration as a JSON-formatted string.
 *
 * If the configuration is larger than the space provided by \p json and
 * \p jsonsize then this function returns YOGI_ERR_BUFFER_TOO_SMALL, however,
 * \p str will still be populated as much as possible (and null-terminated). In
 * this case, \p json will *not* contain valid JSON data.
 *
 * \param[in]  config   The configuration
 * \param[out] json     Pointer to a string for storing the configuration
 * \param[in]  jsonsize Maximum number of bytes to write to \p json
 * \param[in]  resvars  Set to YOGI_TRUE to resolve any variables before writing
 *                      the configuration to \p json and YOGI_FALSE otherwise
 * \param[in]  indent   Indentation size (number of space characters to use);
 *                      -1 omits new lines as well
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_ConfigurationDump(void* config, char* json, int jsonsize,
                                    int resvars, int indent);

/***************************************************************************//**
 * Writes a configuration to a file in JSON format.
 *
 * This is useful for debugging purposes.
 *
 * \param[in] config   The configuration
 * \param[in] filename Path to the output file
 * \param[in] resvars  Set to YOGI_TRUE to resolve any variables before writing
 *                     the configuration to the file and YOGI_FALSE otherwise
 * \param[in] indent   Indentation size (number of space characters to use);
 *                     -1 omits new lines as well
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_ConfigurationWriteToFile(void* config, const char* filename,
                                           int resvars, int indent);

/***************************************************************************//**
 * Creates a context for the execution of asynchronous operations.
 *
 * \param[out] context Pointer to the context handle
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_ContextCreate(void** context);

/***************************************************************************//**
 * Runs the context's event processing loop to execute ready handlers.
 *
 * This function runs handlers (internal and user-supplied such as functions
 * registered through YOGI_ContextPost()) that are ready to run, without
 * blocking, until the YOGI_ContextStop() function has been called or there
 * are no more ready handlers.
 *
 * This function must be called from outside any handler functions that are being
 * executed through the context.
 *
 * \param[in]  context The context to use
 * \param[out] count   Number of executed handlers (may be set to NULL)
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_ContextPoll(void* context, int* count);

/***************************************************************************//**
 * Runs the context's event processing loop to execute at most one ready
 * handler.
 *
 * This function runs at most one handler (internal and user-supplied such as
 * functions registered through YOGI_ContextPost()) that is ready to run,
 * without blocking.
 *
 * This function must be called from outside any handler functions that are being
 * executed through the context.
 *
 * \param[in]  context The context to use
 * \param[out] count   Number of executed handlers (may be set to NULL)
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_ContextPollOne(void* context, int* count);

/***************************************************************************//**
 * Runs the context's event processing loop for the specified duration.
 *
 * This function blocks while running the context's event processing loop and
 * calling dispatched handlers (internal and user-supplied such as functions
 * registered through YOGI_ContextPost()) for the specified duration unless
 * YOGI_ContextStop() is called within that time.
 *
 * This function must be called from outside any handler functions that are being
 * executed through the context.
 *
 * \param[in]  context  The context to use
 * \param[out] count    Number of executed handlers (may be set to NULL)
 * \param[in]  duration Duration in nanoseconds (-1 for infinity)
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_ContextRun(void* context, int* count, long long duration);

/***************************************************************************//**
 * Runs the context's event processing loop for the specified duration to
 * execute at most one handler.
 *
 * This function blocks while running the context's event processing loop and
 * calling dispatched handlers (internal and user-supplied such as functions
 * registered through YOGI_ContextPost()) for the specified duration until a
 * single handler function has been executed, unless YOGI_ContextStop() is
 * called within that time.
 *
 * This function must be called from outside any handler functions that are being
 * executed through the context.
 *
 * \param[in]  context  The context to use
 * \param[out] count    Number of executed handlers (may be set to NULL)
 * \param[in]  duration Duration in nanoseconds (-1 for infinity)
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_ContextRunOne(void* context, int* count, long long duration);

/***************************************************************************//**
 * Starts an internal thread for running the context's event processing loop.
 *
 * This function starts a threads that runs the context's event processing loop
 * in the background. It relieves the user from having to start a thread and
 * calling the appropriate YOGI_ContextRun... or YOGI_ContextPoll... functions
 * themself. The thread can be stopped using YOGI_ContextStop().
 *
 * This function must be called from outside any handler functions that are being
 * executed through the context.
 *
 * \param[in] context The context to use
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_ContextRunInBackground(void* context);

/***************************************************************************//**
 * Stops the context's event processing loop.
 *
 * This function signals the context to stop running its event processing loop.
 * This causes YOGI_ContextRun... functions to return as soon as possible and it
 * terminates the thread started via YOGI_ContextRunInBackground().
 *
 * \param[in] context The context to use
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_ContextStop(void* context);

/***************************************************************************//**
 * Blocks until the context's event processing loop is being run or until the
 * specified timeout is reached.
 *
 * If the \p duration parameter is set to 0 then the function works in polling
 * mode.
 *
 * If the event processing loop has not been running after the specified
 * timeout, then the YOGI_ERR_TIMEOUT error is returned. This also applies when
 * this function is used in polling mode as described above.
 *
 * This function must be called from outside any handler functions that are being
 * executed through the context.
 *
 * \param[in] context  The context to use
 * \param[in] duration Maximum time to wait in nanoseconds (-1 for infinity)
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_ContextWaitForRunning(void* context, long long duration);

/***************************************************************************//**
 * Blocks until no thread is running the context's event processing loop or
 * until the specified timeout is reached.
 *
 * If the \p duration parameter is set to 0 then the function works in polling
 * mode.
 *
 * If a thread is still running the event processing loop after the specified
 * timeout, then the YOGI_ERR_TIMEOUT error is returned. This also applies when
 * this function is used in polling mode as described above.
 *
 * This function must be called from outside any handler functions that are being
 * executed through the context.
 *
 * \param[in] context  The context to use
 * \param[in] duration Maximum time to wait in nanoseconds (-1 for infinity)
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_ContextWaitForStopped(void* context, long long duration);

/***************************************************************************//**
 * Adds the given function to the context's event processing queue to be
 * executed and returns immediately.
 *
 * The handler \p fn will only be executed after this function returns and only
 * by a thread running the context's event processing loop. The only parameter
 * for \p fn will be set to the value of the \p userarg parameter.
 *
 * \param[in] context The context to use
 * \param[in] fn      The function to call from within the given context
 * \param[in] userarg User-specified argument to be passed to \p fn
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_ContextPost(void* context, void (*fn)(void* userarg),
                              void* userarg);

/***************************************************************************//**
 * Raises a signal.
 *
 * Signals in Yogi are intended to be used similar to POSIX signals. They have
 * to be raised explicitly by the user (e.g. when receiving an actual POSIX
 * signal like SIGINT) via this function. A signal will be received by all
 * signal sets containing that signal.
 *
 * The \p sigarg parameter can be used to deliver user-defined data to the
 * signal handlers. The cleanup handler \p fn will be called once all signal
 * handlers have been called; it is intended to be used for memory management
 * purposes. Once \p fn has been called, \p sigarg is not used any more and
 * can be destroyed.
 *
 * Note: The cleanup handler \p fn can get called either from within the
 *       YOGI_RaiseSignal() function or from any context within the program.
 *
 * \param[in] signal  The signal to raise (see \ref SIG)
 * \param[in] sigarg  User-defined data to pass to the signal handlers
 * \param[in] fn      Function to be called once all signal handlers have been
 *                    executed (can be NULL)
 * \param[in] userarg User-specified argument to be passed to \p fn
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_RaiseSignal(int signal, void* sigarg,
                              void (*fn)(void* sigarg, void* userarg),
                              void* userarg);

/***************************************************************************//**
 * Creates a new signal set.
 *
 * Signal sets are used to receive signals raised via YOGI_RaiseSignal().
 * The signals are queued until they can be delived by means of calls to
 * YOGI_SignalSetAwaitSignal().
 *
 * \param[out] sigset  Pointer to the signal set handle
 * \param[in]  context The context to use
 * \param[in]  signals The signals to listen for
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_SignalSetCreate(void** sigset, void* context, int signals);

/***************************************************************************//**
 * Waits for a signal to be raised.
 *
 * The handler \p fn will be called after one of the signals in the signal set
 * is caught. The parameters passed to \p fn are:
 * -# *res*: YOGI_OK or error code in case of a failure (see \ref EC)
 * -# *sig*: The caught signal (see \ref SIG)
 * -# *sigarg*: User-defined parameter passed to YOGI_RaiseSignal()
 * -# *userarg*: Value of the user-specified \p userarg parameter
 *
 * Note: Calling this function on the same context again before the signal has
 *       been caught will cause the previously registered handler function to
 *       be called with the the YOGI_ERR_CANCELED error.
 *
 * \param[in] sigset  The signal set
 * \param[in] fn      The function to call
 * \param[in] userarg User-specified argument to be passed to \p fn
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_SignalSetAwaitSignal(void* sigset,
                                       void (*fn)(int res, int sig,
                                                  void* sigarg, void* userarg),
                                       void* userarg);

/***************************************************************************//**
 * Cancels waiting for a signal.
 *
 * Causes the handler function registered via YOGI_SignalSetAwaitSignal() to be
 * called with YOGI_ERR_CANCELED.
 *
 * \param[in] sigset The signal set
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_SignalSetCancelAwaitSignal(void* sigset);

/***************************************************************************//**
 * Creates a new timer.
 *
 * \param[out] timer   Pointer to the timer handle
 * \param[in]  context The context to use
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_TimerCreate(void** timer, void* context);

/***************************************************************************//**
 * Starts the given timer.
 *
 * If the timer is already running, the timer will be canceled first, as if
 * YOGI_TimerCancel() were called explicitly.
 *
 * The parameters of the handler function \p fn are:
 *  -# *res*: YOGI_OK or error code in case of a failure (see \ref EC)
 *  -# *userarg*: Value of the user-specified \p userarg parameter
 *
 * \param[in] timer    The timer to start
 * \param[in] duration Duration in nanoseconds (-1 for infinity)
 * \param[in] fn       The function to call after the given time passed
 * \param[in] userarg  User-specified argument to be passed to \p fn
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_TimerStart(void* timer, long long duration,
                             void (*fn)(int res, void* userarg), void* userarg);

/***************************************************************************//**
 * Cancels the given timer.
 *
 * Canceling a timer will result in the handler function registered via
 * YOGI_TimerStart() to be called with the YOGI_ERR_CANCELED error as first
 * parameter. Note that if the handler is already scheduled for executing, it
 * will be called with YOGI_OK instead.
 *
 * If the timer has not been started or it already expired, this function will
 * return YOGI_ERR_TIMER_EXPIRED.
 *
 * \param[in] timer The timer to cancel
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_TimerCancel(void* timer);

/***************************************************************************//**
 * Creates a new branch.
 *
 * A branch represents an entry point into a Yogi network. It advertises itself
 * via IP broadcasts/multicasts with its unique ID and information required for
 * establishing a connection. If a branch detects other branches on the network,
 * it connects to them via TCP to retrieve further information such as their
 * name, description and network name. If the network names match, two branches
 * attempt to authenticate with each other by securely comparing passwords.
 * Once authentication succeeds and there is no other known branch with the same
 * path then the branches can actively communicate as part of the Yogi network.
 *
 * Advertising and establishing connections can be limited to certain network
 * interfaces via the \p interface parameter. The default is to use all
 * available interfaces.
 *
 * The \p advint parameter can be set to -1 which prevents the branch from
 * actively participating in the Yogi network, i.e. the branch will not
 * advertise itself and it will not authenticate in order to join a network.
 * However, the branch will temporarily connect to other branches in order to
 * obtain more detailed information such as name, description, network name
 * and so on. This is useful for obtaining information about active branches
 * without actually becoming part of the Yogi network.
 *
 * Note: Even though the authentication process via passwords is done in a
 *       secure manner, any further communication is done in plain text.
 *
 * \param[out] branch      Pointer to the branch handle
 * \param[in]  context     The context to use
 * \param[in]  name        Name of the branch (set to NULL to use the format
 *                         PID@hostname with PID being the process ID)
 * \param[in]  description Description of the branch (set to NULL for none)
 * \param[in]  netname     Name of the network to join (set to NULL to use the
 *                         machine's hostname)
 * \param[in]  password    Password for the network (set to NULL for none)
 * \param[in]  path        Path of the branch in the network (set to NULL to use
 *                         the format /name where name is the branch's name)
 * \param[in]  advaddr     Multicast address to use; e.g. 239.255.0.1 for IPv4
 *                         or ff31::8000:1234 for IPv6 (set to NULL for default)
 * \param[in]  advport     Advertising port (set to 0 for default)
 * \param[in]  advint      Advertising interval in nanoseconds (set to 0 for
 *                         default; set to -1 for no advertising and no joining
 *                         networks; must be at least 1 millisecond)
 * \param[in]  timeout     Maximum time of inactivity before a remote branch is
 *                         considered to be dead (set to 0 for default; set to
 *                         -1 for infinity; must be at least 1 millisecond)
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_BranchCreate(void** branch, void* context, const char* name,
                               const char* description, const char* netname,
                               const char* password, const char* path,
                               const char* advaddr, int advport,
                               long long advint, long long timeout);

/***************************************************************************//**
 * Retrieves information about a local branch.
 *
 * This function writes the branch's UUID (16 bytes) in binary form to \p uuid.
 * Any further information is written to \p json in JSON format. The function
 * call fails with the YOGI_ERR_BUFFER_TOO_SMALL error if the produced JSON
 * string does not fit into \p json, i.e. if \p jsonsize is too small. However,
 * in that case, the first \p jsonsize - 1 characters and a trailing zero will
 * be written to \p json.
 *
 * The produced JSON string is as follows, without any unnecessary whitespace:
 *
 *    {
 *      "uuid":                 "6ba7b810-9dad-11d1-80b4-00c04fd430c8",
 *      "name":                 "Fan Controller",
 *      "description":          "Controls a fan via PWM",
 *      "net_name":             "Hardware Control",
 *      "path":                 "/Cooling System/Fan Controller",
 *      "hostname":             "beaglebone",
 *      "pid":                  4124,
 *      "advertising_address":  "ff31::8000:2439",
 *      "advertising_port":     13531,
 *      "advertising_interval": 1.0,
 *      "tcp_server_address":   "::",
 *      "tcp_server_port":      53332,
 *      "start_time":           "2018-04-23T18:25:43.511Z",
 *      "timeout":              3.0
 *    }
 *
 * \param[in]  branch   The branch handle
 * \param[out] uuid     Pointer to 16 byte array for storing the UUID (can be
 *                      set to NULL)
 * \param[out] json     Pointer to a char array for storing the information (can
 *                      be set to NULL)
 * \param[in]  jsonsize Maximum number of bytes to write to \p json
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_BranchGetInfo(void* branch, void* uuid, char* json,
                                int jsonsize);

/***************************************************************************//**
 * Retrieves information about all connected remote branches.
 *
 * For each of the connected remote branches, this function will:
 * -# Write the branch's UUID (16 bytes) in binary form to \p uuid.
 * -# Generate a JSON string containing further information to \p json.
 * -# Execute the handler \p fn with YOGI_OK as first argument if \p jsonsize
 *    is as least as large as the length of the generated JSON string
 *
 * If the produced JSON string for the branch does not fit into \p json, i.e. if
 * \p jsonsize is too small, then \p json will be filled with the first
 * \p jsonsize - 1 characters and a trailing zero and \p fn will be called with
 * the YOGI_ERR_BUFFER_TOO_SMALL error for that particular branch.
 *
 * This function will return YOGI_ERR_BUFFER_TOO_SMALL if \p json is not large
 * enough to hold each one of the JSON strings. However, \p fn will still be
 * called for each discovered branch.
 *
 * The produced JSON string is as follows, without any unnecessary whitespace:
 *
 *    {
 *      "uuid":                 "123e4567-e89b-12d3-a456-426655440000",
 *      "name":                 "Pump Safety Logic",
 *      "description":          "Monitors the pump for safety",
 *      "net_name":             "Hardware Control",
 *      "path":                 "/Cooling System/Pump/Safety",
 *      "hostname":             "beaglebone",
 *      "pid":                  3321,
 *      "tcp_server_address":   "fe80::f086:b106:2c1b:c45",
 *      "tcp_server_port":      43384,
 *      "start_time":           "2018-04-23T18:25:43.511Z",
 *      "timeout":              3.0,
 *      "advertising_interval": 1.0
 *    }
 *
 * \param[in]  branch   The branch handle
 * \param[out] uuid     Pointer to 16 byte array for storing the UUID (can be
 *                      set to NULL)
 * \param[out] json     Pointer to a char array for storing the information (can
 *                      be set to NULL)
 * \param[in]  jsonsize Maximum number of bytes to write to \p json
 * \param[in]  fn       Handler to call for each connected branch
 * \param[in]  userarg  User-specified argument to be passed to \p fn
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_BranchGetConnectedBranches(void* branch, void* uuid,
                                             char* json, int jsonsize,
                                             void (*fn)(int res, void* userarg),
                                             void* userarg);

/***************************************************************************//**
 * Wait for a branch event to occur.
 *
 * This function will register \p fn to be called once one of the given branch
 * \p events occurs on the given \p branch. The parameters passed to \p fn are:
 *  -# *res*: YOGI_OK or error code associated with the wait operation
 *  -# *event*: The branch event that occurred (see \ref BEV)
 *  -# *evres*: YOGI_OK or error code assitiated with the event
 *  -# *userarg*: Value of the user-specified \p userarg parameter
 *
 * If this function is called while a previous wait operation is still active
 * then the previous operation will be canceled, i.e. \p fn for the previous
 * operation will be called with the YOGI_ERR_CANCELED error.
 *
 * The \p uuid parameter will be populated with the UUID of the branch that
 * caused the event, i.e. if the remote branch B gets discovered, causing the
 * YOGI_BEV_BRANCH_DISCOVERED event to be generated, then \p uuid will be
 * populated with B's UUID.
 *
 * The \p json parameter will be populated with a string in JSON format
 * containing additional event information such as branch information See
 * \ref BEV for event-specific details.
 *
 * If the produced JSON string for the branch does not fit into \p json, i.e. if
 * \p jsonsize is too small, then \p json will be filled with the first
 * \p jsonsize - 1 characters and a trailing zero and \p fn will be called with
 * the YOGI_ERR_BUFFER_TOO_SMALL error for that particular branch.
 *
 * Note: Make sure that the two supplied buffers \p uuid and \p json remain
 *       valid until \p fn has been executed.
 *
 * \param[in]  branch   The branch handle
 * \param[in]  events   Events to observe (see \ref BEV)
 * \param[out] uuid     Pointer to 16 byte array for storing the UUID
 *                      (can be set to NULL)
 * \param[out] json     Pointer to a char array for storing event information
 *                      (can be set to NULL)
 * \param[in]  jsonsize Maximum number of bytes to write to \p json
 * \param[in]  fn       Handler to call
 * \param[in]  userarg  User-specified argument to be passed to \p fn
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_BranchAwaitEvent(
    void* branch, int events, void* uuid, char* json, int jsonsize,
    void (*fn)(int res, int event, int evres, void* userarg), void* userarg);

/***************************************************************************//**
 * Cancels waiting for a branch event.
 *
 * Calling this function will cause the handler registered via
 * YOGI_BranchAwaitEvent() to be called with the YOGI_ERR_CANCELED error.
 *
 * \param[in] branch The branch handle
 *
 * \returns [=0] #YOGI_OK if successful
 * \returns [<0] An error code in case of a failure (see \ref EC)
 ******************************************************************************/
YOGI_API int YOGI_BranchCancelAwaitEvent(void* branch);

/***************************************************************************//**
 * Destroys an object.
 *
 * Tries to destroy the object belonging to the given handle. The call fails and
 * returns #YOGI_ERR_OBJECT_STILL_USED if the object is still being used by other
 * objects that have been created via other library calls.
 *
 * Never destroy a context object from within a handler function that is executed
 * through the same context.
 *
 * Destroying an object will cause any active asynchronous operations to get
 * canceled and the corresponding completion handlers will be invoked with an
 * error code of #YOGI_ERR_CANCELED.
 ******************************************************************************/
YOGI_API int YOGI_Destroy(void* object);

/***************************************************************************//**
 * Destroys all objects.
 *
 * Destroys all previously created objects. All handles will be invalidated and
 * must not be used any more. This effectively resets the library.
 *
 * This function must be called outside of any handler function that is executed
 * through a context's event loop.
 *
 * Destroying objects will cause any active asynchronous operations to get
 * canceled and the corresponding completion handlers will be invoked with an
 * error code of #YOGI_ERR_CANCELED.
 ******************************************************************************/
YOGI_API int YOGI_DestroyAll();

//! @}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // YOGI_CORE_H
