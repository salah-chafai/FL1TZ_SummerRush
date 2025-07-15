FROM ubuntu:22.04

RUN apt-get update && apt-get install -y gdb gdbserver && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

RUN groupadd -r ctf && useradd -r -g ctf -d /home/ctf -s /bin/bash ctf

COPY . /home/ctf/

RUN set -eux; \
    cd /home/ctf; \
    random_suffix=$(head /dev/urandom | tr -cd 'a-f0-9' | head -c 16); \
    mv flag "flag${random_suffix}"; \
    chmod 440 "flag${random_suffix}"; \
    chown -R ctf:ctf /home/ctf

USER ctf

WORKDIR /home/ctf

CMD ["gdbserver", "localhost:5001", "./main"]
