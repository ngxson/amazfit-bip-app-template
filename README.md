# Amazfit Bip app template

Inspired by https://github.com/MNVolkov/Template

This template aims to provide a way to build BipOS apps using Docker.

## How to use

Modify variables inside `build.sh`

```bash
PROJECT_NAME="my_awesome_app"
LIBBIP_PATH="libbip"
LIBBIP_A_PATH="${LIBBIP_PATH}/libbip.a"
```

Then, run `docker compose up` to build.

## Change libbip.a

Where to get different versions of libbip.a?

Here ==> https://github.com/MNVolkov/libbip
