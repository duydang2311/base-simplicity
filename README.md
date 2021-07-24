# base-simplicity

[![sampctl](https://img.shields.io/badge/sampctl-base--simplicity-2f2f2f.svg?style=for-the-badge)](https://github.com/duydang2311/base-simplicity)

EDIT: Seems like I have written many messy PawnPlus task asynchronous operations and over complicated them when they could have been more simplified.

## Introduction

A simple base using Pawn along with NodeJS + MongoDB

Mainly using RESTful API to send request and response to each other

## Requirement

Having these installed:

- NodeJS

- MongoDB

## Installation

Simply clone this repository:

```bash
git clone https://github.com/duydang2311/base-simplicity.git
```

## Setup

Inside the main folder, run the following command to install all the `Pawn` dependencies:

```bash
sampctl package ensure
```

Inside the folder `node-db`, run the following command to install all the `npm` packages:

```bash
npm install
```

## Usage

To start the gamemode, be sure to run the MongoDB server first by running this following command inside the folder `node-db`:

```bash
npm start
```

After that, start your gamemode (`samp-server.exe` or `sampctl package run`)

## Testing

To test, simply run the package:

```bash
sampctl package run
```
