# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.241.1/containers/ubuntu/.devcontainer/base.Dockerfile

# [Choice] Ubuntu version (use ubuntu-22.04 or ubuntu-18.04 on local arm64/Apple Silicon): ubuntu-22.04, ubuntu-20.04, ubuntu-18.04
ARG VARIANT="jammy"
FROM mcr.microsoft.com/vscode/devcontainers/base:0-${VARIANT}

SHELL ["/bin/bash", "-c"]

ARG USER_UID=1000
ARG USER_GID=$USER_UID
# Install all packages for c/c++ development
RUN apt-get update --fix-missing && export DEBIAN_FRONTEND=noninteractive \
        && apt-get -y install --no-install-recommends --fix-missing \
        build-essential \
        cmake \
        doxygen \
        clang-tidy \
        clang-format

RUN apt-get update --fix-missing && export DEBIAN_FRONTEND=noninteractive \
        && apt-get -y install --no-install-recommends --fix-missing \
        mesa-common-dev \
        libxi-dev \
        libxcursor-dev \
        libxinerama-dev \
        libxrandr-dev \
        libx11-dev \
        xvfb \
        xorg

# [Optional] Uncomment this section to install additional OS packages.
#RUN apt-get update --fix-missing && export DEBIAN_FRONTEND=noninteractive \
#        && apt-get -y install --no-install-recommends --fix-missing \
#        some more packages
