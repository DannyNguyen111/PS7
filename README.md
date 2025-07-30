# PS7
Kronos Log Parsing

In PS7, I developed a C++ log parser that processes Kronos device boot logs to generate a detailed boot report. Using Boost libraries for date-time parsing and regex matching, the program scans each line of the input log file to detect boot start and completion events based on specific message patterns. It records timestamps of these events, calculates boot durations with high precision, and outputs a formatted report including incomplete boots if any. The parser handles file I/O robustly, uses lambda functions for cleaner time extraction, and formats timestamps with zero-padded numeric output for readability. This project demonstrates efficient log parsing, precise time computations, and clear report generation, facilitating analysis of device boot performance.

The program starts by including necessary headers for I/O, string manipulation, and Boost libraries for date-time and regex handling. The function format_ptime_numeric formats a Boost ptime object into a zero-padded, human-readable timestamp string (YYYY-MM-DD HH:MM:SS), improving report clarity.

In main, the program accepts a log filename as a command-line argument and prepares input and output file streams, ensuring files open successfully before proceeding. It uses two Boost regular expressions: startMessage detects lines indicating the start of a device boot sequence, and endMessage identifies when the boot process completes.

A boolean flag stillBooting tracks if a boot is in progress, while startTime and endTime store the corresponding timestamps. The program reads the input file line-by-line, using regex matching to detect boot start and completion events. When a start line is found, it extracts the timestamp substring (first 19 characters) via a lambda and converts it to a ptime object. If a previous boot was flagged as ongoing without completion, the parser outputs a warning for incomplete booting before starting a new boot record.

Upon detecting a boot completion line, the program records the end time, calculates the duration by subtracting startTime from endTime as a time_duration, and writes the boot completion time and total boot time in milliseconds to the report.

If the file ends while a boot is still flagged as ongoing, the program appends an incomplete boot warning to the report.

The parser outputs a clean, formatted report including file metadata, line numbers, timestamps, boot events, and durations. It demonstrates robust error handling, use of Boost’s advanced date-time and regex capabilities, and clear separation of concerns by encapsulating formatting and parsing logic in reusable components.
