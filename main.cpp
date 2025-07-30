// Copyright 2025 Danny Nguyen
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/time_parsers.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/regex.hpp>

using boost::posix_time::time_duration;

std::string format_ptime_numeric(const boost::posix_time::ptime &pt) {
    std::ostringstream oss;
    boost::gregorian::date d = pt.date();
    boost::posix_time::time_duration t = pt.time_of_day();
    oss << std::setw(4) << std::setfill('0') << static_cast<int>(d.year()) << "-"
        << std::setw(2) << std::setfill('0') << static_cast<unsigned>(d.month()) << "-"
        << std::setw(2) << std::setfill('0') << d.day() << " "
        << std::setw(2) << std::setfill('0') << t.hours() << ":"
        << std::setw(2) << std::setfill('0') << t.minutes() << ":"
        << std::setw(2) << std::setfill('0') << t.seconds();
    return oss.str();
}

int main(int argc, char *argv[]) {
    int numLines = 1;

    std::string str, inputfilename;
    boost::regex startMessage("( \\(log.c.166\\) server started)");
    boost::regex endMessage(
    R"(^([0-9]{4}-[0-9]{2}-[0-9]{2}\s[0-9]{2}:[0-9]{2}:[0-9]{2}).*oejs\.AbstractConnector:Started SelectChannelConnector@0\.0\.0\.0:9080)");
    inputfilename = argv[1];
    std::ifstream myFile(inputfilename);
    std::ofstream output_file;
    std::string outputname = inputfilename + ".rpt";

    bool stillBooting = false;
    boost::posix_time::ptime startTime, endTime;

    if (!myFile) {
        std::cout << "Could not open file\n";
        exit(-1);
    }
    output_file.open(outputname);
    if (!output_file.is_open()) {
        std::string out_err = "Unable to create file: " + outputname;
        exit(1);
    }
    auto get_time = [](std::string time_str) {
        return time_str.substr(0, 19);
    };

    if (output_file.is_open()) {
        output_file << "Device Boot Report\n";
        output_file << "InTouch log file: " << argv[1] << "\n\n";
    }

    while (getline(myFile, str)) {
        if (regex_search(str, startMessage)) {
            startTime = boost::posix_time::time_from_string(get_time(str));

            if (stillBooting) {
                output_file << "**** Incomplete Booting **** \n\n";
                stillBooting = false;
            }

            stillBooting = true;
            output_file << "=== Device Boot ===\n";
            output_file << numLines << "(" << argv[1] << "): "
                << format_ptime_numeric(startTime) << " Boot Start\n";

        } else if (regex_search(str, endMessage)) {
            stillBooting = false;
            endTime = boost::posix_time::time_from_string(get_time(str));

            time_duration duration = endTime - startTime;

            output_file << numLines << "(" << argv[1] << "): "
                << format_ptime_numeric(endTime) << " Boot Completed\n\tBoot Time: "
                << duration.total_milliseconds() << "ms\n\n";
        }
        numLines++;
    }

    // If reached end of file and still hadn't completed
    if (stillBooting) {
        output_file << "**** Incomplete Booting **** \n\n";
    }

    return 0;
}
