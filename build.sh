#!/bin/bash

docker build --target=teda-build .
docker build --tag=ddp822/teda:1.0.0 --target=teda-deploy .
docker push ddp822/teda:1.0.0