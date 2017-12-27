## requirements

PDFs encrypted using the scheme described in our 34C3 talk, DPRK Consumer Technology (first 8 bytes should be 01 12 78 19 03 12 78 19).

decrypt should compile on just about anything. trim_pdf probably requires a \*nix host due to using ftruncate().

## building

```
gcc -o decrypt decrypt.c
gcc -o trim_pdf trim_pdf.c
```

## running

To decrypt a single PDF using the brute-force method:

```
./decrypt encrypted.pdf > decrypted.pdf
```

To truncate a PDF after the final %%EOF marker, removing RedStar watermarks and any other trailing garbage:

```
./trim_pdf decrypted.pdf
```

To decrypt and trim an entire directory of PDFs:

```
find dir-of-encrypted-pdfs -type f -iname \*.pdf -exec bash release-helper.sh {} \;
```
