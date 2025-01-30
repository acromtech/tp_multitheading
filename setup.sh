sudo apt-get update
sudo apt-get install libssl-dev
pkg-config --modversion openssl
cmake -B build -S . -DOPENSSL_ROOT_DIR=/usr -DOPENSSL_LIBRARIES=/usr/lib
uv sync
