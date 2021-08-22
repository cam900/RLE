# RLE
RLE compressor/decompressor

## Compression/Decompression example:


01 23 45 45       = 01 23 45 45 (00)

67 67 67 67       = 67 67 (02)

89 89 89 89 89 89 = 89 89 (04)

01 01 01 01 01 23 = 01 01 (03) 23

The number in parentheses (Length value, Variable size) is the number of times to repeat the previous byte.


## Length value format:

0-127:         0 aaa aaaa                       = 1 byte  (a)

128-16511:     1 aaa aaaa 0 bbb bbbb            = 2 bytes (a + (b * 128) + 128)

16512-2113663: 1 aaa aaaa 1 bbb bbbb 0 ccc cccc = 3 bytes (a + (b * 128) + (c * 16384) + 16512)

...
