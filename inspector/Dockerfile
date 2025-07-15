FROM ubuntu:22.04
RUN apt-get update && apt-get install -y socat

RUN groupadd -r ctf && useradd -r -g ctf ctf

ADD --chown=ctf ./ /home/ctf/
RUN mv /home/ctf/flag "/home/ctf/flag$(cat /dev/urandom | tr -cd 'a-f0-9' | head -c 16)" && \
    chmod 440 /home/ctf/flag*
RUN chmod 774 /home/ctf/main

USER ctf
EXPOSE 5001
WORKDIR /home/ctf

CMD socat TCP-LISTEN:5001,reuseaddr,fork EXEC:'timeout 40 ./main'
