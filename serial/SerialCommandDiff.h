#ifndef MSFS_CONTROLLER_SERIALCOMMANDDIFF_H
#define MSFS_CONTROLLER_SERIALCOMMANDDIFF_H
#include <string>

class SerialCommandDiff {

public:
    static std::string diff(std::string command1, std::string command2) {
        std::string result;
        std::string line1, line2;
        size_t pos1 = 0, pos2 = 0;

        while (pos2 < command2.size()) {
            // Extract next line from command1 (if available)
            if (pos1 < command1.size()) {
                size_t newline1 = command1.find('\n', pos1);
                if (newline1 == std::string::npos) newline1 = command1.size();
                line1 = command1.substr(pos1, newline1 - pos1);
                pos1 = newline1 + 1;
            } else {
                line1 = "";
            }

            // Extract next line from command2
            size_t newline2 = command2.find('\n', pos2);
            bool hasNewline = (newline2 != std::string::npos);
            if (newline2 == std::string::npos) newline2 = command2.size();
            line2 = command2.substr(pos2, newline2 - pos2);
            pos2 = newline2 + 1;

            // Compare lines and add to result if different
            if (line1 != line2 && !line2.empty()) {
                if (!result.empty()) result += "\n";
                result += line2;
                if (hasNewline) result += "\n";
            }
        }

        return result;
    }
};

#endif //MSFS_CONTROLLER_SERIALCOMMANDDIFF_H