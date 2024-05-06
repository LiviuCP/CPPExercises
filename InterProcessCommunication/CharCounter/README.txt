1. Introduction

This application parses multiple files character by character. It provides statistics regarding occurrences of each character within these files. Only standard ASCII chars are taken into account.

2. Running the application

The arguments to be provided to the app are following:
- character types to be counted (first argument)
- file paths (second, third, etc)

The first option can take following (case-sensitive) values:
-d for digits
-l for lower case alphanumeric characters
-u for upper case alphanumeric characters
-lu for upper and lower case alphanumeric characters
-ad for lower/upper case alpha and digits

The output is provided within console and consists of the "valid" characters (depending on first option) and their occurrences. Only characters with at least one occurrence are being displayed.

Notes:
- minimum two arguments should be provided to the app executable:
  - a valid parsing option (first argument)
  - a (valid) file path
- maximum file size to be parsed is 16KB. The characters beyond this size are being ignored and only the first 16KB are taken into consideration.
- file paths can be relative or absolute

