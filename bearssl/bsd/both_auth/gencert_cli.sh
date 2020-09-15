#!/bin/sh

# create self-signed server certificate:

echo "Create server key..."

#openssl genrsa -des3 -out client.key 1024 # -des3表示加密key文件
openssl genrsa -out client.key 1024

echo "Create server certificate signing request..."

SUBJECT="/C=US/ST=Mars/L=iTranswarp/O=iTranswarp/OU=iTranswarp/CN=Test"

openssl req -new -subj $SUBJECT -key client.key -out client.csr

echo "Remove password..."

#mv client.key client.origin.key
#openssl rsa -in client.origin.key -out client.key

echo "Sign SSL certificate..."

openssl x509 -req -days 3650 -in client.csr -signkey client.key -out client.crt

echo "TODO:"
echo "Copy client.crt to /etc/nginx/ssl/client.crt"
echo "Copy client.key to /etc/nginx/ssl/client.key"
echo "Add configuration in nginx:"
echo "server {"
echo "    ..."
echo "    listen 443 ssl;"
echo "    ssl_certificate     /etc/nginx/ssl/client.crt;"
echo "    ssl_certificate_key /etc/nginx/ssl/client.key;"
echo "}"

