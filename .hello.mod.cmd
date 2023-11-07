savedcmd_/home/user/Desktop/test/hello.mod := printf '%s\n'   hello.o | awk '!x[$$0]++ { print("/home/user/Desktop/test/"$$0) }' > /home/user/Desktop/test/hello.mod
