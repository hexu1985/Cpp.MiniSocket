#!/bin/sh

# create self-signed server certificate:

read -p "Enter your domain [www.example.com]: " DOMAIN

DOMAIN=${DOMAIN:="www.example.com"}
echo "DOMAIN:$DOMAIN"

echo "Create server key..."

#openssl genrsa -des3 -out server.key 1024 # -des3表示加密key文件
openssl genrsa -out server.key 1024

echo "Create server certificate signing request..."

SUBJECT="/C=US/ST=Mars/L=iTranswarp/O=iTranswarp/OU=iTranswarp/CN=$DOMAIN"

openssl req -new -subj $SUBJECT -key server.key -out server.csr

echo "Remove password..."

#mv server.key server.origin.key
#openssl rsa -in server.origin.key -out server.key

echo "Sign SSL certificate..."

openssl x509 -req -days 3650 -in server.csr -signkey server.key -out server.crt

echo "TODO:"
echo "Copy server.crt to /etc/nginx/ssl/server.crt"
echo "Copy server.key to /etc/nginx/ssl/server.key"
echo "Add configuration in nginx:"
echo "server {"
echo "    ..."
echo "    listen 443 ssl;"
echo "    ssl_certificate     /etc/nginx/ssl/server.crt;"
echo "    ssl_certificate_key /etc/nginx/ssl/server.key;"
echo "}"

