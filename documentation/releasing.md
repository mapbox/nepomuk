# Releasing Nepomuk

To release a new version of Nepomuk, follow these steps:

## Milestones / Issues / Changelog

First ensure that the respective release milestone is closed and all issues (and their respective PRs) have been resolved.
Especially pay attention to the changelog, has it been updated accordingly?

 - [ ] issues in milestone are resolved
 - [ ] no more open pull requests for the milestone
 - [ ] milestone is closed
 - [ ] changelog reflects all important changes (see below) in milestone

If all these conditions are met, we can start the releasing process.

## Preparing a Release

The `master` branch reflects the current development state of Nepomuk. In addition, we maintain a version branch `vMAJOR.MINOR` branch for managing releases. To start a new minor/major release, you should branch off of `master` into the respective release branch.
In case of a bugfix release, cherry-pick the fix into the latest release branch that is considered stable.

We will support bugfixes only for the most recent releases.

When the correct branch is selected:

- [ ] ensure that tests are passing (https://travis-ci.org/mapbox/nepomuk).
- [ ] push binaries to S3 by running `npm commit -m "[publish binary]" --allow-empty
- [ ] check travis again to ensure all binaries are published correctly
- [ ] tag the release `git tag vx.y.z -a` and copy the changelog of the respective release into the commit message
- [ ] push the newly assigned tags `git push; git push --tags`
- [ ] publish the binaries: change to `node/nepomuk` and:
    - *FINAL RELEASES ONLY*: publish the release to npm via `npm publish`
    - *PRE-RELEASES ONLY*: publish the release to npm via `npm publish --tag XXX` and use either `alpha` or `next` (this avoids updating `latest` which matches `*` in npm versions)
