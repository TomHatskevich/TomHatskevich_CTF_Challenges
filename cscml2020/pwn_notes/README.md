# notes (pwn)
###### Creator:  Tom Hatskevich

* **Tell to the participators that the challenge running with glibc version 2.31**


### binary compilation:
```
gcc notes.c -o notes -s
```
### setup docker container:
```bash
sudo docker build --tag notes:1.0 .                                             # build docker image
sudo docker run --detach --publish 12345:12345 --name pwn_notes notes:1.0       # run docker image
sudo docker rm pwn_notes                                                        # delete runnig docker image
```

### Exploit:
* Need to modify the code according to binary location (local/remote)
* ```python
  p = remote('127.0.0.1', 12345)    # remote binary service
  #p = process('./notes')           # local binary
  ```
```bash
python2 exploit.py
```
* Dependencies:
  *  pwntools
