1. Introduction

This application parses multiple files character by character. It provides statistics regarding occurrences of each character within these files. Only standard ASCII chars are taken into account.
The app has only been compiled and tested on Linux and MacOS so far.

2. Running the application

The arguments to be provided to the app are following:
- character types to be counted (first argument)
- aggregation mode (how occurrences are calculated - second argument)
- file paths (third argument and beyond)

The first argument (parsing option) can take following (case-sensitive) values:
-d for digits
-l for lower case alphanumeric characters
-u for upper case alphanumeric characters
-lu for upper and lower case alphanumeric characters
-ad for lower/upper case alpha and digits

The second argument (aggregating option) can take following (case-sensitive) values:
-t for displaying the total number of occurrences of each found char
-m for displaying the minimum number of occurrences (in a file) of each found char
-a for displaying the average number of occurrences of each found char
-M for displaying the maximum number of occurrences (in a file) of each found char

To conclude the app should be launched by entering the following command:
./CharCounter [first argument] [second argument] [first file path] [second file path] ...

An example would be: ./CharCounter -l -t file1.txt file2.txt file3.txt file4.txt
(search for lower-case alphanumeric characters in the 4 files contained within application directory and display total number of occurrences)

The output is provided within console and consists of the "valid" characters (depending on first argument) and their occurrences (depending on second argument). Only characters with at least one occurrence are being displayed (no matter the way occurrences get calculated).

Notes:
- minimum three arguments should be provided to the app executable:
  - a valid parsing option (first argument)
  - a valid aggregating option (second argument)
  - a (valid) file path
- maximum file size to be parsed is 16KB. The characters beyond this size are being ignored and only the first 16KB are taken into consideration.
- file paths can be relative or absolute
- the minimum number of occurrences of a matching character (aggregation option -m) is the minimum taken between files that do contain this character (or 0 if this character is contained in neither files). For example if there are 3 files and only two of them contain the character '0', one with 2 occurrences and the other with 5 occurrences, then the minimum number of occurrences is 2.
- the average number of occurrences takes all files into account. An up-rounding is being applied to ensure the average number of occurrences of each character is not 0 if this character is contained in at least one of the files. For example if character 'A' has a total number of 4 occurrences and there are 6 files, then the average will be 1. If there are 3 files instead, then the average will be 2.
