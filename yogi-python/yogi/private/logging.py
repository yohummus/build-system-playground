from .errors import api_result_handler
from .library import yogi
from .object import Object
from .time import timestamp_to_datetime

import datetime
import inspect
from enum import IntEnum
from typing import Callable, Any, Union
from ctypes import c_int, c_longlong, c_char, c_char_p, c_void_p, CFUNCTYPE, \
    POINTER, create_string_buffer, sizeof, byref


class Verbosity(IntEnum):
    """Levels of how verbose logging output is.

    Attributes:
        FATAL   Fatal errors are errors that require a process restart.
        ERROR   Errors that the system can recover from.
        WARNING Warnings.
        INFO    Useful general information about the system state.
        DEBUG   Information for debugging.
        TRACE   Detailed debugging information.
    """
    FATAL = 0
    ERROR = 1
    WARNING = 2
    INFO = 3
    DEBUG = 4
    TRACE = 5


class Stream(IntEnum):
    """Output streams for writing to the terminal.

    Attributes:
        STDOUT Standard output.
        STDERR Standard error output.
    """
    STDOUT = 0
    STDERR = 1


yogi.YOGI_LogToConsole.restype = api_result_handler
yogi.YOGI_LogToConsole.argtypes = [c_int, c_int, c_int, c_char_p, c_char_p]


def log_to_console(verbosity: Union[Verbosity, None],
                   stream: Stream = Stream.STDERR, color: bool = True,
                   timefmt: str = None, fmt: str = None) -> None:
    """Allows the YOGI to write library-internal and user logging to stdout or
    stderr.

    Setting verbosity to Verbosity.None disables logging to the console.

    This function supports colourizing the output if the terminal that the
    process is running in supports it. The color used for a log entry depends
    on the entry's severity. For example, errors will be printed in red and
    warnings in yellow.

    Each log entry contains the *component* tag which describes which part of
    a program issued the log entry. For entries created by the library itself,
    this parameter is prefixed with the string "Yogi.", followed by the
    internal component name. For example, the component tag for a branch would
    be "Yogi.Branch".

    The timefmt parameter describes the textual format of a log entry's
    timestamp. The following placeholders are supported:
        %Y: Four digit year.
        %m: Month name as a decimal 01 to 12.
        %d: Day of the month as decimal 01 to 31.
        %F: Equivalent to %Y-%m-%d (the ISO 8601 date format).
        %H: The hour as a decimal number using a 24-hour clock (00 to 23).
        %M: The minute as a decimal 00 to 59.
        %S: Seconds as a decimal 00 to 59.
        %T: Equivalent to %H:%M:%S (the ISO 8601 time format).
        %3: Milliseconds as decimal number 000 to 999.
        %6: Microseconds as decimal number 000 to 999.
        %9: Nanoseconds as decimal number 000 to 999.

    The fmt parameter describes the textual format of the complete log entry
    as it will appear on the console. The supported placeholders are:
        $t: Timestamp, formatted according to the timefmt parameter.
        $P: Process ID (PID).
        $T: Thread ID.
        $s: Severity as 3 letters (FAT, ERR, WRN, IFO, DBG or TRC).
        $m: Log message.
        $f: Source file name.
        $l: Source line number.
        $c: Component tag.
        $<: Set console color corresponding to severity.
        $>: Reset the colours (also done after each log entry).
        $$: A $ sign.

    Args:
        verbosity: Maximum verbosity of messages to log.
        stream:    The stream to use.
        color:    Use colours in output.
        timefmt:   Format of the timestamp (see above for placeholders).
        fmt:       Format of a log entry (see above for placeholders).
    """
    if timefmt is not None:
        timefmt = timefmt.encode("utf-8")

    if fmt is not None:
        fmt = fmt.encode("utf-8")

    if verbosity is None:
        yogi.YOGI_LogToConsole(-1, 0, 0, None, None)
    else:
        yogi.YOGI_LogToConsole(verbosity, stream, color, timefmt, fmt)


yogi.YOGI_LogToHook.restype = api_result_handler
yogi.YOGI_LogToHook.argtypes = [
    c_int, CFUNCTYPE(
        None, c_int, c_longlong,
        c_int, c_char_p, c_int, c_char_p, c_char_p, c_void_p), c_void_p]


def log_to_hook(verbosity: Union[Verbosity, None],
                fn: Callable[[Verbosity, datetime.datetime, int, str, int,
                              str, str], Any] = None) -> None:
    """Installs a callback function for receiving log entries.

    This function can be used to get notified whenever the Yogi library itself
    or the user produces log messages. These messages can then be processed
    further in user code.

    Setting verbosity to Verbosity.None disables the hook.

    The parameters passed to fn are (from left to right):
        severity:  Severity (verbosity) of the entry.
        timestamp: Time when the entry was created.
        tid:       ID of the thread that created the entry.
        file:      Source filename.
        line:      Source file line number.
        comp:      Component that created the entry.
        msg:       Log message.

    Args:
        verbosity: Maximum verbosity of messages to log.
        fn:        Callback function (see above for parameters).
    """
    if fn is None:
        def dummy_fn(*args):
            pass
        fn = dummy_fn

    def wrapped_fn(severity, timestamp, tid, file, line, comp, msg, userdata):
        fn(Verbosity(severity), timestamp_to_datetime(timestamp), tid,
           file.decode("utf-8"), line, comp.decode("utf-8"),
           msg.decode("utf-8"))

    c_fn = yogi.YOGI_LogToHook.argtypes[1](wrapped_fn)
    global _log_to_hook_fn
    _log_to_hook_fn = c_fn  # To prevent GC to destroy fn

    if verbosity is None:
        yogi.YOGI_LogToHook(-1, c_fn, None)
    else:
        yogi.YOGI_LogToHook(verbosity, c_fn, None)


yogi.YOGI_LogToFile.restype = api_result_handler
yogi.YOGI_LogToFile.argtypes = [
    c_int, c_char_p, c_char_p, c_int, c_char_p, c_char_p]


def log_to_file(verbosity: Union[Verbosity, None], filename: str = None,
                timefmt: str = None, fmt: str = None) -> str:
    """Creates log file.

    This function opens a file to write library-internal and user logging
    information to. If the file with the given filename already exists then it
    will be overwritten.

    Setting verbosity to Verbosity.None disables logging to a file.

    The filename parameter supports all placeholders that are valid for the
    timefmt parameter (see below).

    The timefmt parameter describes the textual format of a log entry's
    timestamp. The following placeholders are supported:
        %Y: Four digit year.
        %m: Month name as a decimal 01 to 12.
        %d: Day of the month as decimal 01 to 31.
        %F: Equivalent to %Y-%m-%d (the ISO 8601 date format).
        %H: The hour as a decimal number using a 24-hour clock (00 to 23).
        %M: The minute as a decimal 00 to 59.
        %S: Seconds as a decimal 00 to 59.
        %T: Equivalent to %H:%M:%S (the ISO 8601 time format).
        %3: Milliseconds as decimal number 000 to 999.
        %6: Microseconds as decimal number 000 to 999.
        %9: Nanoseconds as decimal number 000 to 999.

    The fmt parameter describes the textual format of the complete log entry
    as it will in the log file. The supported placeholders are:
        $t: Timestamp, formatted according to the timefmt parameter.
        $P: Process ID (PID).
        $T: Thread ID.
        $s: Severity as 3 letters (FAT, ERR, WRN, IFO, DBG or TRC).
        $m: Log message.
        $f: Source file name.
        $l: Source line number.
        $c: Component tag.
        $$: A $ sign.

    Args:
        verbosity: Maximum verbosity of messages to log.
        filename:  Path to the log file (see above for placeholders).
        timefmt:   Format of the timestamp (see above for placeholders).
        fmt:       Format of a log entry (see above for placeholders).

    Returns:
        The generated filename with all placeholders resolved.
    """
    if verbosity is None:
        yogi.YOGI_LogToFile(-1, None, None, 0, None, None)
        return

    assert filename is not None
    filename = filename.encode("utf-8")

    if timefmt is not None:
        timefmt = timefmt.encode("utf-8")

    if fmt is not None:
        fmt = fmt.encode("utf-8")

    gen_filename = create_string_buffer(256)
    yogi.YOGI_LogToFile(verbosity, filename, gen_filename,
                        sizeof(gen_filename), timefmt, fmt)
    return gen_filename.value.decode("utf-8")


yogi.YOGI_LoggerCreate.restype = api_result_handler
yogi.YOGI_LoggerCreate.argtypes = [POINTER(c_void_p), c_char_p]

yogi.YOGI_LoggerGetVerbosity.restype = api_result_handler
yogi.YOGI_LoggerGetVerbosity.argtypes = [c_void_p, POINTER(c_int)]

yogi.YOGI_LoggerSetVerbosity.restype = api_result_handler
yogi.YOGI_LoggerSetVerbosity.argtypes = [c_void_p, c_int]

yogi.YOGI_LoggerSetComponentsVerbosity.restype = api_result_handler
yogi.YOGI_LoggerSetComponentsVerbosity.argtypes = [
    c_char_p, c_int, POINTER(c_int)]

yogi.YOGI_LoggerLog.restype = api_result_handler
yogi.YOGI_LoggerLog.argtypes = [c_void_p, c_int, c_char_p, c_int, c_char_p]


class Logger(Object):
    """Allows generating log entries.

    A logger is an object used for generating log entries that are tagged with
    the logger's component tag. A logger's component tag does not have to be
    unique, i.e. multiple loggers can be created using identical component
    tags.

    The verbosity of a logger acts as a filter. Only messages with a verbosity
    less than or equal to the logger's verbosity are being logged.

    Note: The verbosity of a logger affects only messages logged through that
          particular logger, i.e. if two loggers have identical component tags
          their verbosity settings are still independent from each other.
    """
    @staticmethod
    def set_components_verbosity(components: str, verbosity: Verbosity
                                 ) -> int:
        """Sets the verbosity of all loggers matching a given component tag.

        This function finds all loggers whose component tag matches the regular
        expression given in the components parameter and sets their verbosity
        to the value of the verbosity parameter.

        Args:
            components: Regex (ECMAScript) for the component tags to match.
            verbosity:  Maximum verbosity entries to be logged.

        Returns:
            Number of matching loggers.
        """
        n = c_int()
        yogi.YOGI_LoggerSetComponentsVerbosity(components.encode("utf-8"),
                                               verbosity, byref(n))
        return n.value

    def __init__(self, component: str):
        """Creates a logger.

        The verbosity of new loggers is Verbosity.INFO by default.

        Args:
            component: The component tag to use.
        """
        handle = c_void_p()
        yogi.YOGI_LoggerCreate(byref(handle), component.encode("utf-8"))
        Object.__init__(self, handle)

    @property
    def verbosity(self) -> Verbosity:
        """Verbosity of the logger.
        """
        vb = c_int()
        yogi.YOGI_LoggerGetVerbosity(self._handle, byref(vb))
        return Verbosity(vb.value)

    @verbosity.setter
    def verbosity(self, verbosity: Verbosity):
        yogi.YOGI_LoggerSetVerbosity(self._handle, verbosity)

    def log(self, severity: Verbosity, msg: str, file: str = None,
            line: int = None) -> None:
        """Creates a log entry.

        The file and line parameters are optional. By default, they will be
        set to the file and line of the calling function.

        Args:
            severity: Severity (verbosity) of the entry
            msg:      Log message
            file:     Source file name
            line:     Source file line number
        """
        if file is None and line is None:
            caller = inspect.getframeinfo(inspect.stack()[1][0])
            file = caller.filename
            line = caller.lineno
        else:
            assert (file is not None and line is not None), \
                "File and line must both be set or both be None"

        yogi.YOGI_LoggerLog(self._handle, severity, file.encode("utf-8"),
                            line, msg.encode("utf-8"))


class AppLogger(Logger):
    """Represents the App logger singleton.

    The App logger always exists and uses "App" as its component tag.
    Instances of this class can be created; however, they will always point to
    the same logger, i.e. changing its verbosity will change the verbosity of
    every AppLogger instance.
    """

    def __init__(self):
        """Create an instance of the App logger."""
        Object.__init__(self, None)

    def __del__(self):
        pass

    def __str__(self):
        return self.__class__.__name__


app_logger = AppLogger()
