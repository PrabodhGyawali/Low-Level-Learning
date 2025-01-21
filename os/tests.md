# XV6 Shell Test Cases

## Directory Navigation Tests (1.5 points)
1. Basic Directory Operations (0.5)
   - `cd .`
   - `cd tempdir`

2. Nested Directory Navigation (0.5)
   - `cd dir1/dir2`
   - `cd ..`

3. Whitespace Handling (0.5)
   - `     cd .` (leading spaces)
   - `cd   dir1/dir2` (multiple spaces after cd)
   - `cd dir1/dir2          ` (trailing spaces)

## Echo Command Tests (2.0 points)
1. Special Characters (0.5)
   - `echo TeStInGtHiSsHeLl1@-+=~`

2. Whitespace Handling (0.75)
   - `   echo AbitraryNumberOfSpaces` (3 leading spaces)
   - `echo                     21_spaces` (21 spaces)

3. Multiple Arguments (0.5)
   - `echo String1 String2 String3 String4`
   - `echo String1   String2     String3      String4` (varied spacing)

## File Operations Tests (1.5 points)
1. Directory Listing (1.0)
   - `ls` and `../ls` (after cd into directory)
   - `  ls` (with leading space)
   - `     ls` (with multiple leading spaces)

2. File Reading (0.5)
   - Multiple `cat README` executions (8 times)

## I/O Redirection Tests (7.0 points)
1. Basic Redirection (3.0)
   - `echo SomeText Here > file2` followed by `cat < file2`

2. Complex Content (3.0)
   - `echo Text With Spaces Numbers975 and Special chars@£ > file1`
   - `cat file1`

3. Advanced Redirection (1.0)
   - `cat < README > file2`

## Pipeline Tests (8.0 points)
1. Basic Pipelines (5.0)
   - `ls | grep stres` (2.5)
   - `cat README | grep xv6` (2.5)

2. Complex Pipelines (3.0)
   - `cat README|grep xv6` (no spaces around pipe)
   - `ls | grep test | cat > file4.txt`
   - `cat README | grep the | grep Version`
   - `cat README|grep the|grep Version`
   - `cat README | grep the | grep Version > file1`
   - `cat < README | grep the | grep Version > file3`

## Command Chaining Tests (1.0 points)
1. Semicolon Operator
   - ```bash
     echo nospace;echo one space before ;echo one space after; echo spaces around semicolon ; echo done
     ```

## Shell Interface Tests (0.5 points)
1. Prompt Display
   - Verify `>>>` prompt is shown consistently

Total Points: 21.5

## Test Execution Environment
- Operating System: xv6
- Shell: Custom implementation
- Required Files: README, test files (created during testing)

## Notes
- All tests should be executed in sequence
- Each test assumes a clean environment
- File operations may require cleanup between tests
