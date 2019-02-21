# Contributing
This document lists some basic guidelines for contributing to this project. 

## Header files
- Header files should not use `#ifndef / #endif` pre-processor directives for the purposes of ensuring they are not included multiple times.
- Use the `#pragma once` directive at the top of header files instead. 

## Testing
- Catch2 is the test framework used for unit-testing in this project
- Only **visible** methods should be tested (e.g. no testing of private methods using hacks like `#define private public` or more legitimate approaches like `friend` classes). 
- If a private method implements a large amount of functionality that is worthy of testing, it should likely be moved into a class of its own with its own public interface. 
- Ensure that the common classes of success and failure for your functions are covered by unit testing 

## Pull Requests
- Pull requests that are to be discussed / not ready to merge should be marked with a [WIP] tag in the title.
- At least one reviewer is required before a pull request can be merged.
- Keep pull requests small, but they should still represent a logical unit of functionality.
- For large pull requests (non-trivial changes like a SPA component section), please ensure that some planning and/or discussion was done in an Issue first.
- Make sure all tests are passing.
- Make sure that your code is properly formatted (with clang-format).
- Make sure that public interfaces are appropriately documented.

## Error Handling
- Refer to https://github.com/jethrokuan/SPA/issues/19
- Return value-based error handling should be used when the error is handled immediately after the call
- Exception-based error handling should be used when the error is handled further up the call chain 

## noexcept and const annotations
- Whereever possible, annotate functions that have no chance of causing an exception with `noexept`
- Whereever possible, annotate parameters/variable/etc with `const` if they are not going to be modifies
