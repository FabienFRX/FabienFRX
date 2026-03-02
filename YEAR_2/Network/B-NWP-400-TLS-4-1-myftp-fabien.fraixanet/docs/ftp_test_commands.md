# FTP Server Test Commands Guide

## 1. Server Setup and Launch

### Compilation
```bash
make
make clean
make fclean
make re
```

### Launch Server
```bash
./myftp 2121 /tmp/ftp_home
```

## 2. Basic Connection Test

### Connect with Standard FTP Client
```bash
ftp localhost 2121
```

### Manual Connection Test (Raw Protocol)
```bash
telnet localhost 2121
```

## 3. Authentication Sequence

```
USER Anonymous
PASS
```

## 4. Basic Server Information Commands

```
SYST
FEAT
NOOP
PWD
```

## 5. Directory Operations

```
PWD
LIST
CWD subdirectory
PWD
CDUP
PWD
MKD testdir
CWD testdir
PWD
CDUP
RMD testdir
```

## 6. File Transfer Mode Setup

### ASCII Mode
```
TYPE A
```

### Binary Mode
```
TYPE I
```

## 7. Passive Mode File Transfer

```
PASV
LIST
PASV
RETR filename.txt
PASV
STOR uploadfile.txt
```

## 8. Active Mode File Transfer

```
PORT 127,0,0,1,20,21
LIST
PORT 127,0,0,1,20,22
RETR filename.txt
PORT 127,0,0,1,20,23
STOR uploadfile.txt
```

## 9. File Operations

```
SIZE filename.txt
DELE filename.txt
RNFR oldname.txt
RNTO newname.txt
```

## 10. Multiple Client Testing

### Terminal 1
```bash
ftp localhost 2121
# Execute commands above
```

### Terminal 2 (Simultaneous)
```bash
ftp localhost 2121
# Execute same commands to test concurrent connections
```

### Terminal 3 (Simultaneous)
```bash
ftp localhost 2121
# Execute same commands to test multiple clients
```

## 11. Disconnection

```
QUIT
```

## 12. Alternative FTP Clients Testing

### Using ncftp
```bash
ncftp -u Anonymous -p "" localhost 2121
ls
cd subdirectory
get filename.txt
put localfile.txt
quit
```

### Using lftp
```bash
lftp -u Anonymous, localhost -p 2121
ls
cd subdirectory
get filename.txt
put localfile.txt
quit
```

### Using curl
```bash
# List directory
curl -u Anonymous: ftp://localhost:2121/

# Download file
curl -u Anonymous: ftp://localhost:2121/filename.txt -o downloaded.txt

# Upload file
curl -u Anonymous: -T localfile.txt ftp://localhost:2121/
```

## 13. Error Testing

### Invalid Commands
```
INVALIDCMD
USER
PASS wrongpass
CWD nonexistentdir
RETR nonexistentfile.txt
DELE nonexistentfile.txt
```

### Invalid Modes
```
TYPE X
PORT 300,300,300,300,300,300
```

## 14. Stress Testing

### Rapid Commands
```
NOOP
NOOP
NOOP
PWD
PWD
PWD
LIST
LIST
LIST
```

### Large File Transfer Test
```bash
# Create large test file first
dd if=/dev/zero of=largefile.bin bs=1M count=100

# Then in FTP session:
TYPE I
PASV
STOR largefile.bin
PASV
RETR largefile.bin
```

## 15. FileZilla GUI Testing

1. Open FileZilla
2. Host: `localhost`
3. Port: `2121`
4. Username: `Anonymous`
5. Password: (empty)
6. Click "Quickconnect"
7. Test file upload/download via GUI

## 16. Cleanup Commands

```bash
# Remove test files
rm -f downloaded.txt largefile.bin uploadfile.txt

# Stop server (Ctrl+C)
```

## 17. Test Script Example

```bash
#!/bin/bash
# automated_test.sh

# Start server in background
./myftp 2121 /tmp/ftp_home &
SERVER_PID=$!

sleep 2

# Test connection
echo "Testing FTP connection..."
ftp -n localhost 2121 << EOF
user Anonymous
pass
pwd
list
quit
EOF

# Kill server
kill $SERVER_PID
```

## 18. Expected Responses

### Successful Connection
```
220 Service ready for new user
331 User name okay, need password
230 User logged in, proceed
```

### Directory Listing
```
150 File status okay; about to open data connection
226 Closing data connection
```

### File Transfer
```
150 File status okay; about to open data connection
226 Closing data connection
```

## 19. Common Test Files to Prepare

Create these files in your FTP home directory:
```bash
mkdir -p /tmp/ftp_home/subdirectory
echo "Hello World" > /tmp/ftp_home/test.txt
echo "Binary data test" > /tmp/ftp_home/binary.bin
touch /tmp/ftp_home/empty.txt
```

## 20. Performance Testing

### Multiple simultaneous transfers
```bash
# Terminal 1
ftp localhost 2121
binary
get largefile1.bin

# Terminal 2 (simultaneously)
ftp localhost 2121
binary
get largefile2.bin

# Terminal 3 (simultaneously)
ftp localhost 2121
binary
put uploadfile.bin
```