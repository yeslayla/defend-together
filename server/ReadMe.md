# Defend Together Server

Here are the instructions below for running the server locally on linux.

## Build Requirements

_Instructions are for Fedora Linux_

Install dependencies:

`dnf install enet-devel g++`

For testing:

`dnf install gtest gmock gmock-devel gtest-devel`

## Building Server

Within the server directory run: `bash build.sh`

## Running Tests

Within the server directory run: `bash run_tests.sh`

## Running Server

If you have already built the server, you can run the binary location at `builds/server.out`, otherwise within the server directory, you can run `run_build.sh` to build and run the server.

# Running in Docker

A `Dockerfile` is provided in this project. So you can run `docker build -t defend-together .` and then `docker run -p 7777:7777 defend-together` to run locally on port `7777` in docker.