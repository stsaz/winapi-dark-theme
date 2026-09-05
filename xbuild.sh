#!/bin/bash

IMAGE_NAME=windarktheme-builder
CONTAINER_NAME=windarktheme_build
ARGS=${@@Q}

set -xe

WDT_DIR="$(dirname "$0")"

image() {
	cat <<EOF | podman build -t $IMAGE_NAME -f - .
FROM debian:trixie-slim
RUN apt update && \
 apt install -y \
  make
RUN apt install -y \
 clang lld
RUN apt install -y \
 gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64
EOF
}

if ! podman container exists $CONTAINER_NAME ; then
	if ! podman image exists $IMAGE_NAME ; then
		image
	fi

	# Create builder container
	podman create --attach --tty \
	 -v "$(pwd)":/build \
	 -v "$WDT_DIR/..":/src \
	 --workdir /build \
	 --name $CONTAINER_NAME \
	 $IMAGE_NAME \
	 sleep 3600
fi

# Start container in background
if ! podman container top $CONTAINER_NAME ; then
	podman start --attach $CONTAINER_NAME &
	while ! podman container top $CONTAINER_NAME ; do
		sleep .5
	done
fi

# Prepare build script
cat >build.sh <<EOF
set -xe

export PATH=\$PATH:/usr/lib/llvm-19/bin
mkdir -p _win-amd64
make -j8 \
 -C _win-amd64 \
 -f /src/winapi-dark-theme/example/Makefile \
 WDT=/src/winapi-dark-theme \
 $ARGS
EOF

# Build inside the container
podman exec $CONTAINER_NAME \
 bash build.sh
