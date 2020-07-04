FROM ubuntu:20.04

ADD . /app

WORKDIR /app

# set root password, create local unprivilege user, set read-execute permissions to ctf directory+files
RUN apt-get update
RUN adduser --disabled-password --gecos "" ctf
RUN echo 'root:3WdZAY45P5TQXn6U' | chpasswd
RUN mv flag.txt /home/ctf/flag.txt
RUN apt-get install build-essential -y
RUN apt-get install -y socat
RUN gcc /app/notes.c -o /home/ctf/notes -s
RUN rm -r /app
RUN chown root:root /home/ctf
RUN chmod -R 705 /home/ctf

WORKDIR /home/ctf

EXPOSE 12345

USER ctf

# listen for incoming challenge connections
CMD socat TCP4-LISTEN:12345,reuseaddr,fork EXEC:'/home/ctf/notes'
