# Guidelines for contributing to Nepomuk

This guide describes what you should follow when considering contributions to Nepomuk.

## Format

We use the supplied style file in combination with clang-format 3.8.1. Ensure that your code is formatted (you can use scripts/format.sh to do so).

## Versioning

We follow http://semver.org/ and give the following guarantees:

### Major Version Change

- API format can be changed (will be noted as **BREAKING**) in the changelog
- FILEFORMATs can be changed (will be noted as **BREAKING**) in the changelog

### Minor Version Change

- Forward compatible for:
  - http API
  - C++ library interface
  - node API
- FILEFORMATs are not guaranteed and
  -  might need to be recreated (**REPROCESS**) to work at all
  -  or updated (**UPDATE**) to gain access to new functionality

We consider additions to the API forward-compatible. This excludes any additions to the API that would require dedicated treatment in a consumer like adding a new segment type.

### Patch Version Change

- Compatible
  - http API
  - C++ library interface
  - node API
  - FILEFORMATS might require recreation to gain access to new functionality (**UPDATE**)

## Changelog

Always update the changelog when providing a new PR. Ensure that each bugfix refers to the respective issue.
On adding new features, provide a concise summary of the feature and (if available) a link to a discussion ticket.

**Breaking Changes** have to be marked as such by adding **BREAKING** in front of them.
Whenever a change breaks the internal fileformat and requires the files to be recreated, mark the change as **REPROCESS**.
If new functionaity requires updating internal files, mark the features as **UPDATE**. **UPDATE** requires non-updated files to continue working, though.
