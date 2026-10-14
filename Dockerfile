FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive
ARG USER_ID=1000
ARG GROUP_ID=1000

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        binutils \
        build-essential \
        file \
        gdb \
        gcc-multilib \
        libc6-dev-i386 \
        make \
        nasm \
        strace \
        tmux \
        vim \
    && rm -rf /var/lib/apt/lists/*

# Usa el UID/GID del host para que los artefactos del volumen no pertenezcan a root.
RUN groupadd --gid "${GROUP_ID}" woody \
    && useradd --uid "${USER_ID}" --gid "${GROUP_ID}" \
        --create-home --shell /bin/bash woody

USER woody
WORKDIR /workspace

# Mantiene el contenedor activo para entrar posteriormente con `make attach`.
CMD ["sleep", "infinity"]
