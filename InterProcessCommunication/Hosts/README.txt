1. Introduction

The project has been tested on Linux, MacOS and Windows.

2. Input/Output

The CSV examples are contained within build directory:
- InputCSVFilesExamples
- exampleoutput.csv

When building the project, these examples are copied into the build directory. When executing the application, the output CSV file is written onto the same directory level with the input csv directory. The example output file has been provided as reference to compare the output resulted from the input examples.

The input formatting obeys the following rules:
- every CSV field requires quotes otherwise the row is discarded. Whitespace is allowed within the quotes by only on the left/right side of the useful string (payload). It gets removed (trimmed) before parsing the string.
- the MAC address is in hex format, no other characters are allowed
- for the IP address there should be exactly the 4 required fields (containing positive integers between 0 and 255) separated by dot
- the fields order is: hostname, mac address, ip address
- no other fields are allowed; any row containing more fields than required (even if the first three are valid) is discarded

The output is formatted by adding quotes and by having it ordered by hostname. Any two rows containing identical hostnames are considered duplicate and only the first one is taken into consideration when consolidating output.

Note:
- the output mac addresses and ip address classes are provided in upper case
- the output host names are provided in lower case
- same order of fields in output as in input, one extra column: hostname, mac, ip, ip class

3. IP Address Filtering

All entered IP addresses are allowed except:
- addresses starting with prefix 0
- localhost addresses (starting with 127)
- broadcast (anyhost) addresses, each one depending on class
- addresses representing only the network part (host part filled with 0s)
- addresses starting with prefix 240 and above (class E)

4. Error logging

For each CSV file, error logging is being provided in case any parsing errors occurred. A log file with the same name as the CSV but a different suffix (.csv replaced by _error.txt) is being created.

Following errors are being logged:
- invalid IP
- invalid MAC
- invalid hostname
- less info (fields) than required
- more info (fields) than required
- more csv rows than allowed

5. Additional notes
- the CSV extension is case insensitive, i.e. files ending with .cSv are also being taken into consideration
- a limit of parsed CSV files has been implemented. Currently this is 5 files. An error is triggered by application if this limit is exceeded and neither CSV file gets parsed.
- a limit of parsed rows per file has been implemented. Currently this is 100 rows. Additional rows get discarded.
- any existing error files get removed by application prior to parsing the input csv files

