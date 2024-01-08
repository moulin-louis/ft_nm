#!/bin/bash

# Build image and add a descriptive tag
docker build --tag=ft_nm .

# Run flask app
docker run -it ft_nm:latest