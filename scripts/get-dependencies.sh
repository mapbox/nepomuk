#!/usr/bin/env bash

set -o errexit
set -o pipefail
set -o nounset

ZEROMQPP_GIT="https://github.com/zeromq/cppzmq.git"
ZEROMQPP_TAG=v4.2.1

read -p "Update ZeroMQPP? [y/n]" ok
if [[ $ok =~ [yY] ]]
then
  if [ -d "third_party/zmqpp" ]; then
    echo git subtree pull -P third_party/zmqpp/ $ZEROMQPP_GIT $ZEROMQPP_TAG --squash
  else
    git subtree add -P third_party/zmqpp/ $ZEROMQPP_GIT $ZEROMQPP_TAG --squash
  fi
fi
