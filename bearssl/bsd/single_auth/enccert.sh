read -p "Enter your domain [www.example.com]: " DOMAIN

brssl ta $DOMAIN.crt > trust-anchor.h 
brssl chain $DOMAIN.crt > chain-rsa.h 
brssl skey -C $DOMAIN.key | grep -v 'RSA key (1024 bits)' > key-rsa.h 

