# A collection of encodings
implemented in c++

## Usage:
`main.exe [code] [command] [message]`
- `code`: the coding scheme to use (`hamming`, `rs`, `aes`, `rsa`)
- `command`: to `encode` or `decode` the message
- `mesage`: the message data, ascii characters for encoding, hex for decoding 

## Error correction codes:
- Hamming code (`hamming`)
- Reed Solomon (`rs`) (need to implement partial encoding)

## Cryptography codes:
- AES (`aes`) (incomplete)
- RSA (`rsa`) (incomplete, needs big int implementation)

