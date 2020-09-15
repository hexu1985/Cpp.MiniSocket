brssl ta client.crt | sed 's/TA/CLIENT_TA/g' > client-trust-anchor.h 
brssl chain client.crt | sed 's/CERT/CLIENT_CERT/g' | sed 's/CHAIN/CLIENT_CHAIN/g' > client-chain-rsa.h 
brssl skey -C client.key | grep -v 'RSA key (1024 bits)' | sed 's/RSA/CLIENT_RSA/g' > client-key-rsa.h 

