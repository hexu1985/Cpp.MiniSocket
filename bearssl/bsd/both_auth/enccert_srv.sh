brssl ta server.crt | sed 's/TA/SERVER_TA/g' > server-trust-anchor.h 
brssl chain server.crt | sed 's/CERT/SERVER_CERT/g' | sed 's/CHAIN/SERVER_CHAIN/g' > server-chain-rsa.h 
brssl skey -C server.key | grep -v 'RSA key (1024 bits)' | sed 's/RSA/SERVER_RSA/g' > server-key-rsa.h 

